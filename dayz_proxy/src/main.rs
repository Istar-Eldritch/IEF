mod api;
mod data;
use crate::api::AppState;
use actix_web::{web, App, HttpServer};
use clap::Parser;
use reqwest::Client;
use sqlx::postgres::PgPoolOptions;

#[derive(Parser, Debug)]
#[command(version, about, long_about = None)]
struct Args {
    #[arg(long, env = "POSTGRES_URI")]
    postgres_uri: String,
    #[arg(long, env = "INFLUXDB_URI")]
    influxdb_uri: String,
    #[arg(long, env = "INFLUXDB_TOKEN")]
    influxdb_token: String,
    /// Name of the person to greet
    #[arg(short, long, env = "LOG_LEVEL", default_value = "dayz_proxy=info")]
    log_level: String,
}

#[actix_web::main]
async fn main() -> Result<(), impl std::error::Error> {
    let args = Args::parse();

    std::env::set_var("RUST_LOG", args.log_level);
    env_logger::init();

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
