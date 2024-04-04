use actix_web::body::MessageBody;
use actix_web::{
    http::{header::ContentType, StatusCode},
    post, web, HttpResponse, Responder,
};
use derive_more::{Display, Error};
use reqwest::header::{HeaderMap, HeaderName, HeaderValue};
use serde::Deserialize;
use uuid::Uuid;

use crate::api::AppState;
use crate::data::Application;


#[derive(Deserialize)]
struct ProxyRequest {
    token: Uuid,
    url: String,
    method: String,
    headers: Vec<(String, String)>,
    query_parameters: Vec<(String, String)>,
    body: Option<String>,
}

#[derive(Debug, Display, Error)]
enum ProxyError {
    #[display(fmt = "internal error")]
    InternalError,

    #[display(fmt = "Unauthorized")]
    Unauthorized,
}

impl actix_web::error::ResponseError for ProxyError {
    fn error_response(&self) -> HttpResponse {
        HttpResponse::build(self.status_code())
            .insert_header(ContentType::plaintext())
            .body(self.to_string())
    }

    fn status_code(&self) -> StatusCode {
        match *self {
            ProxyError::InternalError => StatusCode::INTERNAL_SERVER_ERROR,
            ProxyError::Unauthorized => StatusCode::UNAUTHORIZED,
        }
    }
}

#[post("/proxy")]
pub async fn proxy_handler(
    data: web::Data<AppState>,
    body: web::Json<ProxyRequest>,
) -> Result<impl Responder, ProxyError> {
    let app = Application::find_by_token(&data.pg, body.token)
        .await
        .map_err(|err| {
            log::error!("Error finding application by token: {:?}", err);
            ProxyError::InternalError
        })?;

    if let Some(_) = app {
        let method = &body.method.clone().try_into_bytes().map_err(|e| {
            log::error!("Error parsing method: {:?}", e);
            ProxyError::InternalError
        })?;

        let method = reqwest::Method::from_bytes(method).map_err(|e| {
            log::error!("Error parsing method from bytes: {:?}", e);
            ProxyError::InternalError
        })?;

        let mut reqbuilder = data.http_client.clone().request(method, &body.url);
        let mut headers = HeaderMap::new();

        for (k, v) in body.headers.iter() {
            let header_name = HeaderName::from_lowercase(k.as_bytes()).map_err(|e| {
                log::error!("Error parsing header name: {} - {:?}", k, e);
                ProxyError::InternalError
            })?;
            let header_value = HeaderValue::from_str(v).map_err(|e| {
                log::error!("Error parsing header value: {:?}", e);
                ProxyError::InternalError
            })?;
            headers.insert(header_name, header_value);
        }

        reqbuilder = reqbuilder.headers(headers);
        reqbuilder = reqbuilder.query(&body.query_parameters);

        if let Some(body) = &body.body {
            reqbuilder = reqbuilder.body(body.to_string());
        }

        let response = reqbuilder.send().await.map_err(|e| {
            log::error!("Error sending request: {:?}", e);
            ProxyError::InternalError
        })?;

        let res_headers = response.headers();
        let mut response_builder = HttpResponse::build(response.status());

        for header in res_headers.iter() {
            response_builder.insert_header(header);
        }

        let res_body = response.bytes().await.map_err(|e| {
            log::error!("Error parsing response: {:?}", e);
            ProxyError::InternalError
        })?;

        Ok(response_builder.body(res_body))
    } else {
        Err(ProxyError::Unauthorized)
    }
}
