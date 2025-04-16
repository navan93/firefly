#ifndef _BSP_H_
#define _BSP_H_

#define LED_PIN         3 // LED is placed on the PA3 pin (Port A, Bit 3), which is also TM2PWM
#define ALS_SENSE_PIN   4 // Ambient Light Sensor is placed on the PA4 pin (Port A, Bit 4)
#define ALS_PWR_PIN     6 // Ambient Light Sensor Power is placed on the PA6 pin (Port A, Bit 6)

// Timer16 configuration values for each delay based on the ILRC clock 55kHz
#define T16_CFG_4_65MS        (T16M_CLK_ILRC | T16M_CLK_DIV1 | T16M_INTSRC_8BIT)    // 4.65ms
#define T16_CFG_9_31MS        (T16M_CLK_ILRC | T16M_CLK_DIV1 | T16M_INTSRC_9BIT)    // 9.31ms
#define T16_CFG_18_62MS       (T16M_CLK_ILRC | T16M_CLK_DIV1 | T16M_INTSRC_10BIT)   // 18.62ms
#define T16_CFG_37_24MS       (T16M_CLK_ILRC | T16M_CLK_DIV1 | T16M_INTSRC_11BIT)   // 37.24ms
#define T16_CFG_74_47MS       (T16M_CLK_ILRC | T16M_CLK_DIV1 | T16M_INTSRC_12BIT)   // 74.47ms
#define T16_CFG_148_95MS      (T16M_CLK_ILRC | T16M_CLK_DIV1 | T16M_INTSRC_13BIT)   // 148.95ms
#define T16_CFG_297_89MS      (T16M_CLK_ILRC | T16M_CLK_DIV1 | T16M_INTSRC_14BIT)   // 297.89ms
#define T16_CFG_595_78MS      (T16M_CLK_ILRC | T16M_CLK_DIV1 | T16M_INTSRC_15BIT)   // 595.78ms
#define T16_CFG_1191_56MS     (T16M_CLK_ILRC | T16M_CLK_DIV4 | T16M_INTSRC_14BIT)   // 1.19156s
#define T16_CFG_2383_13MS     (T16M_CLK_ILRC | T16M_CLK_DIV4 | T16M_INTSRC_15BIT)   // 2.38313s
#define T16_CFG_4766_25MS     (T16M_CLK_ILRC | T16M_CLK_DIV16 | T16M_INTSRC_14BIT)  // 4.76625s
#define T16_CFG_9532_51MS     (T16M_CLK_ILRC | T16M_CLK_DIV16 | T16M_INTSRC_15BIT)  // 9.53251s
#define T16_CFG_19065_02MS    (T16M_CLK_ILRC | T16M_CLK_DIV64 | T16M_INTSRC_14BIT)  // 19.06502s
#define T16_CFG_38130_04MS    (T16M_CLK_ILRC | T16M_CLK_DIV64 | T16M_INTSRC_15BIT)  // 38.13s

#define T16_CFG_STR(x)    T16_CFG_##x
#define LPM_SLEEP(x)      T16M = T16_CFG_STR(x); T16C = 0; __stopexe(); T16M = T16M_CLK_DISABLE;

#endif
