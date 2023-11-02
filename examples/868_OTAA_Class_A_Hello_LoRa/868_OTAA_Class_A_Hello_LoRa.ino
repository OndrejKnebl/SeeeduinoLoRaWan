/*******************************************************************************
 * The Things Network - OTAA Seeeduino
 * 
 * Copyright (c) 2023 Ondřej Knebl, LoRa@VSB
 *
 * Permission is hereby granted, free of charge, to anyone
 * obtaining a copy of this document and accompanying files,
 * to do whatever they want with them without any restriction,
 * including, but not limited to, copying, modification and redistribution.
 * NO WARRANTY OF ANY KIND IS PROVIDED.
 * 
 * Hello, LoRa!, 24. 6. 2022
 *******************************************************************************/

#include <LoRaWan.h>
LoRaWanClass lora;

//-------------- Here change your keys --------------
#define APP_EUI "0000000000000000"
#define DEV_EUI "0000000000000000"
#define APP_KEY "00000000000000000000000000000000"
//---------------------------------------------------

const unsigned TX_INTERVAL = 60;                                // Transmission interval in seconds
static uint8_t mydata[] = "Hello, LoRa!";


void sendAndReceiveData() {
  
    SerialUSB.println("Sending - Hello, LoRa!");
    bool result = lora.transmitPacket(mydata, sizeof(mydata)-1);

    if(result) {
        SerialUSB.println("Data sent successfully!");
        short length;
        char buffer[256];
        short rssi;

        memset(buffer, 0, 256);
        length = lora.receivePacket(buffer, 256, &rssi);
        
        if(length) {
            SerialUSB.print("Length: ");
            SerialUSB.println(length);
            SerialUSB.print("RSSI: ");
            SerialUSB.println(rssi);
            SerialUSB.print("Data: ");
            for(unsigned char i = 0; i < length; i ++) {
                SerialUSB.print("0x");
                SerialUSB.print(buffer[i], HEX);
                SerialUSB.print(" ");
            }
            SerialUSB.println();
        }
    }
}


void setup(void) {
    lora.init();

    SerialUSB.begin(9600);
    while(!SerialUSB);     

    lora.setDeviceReset();
    lora.getVersion();
    lora.setActivation(LWOTAA);
    lora.setKeysOTAA(APP_EUI, DEV_EUI, APP_KEY);
    lora.setEU868();
    lora.setClassType(CLASS_A);
    lora.setPort(1);

    while(!lora.setOTAAJoin(JOIN, 10));
}


void loop(void) {
    while(!lora.setOTAAJoin(JOIN, 1));

    sendAndReceiveData();

    delay(TX_INTERVAL*1000);
}