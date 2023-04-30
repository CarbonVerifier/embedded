# Embedded device

### Components:
- Main microcontroler `ESP32Dev`
- Temperature and humidity sensor `DHT11`
- Light sensor `BH1750` ( `GY-30` )
- CO2 sensor `MQ135`

### Runtime
The main controller run the update of the sensors every one seconds and send then to `MQTT` server using the `ESP32` wifi connection, on topics:
- `temperature`
- `humidity`
- `light`
- `CO2`

### Dev environment
- `PlatformIO` with `VSCode` or other accepted IDE.
- the code is written in `C++` using `Arduino` framework separated `src` and `include` folders, contains the sensors classes and the main `main.cpp` file..
