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
 * File:   IOManager.h
 * Author: oneso
 *
 * Created on September 29, 2017, 12:31 PM
 */

#ifndef IOMANAGER_H
#define IOMANAGER_H

#include "IO/IO_mcp23s17.h"

class IOManager
{
public:
    static constexpr unsigned short NodesSize = 8;
    static constexpr unsigned short PinsSize = 16;
    static constexpr unsigned short channel = 0;
    
    typedef int (*PINCHG_FUNC)(bool state);
    
    static void Init();
    static void Update();
    static void Shutdown();
    ~IOManager(){
        Shutdown();
    }
    
    static void setPinFunc(int pin, PINCHG_FUNC func);
    static void setPinMode(int pin, io_mcp23s17::OUTPUTMODE mode);
    static void setPinPullup(int pin, bool do_pullup);
    static void setPinLogicL(int pin, bool level);
       
private:
    
    typedef struct Pin{
        unsigned short devID;
        unsigned short channel;
        io_mcp23s17::OUTPUTMODE mode;
        PINCHG_FUNC func;
        bool needsUpdate;
        bool doPullup;
        bool logicLevel;
    } Pin;
    
    //disallow instancing of this class
    IOManager();
    
    //
    static bool pinCondition(const Pin *p);
        
    static bool initialized;
    
    //all pins
    static Pin pins[NodesSize * PinsSize];
};

#endif /* IOMANAGER_H */

