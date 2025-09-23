# Em parceria com a Mottu, apresentamos o Mottu Mottion

**Projeto de rastreamento e gestão de motos em pátios da Mottu utilizando IoT, Node-RED e MySQL.**


## 1. Descrição

O **Mottu Mottion** surge como uma solução tecnológica para os desafios enfrentados pela **Mottu** na gestão física de seus pátios de motos. Com mais de 100 filiais pelo Brasil, cada unidade possui layouts distintos e fluxos operacionais próprios.

O problema central detectado é a **falta de rastreabilidade e padronização**, resultando em:

* Dificuldade em localizar motos rapidamente;
* Falta de registro preciso de manutenções e reparos;
* Riscos de extravios, furtos ou movimentação indevida de veículos.

O Mottu Mottion digitaliza e automatiza o controle de motos com:

* Sensores ESP32 conectados por Wi-Fi;
* Comunicação via MQTT;
* Controle de status por LEDs coloridos;
* Dashboards em Node-RED;
* Armazenamento direto em MySQL.

## 2. Objetivos

* Automatizar o controle de motos no pátio;
* Padronizar processos entre todas as filiais;
* Monitorar setores em tempo real;
* Armazenar histórico de movimentações para análise;
* Disponibilizar interface visual intuitiva via dashboards Node-RED.

## 3. Arquitetura do Sistema

### 3.1 Camadas

1. **Hardware/IoT**

   * Sensores ESP32 por setor;
   * LEDs indicadores de status;
   * Comunicação via Wi-Fi e MQTT.

2. **Banco de Dados (MySQL)**

   * Registra movimentações, motos, setores, funcionários e clientes;
   * Tabelas principais: `dados_moto_sensor`, `motos`, `patios`, `usuarios`, `funcionarios`, `vagas`.

3. **Interface Node-RED**

   * Recebe dados via MQTT;
   * Salva diretamente no MySQL;
   * Atualiza dashboards em tempo real (gauges, contadores e filtros por setor/status).

---

### 3.2 Comunicação IoT

Cada setor possui Wi-Fi dedicado, e cada sensor ESP32 monitora o setor correspondente, garantindo:

* Operação isolada por setor;
* Redundância e resiliência;
* Modularidade e escalabilidade.

**Fluxo do sensor:**

1. Detecta status ou movimentação da moto;
2. Publica dados JSON no broker MQTT:

```json
{
  "id_sensor": "01111",
  "id_moto": "45124",
  "setor": "Agendada para manutenção",
  "timestamp": 123456789
}
```

3. Node-RED recebe, verifica mudanças e:

   * Salva diretamente no MySQL;
   * Atualiza dashboards em tempo real.

4. Cada alteração de LED/status é publicada de volta via MQTT.

---

### 3.3 Comunicação MQTT

* Broker: `broker.hivemq.com`
* Tópico de publicação: `iot/mottu-mottion/sensormo`
* Tópico de assinatura (controle LEDs): `iot/mottu-mottion/statusled`
* Protocolo: MQTT sobre TCP (porta 1883)

**Benefícios:** comunicação bidirecional, baixo consumo de banda e suporte a múltiplos sensores/dashboards.

---

### 3.4 Node-RED + MySQL sem API

1. Sensor ESP32 envia dados via MQTT;
2. Node-RED processa os dados com Function nodes;
3. Insere ou atualiza registros no MySQL usando MySQL node;
4. Dashboards atualizados em tempo real.

**Vantagens:** menor latência, simplicidade e histórico completo de movimentações.

---

### 3.5 Estrutura de Banco de Dados

**Tabela principal `dados_moto_sensor`:**

```sql
CREATE TABLE dados_moto_sensor (
    id INT AUTO_INCREMENT PRIMARY KEY,
    id_sensor VARCHAR(255),
    id_moto VARCHAR(255),
    setor VARCHAR(255),
    observacao TEXT,
    timestamp_millis BIGINT,
    data_hora_registro TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```

Outras tabelas: `motos`, `patios`, `funcionarios`, `vagas`.

---

## 4. Implementação do Firmware ESP32

* Desenvolvido em **Arduino C++** integrando Wi-Fi, MQTT e LEDs;
* Publica status em intervalos configuráveis (ex.: 5 segundos);
* Recebe comandos de alteração de LEDs/setores via MQTT;
* Código modular para inclusão de novos sensores/setores.

**LEDs configuráveis por setor:**

| Cor LED      | Setor Correspondente     |
| ------------ | ------------------------ |
| Vermelho     | Danos Estruturais        |
| Azul         | Reparos Simples          |
| Verde Claro  | Minha Mottu              |
| Verde Escuro | Pronta para Aluguel      |
| Amarelo      | Pendência                |
| Cinza        | Agendada para manutenção |
| Roxo         | Sem Placa                |
| Bordô        | Motor com defeito        |

**Fluxo do ESP32:**

1. Conecta ao Wi-Fi e MQTT;
2. Publica status inicial;
3. Aguarda comandos de alteração;
4. Atualiza status e republica periodicamente.

---

## 5. Interface Node-RED

* Dashboard responsivo com gauges para cada setor;
* Filtragem por setor, status e data;
* Atualização em tempo real via MQTT;
* Envio de comandos para alteração de LEDs.

> Não é necessário utilizar API intermediária para registro de movimentações.

---

## 6. Backend Java + Spring Boot (Opcional)

* Gerencia motos, setores, funcionários e clientes;
* Persistência via Spring Data JPA;
* Endpoints principais:

  * `GET /motos` – Lista motos
  * `POST /motos` – Cadastra nova moto
  * `POST /movimentacao` – Registra movimentação
  * `GET /dashboards/setores` – Contagem de motos por setor

---

## 7. Diferenciais

* Modularidade e escalabilidade;
* Atualizações em tempo real via MQTT;
* Padronização entre filiais;
* Rastreabilidade completa da frota;
* Integração total: IoT + Node-RED + MySQL.

---

## 8. Ferramentas Utilizadas

* **Hardware:** ESP32, LEDs, sensores simulados via Wokwi
* **Software:** Node-RED, Arduino IDE, Java + Spring Boot, MySQL, MQTT Broker HiveMQ
* **Simuladores Wokwi:**

  * Sensor 1: [Link](https://wokwi.com/projects/442719596591605761)
  * Sensor 2: [Link](https://wokwi.com/projects/442741085348602881)
  * Sensor 3: [Link](https://wokwi.com/projects/442741385511878657)
  * Sensor 4: [Link](https://wokwi.com/projects/442741436895251457)

---

## 9. Conclusão

O **Mottu Mottion** oferece:

* Gestão eficiente e automatizada da frota;
* Rastreabilidade e segurança em tempo real;
* Dashboards intuitivos e atualizações automáticas;
* Histórico de movimentações para análise e otimização de processos.

---

## 10. Roadmap Futuro

* Integração com aplicação mobile para clientes e funcionários;
* Dashboard em tempo real com Node-RED e Grafana;
* Relatórios em PDF e Excel;
* Expansão para monitoramento via IoT em escala.

---

## 11. Authors

* Giovanna Revito Roz – RM558981
* Kaian Gustavo de Oliveira Nascimento – RM558986
* Lucas Kenji Kikuchi – RM554424

---

