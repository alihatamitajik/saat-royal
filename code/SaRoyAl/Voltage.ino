#define BATTERY_VOLTAGE A0

uint8_t getBatterVoltage(){
  // Should be tested and implemented
  // I dont know how is the maximum and should be examined when full charged
  // but the minimum was 240
  return analogRead(BATTERY_VOLTAGE);
}

void updateBattery() {
   // should be implemented using charge chart
   RemoteXY.battery_level = 80;
}
