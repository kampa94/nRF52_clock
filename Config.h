#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

/* Alimentazione periferiche */
#define SOC_GPIO_PIN_T114_ADC_EN 6
#define SOC_GPIO_PIN_T114_VEXT_EN 21

/* TFT */
#define TFT_HEIGHT 135
#define TFT_WIDTH 240
#define SOC_GPIO_PIN_T114_TFT_SS 11
#define SOC_GPIO_PIN_T114_TFT_DC 12
#define SOC_GPIO_PIN_T114_TFT_RST 2
#define SOC_GPIO_PIN_T114_TFT_EN 3
#define SOC_GPIO_PIN_T114_TFT_BLGT 15

/* GPS */
#define PIN_GPS_EN 21
#define PIN_GPS_RESET 38
#define GPS_BAUD 9600

/* BUTTONS */
#define BUTTON_PIN (32 + 10)
#endif