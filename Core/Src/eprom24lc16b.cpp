#include "eprom24lc16b.h"

#define address_24LC16B 0b1010

Eprom24LC16B::Eprom24LC16B() {
}

uint8_t Eprom24LC16B::get_HighAddress(int address) {
    return (uint8_t)((address) >> 8);
}

uint8_t Eprom24LC16B::get_LowAddress(int address) {
    return (uint8_t)((address & 0xFF));
}

uint8_t Eprom24LC16B::makeDevAddress(int address) {
    return (uint8_t)((address_24LC16B << 3) | ((get_HighAddress(address)) & 0x07));
}

void Eprom24LC16B::write(int address, uint8_t data) {
    //    Wire.beginTransmission(makeDevAddress(address));
    //    Wire.send(get_LowAddress(address));
    //    Wire.send(data);
    //    Wire.endTransmission();
}

void Eprom24LC16B::write(int address, uint8_t* buffer, int length) {
    //    int page_i = 0;
    //    uint8_t c = 0;
    //    while (c < length) {
    //        Wire.beginTransmission(makeDevAddress(address + c));
    //        Wire.send(get_LowAddress(address + c));
    //        page_i = 0;
    //        while ((page_i < 16) && (c < length)) {
    //            Wire.send(buffer[c]);
    //            page_i++;
    //            c++;
    //        }
    //        Wire.endTransmission();
    //        delay(5);
    //    }
}

uint8_t Eprom24LC16B::read_byte(int address) {
    //    Wire.beginTransmission(makeDevAddress(address));
    //    Wire.send(get_LowAddress(address));
    //    Wire.endTransmission();
    uint8_t rdata = 0x00;
    //    Wire.requestFrom((makeDevAddress(address)), (uint8_t)1);
    //    if (Wire.available()) {
    //        rdata = Wire.receive();
    //    }
    return rdata;
}

int Eprom24LC16B::read_int(int address) {
    int rdata = 0;
    //    read_buffer(address, (uint8_t*)rdata, sizeof(rdata));
    return rdata;
}

void Eprom24LC16B::read_buffer(int address, uint8_t* buffer, int length) {
    //    Wire.beginTransmission(makeDevAddress(address));
    //    Wire.send(get_LowAddress(address));
    //    Wire.endTransmission();
    //    Wire.requestFrom(makeDevAddress(address), (uint8_t)length);
    //    int c = 0;
    //    for (c = 0; c < length; c++)
    //        if (Wire.available())
    //            buffer[c] = Wire.receive();
}
