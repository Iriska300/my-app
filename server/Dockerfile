FROM gcc:13-bookworm AS builder
WORKDIR /app
COPY main.cpp .
RUN g++ -O3 -static -o server main.cpp -lpthread

FROM alpine:3.20
RUN addgroup -S app && adduser -S -G app app
WORKDIR /app
COPY --from=builder /app/server ./server
RUN chown app:app /app/server
USER app
EXPOSE 5000
HEALTHCHECK --interval=30s --timeout=3s \
  CMD wget -q -O /dev/null http://127.0.0.1:5000/healthz || exit 1
CMD ["./server"]
