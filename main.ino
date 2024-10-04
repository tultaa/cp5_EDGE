#include <WiFi.h>
#include <PubSubClient.h> 
#include "DHT.h"
#include <Wire.h>
#include <stdio.h>

#define TOPICO_SUBSCRIBE    "/TEF/device022/cmd"       
#define TOPICO_PUBLISH      "/TEF/device022/attrs"      
#define TOPICO_PUBLISH_2    "/TEF/device022/attrs/l"    
#define TOPICO_PUBLISH_3    "/TEF/device022/attrs/h"    
#define TOPICO_PUBLISH_4    "/TEF/device022/attrs/t"    

#define ID_MQTT  "fiware_022"
  
#define DHTTYPE DHT22   
#define DHTPIN 4 
DHT dht(DHTPIN, DHTTYPE);
 
// WIFI
const char* SSID = "Wokwi-GUEST";
const char* PASSWORD = ""; 
  
// MQTT
const char* BROKER_MQTT = "192.186.XX.XX";
int BROKER_PORT = 1883;

int D4 = 2;     


WiFiClient espClient; 
PubSubClient MQTT(espClient); 
char EstadoSaida = '0';  
  
//Prototypes
void initSerial();
void initWiFi();
void initMQTT();
void reconectWiFi(); 
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void VerificaConexoesWiFIEMQTT(void);
void InitOutput(void);
 
void setup() 
{
    //inicializações:
    InitOutput();
    initSerial();
    initWiFi();
    initMQTT();
    dht.begin();
    MQTT.publish(TOPICO_PUBLISH, "s|off");
}
  

void initSerial() 
{
    Serial.begin(115200);
}
 

void initWiFi() 
{
    delay(10);
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(SSID);
    Serial.println("Aguarde");
     
    reconectWiFi();
}
  

void initMQTT() 
{
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);   //informa qual broker e porta deve ser conectado
    MQTT.setCallback(mqtt_callback);            //atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega)
}
  

void mqtt_callback(char* topic, byte* payload, unsigned int length) 
{
    String msg;
     
    //obtem a string do payload recebido
    for(int i = 0; i < length; i++) 
    {
       char c = (char)payload[i];
       msg += c;
    }
    
    Serial.print("- Mensagem recebida: ");
    Serial.println(msg);
  
    if (msg.equals("device022@on|"))
    {
        digitalWrite(D4, HIGH);
        EstadoSaida = '1';
    }
 

    if (msg.equals("device022@off|"))
    {
        digitalWrite(D4, LOW);
        EstadoSaida = '0';
    }
}
  
void reconnectMQTT() 
{
    while (!MQTT.connected()) 
    {
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) 
        {
            Serial.println("Conectado com sucesso ao broker MQTT!");
            MQTT.subscribe(TOPICO_SUBSCRIBE); 
        } 
        else
        {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Havera nova tentatica de conexao em 2s");
            delay(2000);
        }
    }
}
  

void reconectWiFi() 
{

    if (WiFi.status() == WL_CONNECTED)
        return;
         
    WiFi.begin(SSID, PASSWORD); // Conecta na rede WI-FI
     
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(100);
        Serial.print(".");
    }
   
    Serial.println();
    Serial.print("Conectado com sucesso na rede ");
    Serial.print(SSID);
    Serial.println("IP obtido: ");
    Serial.println(WiFi.localIP());
}
 

void VerificaConexoesWiFIEMQTT(void)
{
    if (!MQTT.connected()) 
        reconnectMQTT();
     
     reconectWiFi(); 
}
 

void EnviaEstadoOutputMQTT(void)
{
    if (EstadoSaida == '1')
    {
      MQTT.publish(TOPICO_PUBLISH, "s|on");
      Serial.println("- Led Ligado");
    }
    if (EstadoSaida == '0')
    {
      MQTT.publish(TOPICO_PUBLISH, "s|off");
      Serial.println("- Led Desligado");
    }
    Serial.println("- Estado do LED onboard enviado ao broker!");
    delay(1000);
}
 

void InitOutput(void)
{

    pinMode(D4, OUTPUT);
    digitalWrite(D4, HIGH);
    
    boolean toggle = false;

    for(int i = 0; i <= 10; i++)
    {
        toggle = !toggle;
        digitalWrite(D4,toggle);
        delay(200);           
    }

    digitalWrite(D4, LOW);
}
 
 
//Começo
void loop() 
{   
    const int potPin = 34;
    
    char msgBuffer[4];

    VerificaConexoesWiFIEMQTT();
 

    EnviaEstadoOutputMQTT();

    //luminosidade
    int sensorValue = analogRead(potPin);  
    float voltage = sensorValue * (3.3 / 4096.0); 
    float luminosity = map(sensorValue, 0, 4095, 0, 100); // 
    Serial.print("Luminosidade: ");
    Serial.print(luminosity);
    Serial.println("%");

    //Publicação tópico MQTT Luminosidade
    dtostrf(luminosity, 4, 1, msgBuffer);
    MQTT.publish(TOPICO_PUBLISH_2,msgBuffer);

    // Leitura do sensor DHT11
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    if (isnan(humidity) || isnan(temperature))
    {
      Serial.println(F("Falha leitura do sensor DHT-22!"));
    }

    Serial.print(F("Umidade: "));
    Serial.print(humidity);
    Serial.print(F("% Temperatura: "));
    Serial.print(temperature);
    Serial.println(F("°C "));
    

    dtostrf(humidity, 4, 1, msgBuffer);
    MQTT.publish(TOPICO_PUBLISH_3,msgBuffer);
    

    dtostrf(temperature, 4, 1, msgBuffer);
    MQTT.publish(TOPICO_PUBLISH_4,msgBuffer);
    

    MQTT.loop();
}
