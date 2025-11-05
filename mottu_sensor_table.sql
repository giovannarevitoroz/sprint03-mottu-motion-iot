
CREATE DATABASE IF NOT EXISTS sensor_table;
USE sensor_table;

DROP TABLE IF EXISTS dados_moto_sensor;

CREATE TABLE dados_moto_sensor (
    id INT AUTO_INCREMENT PRIMARY KEY,
    id_sensor VARCHAR(255),
    id_moto VARCHAR(255),
    setor VARCHAR(255),
    observacao TEXT,
    timestamp_millis BIGINT,
    data_hora_registro TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

ALTER TABLE dados_moto_sensor ADD COLUMN estado VARCHAR(100) AFTER setor;

SELECT * FROM dados_moto_sensor;



