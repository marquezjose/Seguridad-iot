# Seguridad IoT - Tecnicatura Superior en Telecomunicaciones (ISPC)

Repositorio de prácticos de la materia **Seguridad IoT** correspondiente a la Tecnicatura Superior en Telecomunicaciones del Instituto Superior Politécnico de Córdoba (ISPC).

## Objetivo General

Comprender y aplicar conceptos de seguridad en sistemas IoT, implementando protocolos de comunicación seguros (TLS/SSL), analizando vulnerabilidades de red y desarrollando habilidades de pentesting para dispositivos conectados.

## Estructura del Repositorio

```
Seguridad-iot/
├── Practico-clase5-protocolos-seguros/    # Seguridad MQTT y TLS
│   ├── docker/                           # Infraestructura EMQX
│   ├── firmware_ej1_sintls/              # ESP32 sin cifrado TLS
│   ├── firmware_ej2_contls/              # ESP32 con cifrado TLS
│   ├── docs/                             # Documentación técnica
│   ├── capturas/                         # Capturas de red Wireshark
│   └── A-investigacion/                  # Material de investigación
├── Practico-clase7-pentesting-kali-linux/ # Pentesting con Kali Linux
│   └── capturas/                         # Evidencias de práctica
└── README.md                             # Este archivo
```

---

## Práctica 05: Protocolos Seguros en IoT (MQTT vs MQTTS)

### Descripción

Implementación y comparación de una arquitectura de comunicación IoT utilizando el protocolo MQTT, evidenciando las vulnerabilidades de la transmisión en texto plano y mitigándolas mediante la implementación de seguridad **TLS**.

### Arquitectura del Sistema

| Componente | Tecnología | Descripción |
|------------|------------|-------------|
| **Broker MQTT** | EMQX 5.5.0 | Ejecutándose en Docker, soporta conexiones sin TLS (puerto 1883) y con TLS (puerto 8883) |
| **Cliente IoT** | ESP32 | Microcontrolador programado en C++ (PlatformIO) que simula un sensor de temperatura |
| **Análisis de Red** | Wireshark | Captura e inspección de paquetes TCP/IP para validación de seguridad |
| **Certificados** | OpenSSL | PKI local con CA raíz y certificados de servidor X.509 |

### Ejercicios Desarrollados

#### Ejercicio 1: MQTT sin TLS (Puerto 1883)

- **Archivo:** `firmware_ej1_sintls/src/main.cpp`
- **Comportamiento:** El ESP32 se conecta al broker enviando credenciales y payload en texto plano
- **Vulnerabilidad expuesta:** Credenciales (`alumno_ispc`/`mqtt123`) y datos del sensor visibles en la red
- **Análisis Wireshark:** Filtros `mqtt` o `tcp.port == 1883`

#### Ejercicio 2: MQTT con TLS (Puerto 8883)

- **Archivo:** `firmware_ej2_contls/src/main.cpp`
- **Implementación:** Uso de `WiFiClientSecure` con certificado CA incrustado
- **Clase personalizada:** `IPBasedWiFiClientSecure` para validación de certificados por IP
- **Seguridad:** Cifrado TLS 1.2/1.3 con handshake completo
- **Análisis Wireshark:** Filtros `tls` o `tcp.port == 8883`

### Métricas Comparativas

| Parámetro | MQTT sin TLS | MQTT con TLS | Impacto |
|-----------|--------------|--------------|---------|
| **Latencia Conexión** | ~50-80 ms | ~800-1500 ms | Handshake asimétrico |
| **Latencia Publicación** | ~0.5-1 ms | ~2-4 ms | Cifrado simétrico AES |
| **Heap RAM Libre** | ~252 KB | ~215 KB | Buffers TLS (~16KB c/u) |
| **Tamaño Paquetes** | ~100 bytes | ~190 bytes | Headers TLS + MAC |
| **Seguridad** | Nula | Alta | Cifrado completo |
| **Credenciales** | Visibles | Protegidas | Texto plano vs cifrado |

### Infraestructura Docker

**Puertos configurados:**

| Servicio | Puerto | Protocolo |
|----------|--------|-----------|
| MQTT sin TLS | 1883 | mqtt |
| MQTT con TLS | 8883 | mqtts |
| WebSockets | 8083 | ws |
| WebSockets TLS | 8084 | wss |
| Dashboard EMQX | 18083 | HTTP |

**Inicio del broker:**

```bash
cd Practico-clase5-protocolos-seguros/docker
docker compose up -d
```

**Dashboard:** http://localhost:18083

### Certificados TLS

**Infraestructura PKI:**

```
docker/emqx/certs/
├── ca.crt         # Certificado CA (autofirmado) - CN: ISPC_CA
├── ca.key         # Clave privada CA (SECRETO)
├── server.crt     # Certificado servidor EMQX - CN: mqtt.local
└── server.key     # Clave privada servidor (SECRETO)
```

**Características:**
- Algoritmo: RSA 2048 bits con SHA-256
- Validez: 365 días
- SAN: `DNS:mqtt.local, IP:192.168.100.19`

### Configuración Inicial

1. Crear archivo `secrets.h` en `firmware_ej1_sintls/include/`:

```cpp
#ifndef SECRETS_H
#define SECRETS_H

const char* WIFI_SSID = "TU_RED_WIFI";
const char* WIFI_PASSWORD = "TU_CONTRASEÑA";
const char* MQTT_BROKER = "192.168.X.X";
const int MQTT_PORT = 1883;
const char* MQTT_USER = "alumno_ispc";
const char* MQTT_PASSWORD = "secreto_mqtt_123";

#endif
```

