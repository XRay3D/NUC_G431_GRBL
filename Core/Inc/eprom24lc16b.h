#pragma once

#include <stdint.h>

class Eprom24LC16B {
private:
    uint8_t get_HighAddress(int address);
    uint8_t get_LowAddress(int address);
    uint8_t makeDevAddress(int address);

public:
    Eprom24LC16B();

    // write one byte
    void write(int address, uint8_t data);
    // write buffer
    void write(int address, uint8_t* buffer, int length);

    uint8_t read_byte(int address);
    int read_int(int address);
    void read_buffer(int address, uint8_t* buffer, int length);
};
