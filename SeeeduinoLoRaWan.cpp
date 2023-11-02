/*
  SeeeduinoLoRaWan.cpp
  2013 Copyright (c) Seeed Technology Inc.  All right reserved.

  Author: Wayne Weng
  Date: 2016-10-17

  add rgb backlight fucnction @ 2013-10-15
  
  The MIT License (MIT)

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.1  USA


  Modified for LoRa@VSB by Ondřej Knebl, 2. 11. 2023
*/

#include "SeeeduinoLoRaWan.h"


LoRaWanClass::LoRaWanClass(void)
{
    memset(_buffer, 0, 256);
}


void LoRaWanClass::init(void)
{
    SerialLoRa.begin(9600);
}


void LoRaWanClass::setEU433(void)
{
    setDataRate(EU433);

    const float EU_433[8] = {433.175, 433.375, 433.575, 433.775, 433.975, 434.175, 434.375, 434.575};

    for(int i = 0; i < 8; i++)
    {
        if(EU_433[i] != 0)
        {
            setChannel(i, EU_433[i], DR0, DR5);
         
        }
    }
    
    setReceiveWindowSecond(434.665, DR0);
    setPower(0);
    setAdaptiveDataRate(true);
    setDutyCycle(true);
    setJoinDutyCycle(true);

    setUnconfirmedMessageRepeatTime(0);
    setConfirmedMessageRetryTime(0);

    setReceiveWindowDelay(RECEIVE_DELAY1, 1);
    setReceiveWindowDelay(RECEIVE_DELAY2, 2);
    setReceiveWindowDelay(JOIN_ACCEPT_DELAY1, 5);
    setReceiveWindowDelay(JOIN_ACCEPT_DELAY2, 6);
}


void LoRaWanClass::setEU868(void)
{
    setDataRate(EU868);

    const float EU_868[8] = {868.1, 868.3, 868.5, 867.1, 867.3, 867.5, 867.7, 867.9};

    for(int i = 0; i < 8; i++)
    {
        if(EU_868[i] != 0)
        {
            setChannel(i, EU_868[i], DR0, DR5);
         
        }
    }
    
    setReceiveWindowSecond(869.525, DR0);
    setPower(0);
    setAdaptiveDataRate(true);
    setDutyCycle(true);
    setJoinDutyCycle(true);

    setUnconfirmedMessageRepeatTime(0);
    setConfirmedMessageRetryTime(0);

    setReceiveWindowDelay(RECEIVE_DELAY1, 1);
    setReceiveWindowDelay(RECEIVE_DELAY2, 2);
    setReceiveWindowDelay(JOIN_ACCEPT_DELAY1, 5);
    setReceiveWindowDelay(JOIN_ACCEPT_DELAY2, 6);
}


void LoRaWanClass::getVersion(void)
{
    sendCommand("AT+VER=?\r\n");
    loraPrint(DEFAULT_DEBUGTIME);
}


void LoRaWanClass::getId(void)
{
    while(SerialLoRa.available())SerialLoRa.read();
    sendCommand("AT+ID=?\r\n");
    loraPrint(DEFAULT_DEBUGTIME);
}


void LoRaWanClass::setKeysOTAA(char *AppEUI, char *DevEUI, char *AppKey )
{
    char cmd[64];

    if(AppEUI)
    {
        memset(cmd, 0, 64);
        sprintf(cmd, "AT+ID=AppEui,\"%s\"\r\n", AppEUI);
        sendCommand(cmd);
        loraPrint(DEFAULT_DEBUGTIME);
    }

    if(DevEUI)
    {
        memset(cmd, 0, 64);
        sprintf(cmd, "AT+ID=DevEui,\"%s\"\r\n", DevEUI);
        sendCommand(cmd);
        loraPrint(DEFAULT_DEBUGTIME);
    }

    if(AppKey)
    {
        memset(cmd, 0, 64);
        sprintf(cmd, "AT+KEY= APPKEY,\"%s\"\r\n", AppKey);
        sendCommand(cmd);
        loraPrint(DEFAULT_DEBUGTIME);
    }
}


