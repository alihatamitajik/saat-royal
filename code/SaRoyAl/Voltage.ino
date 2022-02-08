#define ANALOG_IN_PIN A0

// Floats for ADC voltage & Input voltage
float adc_voltage = 0.0;
 
// Float for Reference Voltage
float ref_voltage = 3.3;
 
// Integer for ADC value
int adc_value = 0;

float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float getBatterVoltage(){
   // Read the Analog Input
   adc_value = analogRead(ANALOG_IN_PIN);
   
   // Determine voltage at ADC input
   adc_voltage  = (adc_value * ref_voltage) / 1024.0; 
   
   // Calculate voltage at divider input
   return adc_voltage / 0.2 ;
   
}

/*
 * based on chart in 
 * https://lygte-info.dk/pic/BatteryChargePercent/BatteryChargePercent.png
 */
uint8_t getBatteryPercentage() {
  float voltage = getBatterVoltage();
  if (voltage >= 4.2) {
    return 100;
  } else if (voltage >= 4.1) {
    return (uint8_t)mapf(voltage, 4.1, 4.2, 94.0, 100.0);
  } else if (voltage >= 4.0) {
    return (uint8_t)mapf(voltage, 4.0, 4.1, 83.0, 94.0);
  } else if (voltage >= 3.9) {
    return (uint8_t)mapf(voltage, 3.9, 4.1, 72.0, 83.0);
  } else if (voltage >= 3.8) {
    return (uint8_t)mapf(voltage, 3.8, 3.9, 59.0, 72.0);
  } else if (voltage >= 3.7) {
    return (uint8_t)mapf(voltage, 3.7, 3.8, 50.0, 59.0);
  } else if (voltage >= 3.6) {
    return (uint8_t)mapf(voltage, 3.6, 3.7, 33.0, 50.0);
  } else if (voltage >= 3.5) {
    return (uint8_t)mapf(voltage, 3.5, 3.6, 15.0, 33.0);
  } else if (voltage >= 3.4) {
    return (uint8_t)mapf(voltage, 3.8, 3.9, 6.0, 15.0);
  } else return 0;
}

void updateBatteryPercentage() {
  batteryPercent = getBatteryPercentage()*60/100;
}
