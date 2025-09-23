# Mottu Mottion

**Projeto de rastreamento e gestão de motos em pátios da Mottu utilizando IoT, Node-RED e MySQL.**

---

## 1. Descrição

O **Mottu Mottion** surge como uma solução tecnológica para os desafios enfrentados pela **Mottu** na gestão física de seus pátios de motos. Com mais de 100 filiais pelo Brasil, cada unidade possui layouts distintos e fluxos operacionais próprios.

O problema central detectado é a **falta de rastreabilidade e padronização**, resultando em:

* Dificuldade em localizar motos rapidamente
* Falta de registro preciso de manutenções e reparos
* Riscos de extravios, furtos ou movimentação indevida de veículos

O Mottu Mottion digitaliza e automatiza o controle de motos com:

* Sensores ESP32 conectados por Wi-Fi
* Comunicação via **MQTT**
* Controle de status por LEDs coloridos
* Dashboards em **Node-RED**
* Armazenamento direto em **MySQL**

---

## 2. Objetivos

* Automatizar o controle de motos no pátio
* Padronizar processos entre todas as filiais
* Monitorar setores em tempo real
* Armazenar histórico de movimentações para análise
* Interface visual intuitiva via dashboards Node-RED

---

## 3. Arquitetura do Sistema

### Camadas:

1. **Hardware/IoT**

   * Sensores ESP32 por setor
   * LEDs indicadores de status
   * Comunicação via Wi-Fi e MQTT

2. **Backend (Java + Spring Boot)**

   * Opcional: API RESTful para registro e consulta de motos
   * Integração com banco MySQL

3. **Banco de Dados**

   * MySQL para registros de movimentações, motos, setores, funcionários e clientes

4. **Interface Node-RED**

   * Recebe dados MQTT e salva diretamente no MySQL
   * Atualiza dashboards em tempo real

---

### Comunicação IoT + MQTT

* **Broker:** `broker.hivemq.com`
* **Tópico de publicação (sensor → Node-RED):** `iot/mottu-mottion/sensormo`
* **Tópico de comando (Node-RED → sensor/LEDs):** `iot/mottu-mottion/statusled`
* **Protocolo:** MQTT sobre TCP (porta 1883)

---

### Estrutura de Banco de Dados (MySQL)

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

---

### LEDs por setor

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

---

## 4. Node-RED + MySQL

* Recebe dados via MQTT
* Processa e salva **diretamente no MySQL**
* Atualiza dashboards em tempo real (gauges, filtros e contadores por setor)
* Envia comandos de alteração de LED via MQTT

> Não é necessário utilizar API intermediária para registrar movimentações.

---

## 5. Backend Java + Spring Boot (Opcional)

* Gerencia motos, setores, funcionários e clientes
* Persistência via Spring Data JPA
* Endpoints principais:

  * `GET /motos` – Lista motos
  * `POST /motos` – Cadastra nova moto
  * `POST /movimentacao` – Registra movimentação
  * `GET /dashboards/setores` – Contagem de motos por setor

---

## 6. Diferenciais

* Modularidade e escalabilidade
* Atualizações em tempo real via MQTT
* Padronização entre filiais
* Rastreabilidade completa da frota
* Integração total: IoT + Node-RED + MySQL

---

## 7. Ferramentas Utilizadas

* **Hardware:** ESP32, LEDs, sensores simulados via Wokwi
* **Software:** Node-RED, Arduino IDE, Java + Spring Boot, MySQL, MQTT Broker HiveMQ
* **Simuladores Wokwi:**

  * Sensor 1: [Link](https://wokwi.com/projects/442719596591605761)
  * Sensor 2: [Link](https://wokwi.com/projects/442741085348602881)
  * Sensor 3: [Link](https://wokwi.com/projects/442741385511878657)
  * Sensor 4: [Link](https://wokwi.com/projects/442741436895251457)

---

## 8. Conclusão

O **Mottu Mottion** oferece:

* Gestão eficiente e automatizada da frota
* Rastreabilidade e segurança em tempo real
* Dashboards intuitivos e atualizações automáticas
* Histórico de movimentações para análise e otimização de processos

---

## Roadmap Futuro

* Integração com aplicação mobile para clientes e funcionários
* Dashboard em tempo real com Node-RED e Grafana
* Relatórios em PDF e Excel
* Expansão para monitoramento via IoT em escala

---

## Authors

* Giovanna Revito Roz – RM558981
* Kaian Gustavo de Oliveira Nascimento – RM558986
* Lucas Kenji Kikuchi – RM554424

Quer que eu faça isso?
