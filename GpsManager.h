#ifndef GPS_MANAGER_H
#define GPS_MANAGER_H

#include <TinyGPSPlus.h>
#include "Config.h"

extern TinyGPSPlus gps;

void setupGps();
void processGpsData();
String getTimeStr();
String getDateStr();
String getLatStr();
String getLonStr();
String getAltStr();
String getSatStr();
#endif