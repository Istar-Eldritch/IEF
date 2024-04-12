use chrono::{DateTime, Utc};
use sqlx::{Executor, FromRow, Postgres};
use uuid::Uuid;

#[derive(FromRow)]
pub struct Application
{
    pub app_id: Uuid,
    pub app_name: String,
    pub user_name: String,
    pub discord_guild_id: Option<String>
}

impl Application
{
    pub async fn find_by_token<'a>(ex: impl Executor<'a, Database = Postgres>, token: Uuid) -> Result<Option<Application>, sqlx::error::Error> {

        let user = sqlx::query_as(r#"
            select applications.id as app_id,
                applications.name as app_name,
                applications.discord_guild_id as discord_guild_id,
                users.name as user_name
            from applications
            inner join users on users.id = applications.user_id
            where applications.token = $1
        "#)
        .bind(token)
        .fetch_optional(ex).await?;

        Ok(user)
    }
}

#[derive(FromRow)]
pub struct Player
{
    pub id: Uuid,
    pub discord_id: String,
    pub steam_id: String,
    pub verified: bool,
    pub created_at: DateTime<Utc>
}

impl Player
{
    pub async fn find_by_ids<'a>(ex: impl Executor<'a, Database = Postgres>, steam_id: &str, discord_id: &str) -> Result<Option<Player>, sqlx::error::Error> {
        let player = sqlx::query_as(r#"
            select * from players where steam_id = $1 and discord_id = $2
        "#)
        .bind(steam_id)
        .bind(discord_id)
        .fetch_optional(ex).await?;
        Ok(player)
    }

    pub async fn create_player<'a>(&self, ex: impl Executor<'a, Database = Postgres>) -> Result<(), sqlx::error::Error> {
        sqlx::query_as(
            r#"
            insert into players
                (   id,
                    discord_id,
                    steam_id,
                    verified,
                    created_at
                )
            values
                ($1, $2, $3, $4, $5)
            returning *
        "#,
        )
        .bind(self.id)
        .bind(&self.discord_id)
        .bind(&self.steam_id)
        .bind(self.verified)
        .bind(self.created_at)
        .fetch_one(ex)
        .await
    }
}