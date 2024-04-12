
use std::str::FromStr;

use actix_web::{
    http::{header::ContentType, StatusCode},
    post, web, HttpResponse,
};
use derive_more::{Display, Error};
use serde::{Deserialize, Serialize};
use serenity::all::GuildId;
use uuid::Uuid;

use crate::api::AppState;
use crate::data::{Application, Player};

use chrono::Utc;


#[derive(Deserialize)]
struct PlayerRolesRequest {
    token: Uuid,
    steam_id: String,
    player_name: String,
}

#[derive(Serialize)]
#[serde(untagged)]
#[repr(u8)]
enum PlayerRolesErrorCodes {
    NonExistingDiscordUser
}

#[derive(Serialize)]
struct PlayerInfo {
    verified: bool,
    discord_name: String,
    roles: Vec<String>,
}

#[derive(Serialize)]
struct PlayerRolesResponse {
    ok: bool,
    error_code: Option<PlayerRolesErrorCodes>,
    player_info: Option<PlayerInfo>
}

#[derive(Debug, Display, Error)]
enum PlayerRolesError {
    #[display(fmt = "internal error")]
    InternalError,

    #[display(fmt = "Unauthorized")]
    Unauthorized,
}

impl actix_web::error::ResponseError for PlayerRolesError {
    fn error_response(&self) -> HttpResponse {
        HttpResponse::build(self.status_code())
            .insert_header(ContentType::plaintext())
            .body(self.to_string())
    }

    fn status_code(&self) -> StatusCode {
        match *self {
            PlayerRolesError::InternalError => StatusCode::INTERNAL_SERVER_ERROR,
            PlayerRolesError::Unauthorized => StatusCode::UNAUTHORIZED,
        }
    }
}

#[post("/discord/get-player-roles")]
pub async fn get_player_roles_handler(
    data: web::Data<AppState>,
    conn_info: actix_web::dev::ConnectionInfo,
    body: web::Json<PlayerRolesRequest>,
) -> Result<HttpResponse, PlayerRolesError> {
    let server_ip: String = conn_info.realip_remote_addr().unwrap_or_default().into();
    log::debug!("Receiving request on /discord/get-player-roles");
    let app = Application::find_by_token(&data.pg, body.token)
        .await
        .map_err(|err| {
            log::error!("Error finding application by token: {:?}", err);
            PlayerRolesError::InternalError
        })?;

    if let Some(app) = app {
        if let Some(guild_id) = app.discord_guild_id {
            let guild = data.discord_client.http.get_guild(GuildId::from_str(&guild_id).unwrap()).await.map_err(|err| {
                log::error!("Error getting guild info from discord: {:?}", err);
                PlayerRolesError::InternalError
            })?;
    
            let members = guild.search_members(&data.discord_client.http, &body.player_name, Some(1)).await.map_err(|err| {
                log::error!("Error getting member information from discord: {:?}", err);
                PlayerRolesError::InternalError
            })?;

            if let Some(member) = members.first() {
                let player = Player::find_by_ids(&data.pg, &body.steam_id, &member.user.id.to_string()).await.map_err(|err| {
                    log::error!("Error getting player from db: {:?}", err);
                    PlayerRolesError::InternalError
                })?;
                let roles: Vec<String> = if let Some(roles) = member.roles(&data.discord_client.cache) {
                    roles.into_iter().map(|r| r.name).collect()
                } else {
                    let mut roles = vec![];
                    for role_id in member.roles.iter() {
                        if let Some(role) = guild.roles.get(role_id) {
                            roles.push(role.name.clone());
                        }
                    }
                    roles
                };
                let verified = if let Some(player) = player {
                    player.verified
                } else {
                    let player = Player {
                        id: uuid::Uuid::new_v4(),
                        discord_id: member.user.id.to_string(),
                        steam_id: body.steam_id.clone(),
                        verified: false,
                        created_at: Utc::now()
                    };
                    player.create_player(&data.pg).await.unwrap_or_else(|err| {
                        // Continue if it cant be persisted but log error
                        log::error!("Error persisting player metadata: {:?}", err);
                    });
                    false                    
                };
                Ok(HttpResponse::build(StatusCode::OK).json(PlayerRolesResponse {
                    ok: true,
                    error_code: Some(PlayerRolesErrorCodes::NonExistingDiscordUser),
                    player_info: Some(PlayerInfo {
                        verified,
                        roles,
                        discord_name: member.nick.clone().unwrap_or_else(|| member.user.name.clone())
                    })
                }))
            } else {
                Ok(HttpResponse::build(StatusCode::OK).json(PlayerRolesResponse {
                    ok: true,
                    error_code: Some(PlayerRolesErrorCodes::NonExistingDiscordUser),
                    player_info: None
                }))
            }
        }
        else {
            log::warn!("Application {} ({}) doesn't have a discord_guild configured", app.app_name, app.app_id);
            Err(PlayerRolesError::InternalError)
        }

    } else {
        log::warn!("Unauthorized access from: {}", server_ip);
        Err(PlayerRolesError::Unauthorized)
    }
}
