/* 
 * File:   hx711.c
 * Author: Raphael
 *
 * Created on June 2, 2019, 9:16 PM
 */

#include "hx711.h"
#include "mcc_generated_files/device_config.h"
#include "mcc_generated_files/spi_driver.h"

void HX711_begin(uint8_t dout, uint8_t pd_sck, uint8_t gain)
{
    //PD_SCK = pd_sck;
    //DOUT = dout;

    sckTris = 0;
    doutTris = 1;

    HX711_set_gain(gain);
}

bool HX711_is_ready()
{
    return (!doutPin); //digitalRead(DOUT) == LOW;
}

void HX711_set_gain(uint8_t gain)
{
    switch(gain)
    {
        case 128: // channel A, gain factor 128
            GAIN = 1;
            break;
        case 64: // channel A, gain factor 64
            GAIN = 3;
            break;
        case 32: // channel B, gain factor 32
            GAIN = 2;
            break;
    }

    sckLat = 0;
    HX711_read();
}

int32_t HX711_read_new()
{
    // Define structures for reading data into.
    int32_t value = 0;
    uint8_t data[3] = {0};
    uint8_t filler = 0;
    
    // Wait for the chip to become ready.
    while(doutPin);
    
    __delay_us(10);
    
    // Pulse the clock pin 24 times to read the data.
    data[2] = spi_exchangeByte(0); //SHIFTIN_WITH_SPEED_SUPPORT(DOUT, PD_SCK, MSBFIRST);
    data[1] = spi_exchangeByte(0); //SHIFTIN_WITH_SPEED_SUPPORT(DOUT, PD_SCK, MSBFIRST);
    data[0] = spi_exchangeByte(0); //SHIFTIN_WITH_SPEED_SUPPORT(DOUT, PD_SCK, MSBFIRST);

    // Set the channel and the gain factor for the next reading using the clock pin.
    for(unsigned int i = 0; i < GAIN; i++)
    {
        sckPin = 1;
        __delay_us(1);
        sckPin = 0;
        __delay_us(1);
    }

    // Replicate the most significant bit to pad out a 32-bit signed integer
    if(data[2] & 0x80)
    {
        filler = 0xFF;
    }
    else
    {
        filler = 0x00;
    }

    // Construct a 32-bit signed integer
    value = ((uint32_t)(filler) << 24
            | (uint32_t)(data[2]) << 16
            | (uint32_t)(data[1]) << 8
            | (uint32_t)(data[0]));
    
    printf("%03i  ", data[2]);
    printf("%03i  ", data[1]);
    printf("%03i  \n", data[0]);

    return (value);
}

int32_t HX711_read()
{
    int32_t value_32aux;

    union charto32bits
    {
        uint32_t value_32;
        uint8_t value_8[3];
    } value;
    
    // Wait for the chip to become ready.
    while(doutPin);

    __delay_us(1);

    // Pulse the clock pin 24 times to read the data.
    value.value_8[2] = spi_exchangeByte(0);
    value.value_8[1] = spi_exchangeByte(0);
    value.value_8[0] = spi_exchangeByte(0);

    // Set the channel and the gain factor for the next reading using the clock pin.
    for(uint16_t i = 0; i < GAIN; i++)
    {
        sckLat = 1;
        __delay_us(1);
        sckLat = 0;
        __delay_us(1);
    }
    
    // Replicate the most significant bit to pad out a 32-bit signed integer
    if(value.value_8[2] & 0x80)
    {
        value_32aux = value.value_32 * (-1);
    }
    else
    {
        value_32aux = value.value_32;
    }

    return value_32aux;
}

void HX711_wait_ready(uint32_t time)
{
    // Wait for the chip to become ready.
    // This is a blocking implementation and will
    // halt the sketch until a load cell is connected.
    while(!HX711_is_ready())
    {
        // Probably will do no harm on AVR but will feed the Watchdog Timer (WDT) on ESP.
        // https://github.com/bogde/HX711/issues/73
        //__delay_ms(time);
    }
}

bool HX711_wait_ready_retry(int16_t retries, uint32_t time)
{
    // Wait for the chip to become ready by
    // retrying for a specified amount of attempts.
    // https://github.com/bogde/HX711/issues/76
    int count = 0;
    while(count < retries)
    {
        if(HX711_is_ready())
        {
            return true;
        }
        //__delay_ms(time);
        count++;
    }
    return false;
}

bool HX711_wait_ready_timeout(uint32_t timeout, uint32_t time)
{
    // Wait for the chip to become ready until timeout.
    // https://github.com/bogde/HX711/pull/96
    //unsigned long millisStarted = millis();
    /*while(millis() - millisStarted < timeout)
    {
        if(HX711_is_ready())
        {
            return true;
        }
        __delay_ms(time);
    }*/
    return false;
}

float HX711_read_average(uint8_t times)
{
    float sum = 0;
    
    for(uint8_t i = 0; i < times; i++)
    {
        sum += HX711_read();
        __delay_ms(30);
    }
    return (sum / times);
}

float HX711_get_value(uint8_t times)
{
    return HX711_read_average(times) - OFFSET;
}

float HX711_get_units(uint8_t times)
{
    return HX711_get_value(times) / SCALE;
}

void HX711_tare(uint8_t times)
{
    double sum = HX711_read_average(times);
    HX711_set_offset(sum);
}

void HX711_set_scale(float scale)
{
    SCALE = scale;
}

float HX711_get_scale()
{
    return SCALE;
}

void HX711_set_offset(int32_t offset)
{
    OFFSET = offset;
}

int32_t HX711_get_offset()
{
    return OFFSET;
}

void HX711_power_down()
{
    sckLat = 0;
    sckLat = 1;
}

void HX711_power_up()
{
    sckLat = 0;
}

uint8_t shiftIn(uint8_t bitOrder)
{

    uint8_t value = 0;

    uint8_t i;

    for(i = 0; i < 8; ++i)
    {

        sckLat = 1;

        if(bitOrder == LSBFIRST)

            value |= doutPin << i;

        else

            value |= doutPin << (7 - i);

        sckLat = 0;

    }

    return value;

}