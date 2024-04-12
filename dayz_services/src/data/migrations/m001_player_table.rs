use sqlx::{Postgres, Transaction};
use async_trait::async_trait;
use super::Migration;

pub struct PlayersTable;

#[async_trait]
impl Migration for PlayersTable {
    fn version(&self) -> i32 {
        1
    }

    async fn up(&self, tx: &mut Transaction<'_, Postgres>) -> Result<(), Box<dyn std::error::Error>> {
        sqlx::query(
            r#"
            create table if not exists players (
                id uuid primary key,
                steam_id varchar not null,
                discord_id varchar not null,
                verified boolean not null,
                created_at timestamptz not null,
                UNIQUE(steam_id, discord_id)
            )
            "#,
        )
        .execute(&mut **tx)
        .await?;

        sqlx::query(
            r#"
            alter table applications
            add column discord_guild_id varchar;
            "#,
        )
        .execute(&mut **tx)
        .await?;

        Ok(())
    }
}
