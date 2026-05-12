FROM gradle:9.2-jdk25-alpine AS build
WORKDIR /workspace
COPY --chown=gradle:gradle . .
RUN gradle bootJar --no-daemon

FROM eclipse-temurin:25-jre-alpine
WORKDIR /app

RUN apk add --no-cache curl

RUN addgroup -S spring && adduser -S spring -G spring
USER spring:spring

COPY --from=build /workspace/build/libs/application.jar ./app.jar

ENV SPRING_PROFILES_ACTIVE=docker

EXPOSE 8090

HEALTHCHECK --interval=30s --timeout=5s --start-period=60s --retries=3 \
  CMD curl -fsS http://127.0.0.1:8090/ || exit 1

ENTRYPOINT ["java", "-jar", "app.jar"]
