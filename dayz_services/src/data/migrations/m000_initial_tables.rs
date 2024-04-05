use sqlx::{Postgres, Transaction};
use async_trait::async_trait;
use super::Migration;

pub struct InitialTables;

#[async_trait]
impl Migration for InitialTables {
    fn version(&self) -> i32 {
        0
    }

    async fn up(&self, tx: &mut Transaction<'_, Postgres>) -> Result<(), Box<dyn std::error::Error>> {
        sqlx::query(
            r#"
            create table if not exists users (
                id uuid primary key,
                name varchar not null,
                email varchar not null,
                created_at timestamptz not null
            )
            "#,
        )
        .execute(&mut **tx)
        .await?;

        sqlx::query(
            r#"
            create table if not exists applications (
                id uuid primary key,
                user_id uuid references users(id),
                name varchar not null,
                token uuid not null,
                created_at timestamptz not null
            )
            "#,
        )
        .execute(&mut **tx)
        .await?;

        Ok(())
    }
}
