#include <Arduino.h>
#include <esp32ModbusRTU.h>
#include <HardwareSerial.h> 
#include <algorithm>  
#define TIMEOUT_MS 5000
#define QUEUE_SIZE 20

esp32ModbusRTU modbus(&Serial2, 4);  //CS of the NextPM to 4

long valLong = 0;

void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8E1);

   modbus.onData([](uint8_t serverAddress, esp32Modbus::FunctionCode fc, uint16_t address, uint8_t* data, size_t length) {
    Serial.printf("id 0x%02x fc 0x%02x len %u: 0x", serverAddress, fc, length);
    
    Serial.print("Data: 0x");
for (size_t i = 0; i < length; ++i) {
  Serial.printf("%02x", data[i]);
  if(i<length-1){
    Serial.print(" 0x");
    }
  
}
Serial.print("\n");

    std::reverse(data, data + 4);  // fix endianness
Serial.print("Data: 0x");
for (size_t i = 0; i < length; ++i) {
  Serial.printf("%02x", data[i]);
  valLong = (valLong << 8) | data[i];
   if(i<length-1){
    Serial.print(" 0x");
    }
}
Serial.print("\n");
Serial.println(valLong);
    
    });
  
  modbus.onError([](esp32Modbus::Error error) {
    Serial.printf("error: 0x%02x\n\n", static_cast<uint8_t>(error));
  });
  modbus.begin();

}

void loop() {

static uint32_t lastMillis = 0;
  if (millis() - lastMillis > 5000) {
    lastMillis = millis();
    Serial.print("sending Modbus request...\n");
    modbus.readHoldingRegisters(0x01,56,2); //Meaure PM1 ug/m3
  }

}
