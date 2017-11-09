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

#include "IO_spi.h"

#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "../RPIO.h"

//adding static limits the scope to this file
static const char spiDevice0[] = "/dev/spidev0.0";
static const char spiDevice1[] = "/dev/spidev0.1";

static int fileRef = 0;
static const unsigned int spiBitsPerWord = 8;
static unsigned int speedHz = 0;

int io_spi::spiSetup(unsigned int channel, unsigned int speed, unsigned int mode)
{
    int file;
    int out = 0;

    mode &= 3;//0 1 2 or 3
    channel &= 1;//0 or 1
    speedHz = speed;
    
    //opens the device
    file = open(channel == 0 ? spiDevice0 : spiDevice1, O_RDWR);
    if(file < 0)
    {
        RPIO_Logging::RPIOERROR(RPIO_Logging::FATALITY::ERR_FATAL, 
                "Could not open SPI device. " + std::string(strerror(errno)));
    }
    
    out = ioctl(file, SPI_IOC_WR_MODE, &mode);
    if(out != 0)
    {
        RPIO_Logging::RPIOERROR(RPIO_Logging::FATALITY::ERR_FATAL, 
                "Could not set SPI logic mode. " + std::string(strerror(errno)));
        return -1;
    }
    
    out = ioctl(file, SPI_IOC_WR_BITS_PER_WORD, &spiBitsPerWord);
    if(out != 0)
    {
        RPIO_Logging::RPIOERROR(RPIO_Logging::FATALITY::ERR_FATAL, 
                "Could not set SPI bits per word. " + std::string(strerror(errno)));
        return -1;
    }
    
    out = ioctl(file, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if(out != 0)
    {
        RPIO_Logging::RPIOERROR(RPIO_Logging::FATALITY::ERR_FATAL, 
                "Could not set SPI speed. " + std::string(strerror(errno)));
        return -1;
    }
    fileRef = file;
   
    return 0;
}

//sends data to spi and reads whatever comes back
//overwrites data in transmit buffer
int io_spi::spiReadWrite(unsigned char *data, unsigned int len)
{   
    struct spi_ioc_transfer spi = {};

    spi.bits_per_word = spiBitsPerWord;
    spi.delay_usecs = 0;
    spi.tx_buf = (unsigned long)data;
    spi.rx_buf = (unsigned long)data;
    spi.len = len;
    spi.speed_hz = speedHz;

    return ioctl(fileRef, SPI_IOC_MESSAGE(1), &spi);
}