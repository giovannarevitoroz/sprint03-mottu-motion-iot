# Mottu Mottion – Rastreamento de Motos com IoT, Node-RED e MySQL

![Logo](https://dev-to-uploads.s3.amazonaws.com/uploads/articles/th5xamgrr6se0x5ro4g6.png)

Projeto de rastreamento e gestão de motos em pátios da Mottu utilizando IoT, Node-RED e MySQL.
Voltado para gestores de filiais, equipe de manutenção e operação logística, oferecendo rastreabilidade, controle de setores e visualização em tempo real.

---

## Etiquetas

[![MIT License](https://img.shields.io/badge/License-MIT-green.svg)](https://choosealicense.com/licenses/mit/)
[![Node-RED](https://img.shields.io/badge/Node--RED-v3.1-orange.svg)](https://nodered.org/)
[![MySQL](https://img.shields.io/badge/MySQL-8.0-blue.svg)](https://www.mysql.com/)
[![IoT](https://img.shields.io/badge/IoT-ESP32-brightgreen.svg)](https://www.espressif.com/en/products/socs/esp32)

---

## Demonstração

* Vídeo: [Link do vídeo](#)
* Simuladores Wokwi:

  * [Sensor 1](https://wokwi.com/projects/446531558567289857)
  * [Sensor 2](https://wokwi.com/projects/446531553727060993)
  * [Sensor 3](https://wokwi.com/projects/446531543184675841)
  * [Sensor 4](https://wokwi.com/projects/446531548506200065)

---

## Funcionalidades

* Rastreamento em tempo real de motos por setor
* Dashboards Node-RED com gauges e contadores
* Filtragem por setor, status e data
* Atualização de LEDs via MQTT
* Armazenamento histórico no MySQL
* Modularidade e escalabilidade por filial
* Interface web responsiva

---

## Arquitetura do Sistema

### 1. IoT / Firmware ESP32

* Detecta movimentações e mudanças de setor.
* Publica dados JSON no broker MQTT:

```json
{
  "id_sensor": "01111",
  "id_moto": "45124",
  "setor": "Agendada para manutenção",
  "timestamp": 123456789
}
```

* Recebe comandos de alteração de LEDs via MQTT.
* LEDs coloridos indicam status de cada moto por setor.

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

### 2. Node-RED

* Recebe dados via MQTT, processa com `Function nodes` e atualiza dashboards.
* Insere registros no MySQL apenas se houver mudança de setor.
* Atualização instantânea de gauges, contadores e status de LEDs.

**Função `Atualizar Gauges`:**

```javascript
let data = msg.payload.contagemSetor || [];

// DEBUG: Verificar o que está chegando
node.warn("Dados para gauges: " + JSON.stringify(data));

// Criar um objeto para mapear setor -> quantidade
let setorQuantidade = {};
data.forEach(item => {
    setorQuantidade[item.setor] = item.qtd;
    node.warn(`Setor: ${item.setor}, Qtd: ${item.qtd}`);
});

// Enviar mensagens para cada gauge específico
let messages = [];

// Agendada para manutenção
messages.push({
    payload: setorQuantidade["Agendada para manutenção"] || 0,
    topic: "Agendada para manutenção"
});

// Danos Estruturais
messages.push({
    payload: setorQuantidade["Danos Estruturais"] || 0,
    topic: "Danos Estruturais"
});

// Reparos Simples
messages.push({
    payload: setorQuantidade["Reparos Simples"] || 0,
    topic: "Reparos Simples"
});

// Sem Placa
messages.push({
    payload: setorQuantidade["Sem Placa"] || 0,
    topic: "Sem Placa"
});

// Motor com defeito
messages.push({
    payload: setorQuantidade["Motor com defeito"] || 0,
    topic: "Motor com defeito"
});

// Pronta para aluguel
messages.push({
    payload: setorQuantidade["Pronta para aluguel"] || 0,
    topic: "Pronta para aluguel"
});

// Pendência
messages.push({
    payload: setorQuantidade["Pendência"] || 0,
    topic: "Pendência"
});

// Minha Mottu (Verde Claro)
messages.push({
    payload: setorQuantidade["Minha Mottu (Verde Claro)"] || 0,
    topic: "Minha Mottu (Verde Claro)"
});

// Enviar todas as mensagens
return [messages];
```

**Explicação:**

* Recebe um array com contagem de motos por setor.
* Cria um objeto `setorQuantidade` para mapeamento rápido.
* Para cada gauge, envia a quantidade correspondente ou 0 se não houver dados.
* Atualiza visualmente todos os gauges no dashboard.

---

### 3. MySQL

* Armazena histórico completo e evita duplicidade de registros.
* Permite análises futuras de movimentação e performance de setores.

---

---

## Instalação e Deploy

```bash
git clone https://link-para-o-projeto
cd my-project
npm install
npm run start
```

Para deploy:

```bash
npm run deploy
```

---

## Aprendizados

* Integração IoT + Node-RED + MySQL sem APIs intermediárias.
* Controle de estado e dashboards em tempo real.
* Modularidade para escalabilidade em múltiplas filiais.
* Persistência de histórico de eventos críticos.

---

## Autores

* Giovanna Revito Roz – RM558981
* Kaian Gustavo de Oliveira Nascimento – RM558986
* Lucas Kenji Kikuchi – RM554424

---

## Referências

* [Awesome README Templates](https://awesomeopensource.com/project/elangosundar/awesome-README-templates)
* [Awesome README](https://github.com/matiassingers/awesome-readme)
* [How to write a Good README](https://bulldogjob.com/news/449-how-to-write-a-good-readme-for-your-github-project)

---
