FROM rust:1.76.0 as builder
WORKDIR /opt
COPY . .
RUN cargo build --release

FROM debian:12-slim
COPY --from=builder /opt/target/release/dayz_proxy /usr/local/bin/dayz_proxy
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        openssl \
        ca-certificates && \
    apt-get clean
EXPOSE 80
ENV RUST_LOG="dayz_proxy=info"
CMD ["dayz_proxy"]