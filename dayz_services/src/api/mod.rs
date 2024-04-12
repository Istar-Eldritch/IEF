pub mod proxy;
pub mod metrics;
pub mod discord;

use reqwest::Client as HttpClient;
use sqlx::{Pool, Postgres};
use serenity::Client as DiscordClient;
use std::sync::Arc;

#[derive(Clone)]
pub struct AppState {
    pub discord_client: Arc<DiscordClient>,
    pub http_client: HttpClient,
    pub influxdb_uri: String,
    pub influxdb_token: String,
    pub pg: Pool<Postgres>,
}