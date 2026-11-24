FROM nginx:1.27-alpine

RUN rm -f /etc/nginx/conf.d/default.conf \
 && mkdir -p /var/cache/nginx /var/run /run \
 && chown -R nginx:nginx /var/cache/nginx /var/run /run /var/log/nginx /etc/nginx/conf.d \
 && sed -i 's|^pid .*|pid /tmp/nginx.pid;|' /etc/nginx/nginx.conf

COPY --chown=nginx:nginx nginx.conf    /etc/nginx/conf.d/default.conf
COPY --chown=nginx:nginx index.html    /usr/share/nginx/html/index.html

USER nginx
EXPOSE 80

HEALTHCHECK --interval=30s --timeout=3s \
  CMD wget -q -O /dev/null http://127.0.0.1/ || exit 1

CMD ["nginx", "-g", "daemon off;"]