void LoRaWanClass::setKeysABP(char *DevAddr, char *NwkSKey, char *AppSKey)
{
    char cmd[64];

    if(DevAddr)
    {
        memset(cmd, 0, 64);
        sprintf(cmd, "AT+ID=DevAddr,\"%s\"\r\n", DevAddr);
        sendCommand(cmd);
        loraPrint(DEFAULT_DEBUGTIME);
    }

    if(NwkSKey)
    {
        memset(cmd, 0, 64);
        sprintf(cmd, "AT+KEY=NWKSKEY,\"%s\"\r\n", NwkSKey);
        sendCommand(cmd);
        loraPrint(DEFAULT_DEBUGTIME);
    }
    
    if(AppSKey)
    {
        memset(cmd, 0, 64);
        sprintf(cmd, "AT+KEY=APPSKEY,\"%s\"\r\n", AppSKey);
        sendCommand(cmd);
        loraPrint(DEFAULT_DEBUGTIME);
    }
}


void LoRaWanClass::setDataRate(_physical_type_t physicalType)
{
    char cmd[32];
    
    if(physicalType == EU433)sendCommand("AT+DR=EU433\r\n");
    else if(physicalType == EU868)sendCommand("AT+DR=EU868\r\n");
    delay(DEFAULT_TIMEWAIT);
    
    memset(cmd, 0, 32);
    sprintf(cmd, "AT+DR=%s\r\n", physicalType);
    sendCommand(cmd);
    loraPrint(DEFAULT_DEBUGTIME);
}


void LoRaWanClass::setPower(short power)
{
    char cmd[32];
    
    memset(cmd, 0, 32);
    sprintf(cmd, "AT+POWER=%d\r\n", power);
    sendCommand(cmd);
    loraPrint(DEFAULT_DEBUGTIME);
}


void LoRaWanClass::setPort(unsigned char port)
{
    char cmd[32];
    
    memset(cmd, 0, 32);
    sprintf(cmd, "AT+PORT=%d\r\n", port);
    sendCommand(cmd);
    loraPrint(DEFAULT_DEBUGTIME);
}


void LoRaWanClass::setAdaptiveDataRate(bool command)
{
    if(command)sendCommand("AT+ADR=ON\r\n");
    else sendCommand("AT+ADR=OFF\r\n");
    loraPrint(DEFAULT_DEBUGTIME);
}


void LoRaWanClass::setChannel(unsigned char channel, float frequency, _data_rate_t dataRataMin, _data_rate_t dataRataMax)
{
    char cmd[32];
    
    memset(cmd, 0, 32);
    sprintf(cmd, "AT+CH=%d,%d.%d,%d,%d\r\n", channel, (short)frequency, short(frequency * 10) % 10, dataRataMin, dataRataMax);
    sendCommand(cmd);
    loraPrint(DEFAULT_DEBUGTIME);
}


bool LoRaWanClass::transmitPacket(char *buffer, unsigned char timeout)
{
    unsigned char length = strlen(buffer);
    
    while(SerialLoRa.available())SerialLoRa.read();
    
    sendCommand("AT+MSG=\"");
    for(unsigned char i = 0; i < length; i ++)SerialLoRa.write(buffer[i]);
    sendCommand("\"\r\n");
    
    memset(_buffer, 0, BEFFER_LENGTH_MAX);
    readBuffer(_buffer, BEFFER_LENGTH_MAX, timeout);

    if(strstr(_buffer, "+MSG: Done"))return true;
    return false;
}


bool LoRaWanClass::transmitPacket(unsigned char *buffer, unsigned char length, unsigned char timeout)
{
    char temp[2] = {0};
    
    while(SerialLoRa.available())SerialLoRa.read();
    
    sendCommand("AT+MSGHEX=\"");
    for(unsigned char i = 0; i < length; i ++)
    {
        sprintf(temp,"%02x", buffer[i]);
        SerialLoRa.write(temp); 
    }
    sendCommand("\"\r\n");
    
    memset(_buffer, 0, BEFFER_LENGTH_MAX);
    readBuffer(_buffer, BEFFER_LENGTH_MAX, timeout);
    
    if(strstr(_buffer, "+MSGHEX: Done"))return true;
    return false;
}


