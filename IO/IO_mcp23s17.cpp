/*
 * Copyright (c) 2017, oneso
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <cstdlib>
#include <stdint.h>

#include "IO_mcp23s17.h"
#include "../RPIO.h"

static int file = 0;

//if returns anything but 0, it is an error
//should implement a verification that register matches written value
// TODO: add verify option
static int writeByte(uint8_t devID, uint8_t devReg, uint8_t data)
{
    unsigned char message[4] = {};
    
    message[0] = MCP23S17_HARDWARE_ADDR | (MCP23S17_CMD_WRITE | (devID << 1));
    message[1] = devReg;
    message[2] = data;
    
    return io_spi::spiReadWrite(message, 3);
}

//if returns anything but zero, error
static uint8_t readByte(uint8_t devID, uint8_t devReg)
{
    unsigned char message[4] = {};
    
    message[0] = MCP23S17_HARDWARE_ADDR | (MCP23S17_CMD_READ | (devID << 1));
    message[1] = devReg;
    
    io_spi::spiReadWrite(message, 3);
    
    return message[2];
}

int io_mcp23s17::setupmcp23s17(int devID)
{
    if(writeByte(devID, MCP23S17_IOCON, MCP23S17_IOCON_SEQOP | MCP23S17_IOCON_HAEN) != 0)
    {
        return -1;
    }
    if(writeByte(devID, MCP23S17_IOCONB, MCP23S17_IOCON_SEQOP | MCP23S17_IOCON_HAEN) != 0)
    {
        return -1;
    }
    return 0;
}

int io_mcp23s17::setPinMode(int devID, int pin, OUTPUTMODE mode)
{
    uint8_t reg, hold;
    
    if(pin < 8)
    {
        reg = MCP23S17_IODIRA;
    }else if(pin < 16)
    {
        reg = MCP23S17_IODIRB;
    }else
    {
        return -1;
    }
    
    uint8_t mask = MCP23S17_GPIO(pin);
    
    hold = readByte(devID, reg);
    
    if(mode == OUTPUTMODE::INPUT)
    {
        hold |= mask;
    }else if(mode == OUTPUTMODE::OUTPUT)
    {
        hold &= ~(mask);
    }
    
    return writeByte(devID, reg, hold);
}

int io_mcp23s17::setPinLogic(int devID, int pin, bool level)
{
    uint8_t reg, hold;
    
    if(pin < 8)
    {
        reg = MCP23S17_GPIOA;
    }else if(pin < 16)
    {
        reg = MCP23S17_GPIOB;
    }else
    {
        return -1;
    }
    
    uint8_t mask = MCP23S17_GPIO(pin);
    
    hold = readByte(devID, reg);
    
    if(level)
    {
        hold |= mask;
    }else
    {
        hold &= ~(mask);
    }
    return writeByte(devID, reg, hold);
}

//Set the output logic level on pins
//overwrites value in OLAT register
//pins 0 through 15, 0-7 bank A, 8-15 bank B

//enable or disable weak pullup on pin
int io_mcp23s17::setPinPullup(int devID, int pin, bool level)
{    
    uint8_t reg, hold;
    
    if(pin < 8)
    {
        reg = MCP23S17_GPPUA;
    }else if(pin < 16)
    {
        reg = MCP23S17_GPPUB;
    }else
    {
        return -1;
    }
    
    uint8_t mask = MCP23S17_GPIO(pin);
    
    hold = readByte(devID, reg);
    
    if(level)
    {
        hold |= mask;
    }else
    {
        hold &= (~mask);
    }
    return writeByte(devID, reg, hold);
}