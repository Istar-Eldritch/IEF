/*!
  Management of the system migrations.
*/

mod m000_initial_tables;
mod m001_player_table;
use async_trait::async_trait;

use chrono::Utc;
use log::info;
use sqlx::{Transaction, PgPool, Postgres};

use m000_initial_tables::InitialTables;
use m001_player_table::PlayersTable;
/// This trait is used by `run_migrations` to run the pending migrations and
/// it must be implemented by all new migrations.
#[async_trait]
trait Migration {
    fn version(&self) -> i32;

    async fn up(
        & self,
        ex: &mut Transaction<'_, Postgres>,
    ) -> Result<(), Box<dyn std::error::Error>>;
}


/// Adds a new migration version to the migration table.
async fn add_migration(
    tx: &mut Transaction<'_, Postgres>,
    version: i32,
) -> Result<(), sqlx::Error> {
    sqlx::query(
        r#"
            insert into migrations (version, created_at)
            values ($1, $2)
        "#,
    )
    .bind(version)
    .bind(Utc::now())
    .execute(&mut **tx)
    .await
    .map(|_| ())
}

/// This contains all the migrations that the system will check and run on startup
/// New migrations must be added to this array for them to take effect.
/// Always add new migrations to the tail of the array, `run_migrations` does not currently sort this array before exiting the migrations
const MIGRATIONS: [&dyn Migration; 2] = [&InitialTables, &PlayersTable];

/// Runs all pending migrations.
///
/// The function follows this steps:
/// - Initialise the migration table if it doesnt exist already
/// - Get the latest version of the migration schema.
/// - Scan trough MIGRATIONS and execute those that have a higher version than the existing db version
///
///  TODO: Write integration tests
pub async fn run_migrations(pool: PgPool) -> Result<(), Box<dyn std::error::Error>> {
    sqlx::query(
        r#"
            create table if not exists migrations (
                version integer primary key,
                created_at timestamptz not null
            )
        "#,
    )
    .execute(&pool)
    .await?;

    // Get DB version, defaulting to -1 if no migrations have run
    let db_version = sqlx::query_as(
        r#"
                select version from migrations
                order by version desc limit 1
            "#,
    )
    .fetch_optional(&pool)
    .await?
    .map(|(v,)| {
        info!("DB running schema v{}", v);
        v
    })
    .unwrap_or_else(|| {
        info!("DB not initialized");
        -1
    });

    for migration in MIGRATIONS.iter() {
        if migration.version() > db_version {
            info!("Running migration {}", migration.version());
            let mut tx = pool.begin().await?;
            add_migration(&mut tx, migration.version()).await?;
            migration.up(&mut tx).await?;
            tx.commit().await?;
        }
    }

    Ok(())
}
