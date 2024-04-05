mod api;
mod data;
use std::str::FromStr;

use crate::api::AppState;
use actix_web::{web, App, HttpServer};
use clap::Parser;
use log::LevelFilter;
use reqwest::Client;
use sqlx::postgres::PgPoolOptions;
use env_logger::Builder;

#[derive(Parser, Debug)]
#[command(version, about, long_about = None)]
struct Args {
    #[arg(long, env = "POSTGRES_URI")]
    postgres_uri: String,
    #[arg(long, env = "INFLUXDB_URI")]
    influxdb_uri: String,
    #[arg(long, env = "INFLUXDB_TOKEN")]
    influxdb_token: String,
    #[arg(long, env = "LOG_LEVEL", default_value = "info")]
    log_level: String,
    #[arg(long, env = "LOG_FILTER", default_value = "dayz_services")]
    log_filter: String,
}

#[actix_web::main]
async fn main() -> Result<(), impl std::error::Error> {
    let args = Args::parse();

    Builder::new()
        .filter(Some(&args.log_filter), LevelFilter::from_str(&args.log_level).unwrap_or(LevelFilter::Info))
        .init();

    let pool = PgPoolOptions::new()
        .max_connections(5)
        .connect(&args.postgres_uri)
        .await
        .expect("To connect to db");

    data::migrations::run_migrations(pool.clone())
        .await
        .expect("To run migrations");

    let data = AppState {
        http_client: Client::new(),
        influxdb_uri: args.influxdb_uri,
        influxdb_token: args.influxdb_token,
        pg: pool,
    };

    log::info!("Starting proxy");

    HttpServer::new(move || {
        App::new()
            .app_data(web::Data::new(data.clone()))
            .service(api::proxy::proxy_handler)
            .service(api::metrics::metric_handler)
    })
    .bind(("0.0.0.0", 80))?
    .run()
    .await
}