2. Repetir para `firmware_ej2_contls/include/`

---

## Práctica 07: Pentesting con Kali Linux

### Descripción

Ejercicios prácticos de hacking ético y pentesting utilizando la distribución Kali Linux, enfocados en la seguridad de dispositivos IoT y análisis de vulnerabilidades.

### Contenido

- **Evidencias de práctica:** Capturas de pantalla del laboratorio
- **Herramientas utilizadas:** Kali Linux, Wireshark, herramientas de análisis de red
- **Archivos de referencia:** `Clase 07.pdf`, `Evidencia 03 - Kali-pentesting.docx`

### Evidencias Visuales

| Descripción | Archivo |
|-------------|---------|
| Instalación de Kali Linux en VMware | `01-VMWare-con-kali-instalado.png` |
| Escritorio de Kali Linux | `02-Escritorio-kali-linux-OK.png` |
| Proceso de actualización | `03-kali-update-upgrade-OK.png` |
| Captura tráfico MQTTS | `wireshark-captura-trafico-mqtts.png` |
| Ataque Man-in-the-Middle | `mitm-wireshark-captura-trafico-mqtts1.png` |

---

## Requisitos Previos

### Software

- [Docker](https://www.docker.com/) y Docker Compose
- [Visual Studio Code](https://code.visualstudio.com/) con extensión PlatformIO
- [Wireshark](https://www.wireshark.org/) para análisis de paquetes
- [OpenSSL](https://www.openssl.org/) para generación de certificados

### Hardware

- Microcontrolador ESP32 (o equivalente con WiFi)
- Computadora con acceso a red WiFi
- Entorno de red local para pruebas

### Dependencias del Firmware

```ini
lib_deps = 
    knolleary/PubSubClient@^2.8
    bblanchon/ArduinoJson@^7.0.4
```

---

## Conceptos Clave

### Seguridad en IoT

1. **Confidencialidad:** Los datos viajan cifrados e inaccesibles
2. **Integridad:** Los datos no pueden ser modificados sin detección
3. **Autenticación:** Verificación de identidad entre cliente y servidor

### Protocolos Comparados

| Protocolo | Puerto | Seguridad |
|-----------|--------|-----------|
| HTTP | 80 | No seguro |
| HTTPS | 443 | TLS |
| MQTT | 1883 | No seguro |
| MQTTS | 8883 | TLS |
| WS | 80 | No seguro |
| WSS | 443 | TLS |

### Limitaciones en Dispositivos Embebidos

- **RAM limitada:** TLS puede consumir 20-40 KB solo para handshake
- **CPU restringida:** Operaciones criptográficas costosas
- **Energía:** Mayor consumo en dispositivos a batería
- **Tiempo real:** Handshake puede bloquear tareas críticas

---

## Herramientas de Análisis

### Wireshark

**Filtros útiles:**

```
mqtt                          # Tráfico MQTT
tcp.port == 1883              # MQTT sin TLS
tcp.port == 8883              # MQTT con TLS
mqtt.msgtype == 1             # Paquetes CONNECT
mqtt.msgtype == 3             # Paquetes PUBLISH
tls.record.content_type == 23 # Datos de aplicación TLS
```

### Métricas ESP32

**Monitoreo de memoria:**

```cpp
ESP.getFreeHeap()      // Heap libre actual
ESP.getMinFreeHeap()   // Mínimo heap registrado
ESP.getHeapSize()      // Heap total disponible
```

**Medición de latencia:**

```cpp
unsigned long inicio = micros();
// Operación a medir
unsigned long latencia = micros() - inicio;
```

---

## Ataques Evitados con TLS

1. **Sniffing:** Captura de datos en texto plano
2. **Man-in-the-Middle (MitM):** Interceptación y modificación de tráfico
3. **Robo de credenciales:** Acceso no autorizado a credenciales
4. **Modificación de mensajes:** Alteración de datos en tránsito

---

## Documentación Adicional

### Archivos Incluidos

| Archivo | Descripción |
|---------|-------------|
| `docs/creacioncertificados.md` | Guía completa de creación de certificados TLS |
| `docs/Seguridad_MQTT_TLS.md` | Arquitectura de seguridad MQTT con TLS |
| `A-investigacion/clase5_text.txt` | Contenido teórico de la clase |
| `A-investigacion/Guia_Medicion_Parametros.md` | Procedimientos de medición comparativa |

---

## Tutor y Institución

- **Asignatura:** Seguridad IoT
- **Tecnicatura:** Superior en Telecomunicaciones
- **Instituto:** ISPC (Instituto Superior Politécnico de Córdoba)
- **Tutor:** Mario Gonzalez

---

## Buenas Prácticas

- Nunca usar MQTT sin TLS en producción
- Validar certificados siempre
- No hardcodear credenciales en el código fuente
- Usar puertos seguros (8883 para MQTTS)
- Implementar autenticación adicional
- Sincronizar hora mediante NTP antes de handshakes TLS

---

## Licencia

Proyecto académico - Uso educativo

---

> **Nota:** Este repositorio contiene credenciales de ejemplo para fines educativos. En un entorno de producción, las credenciales deben almacenarse de forma segura y nunca ser commiteadas al repositorio.
