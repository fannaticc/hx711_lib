/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <xc.h>
#include "spi_driver.h"

#pragma warning disable 520     

void (*spi_interruptHandler)(void);   

inline void spi_close(void)
{
    SSP1CON1bits.SSPEN = 0;
}

//con1 == SSPxCON1, stat == SSPxSTAT, add == SSPxADD, operation == Master/Slave
typedef struct { uint8_t con1; uint8_t stat; uint8_t add; uint8_t operation; } spi_configuration_t;
static const spi_configuration_t spi_configuration[] = {
    { 0xa, 0x40, 0x1, 0 },
    { 0xa, 0x0, 0x1, 0 }
};

//Setup SPI
bool spi_open(spi_modes spiUniqueConfiguration)
{
    if(!SSP1CON1bits.SSPEN)
    {
        //setup PPS pins
        SSPCLKPPS = 14;
        SSPDATPPS = 13;
        RB6PPS = 16;

        //setup SPI
        SSP1STAT = spi_configuration[spiUniqueConfiguration].stat;
        SSP1CON1 = (uint8_t)(spi_configuration[spiUniqueConfiguration].con1 | 0x20);
        SSP1CON2 = 0x00;
        SSP1ADD  = (uint8_t)(spi_configuration[spiUniqueConfiguration].add);

        TRISBbits.TRISB6 = (uint8_t)(spi_configuration[spiUniqueConfiguration].operation);
        return true;
    }
    return false;
}

// Full Duplex SPI Functions
uint8_t spi_exchangeByte(uint8_t b)
{
    SSP1BUF = b;
    while(!PIR1bits.SSP1IF);
    PIR1bits.SSP1IF = 0;
    return SSP1BUF;
}

void spi_exchangeBlock(void *block, size_t blockSize)
{
    uint8_t *data = block;
    while(blockSize--)
    {
        *data = spi_exchangeByte(*data );
        data++;
    }
}

// Half Duplex SPI Functions
void spi_writeBlock(void *block, size_t blockSize)
{
    uint8_t *data = block;
    while(blockSize--)
    {
        spi_exchangeByte(*data++);
    }
}

void spi_readBlock(void *block, size_t blockSize)
{
    uint8_t *data = block;
    while(blockSize--)
    {
        *data++ = spi_exchangeByte(0);
    }
}

void spi_writeByte(uint8_t byte){
    SSP1BUF = byte;
}

uint8_t spi_readByte(void){
    return SSP1BUF;
}

/**
 * Interrupt from MSSP on bit 8 received and SR moved to buffer
 * If interrupts are not being used, then call this method from the main while(1) loop
 */
void spi_isr(void){
    if(PIR1bits.SSP1IF == 1){
        if(spi_interruptHandler){
            spi_interruptHandler();
        }
        PIR1bits.SSP1IF = 0;
    }
}

void spi_setSpiISR(void(*handler)(void)){
    spi_interruptHandler = handler;
}