# DayZ Services

This service provides the following functionality:

- Proxy interface
- Discord bot
- Metrics collection endpoint

## Building DayZ Services

You can build dayz services using the [rust toolchain](https://rustup.rs/). Or [docker](https://www.docker.com/products/docker-desktop/)

To build using docker open a terminal on the [dayz_services](../dayz_services/) folder and run:

```sh
docker build . -t dayz-services
```

## Running DayZ Services.

You can run DayZ services as a standalone binary or using docker. Binary usage:

```
Usage: dayz-services.exe [OPTIONS] --postgres-uri <POSTGRES_URI> --influxdb-uri <INFLUXDB_URI> --influxdb-token <INFLUXDB_TOKEN> --discord-token <DISCORD_TOKEN>

Options:
      --postgres-uri <POSTGRES_URI>      [env: POSTGRES_URI=]
      --influxdb-uri <INFLUXDB_URI>      [env: INFLUXDB_URI=]
      --influxdb-token <INFLUXDB_TOKEN>  [env: INFLUXDB_TOKEN=]
      --discord-token <DISCORD_TOKEN>    [env: DISCORD_TOKEN=]
      --log-level <LOG_LEVEL>            [env: LOG_LEVEL=] [default: info]
      --log-filter <LOG_FILTER>          [env: LOG_FILTER=] [default: dayz_services]
  -h, --help                             Print help
  -V, --version                          Print version
```