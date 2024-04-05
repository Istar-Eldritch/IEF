
use actix_web::{
    http::{header::ContentType, StatusCode},
    post, web, HttpResponse,
};
use derive_more::{Display, Error};
use serde::Deserialize;
use uuid::Uuid;

use crate::api::AppState;
use crate::data::Application;

use chrono::{DateTime, Utc};
use influxdb::{Client,  InfluxDbWriteable};

#[derive(InfluxDbWriteable)]
pub struct ServerStartedWithModMetric
{
    time: DateTime<Utc>,
    #[influxdb(tag)]
    app_name: String,
    #[influxdb(tag)]
    server_ip: String,
    repacked: bool
}

#[derive(Deserialize)]
#[serde(tag = "type")]
pub enum Metric {
    ServerStarted {
        repacked: u8
    }
}

#[derive(Deserialize)]
struct MetricRequest {
    token: Uuid,
    metric: Metric,
}

#[derive(Debug, Display, Error)]
enum MetricsError {
    #[display(fmt = "internal error")]
    InternalError,

    #[display(fmt = "Unauthorized")]
    Unauthorized,
}

impl actix_web::error::ResponseError for MetricsError {
    fn error_response(&self) -> HttpResponse {
        HttpResponse::build(self.status_code())
            .insert_header(ContentType::plaintext())
            .body(self.to_string())
    }

    fn status_code(&self) -> StatusCode {
        match *self {
            MetricsError::InternalError => StatusCode::INTERNAL_SERVER_ERROR,
            MetricsError::Unauthorized => StatusCode::UNAUTHORIZED,
        }
    }
}

#[post("/metric")]
pub async fn metric_handler(
    data: web::Data<AppState>,
    conn_info: actix_web::dev::ConnectionInfo,
    body: web::Json<MetricRequest>,
) -> Result<HttpResponse, MetricsError> {
    let server_ip: String = conn_info.realip_remote_addr().unwrap_or_default().into();

    let app = Application::find_by_token(&data.pg, body.token)
        .await
        .map_err(|err| {
            log::error!("Error finding application by token: {:?}", err);
            MetricsError::InternalError
        })?;

    if let Some(app) = app {
        let time = Utc::now();
        match body.metric {
            Metric::ServerStarted { repacked } => {
                let db_name = format!("{}_mods", &app.user_name);
                let client = Client::new(&data.influxdb_uri, &db_name).with_token(&data.influxdb_token);
                let metric = ServerStartedWithModMetric
                {
                    time,
                    server_ip: server_ip.clone(),
                    app_name: app.app_name.clone(),
                    repacked: repacked == 1
                };

                client.query(metric.into_query("mod")).await
                    .map_err(|err| {
                        log::error!("Error sending metric to influxdb: {:?}", err);
                        MetricsError::InternalError
                    })?;

                log::debug!("Stored ServerStarted metric from: {} @ {}", app.app_name, db_name);
            }
        };
        let response = HttpResponse::Ok().content_type("text/plain").body("ok");
        Ok(response)
    } else {
        log::warn!("Unauthorized access from: {}", server_ip);
        Err(MetricsError::Unauthorized)
    }
}
