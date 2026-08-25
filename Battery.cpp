#include "Battery.h"

uint16_t getBatteryVoltage()
{
    int adcin = PIN_BAT_ADC;
    int adcvalue = 0;
    float mv_per_lsb = 3000.0F / 4096.0F; // 12-bit ADC with 3.0V input range
    analogReference(AR_INTERNAL_3_0);
    analogReadResolution(12);
    pinMode(PIN_BAT_ADC_CTL, OUTPUT);
    digitalWrite(PIN_BAT_ADC_CTL, 1);
    delay(10);
    adcvalue = analogRead(adcin);
    digitalWrite(PIN_BAT_ADC_CTL, 0);
    uint16_t v = (uint16_t)((float)adcvalue * mv_per_lsb * BAT_AMPLIFY);
    Serial.printf("battry volt : %d\r\n", v);
    delay(1000);
    return v;
}

String getBatteryStr()
{
    return String(getBatteryVoltage()) + " mV";
}
