FROM gcc:13-bookworm AS builder
WORKDIR /app
COPY main.cpp .
RUN g++ -O3 -static -o server main.cpp -lpthread

FROM debian:bookworm-slim
WORKDIR /app
COPY --from=builder /app/server .
EXPOSE 5000
CMD ["./server"]
