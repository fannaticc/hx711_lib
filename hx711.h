/* 
 * File:   hx711.h
 * Author: Raphael
 *
 * Created on June 2, 2019, 9:16 PM
 */

#ifndef HX711_H
#define	HX711_H

#include <stdint.h>
#include <stdbool.h>

#include "mcc_generated_files/device_config.h"
#include <xc.h>

#define sckTris TRISBbits.TRISB6
#define doutTris TRISBbits.TRISB4

#define sckPin PORTBbits.RB6
#define doutPin PORTBbits.RB4

#define sckLat LATBbits.LATB6
#define doutLat LATBbits.LATB4

#define MSBFIRST 0
#define LSBFIRST 1


// Ported from: https://github.com/bogde/HX711
// By: Raphael Pereira
// Reference: https://github.com/pierre-muth/pilogger/tree/master/wireless_probe/PIC16F1825
//            https://pierremuth.wordpress.com/tag/pic-microchip/


uint8_t GAIN = 3; // amplification factor
long OFFSET = 0; // used for tare weight
float SCALE = 1; // used to return weight in grams, kg, ounces, whatever


// Initialize library with data output pin, clock input pin and gain factor.
// Channel selection is made by passing the appropriate gain:
// - With a gain factor of 64 or 128, channel A is selected
// - With a gain factor of 32, channel B is selected
// The library default is "128" (Channel A).
void begin(uint8_t dout, uint8_t pd_sck, uint8_t gain); //void begin(uint8_t dout, uint8_t pd_sck, uint8_t gain = 128);

// Check if HX711 is ready
// from the datasheet: When output data is not ready for retrieval, digital output pin DOUT is high. Serial clock
// input PD_SCK should be low. When DOUT goes to low, it indicates data is ready for retrieval.
bool HX711_is_ready();

// Wait for the HX711 to become ready
void HX711_wait_ready(unsigned long time); //void wait_ready(unsigned long delay_ms = 0);
bool HX711_wait_ready_retry(int retries, unsigned long time); //bool wait_ready_retry(int retries = 3, unsigned long delay_ms = 0);
bool HX711_wait_ready_timeout(unsigned long timeout, unsigned long time); //bool wait_ready_timeout(unsigned long timeout = 1000, unsigned long delay_ms = 0);

// set the gain factor; takes effect only after a call to read()
// channel A can be set for a 128 or 64 gain; channel B has a fixed 32 gain
// depending on the parameter, the channel is also set to either A or B
void HX711_set_gain(uint8_t gain); //void set_gain(uint8_t gain = 128);

// waits for the chip to be ready and returns a reading
int32_t HX711_read();

// returns an average reading; times = how many times to read
float HX711_read_average(uint8_t times); //long read_average(uint8_t times = 10);

// returns (read_average() - OFFSET), that is the current value without the tare weight; times = how many readings to do
float HX711_get_value(uint8_t times); //double get_value(uint8_t times = 1);

// returns get_value() divided by SCALE, that is the raw value divided by a value obtained via calibration
// times = how many readings to do
float HX711_get_units(uint8_t times); //float get_units(uint8_t times = 1);

// set the OFFSET value for tare weight; times = how many times to read the tare value
void HX711_tare(uint8_t times); //void tare(uint8_t times = 10);

// set the SCALE value; this value is used to convert the raw data to "human readable" data (measure units)
void HX711_set_scale(float scale); //void set_scale(float scale = 1.f);

// get the current SCALE
float HX711_get_scale();

// set OFFSET, the value that's subtracted from the actual reading (tare weight)
void HX711_set_offset(long offset); //void set_offset(long offset = 0);

// get the current OFFSET
long HX711_get_offset();

// puts the chip into power down mode
void HX711_power_down();

// wakes up the chip after power down mode
void HX711_power_up();

int32_t HX711_read_new();

uint8_t shiftIn(uint8_t bitOrder);

#endif	/* HX711_H */

