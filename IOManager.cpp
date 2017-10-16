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

#include <string.h>
#include <string>

#include "RPIO.h"
#include "IO/IO_spi.h"
#include "IO/IO_mcp23s17.h"


bool IOManager::initialized = false;
IOManager::Pin IOManager::pins[IOManager::NodesSize * IOManager::PinsSize] = {};

//call this first
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
                pp.mode = PINMODE::UNUSED;
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
        if(pp->mode != PINMODE::UNUSED && pp->needsUpdate)
        {
            
            io_mcp23s17::setPinMode(pp->devID, (i & 0x0F), pp->logicLevel);
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
    
}

void IOManager::setPinMode(int pin, PINMODE mode)
{
    
}

void IOManager::setPinPullup(int pin, bool do_pullup)
{
    
}

void IOManager::setPinLogicL(int pin, bool level)
{
    
}

//TODO: Change to const Pin &p
bool IOManager::pinCondition(const Pin* p)
{
    //pullup will only activate if the GPPU register bit is
    //set and port configured as an input
    if(!(p->mode == PINMODE::UNUSED)){
        if(p->mode == PINMODE::INPUT && p->logicLevel == true){
            return false;
        }else if(p->mode == PINMODE::OUTPUT && p->logicLevel == true){
            if(p->doPullup == true){
                return false;
            }
        }
    }
}