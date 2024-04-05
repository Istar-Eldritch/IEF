pub mod proxy;
pub mod metrics;

use reqwest::Client;
use sqlx::{Pool, Postgres};

#[derive(Clone)]
pub struct AppState {
    pub http_client: Client,
    pub influxdb_uri: String,
    pub influxdb_token: String,
    pub pg: Pool<Postgres>,
}