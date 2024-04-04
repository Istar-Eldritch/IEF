use sqlx::{Executor, FromRow, Postgres};
use uuid::Uuid;

#[derive(FromRow)]
pub struct Application
{
    pub id: Uuid,
}

impl Application
{
    pub async fn find_by_token<'a>(ex: impl Executor<'a, Database = Postgres>, token: Uuid) -> Result<Option<Application>, sqlx::error::Error> {

        let user = sqlx::query_as(r#"
            select id
            from applications
            where token = $1
        "#)
        .bind(token)
        .fetch_optional(ex).await?;

        Ok(user)
    }
}