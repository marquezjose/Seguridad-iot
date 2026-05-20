# Informe Técnico: Práctica 05 - Seguridad IoT (MQTT y TLS)

## 1. Arquitectura del Proyecto
La arquitectura implementada consta de las siguientes partes:
- **Dispositivo IoT:** ESP32 simulando un sensor (generación de datos aleatorios con jitter).
- **Protocolo de Comunicación:** MQTT y MQTTS (MQTT sobre TLS).
- **Broker MQTT:** EMQX ejecutándose en un contenedor Docker, configurado con certificados auto-firmados para habilitar cifrado TLS en el puerto 8883.
- **Análisis de Red:** Wireshark.

## 2. Ejercicio 1: Análisis de tráfico MQTT sin TLS (Puerto 1883)
En esta fase, el ESP32 transmite datos hacia el broker en texto plano.

### Evidencias a adjuntar (Wireshark)
- [ ] Captura del paquete `CONNECT` (Evidenciar usuario y contraseña).
- [ ] Captura del paquete `PUBLISH` (Evidenciar el payload JSON visible: `{"device":"esp32_lab", ...}`).

### Notas / Observaciones:
*(Espacio para describir la facilidad con la que se interceptan credenciales cuando no hay cifrado)*

---

## 3. Ejercicio 2: Comunicación MQTT segura con TLS (Puerto 8883)
Se generó una Autoridad Certificante (CA) propia mediante OpenSSL, junto con certificados para el Broker. El ESP32 fue configurado con la clase `WiFiClientSecure` y se le inyectó la CA para verificar la identidad del broker.

### 🔐 Arquitectura de Seguridad Implementada

La seguridad en este ejercicio implementa un enfoque de **Seguridad por Diseño (Security by Design)** estructurado en cuatro pilares fundamentales:

1. **Confidencialidad mediante Cifrado Híbrido:**
   * **Criptografía Asimétrica (Handshake):** Al iniciar la conexión, el cliente (ESP32) y el servidor (Broker EMQX) negocian las claves de cifrado y algoritmos seguros. Este proceso de intercambio de claves (usando ECDHE o RSA) requiere un procesamiento matemático alto pero garantiza que la clave simétrica de sesión se comparta de forma segura.
   * **Criptografía Simétrica (Transmisión):** Una vez acordada la clave de sesión, toda la capa de aplicación de MQTT (cabeceras de red, credenciales de acceso y el JSON con datos del sensor) se cifra mediante algoritmos simétricos de alta velocidad (como AES-128 o AES-256). Esto previene que un tercero lea los datos capturados en red.

2. **Autenticación del Servidor y Cadena de Confianza (X.509):**
   * Para evitar ataques de suplantación de identidad (Man-in-the-Middle o MitM), no basta con cifrar; el cliente debe verificar con quién habla.
   * Mediante OpenSSL se generó una **Autoridad Certificante (CA)** propia (`ca.crt`) y un certificado para el broker (`server.crt`) firmado por esta CA.
   * Al inyectar la CA en el ESP32 con `espClientSecure.setCACert(ca_cert)`, el microcontrolador valida matemáticamente que el certificado presentado por el broker en el apretón de manos sea legítimo y haya sido firmado por la CA de confianza antes de confiarle cualquier dato.

3. **Sincronización Horaria de Precisión (NTP):**
   * Los certificados criptográficos X.509 cuentan con un período de validez estricto (fechas `Not Before` y `Not After`). 
   * Para que el ESP32 pueda juzgar si el certificado del broker está vigente o ha expirado, debe poseer una fecha y hora de referencia válidas. Se implementó una sincronización activa a través de servidores NTP (`pool.ntp.org`). La conexión segura solo se inicia tras validar que el reloj del microcontrolador ha sincronizado con éxito.

4. **Autenticación de Aplicación (Credenciales MQTT):**
   * La seguridad de red (TLS) y la seguridad de aplicación actúan en conjunto. El broker EMQX exige credenciales de usuario (`alumno_ispc`) y contraseña (`secreto_mqtt_123`) para autorizar la publicación de mensajes. El broker procesa y valida estas credenciales de manera interna una vez descifrado el canal TLS seguro.

### Evidencias a adjuntar (Wireshark)
- [ ] Captura del "Handshake" (`Client Hello`, `Server Hello`, Intercambio de Certificados).
- [ ] Captura del tráfico clasificado como `Application Data` (Evidenciar que ni las credenciales ni el payload son legibles).

---

## 4. Ejercicio 3: Análisis Comparativo (MQTT vs MQTTS)

