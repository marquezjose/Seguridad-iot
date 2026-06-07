# 🔐 Práctica 05: Seguridad IoT - Análisis de MQTT y TLS

Este repositorio contiene la resolución del trabajo práctico número 5 sobre protocolos seguros en IoT. El objetivo principal es implementar y comparar una arquitectura de comunicación IoT utilizando el protocolo MQTT, evidenciando las vulnerabilidades de la transmisión en texto plano y mitigándolas mediante la implementación de seguridad **TLS**.

## 🏗️ Arquitectura del Proyecto

El sistema está compuesto por las siguientes piezas fundamentales:
- **Broker MQTT:** EMQX ejecutándose en un contenedor Docker, configurado para soportar conexiones tanto en texto plano (puerto 1883) como cifradas (puerto 8883).
- **Cliente IoT:** Microcontrolador ESP32 programado en C++ (PlatformIO) encargado de simular un sensor, generando datos aleatorios de temperatura con un *jitter* (retraso asíncrono) específico y publicarlos en el broker.
- **Análisis de Red:** Uso de Wireshark para la captura e inspección de paquetes TCP/IP, validación de exposición de credenciales y comprobación del túnel TLS.

## 📂 Estructura del Repositorio

- `docker/`: Contiene el archivo `docker-compose.yml` para levantar la infraestructura del broker EMQX y los certificados locales (CA, server) generados para TLS.
- `firmware_ej1_sintls/`: Código base para el **Ejercicio 1**. El ESP32 se conecta al puerto 1883 enviando credenciales y payload de manera totalmente expuesta (texto plano).
- `firmware_ej2_contls/`: Código para el **Ejercicio 2**. *(En preparación)*. Implementará `WiFiClientSecure` inyectando el certificado CA para establecer una conexión segura al puerto 8883.
- `docs/`: Contiene la plantilla base (`Informe_Practico5.md`) utilizada para volcar la investigación, hallazgos y evidencias gráficas requeridas por el profesor.
- `capturas/`: Directorio destinado a almacenar los archivos de tráfico de red `.pcapng` generados por Wireshark y capturas de pantalla de la ejecución.

## 🚀 Requisitos Previos

Para clonar y ejecutar este proyecto en otra computadora, necesitarás:
1. [Docker](https://www.docker.com/) y Docker Compose instalados.
2. [Visual Studio Code](https://code.visualstudio.com/) con la extensión de **PlatformIO**.
3. [Wireshark](https://www.wireshark.org/) para el análisis de paquetes.

## ⚙️ Configuración Inicial (IMPORTANTE)

Por buenas prácticas de ciberseguridad, las contraseñas reales y credenciales de red no se suben al repositorio. **Al clonar el proyecto en tu otra PC, deberás recrear el archivo de configuración:**

1. Navega hasta el directorio `firmware_ej1_sintls/include/`.
2. Crea un nuevo archivo llamado `secrets.h`.
3. Pega la siguiente estructura y completa con los datos de tu nueva PC/Red:

```cpp
#ifndef SECRETS_H
#define SECRETS_H

// Configuración de WiFi
const char* WIFI_SSID = "TU_RED_WIFI";
const char* WIFI_PASSWORD = "TU_CONTRASEÑA";

// IP de la computadora que está ejecutando el contenedor Docker
const char* MQTT_BROKER = "192.168.X.X"; 
const int MQTT_PORT = 1883; 

// Credenciales configuradas en el Broker EMQX
const char* MQTT_USER = "alumno_ispc";
const char* MQTT_PASSWORD = "secreto_mqtt_123";

#endif
```
*(Nota: Repetir el mismo paso para la carpeta `firmware_ej2_contls` cuando se trabaje en ella).*

## ▶️ Uso del Servidor MQTT

Para levantar el broker con toda su configuración de puertos y certificados lista para la práctica, abre una terminal en la carpeta `/docker` y ejecuta:

```bash
docker compose up -d
```

Para verificar que el broker está funcionando, puedes acceder a su **Dashboard Web** ingresando a `http://localhost:18083` desde el navegador de tu computadora.
