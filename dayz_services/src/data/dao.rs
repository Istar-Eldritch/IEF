use sqlx::{Executor, FromRow, Postgres};
use uuid::Uuid;

#[derive(FromRow)]
pub struct Application
{
    pub app_id: Uuid,
    pub app_name: String,
    pub user_name: String,
}

impl Application
{
    pub async fn find_by_token<'a>(ex: impl Executor<'a, Database = Postgres>, token: Uuid) -> Result<Option<Application>, sqlx::error::Error> {

        let user = sqlx::query_as(r#"
            select applications.id as app_id,
                applications.name as app_name,
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