bool LoRaWanClass::transmitPacketWithConfirmed(char *buffer, unsigned char timeout)
{
    unsigned char length = strlen(buffer);
    
    while(SerialLoRa.available())SerialLoRa.read();
    
    sendCommand("AT+CMSG=\"");
    for(unsigned char i = 0; i < length; i ++)SerialLoRa.write(buffer[i]);
    sendCommand("\"\r\n");
    
    memset(_buffer, 0, BEFFER_LENGTH_MAX);
    readBuffer(_buffer, BEFFER_LENGTH_MAX, timeout);
     
    if(strstr(_buffer, "+CMSG: ACK Received"))return true;
    return false;
}


bool LoRaWanClass::transmitPacketWithConfirmed(unsigned char *buffer, unsigned char length, unsigned char timeout)
{
    char temp[2] = {0};
    
    while(SerialLoRa.available())SerialLoRa.read();
    
    sendCommand("AT+CMSGHEX=\"");
    for(unsigned char i = 0; i < length; i ++)
    {
        sprintf(temp,"%02x", buffer[i]);
        SerialLoRa.write(temp); 
    }
    sendCommand("\"\r\n");
 
    memset(_buffer, 0, BEFFER_LENGTH_MAX);
    readBuffer(_buffer, BEFFER_LENGTH_MAX, timeout);
    
    if(strstr(_buffer, "+CMSGHEX: ACK Received"))return true;
    return false;
}


short LoRaWanClass::receivePacket(char *buffer, short length, short *rssi)
{
    char *ptr;
    short number = 0;
    
    ptr = strstr(_buffer, "RSSI ");
    if(ptr)*rssi = atoi(ptr + 5);
    else *rssi = -255;
    
    ptr = strstr(_buffer, "RX: \"");
    if(ptr)
    {        
        ptr += 5;
        
        uint8_t bitStep = 0;
        if(*(ptr + 2) == ' ')bitStep = 3; // Firmware version 2.0.10
        else bitStep = 2;                   // Firmware version 2.1.15
        
        for(short i = 0; ; i ++)
        {
            char temp[2] = {0};
            unsigned char tmp, result = 0;
            
            temp[0] = *(ptr + i * bitStep);
            temp[1] = *(ptr + i * bitStep + 1);
           
            for(unsigned char j = 0; j < 2; j ++)
            {
                if((temp[j] >= '0') && (temp[j] <= '9'))
                tmp = temp[j] - '0';
                else if((temp[j] >= 'A') && (temp[j] <= 'F'))
                tmp = temp[j] - 'A' + 10;
                else if((temp[j] >= 'a') && (temp[j] <= 'f'))
                tmp = temp[j] - 'a' + 10;

                result = result * 16 + tmp;
            }
            
            if(i < length)buffer[i] = result;

            if(*(ptr + (i + 1) * bitStep) == '\"' && *(ptr + (i + 1) * bitStep + 1) == '\r' && *(ptr + (i + 1) * bitStep + 2) == '\n')
            {
                number = i + 1;
                break;
            }
        }        
    }
       
    memset(_buffer, 0, BEFFER_LENGTH_MAX);
    
    return number;
}


bool LoRaWanClass::transmitProprietaryPacket(char *buffer, unsigned char timeout)
{
    unsigned char length = strlen(buffer);
    
    while(SerialLoRa.available())SerialLoRa.read();
    
    sendCommand("AT+PMSG=\"");
    for(unsigned char i = 0; i < length; i ++)SerialLoRa.write(buffer[i]);
    sendCommand("\"\r\n");
    
    memset(_buffer, 0, BEFFER_LENGTH_MAX);
    readBuffer(_buffer, BEFFER_LENGTH_MAX, timeout);

    if(strstr(_buffer, "+PMSG: Done"))return true;
    return false;
}


