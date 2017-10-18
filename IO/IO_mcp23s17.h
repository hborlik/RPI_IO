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

/* 
 * File:   RPIO_mcp23s17.h
 * Author: oneso
 *
 * Created on September 27, 2017, 8:09 AM
 */

#ifndef IO_MCP23S17_H
#define IO_MCP23S17_H

#include "IO_spi.h"

//Register addresses 

#define MCP23S17_IODIRA             0x00
#define MCP23S17_IODIRB             0x01

#define MCP23S17_IPOLA              0x02
#define MCP23S17_IPOLB              0x03

#define MCP23S17_GPINTENA           0x04
#define MCP23S17_GPINTENB           0x05

#define MCP23S17_DEFVALA            0x06
#define MCP23S17_DEFVALB            0x07

#define MCP23S17_INTCONA            0x08
#define MCP23S17_INTCONB            0x09

#define MCP23S17_IOCON              0x0A
#define MCP23S17_IOCONB             0x0B

#define MCP23S17_GPPUA              0x0C
#define MCP23S17_GPPUB              0x0D

#define MCP23S17_INTFA              0x0E
#define MCP23S17_INTFB              0x0F

#define MCP23S17_INTCAPA            0x10
#define MCP23S17_INTCAPB            0x12

#define MCP23S17_GPIOA              0x13
#define MCP23S17_GPIOB              0x14

#define MCP23S17_OLATA              0x15
#define MCP23S17_OLATB              0x16

//IOCON register
/*
bit 7 BANK: Controls how the registers are addressed
1 = The registers associated with each port are separated into different banks.
0 = The registers are in the same bank (addresses are sequential).
bit 6 MIRROR: INT Pins Mirror bit
1 = The INT pins are internally connected
0 = The INT pins are not connected. INTA is associated with PORTA and INTB is associated with
PORTB
bit 5 SEQOP: Sequential Operation mode bit
1 = Sequential operation disabled, address pointer does not increment.
0 = Sequential operation enabled, address pointer increments.
bit 4 DISSLW: Slew Rate control bit for SDA output
1 = Slew rate disabled
0 = Slew rate enabled
bit 3 HAEN: Hardware Address Enable bit (MCP23S17 only) (Note 1)
1 = Enables the MCP23S17 address pins.
0 = Disables the MCP23S17 address pins.
bit 2 ODR: Configures the INT pin as an open-drain output
1 = Open-drain output (overrides the INTPOL bit.)
0 = Active driver output (INTPOL bit sets the polarity.)
bit 1 INTPOL: This bit sets the polarity of the INT output pin
1 = Active-high
0 = Active-low
bit 0 Unimplemented: Read as ‘0’
 */

#define MCP23S17_IOCON_BANK             0x07
#define MCP23S17_IOCON_MIRROR           0x06
#define MCP23S17_IOCON_SEQOP            0x05
#define MCP23S17_IOCON_DISSLW           0x04
#define MCP23S17_IOCON_HAEN             0x03
#define MCP23S17_IOCON_ODR              0x02
#define MCP23S17_IOCON_INTPOL           0x01
#define MCP23S17_IOCON_UNUSED           0x00

//GPIO pins
/*
 The GPIO register reflects the value on the port.
Reading from this register reads the port. Writing to this
register modifies the Output Latch (OLAT) register.
 */
//creates pin number that is 0 through 7
#define MCP23S17_GPIO(pin)              (1 << (pin & 7))

//SPI commands

#define MCP23S17_HARDWARE_ADDR          0x40

#define MCP23S17_CMD_WRITE              0x00
#define MCP23S17_CMD_READ               0x01

//

#define MCP23S17_PIN_HIGH               0x01
#define MCP23S17_PIN_LOW                0x00

#define MCP23S17_MAX_SPEED              4000000

class io_mcp23s17
{
public:
    enum class OUTPUTMODE {
        OUTPUT,
        INPUT
    };
    
    static int setupmcp23s17(int devID);
    static int setPinMode(int devID, int pin, OUTPUTMODE mode);
    static int setPinLogic(int devID, int pin, bool level);
    static int setPinPullup(int devID, int pin, bool level);
};

#endif /* IO_MCP23S17_H */

