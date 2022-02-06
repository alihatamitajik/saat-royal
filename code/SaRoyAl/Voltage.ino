#define BATTERY_VOLTAGE A0

uint8_t getBatterVoltage(){
  return analogRead(BATTERY_VOLTAGE);
}
