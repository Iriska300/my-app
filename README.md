# CI/CD Pipeline: Client & Server (k3s + Jenkins + Helm)

Проект представляет собой автоматизированный CI/CD-конвейер для развертывания двух микросервисов в локальном кластере Kubernetes (k3s) с использованием Jenkins и Helm.

## Ссылки
* Реестр образов сервера (Docker Hub): https://hub.docker.com/r/iriska2604/server
* Реестр образов клиента (Docker Hub): https://hub.docker.com/r/iriska2604/client

## Архитектура
* Client: Веб-форма на Nginx. Отправляет запросы на /api/ и проксирует их на сервер.
* Server: HTTP API на C++. Возвращает JSON со статусом.
* Инфраструктура: Jenkins, Docker, k3s (Kubernetes), Helm, Docker Hub.

## Как это работает
1. Разработчик делает git push в репозиторий.
2. Jenkins (через Poll SCM) замечает изменения и запускает декларативный пайплайн.
3. Параллельно собираются Docker-образы клиента и сервера.
4. Образы тегируются уникальным номером сборки и хэшем коммита (например, 14-130445a).
5. Образы пушатся в Docker Hub.
6. Выполняется helm upgrade с передачей нового тега через --set image.tag=...
7. Kubernetes выполняет Rolling Update (обновление без простоя), поднимая новые поды и только потом убивая старые.

## Локальный запуск
Для доступа к клиенту из Windows (WSL):
1. Узнать IP-адрес WSL: hostname -I
2. Открыть в браузере: http://<IP_АДРЕС>:30080

## Структура проекта
* client/ — исходный код и Dockerfile для клиента (Nginx).
* server/ — исходный код на C++ и Dockerfile для сервера (multi-stage build).
* helm/ — Helm-чарты для клиента и сервера.
* Jenkinsfile — декларативный пайплайн CI/CD.
