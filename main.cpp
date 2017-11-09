/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: oneso
 *
 * Created on September 26, 2017, 3:10 PM
 */

#include <cstdlib>
#include <iostream>

#include "RPIO.h"
#include "IO/IO_spi.h"
#include "IO/IO_mcp23s17.h"
#include "IOManager.h"

using namespace std;


int mainLoop();
/*
 * 
 */
int main(int argc, char** argv) 
{
    IOManager::Init();
    cout << "Started" << endl;
    
    return mainLoop();
}

int mainLoop()
{
    while(1)
    IOManager::Update();
}