## Integrantes
- Arthur Fellipe Estevão da Silva RM553320
- Eduardo Pires Escudero RM556527
- Leonardo Munhoz Prado RM556824

# ESP32 MQTT Project - CP5

## Descrição do Projeto
Este projeto utiliza um microcontrolador ESP32 para monitorar e controlar dispositivos através de um broker MQTT. O ESP32 lê dados de um sensor de temperatura e umidade DHT22, além de um sensor de luminosidade, e publica essas informações em tópicos MQTT. Ele também pode receber comandos para ligar e desligar um LED.

## Estrutura do Projeto
- Código fonte para o ESP32 (`main.ino`)
- Diagrama de conexão
- Instruções para configuração e execução

## Componentes Necessários
- Microcontrolador ESP32
- Sensor DHT22
- Potenciômetro (para simular o sensor de luminosidade)
- LED
- Resistores
- Fios de conexão
- Fonte de alimentação para o ESP32


### Conexões
1. **DHT22**
   - VCC: Conectado ao 3.3V do ESP32
   - GND: Conectado ao GND do ESP32
   - Data: Conectado ao pino GPIO 4 do ESP32

2. **Sensor de Luminosidade (Potenciômetro)**
   - Um terminal conectado ao GND
   - Outro terminal conectado ao 3.3V
   - Terminal central (wiper) conectado ao pino GPIO 34 do ESP32

3. **LED**
   - Anodo (+) conectado ao pino GPIO 2 do ESP32
   - Catodo (-) conectado ao GND, através de um resistor.

## Instruções de Configuração

### 1. Configuração do Ambiente de Desenvolvimento
- Instale o **Arduino IDE** ou **PlatformIO**.
- Instale as bibliotecas necessárias:
  - `DHT sensor library`
  - `PubSubClient`

### 2. Configuração do Código
- Abra o código-fonte `main.ino` e ajuste as seguintes configurações:
  ```cpp
  const char* SSID = "Seu_SSID";           // Substitua por sua rede Wi-Fi
  const char* PASSWORD = "Sua_Senha";      // Substitua pela senha da sua rede
  const char* BROKER_MQTT = "IP_do_Broker"; // Endereço IP do broker MQTT
