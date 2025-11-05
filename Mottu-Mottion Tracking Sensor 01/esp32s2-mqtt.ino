#include <WiFi.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

const char *SSID = "Wokwi-GUEST";
const char *PASSWORD = "";

const char *BROKER_MQTT = "broker.hivemq.com";
const int BROKER_PORT = 1883;

// --- INFORMAÇÕES ÚNICAS DO SENSOR ---
const char* ID_MQTT = "esp32_moto_45124";
const char* ID_SENSOR = "01111";
const char* ID_MOTO = "45124";
const char* SETOR_INICIAL = "Agendada para manutenção";
// -------------------------------------

// Tópicos MQTT unificados para todos os sensores
const char *TOPIC_SUBSCRIBE = "iot/mottu-mottion/comandos";
const char *TOPIC_PUBLISH = "iot/mottu-mottion/status";

#define PIN_LED_VERMELHO      15
#define PIN_LED_AZUL          2
#define PIN_LED_VERDEC        4
#define PIN_LED_VERDEESCURO   5
#define PIN_LED_AMARELO       18
#define PIN_LED_CINZA         19
#define PIN_LED_ROXO          21
#define PIN_LED_BORDO         22
#define PUBLISH_DELAY 5000

WiFiClient espClient;
PubSubClient MQTT(espClient);
unsigned long publishUpdate = 0;
const int TAMANHO = 256;
String setor_atual = SETOR_INICIAL;

void publishStatus() {
    StaticJsonDocument<TAMANHO> doc;
    doc["id_sensor"] = ID_SENSOR;
    doc["id_moto"] = ID_MOTO;
    doc["setor"] = setor_atual;
    doc["timestamp"] = millis();
    doc["estado"] = "Parada"; 
    char buffer[TAMANHO];
    serializeJson(doc, buffer);
    MQTT.publish(TOPIC_PUBLISH, buffer);
    Serial.println("========== DADOS PUBLICADOS ==========");
    Serial.print("Topico: ");
    Serial.println(TOPIC_PUBLISH);
    Serial.println(buffer);
    Serial.println("======================================");
}

void initWiFi() {
    Serial.print("Conectando com a rede: "); Serial.println(SSID);
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) { delay(100); Serial.print("."); }
    Serial.println(); Serial.print("Conectado: "); Serial.println(WiFi.localIP());
}

void initMQTT() {
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);
    MQTT.setCallback(callbackMQTT);
}

void callbackMQTT(char *topic, byte *payload, unsigned int length) {
    String msg = String((char*)payload).substring(0, length);
    StaticJsonDocument<TAMANHO> json;
    DeserializationError error = deserializeJson(json, msg);
    if (error) { return; }
    
    // Verifica se a mensagem é para esta moto
    if (json.containsKey("id_moto") && json["id_moto"] == String(ID_MOTO)) {
      if (json.containsKey("cor_led")) {
          String cor = json["cor_led"].as<String>();
          
          digitalWrite(PIN_LED_VERMELHO, LOW); digitalWrite(PIN_LED_AZUL, LOW);
          digitalWrite(PIN_LED_VERDEC, LOW); digitalWrite(PIN_LED_VERDEESCURO, LOW);
          digitalWrite(PIN_LED_AMARELO, LOW); digitalWrite(PIN_LED_CINZA, LOW);
          digitalWrite(PIN_LED_ROXO, LOW); digitalWrite(PIN_LED_BORDO, LOW);

          if (cor == "vermelho") { 
            digitalWrite(PIN_LED_VERMELHO, HIGH); 
            setor_atual = "Danos Estruturais"; 
          } else if (cor == "azul") { 
            digitalWrite(PIN_LED_AZUL, HIGH); 
            setor_atual = "Reparos Simples"; 
          } else if (cor == "verdec") { 
            digitalWrite(PIN_LED_VERDEC, HIGH); 
            setor_atual = "Minha Mottu (Verde Claro)"; 
          } else if (cor == "verdeescuro") { 
            digitalWrite(PIN_LED_VERDEESCURO, HIGH); 
            setor_atual = "Pronta para aluguel"; 
          } else if (cor == "amarelo") { 
            digitalWrite(PIN_LED_AMARELO, HIGH); 
            setor_atual = "Pendência"; 
          } else if (cor == "cinza") { 
            digitalWrite(PIN_LED_CINZA, HIGH); 
            setor_atual = "Agendada para manutenção"; 
          } else if (cor == "roxo") { 
            digitalWrite(PIN_LED_ROXO, HIGH); 
            setor_atual = "Sem Placa"; 
          } else if (cor == "bordo") { 
            digitalWrite(PIN_LED_BORDO, HIGH); 
            setor_atual = "Motor com defeito"; 
          } else { 
            setor_atual = "Desconhecido"; 
          }
          
          publishStatus();
      }
    }
}

void reconnectMQTT() {
    while (!MQTT.connected()) {
        Serial.print("Conectando ao broker MQTT: "); Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) {
            Serial.println("Conectado ao broker MQTT!");
            MQTT.subscribe(TOPIC_SUBSCRIBE);
        } else {
            Serial.println("Falha. Tentando novamente em 2 segundos."); delay(2000);
        }
    }
}

void checkWiFIAndMQTT() {
    if (WiFi.status() != WL_CONNECTED) { WiFi.begin(SSID, PASSWORD); while (WiFi.status() != WL_CONNECTED) { delay(100); } }
    if (!MQTT.connected()) { reconnectMQTT(); }
}

void setup() {
    Serial.begin(115200);
    pinMode(PIN_LED_VERMELHO, OUTPUT); pinMode(PIN_LED_AZUL, OUTPUT);
    pinMode(PIN_LED_VERDEC, OUTPUT); pinMode(PIN_LED_VERDEESCURO, OUTPUT);
    pinMode(PIN_LED_AMARELO, OUTPUT); pinMode(PIN_LED_CINZA, OUTPUT);
    pinMode(PIN_LED_ROXO, OUTPUT); pinMode(PIN_LED_BORDO, OUTPUT);
    digitalWrite(PIN_LED_VERMELHO, LOW); digitalWrite(PIN_LED_AZUL, LOW);
    digitalWrite(PIN_LED_VERDEC, LOW); digitalWrite(PIN_LED_VERDEESCURO, LOW);
    digitalWrite(PIN_LED_AMARELO, LOW); digitalWrite(PIN_LED_CINZA, LOW);
    digitalWrite(PIN_LED_ROXO, LOW); digitalWrite(PIN_LED_BORDO, LOW);
    initWiFi();
    initMQTT();
    publishStatus();
}

void loop() {
    checkWiFIAndMQTT();
    MQTT.loop();
    if ((millis() - publishUpdate) >= PUBLISH_DELAY) {
        publishUpdate = millis();
        publishStatus();
    }
}