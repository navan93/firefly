/*
  FadeLED

  Uses PWM to fade an LED in and out, repeatedly.

  PFS154 pinout

                      +-\/-+
                VDD  1|    |8  GND
             PA7/X1  2|    |7  PA0/AD10/CO/INT0/PG0PWM
             PA6/X2  3|    |6  PA4/AD9/CIN+/CIN1-/INT1/PG1PWM
   PA5/PRSTB/PG2PWM  4|    |5  PA3/AD8/CIN0-/TM2PWM/PG2PWM
                      +----+

  https://www.padauk.com.cn/upload/doc/PFS154%20datasheet_EN_V107_20240912.pdf
*/

#include <stdint.h>
#include <pdk/device.h>
#include "auto_sysclock.h"
#include "startup.h"
#include "delay.h"
#include "bsp.h"
#include "serial.h"

#define PWM_MAX               255

// ALS is connected in active low configuration
#define turn_als_on()         (PA &= ~(1 << ALS_PWR_PIN))
#define turn_als_off()        (PA |= (1 << ALS_PWR_PIN))

// LED is connected in active low configuration
#define turn_led_on()         (PA &= ~(1 << LED_PIN))
#define turn_led_off()        (PA |= (1 << LED_PIN))

// ALS will read high in dark, so define helper for better readability below
#define isDark()    (PA & (1 << ALS_SENSE_PIN))

void interrupt(void) __interrupt(0) {
  if (INTRQ & INTRQ_TM2) {      // TM2 interrupt request?
    INTRQ &= ~INTRQ_TM2;        // Mark TM2 interrupt request processed
    serial_irq_handler();       // Process next Serial Bit
  }
}

static inline void fade_led(void)
{
  uint8_t fadeValue;

  // Fade in from min to max in increments of 5
  for (fadeValue = 0; fadeValue < PWM_MAX; fadeValue += 5) {
    TM2B = fadeValue;         // Set the LED PWM duty value
    _delay_ms(30);              // wait for 30 milliseconds to see the dimming effect
  }

  // Fade out from max to min in increments of 5
  for (fadeValue = PWM_MAX; fadeValue > 0; fadeValue -= 5) {
    TM2B = fadeValue;         // Set the LED PWM duty value
    _delay_ms(30);              // wait for 30 milliseconds to see the dimming effect
  }
}

static inline void pwm_init(void)
{
  TM2B = 0x00;                    // Clear the LED PWM duty value
  TM2C = (uint8_t)(TM2C_INVERT_OUT | TM2C_MODE_PWM | TM2C_OUT_PA3 | TM2C_CLK_IHRC);
  TM2S = 0x00;                    // No pre-scaler, 8-bit
}

static inline void comparator_init(void)
{
  // page 67 datasheet
  GPCC = (uint8_t)(GPCC_COMP_PLUS_VINT_R | GPCC_COMP_MINUS_PA4 | GPCC_COMP_ENABLE);
  // bit 7 enable comparator
  // bit 6 plus input < minus input
  // bit 5 result output NOT sampled by TM2_CLK
  // bit 4 polarity is NOT inversed
  // bit 3-1 000 : PA3 selected as -ve input
  // bit 0 internal voltage set as +ve input

  // GPCS = (uint8_t)(GPCS_COMP_RANGE3);
  // bit 7 output to PA0 disabled
  // bit 6 reserved
  // bit 5 high range selected
  // bit 4 low range selected
  // bit 3-0 Selection the internal reference voltage level of comparator
  // 0000 (lowest) ~ 1111 (highest) as a fraction of vdd
}

/* ALS pulls down on PA4 when light is detected,
   with PA4 connected to comparator +ve input, and
   Resistor ladder is connected to comparator -ve input
   we should sweep the ladder from high to low to get the value
*/
uint8_t get_als_value(void)
{
  uint8_t als_value = 0x00;

  while(als_value <= 0x0F) {
    GPCS = (uint8_t)(GPCS_COMP_RANGE4 | als_value);
    _delay_ms(1);
    if (GPCC & GPCC_COMP_RESULT_POSITIVE) { // PA4 < Vref
      break;;
    }
    als_value++;
  }
  return als_value;
}

// Main program
void main() {

  // Initialize hardware
  PAC |= (1 << LED_PIN) | (1 << ALS_PWR_PIN);          // Set LED and ALS power pins as output (all pins are input by default)

  // PADIER |= (1 << ALS_SENSE_PIN);       // Enable ALS as digital input
  // PAPH |= (1 << ALS_SENSE_PIN);         // Enable pull-up resistor for ALS

  // pwm_init();                                          // Initialize the PWM
  comparator_init();

  // Leave ALS ON
  turn_als_on();

  serial_setup();                 // Initialize Serial engine
  INTRQ = 0;
  __engint();                     // Enable global interrupts

  serial_println("Firefly Initialized");


  // Main processing loop
  while (1) {

    uint8_t als_value = get_als_value();


    if (als_value >  10) {
      turn_led_on();
      // serial_println("Is Dark");
    }
    else {
      turn_led_off();
      // serial_println("Not Dark");
    }

    als_value = (als_value < 10) ? (als_value + '0') : (als_value - 10 + 'A');
    serial_println(&als_value);

    _delay_ms(1000);
  }
}

// Startup code - Setup/calibrate system clock
unsigned char STARTUP_FUNCTION(void) {

  // Initialize the system clock (CLKMD register) with the IHRC, ILRC, or EOSC clock source and correct divider.
  // The AUTO_INIT_SYSCLOCK() macro uses F_CPU (defined in the Makefile) to choose the IHRC or ILRC clock source and divider.
  // Alternatively, replace this with the more specific PDK_SET_SYSCLOCK(...) macro from pdk/sysclock.h
  AUTO_INIT_SYSCLOCK();

  // Insert placeholder code to tell EasyPdkProg to calibrate the IHRC or ILRC internal oscillator.
  // The AUTO_CALIBRATE_SYSCLOCK(...) macro uses F_CPU (defined in the Makefile) to choose the IHRC or ILRC oscillator.
  // Alternatively, replace this with the more specific EASY_PDK_CALIBRATE_IHRC(...) or EASY_PDK_CALIBRATE_ILRC(...) macro from easy-pdk/calibrate.h
  AUTO_CALIBRATE_SYSCLOCK(TARGET_VDD_MV);

  return 0;   // Return 0 to inform SDCC to continue with normal initialization.
}