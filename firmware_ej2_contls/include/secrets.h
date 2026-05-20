#ifndef SECRETS_H
#define SECRETS_H

// Configuración de WiFi
const char* WIFI_SSID = "TU_WIFI_SSID";
const char* WIFI_PASSWORD = "TU_WIFI_PASSWORD";

// Configuración del Broker MQTT
// Reemplaza con la IP de tu computadora (donde corre Docker)
const char* MQTT_BROKER = "192.168.X.X"; 
const int MQTT_PORT = 8883; // Puerto 8883 para MQTT sobre TLS (MQTTS)

// Credenciales MQTT (Las configuraremos en EMQX luego)
const char* MQTT_USER = "alumno_ispc";
const char* MQTT_PASSWORD = "secreto_mqtt_123";

// Certificado de la CA para verificar al Broker
const char* ca_cert = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDhTCCAm2gAwIBAgIUb5qkfKxSk3XBVaJumNhdimYje5UwDQYJKoZIhvcNAQEL\n" \
"BQAwUjELMAkGA1UEBhMCQVIxEDAOBgNVBAgMB0NvcmRvYmExEDAOBgNVBAcMB0Nv\n" \
"cmRvYmExDTALBgNVBAoMBElTUEMxEDAOBgNVBAMMB0lTUENfQ0EwHhcNMjYwNTEy\n" \
"MjMwMTUxWhcNMjcwNTEyMjMwMTUxWjBSMQswCQYDVQQGEwJBUjEQMA4GA1UECAwH\n" \
"Q29yZG9iYTEQMA4GA1UEBwwHQ29yZG9iYTENMAsGA1UECgwESVNQQzEQMA4GA1UE\n" \
"AwwHSVNQQ19DQTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMAeM6Sp\n" \
"lvKPx70m0JAzU27jwFmrn6W0CUzx5hL7yTBuMfsihp/YhNfLZovbMA7tf+raZYGd\n" \
"nJc1qBqrjRKaceHoIxsucLBQr7xhAvP8rRlksymTm1rTwfvy2nQw9DaWdP//Rqfp\n" \
"yykhKASCMLIJTVNGAuvsO0klcM0aVSJaPTT9Bkp5U2D4OFOmD15heMnMXQzW362A\n" \
"BuFxLbSCvhyMznWyaTupMJX/72qH5t1fmjpPw2JxLv+MbHb/r36S0v1PRT2P4mdN\n" \
"e3sfI2w7DQSJfAK1IgpfQ0lpm1Tn6+fnI0zoO99WX2prRwk9Ki6erjEW9yc5iUmO\n" \
"jm6NUIIYjdyywPUCAwEAAaNTMFEwHQYDVR0OBBYEFA4y+2hwz2t7B6RWNGP8IP7r\n" \
"SIXrMB8GA1UdIwQYMBaAFA4y+2hwz2t7B6RWNGP8IP7rSIXrMA8GA1UdEwEB/wQF\n" \
"MAMBAf8wDQYJKoZIhvcNAQELBQADggEBACuX7ZLdiPC2+V78eomNhQl1CxZAzp4S\n" \
"v4741BD0P1OgkZHt8gSkf7v4zl+hUle/yE2DYha2j6GHnV8TcYObhI3qvQg7ycgK\n" \
"TZVn+H8PN4/Uw2i+4HmQp05ynrTPOtoPV6x3dw1wLvonQksg70bdllZio5h0GgDh\n" \
"pcNBaaof7ALNA2rQD/GToyg/QtX1mJ2WxtRiEZO7yl929mwYLz0COVsweBplqfag\n" \
"P83UKEq9H+oIQ5G3wwZLANCJe/koDhyoXJur7DPZ2uOIfIvlpLInwF7Jb7E6I+4f\n" \
"DjeAiZ/h9J+7m9Cgnp+I66eZfZ3sfO+O4Noc/3KYNUlALtZc+Z+18rw=\n" \
"-----END CERTIFICATE-----";

#endif
