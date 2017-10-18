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

#include "IOManager.h"

#include "RPIO.h"

#include <string.h>
#include <string>

#include "IO/IO_spi.h"
#include "IO/IO_mcp23s17.h"


bool IOManager::initialized = false;
IOManager::Pin IOManager::pins[IOManager::NodesSize * IOManager::PinsSize] = {};

void IOManager::Init()
{
    if(!initialized)
    {
        //setup spi interface
        if(io_spi::spiSetup(channel, MCP23S17_MAX_SPEED, 3) != 0)
        {
            RPIO_Logging::RPIOERROR(RPIO_Logging::FATALITY::ERR_NON_FATAL, 
                    "Failed to setup SPI interface. " + std::string(strerror(errno)));
            return;
        }
    
        for(int i = 0; i < NodesSize; ++i)
        {
            //setup the multiplexer
            if(io_mcp23s17::setupmcp23s17(i) != 0)
            {
                RPIO_Logging::RPIOERROR(RPIO_Logging::FATALITY::ERR_NON_FATAL, 
                        "Failed to setup MCP23S17.");
            }
            
            for(int p = 0; p < PinsSize; ++p)
            {
                Pin &pp = pins[i*16+p];
                pp.devID = i;
                pp.channel = channel;
                pp.func = nullptr;
                pp.logicLevel = false;
                pp.doPullup = false;
                pp.needsUpdate = false;
                pp.mode = io_mcp23s17::OUTPUTMODE::OUTPUT;
            }
        }
    }
}

//check if node is active, then update pins if pin update flag is set
void IOManager::Update()
{
    for(int i = 0; i < NodesSize * PinsSize; ++i)
    {
        Pin *pp = &pins[i];
        if(pp->needsUpdate)
        {
            io_mcp23s17::setPinMode(pp->devID, (i & 0x0F), pp->mode);
            io_mcp23s17::setPinLogic(pp->devID, (i & 0x0F), pp->logicLevel);
            pp->needsUpdate = false;
        }
    }
}


void IOManager::Shutdown()
{
    if(initialized)
    {
        //set all gpio states to 0
        //or reset chips
    }
}

void IOManager::setPinFunc(int pin, PINCHG_FUNC func)
{
    pins[pin].func = func;
}

void IOManager::setPinMode(int pin, io_mcp23s17::OUTPUTMODE mode)
{
    pins[pin].mode = mode;
    pins[pin].needsUpdate = true;
}

void IOManager::setPinPullup(int pin, bool do_pullup)
{
    pins[pin].doPullup = do_pullup;
    pins[pin].needsUpdate = true;
}

void IOManager::setPinLogicL(int pin, bool level)
{
    pins[pin].logicLevel = level;
    pins[pin].needsUpdate = true;
}

//TODO: Change to const Pin &p
bool IOManager::pinCondition(const Pin* p)
{
    //pullup will only activate if the GPPU register bit is
    //set and port configured as an input

    if(p->mode == io_mcp23s17::OUTPUTMODE::INPUT && p->logicLevel == true){
        return false;
    }else if(p->mode == io_mcp23s17::OUTPUTMODE::OUTPUT && p->logicLevel == true){
        if(p->doPullup == true){
            return false;
        }
    }
}