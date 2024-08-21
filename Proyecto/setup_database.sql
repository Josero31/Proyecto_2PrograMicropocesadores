-- Crear base de datos
CREATE DATABASE IF NOT EXISTS TrackingDB;

-- Seleccionar la base de datos
USE TrackingDB;

-- Crear tabla para almacenar las coordenadas GPS
CREATE TABLE IF NOT EXISTS CoordenadasGPS (
    id INT AUTO_INCREMENT PRIMARY KEY,
    latitud FLOAT NOT NULL,
    longitud FLOAT NOT NULL,
    altitud INT NOT NULL,
    fecha DATE NOT NULL,
    hora TIME NOT NULL
);

-- Aquí se agregarían los datos de ejemplo del archivo coordenadas.plt
-- Ejemplo de cómo sería una inserción de datos (los datos reales se deben procesar desde el archivo PLT):
INSERT INTO CoordenadasGPS (latitud, longitud, altitud, fecha, hora) VALUES 
(39.984702, 116.318417, 492, '2008-10-23', '02:53:04'),
(39.984683, 116.318450, 492, '2008-10-23', '02:53:10'),
-- Agrega más datos según sea necesario
;