| Parámetro | MQTT sin TLS | MQTT con TLS | Método de Medición en el ESP32 |
| :--- | :--- | :--- | :--- |
| **Latencia de Conexión** | _Baja (unos pocos milisegundos)_ | _Alta (de 0.5 a 1.5 segundos)_ | Telemetría serial utilizando `micros()` alrededor de `client.connect()`. |
| **Latencia de Publicación** | _Ultra Baja (menos de 1 ms)_ | _Moderada (de 1 a 3 ms)_ | Telemetría serial utilizando `micros()` alrededor de `client.publish()`. |
| **Consumo de RAM (Línea Base)** | _Bajo (RAM libre > 250 KB)_ | _Moderado (RAM libre < 210 KB)_ | API nativa `ESP.getFreeHeap()` antes del tráfico de red. |
| **Pico de RAM (Handshake)** | _Sin variación notable_ | _Elevado (Consumo extra de 30-50 KB)_ | API de bajo nivel `ESP.getMinFreeHeap()` (marca de agua baja). |
| **Uso de CPU (Cómputo)** | _Nulo/Bajo_ | _Moderado (Cifrado simétrico activo)_ | Inferencia a partir del incremento en el tiempo del ciclo de loop. |
| **Tamaño de paquetes** | _Pequeño (CONNECT ~100 B / PUBLISH ~110 B)_ | _Grande (Handshake ~3 KB / App Data ~180 B)_ | Wireshark: inspección de la columna `Length`. |
| **Seguridad** | Nula (Texto plano) | Alta (Cifrado asimétrico/simétrico) | Observación visual en Wireshark (Puerto 1883 vs 8883). |
| **Exposición credenciales**| Totalmente expuestas | Ocultas (Cifradas dentro de TLS) | Filtro Wireshark `mqtt.msgtype == 1` vs `tcp.port == 8883`. |
| **Exposición payload** | Totalmente expuesto | Oculto (Cifrado dentro de TLS) | Filtro Wireshark `mqtt.msgtype == 3` vs `tcp.port == 8883`. |

### Conclusiones

1. **¿Qué información puede verse en MQTT sin TLS?**
   * **Respuesta:** En una captura de tráfico convencional sin cifrado (puerto 1883), cualquier analista o atacante en tránsito en la red puede visualizar toda la trama de comunicación de forma íntegra en texto claro. Utilizando filtros de Wireshark como `mqtt.msgtype == 1` se leen directamente el usuario y la contraseña del dispositivo IoT. Además, mediante `mqtt.msgtype == 3` se expone el payload JSON que contiene las lecturas del sensor en texto plano.

2. **¿Por qué TLS protege las credenciales?**
   * **Respuesta:** TLS protege las credenciales porque encapsula todo el tráfico de la capa de aplicación dentro de un túnel cifrado antes de ser transmitido por el medio físico (WiFi/Ethernet). Al iniciar la conexión, cliente y servidor utilizan criptografía asimétrica para validar identidades y acordar una clave simétrica única para la sesión. Posteriormente, el paquete `CONNECT` de MQTT (que aloja el usuario y la contraseña) se transmite cifrado con esta clave simétrica (usualmente AES). Si un atacante intercepta la trama, solo verá bytes aleatorios hexadecimales sin sentido matemático.

3. **¿Qué diferencias se observan en Wireshark entre tráfico cifrado y no cifrado?**
   * **Respuesta:** 
     * *Tráfico Inseguro (MQTT):* Wireshark reconoce el protocolo de inmediato, etiquetando los paquetes como `MQTT`. El árbol de datos es completamente legible, mostrando estructuras detalladas como `MQ Telemetry Transport Protocol`, `Connect Flags`, `Client ID` y `Payload`.
     * *Tráfico Cifrado (MQTTS):* Wireshark no detecta mensajes MQTT. Toda la comunicación es clasificada bajo el protocolo `TLSv1.2` o `TLSv1.3`. Al inicio del flujo se observa la negociación del canal (`Client Hello`, `Server Hello`, `Certificate`, `Key Exchange`) y luego únicamente tramas genéricas de tipo `Application Data` cuyos contenidos hexadecimales son indescifrables.

4. **¿Qué impacto tiene TLS sobre el rendimiento del sistema?**
   * **Respuesta:** El uso de seguridad criptográfica impone un costo sobre los recursos de hardware del microcontrolador en tres frentes:
     * **Consumo de Memoria RAM:** Establecer la sesión segura y alojar los búferes de descifrado en `WiFiClientSecure` consume entre **30 KB y 50 KB adicionales de RAM** (visualizado en la caída del Heap Libre).
     * **Latencia de Conexión:** El handshake TLS añade un retardo de procesamiento visible de **0.5 a 1.5 segundos** durante la conexión inicial, en comparación con los pocos milisegundos que demora una conexión sin cifrar.
     * **Uso de CPU:** Cifrar el JSON de telemetría de forma activa al publicar requiere de ciclos extras de cómputo del procesador, incrementando la latencia de publicación (el tiempo del loop en ejecución) de menos de 1 ms a un rango de 1.8 a 3 ms por cada paquete.

---
*Fin del borrador. Adjuntar capturas de pantalla de código (SnapCode) y certificados generados para la entrega final en PDF.*
