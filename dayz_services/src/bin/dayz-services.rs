
use std::str::FromStr;
use std::sync::Arc;
use actix_web::{web, App, HttpServer};
use clap::Parser;
use log::LevelFilter;
use reqwest::Client as HttpClient;
use serenity::all::GatewayIntents;
use sqlx::postgres::PgPoolOptions;
use env_logger::Builder;
use dayz_services::api::AppState;
use dayz_services::data::migrations::run_migrations;
use dayz_services::api::{
    proxy::proxy_handler,
    metrics::metric_handler,
    discord::get_player_roles_handler
};
use serenity::Client as DiscordClient;

#[derive(Parser, Debug)]
#[command(version, about, long_about = None)]
struct Args {
    #[arg(long, env = "POSTGRES_URI")]
    postgres_uri: String,
    #[arg(long, env = "INFLUXDB_URI")]
    influxdb_uri: String,
    #[arg(long, env = "INFLUXDB_TOKEN")]
    influxdb_token: String,
    #[arg(long, env = "DISCORD_TOKEN")]
    discord_token: String,
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

    run_migrations(pool.clone())
        .await
        .expect("To run migrations");

    let intents = GatewayIntents::GUILD_MEMBERS;

    // Create a new instance of the Client, logging in as a bot.
    let discord_client = DiscordClient::builder(&args.discord_token, intents)
        .await
        .expect("To initialize discord client");

    let data = AppState {
        discord_client: Arc::new(discord_client),
        http_client: HttpClient::new(),
        influxdb_uri: args.influxdb_uri,
        influxdb_token: args.influxdb_token,
        pg: pool,
    };

    log::info!("Starting proxy");

    HttpServer::new(move || {
        App::new()
            .app_data(web::Data::new(data.clone()))
            .service(proxy_handler)
            .service(metric_handler)
            .service(get_player_roles_handler)
    })
    .bind(("0.0.0.0", 80))?
    .run()
    .await
}