bool LoRaWanClass::transmitProprietaryPacket(unsigned char *buffer, unsigned char length, unsigned char timeout)
{
    char temp[2] = {0};
    
    while(SerialLoRa.available())SerialLoRa.read();
    
    sendCommand("AT+PMSGHEX=\"");
    for(unsigned char i = 0; i < length; i ++)
    {
        sprintf(temp,"%02x", buffer[i]);
        SerialLoRa.write(temp); 
    }
    sendCommand("\"\r\n");
    
    memset(_buffer, 0, BEFFER_LENGTH_MAX);
    readBuffer(_buffer, BEFFER_LENGTH_MAX, timeout);
   
    if(strstr(_buffer, "+PMSGHEX: Done"))return true;
    return false;
}


void LoRaWanClass::setUnconfirmedMessageRepeatTime(unsigned char time)
{
    char cmd[32];
    
    if(time > 15) time = 15;
    else if(time == 0) time = 1;
    
    memset(cmd, 0, 32);
    sprintf(cmd, "AT+REPT=%d\r\n", time);
    sendCommand(cmd);
    loraPrint(DEFAULT_DEBUGTIME);
}


void LoRaWanClass::setConfirmedMessageRetryTime(unsigned char time)
{
    char cmd[32];
    
    if(time > 15) time = 15;
    else if(time == 0) time = 1;
    
    memset(cmd, 0, 32);
    sprintf(cmd, "AT+RETRY=%d\r\n", time);
    sendCommand(cmd);
    loraPrint(DEFAULT_DEBUGTIME);   
}


void LoRaWanClass::setReceiveWindowFirst(bool command)
{
    if(command)sendCommand("AT+RXWIN1=ON\r\n");
    else sendCommand("AT+RXWIN1=OFF\r\n");
    loraPrint(DEFAULT_DEBUGTIME);
}


void LoRaWanClass::setReceiveWindowFirst(unsigned char channel, float frequency)
{
    char cmd[32];
    
    memset(cmd, 0, 32);
    sprintf(cmd, "AT+RXWIN1=%d,%d.%d\r\n", channel, (short)frequency, short(frequency * 10) % 10);
    sendCommand(cmd);
    loraPrint(DEFAULT_DEBUGTIME);
}


void LoRaWanClass::setReceiveWindowSecond(float frequency, _data_rate_t dataRate)
{
    char cmd[32];
    
    memset(cmd, 0, 32);
    sprintf(cmd, "AT+RXWIN2=%d.%d,%d\r\n", (short)frequency, short(frequency * 10) % 10, dataRate);
    sendCommand(cmd);
    loraPrint(DEFAULT_DEBUGTIME);
}


void LoRaWanClass::setReceiveWindowSecond(float frequency, _spreading_factor_t spreadingFactor, _band_width_t bandwidth)
{
    char cmd[32];
    
    memset(cmd, 0, 32);
    sprintf(cmd, "AT+RXWIN2=%d.%d,%d,%d\r\n", (short)frequency, short(frequency * 10) % 10, spreadingFactor, bandwidth);
    sendCommand(cmd);
    loraPrint(DEFAULT_DEBUGTIME);
}


void LoRaWanClass::setDutyCycle(bool command)
{
    if(command)sendCommand("AT+LW=DC, ON\r\n");
    else sendCommand("AT+LW=DC, OFF\r\n");  
    loraPrint(DEFAULT_DEBUGTIME);
}


void LoRaWanClass::setJoinDutyCycle(bool command)
{
    if(command)sendCommand("AT+LW=JDC,ON\r\n");
    else sendCommand("AT+LW=JDC,OFF\r\n");  
    loraPrint(DEFAULT_DEBUGTIME);
}


void LoRaWanClass::setReceiveWindowDelay(_window_delay_t command, unsigned short _delay)
{
    char cmd[32];
    
    memset(cmd, 0, 32);
    if(command == RECEIVE_DELAY1) sprintf(cmd, "AT+DELAY=RX1,%d\r\n", _delay);
    else if(command == RECEIVE_DELAY2) sprintf(cmd, "AT+DELAY=RX2,%d\r\n", _delay);
    else if(command == JOIN_ACCEPT_DELAY1) sprintf(cmd, "AT+DELAY=JRX1,%d\r\n", _delay);
    else if(command == JOIN_ACCEPT_DELAY2) sprintf(cmd, "AT+DELAY=JRX2,%d\r\n", _delay); 
    sendCommand(cmd);
    loraPrint(DEFAULT_DEBUGTIME);
}


void LoRaWanClass::setClassType(_class_type_t type)
{
    if(type == CLASS_A)sendCommand("AT+CLASS=A\r\n");
    else if(type == CLASS_C)sendCommand("AT+CLASS=B\r\n");
    else if(type == CLASS_C)sendCommand("AT+CLASS=C\r\n");
    loraPrint(DEFAULT_DEBUGTIME);
}


void LoRaWanClass::setActivation(_device_mode_t mode)
{
    if(mode == LWABP)sendCommand("AT+MODE=LWABP\r\n");
    else if(mode == LWOTAA)sendCommand("AT+MODE=LWOTAA\r\n");
    loraPrint(DEFAULT_DEBUGTIME);
}


bool LoRaWanClass::setOTAAJoin(_otaa_join_cmd_t command, unsigned char timeout)
{
    char *ptr;
    
    if(command == JOIN)sendCommand("AT+JOIN\r\n");
    else if(command == FORCE)sendCommand("AT+JOIN=FORCE\r\n"); 
    delay(DEFAULT_TIMEWAIT);
    
    memset(_buffer, 0, BEFFER_LENGTH_MAX);
    readBuffer(_buffer, BEFFER_LENGTH_MAX, timeout);

    ptr = strstr(_buffer, "+JOIN: Join failed");
    if(ptr)return false;
    ptr = strstr(_buffer, "+JOIN: LoRaWAN modem is busy");
    if(ptr)return false;
    
    return true;
}


void LoRaWanClass::setDeviceLowPower(void)
{
    sendCommand("AT+LOWPOWER\r\n");
    loraPrint(DEFAULT_DEBUGTIME);
}


void LoRaWanClass::setDeviceReset()
{
    sendCommand("AT+RESET\r\n");
    delay(1000);
    loraPrint(DEFAULT_DEBUGTIME);
}


void LoRaWanClass::setDeviceDefault(void)
{
    sendCommand("AT+FDEFAULT=RISINGHF\r\n");
    delay(1000);
    loraPrint(DEFAULT_DEBUGTIME);
}


short LoRaWanClass::getBatteryVoltage(void)
{
    int battery;
    
    pinMode(CHARGE_STATUS_PIN, OUTPUT);
    digitalWrite(CHARGE_STATUS_PIN, LOW);
    delay(DEFAULT_TIMEWAIT);
    battery = (analogRead(BATTERY_POWER_PIN) * 3300 * 11) >> 10;
    pinMode(CHARGE_STATUS_PIN, INPUT);
    
    return battery;
}


void LoRaWanClass::sendCommand(char *command)
{
    SerialLoRa.print(command);
}


short LoRaWanClass::readBuffer(char *buffer, short length, unsigned char timeout)
{
    short i = 0;
    unsigned long timerStart, timerEnd;

    timerStart = millis();

    while(1)
    {
        if(i < length)
        {
            while(SerialLoRa.available())
            {
                char c = SerialLoRa.read();  
                buffer[i ++] = c;
            }  
        }
        
        timerEnd = millis();
        if(timerEnd - timerStart > 1000 * timeout)break;
    }

    SerialUSB.print(buffer);

    return i;
}

void LoRaWanClass::loraPrint(unsigned char timeout)
{
    unsigned long timerStart, timerEnd;

    timerStart = millis();
    
    while(1)
    {
        while(SerialLoRa.available()) SerialUSB.write(SerialLoRa.read());  
        
        timerEnd = millis();
        if(timerEnd - timerStart > timeout)break;
    }
}
