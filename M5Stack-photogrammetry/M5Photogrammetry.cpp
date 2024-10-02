#include "M5Photogrammetry.h"

M5Photogrammetry::M5Photogrammetry()
{
  bleKeyboard.setName(PHOTOGRAMMETRY_PROJECT);  
}

void M5Photogrammetry::bluetoothStart()
{
  bleKeyboard.begin();                                  // Initialize Bluetooth
}

bool M5Photogrammetry::bluetoothIsConnected()
{
  return bleKeyboard.isConnected();                     // test if bluetooth is connected
}

void M5Photogrammetry::bluetoothTakePhoto()             // Press Return to take a photo
{
  if (bleKeyboard.isConnected())
  {
    bleKeyboard.write(KEY_RETURN);
  }
}

void M5Photogrammetry::bluetoothStop()                  // Stop Bluetooth
{
  bleKeyboard.end();
}

void M5Photogrammetry::i2cWrite(uint8_t _data) {
  Wire.beginTransmission(PHOTOGRAMMETRY_I2C_ADDR);  // Starting transmission
  Wire.write(_data);                                // Sending data
  Wire.endTransmission();                           // Ending transmission
}

void M5Photogrammetry::i2cToggle(uint8_t _data)
{
    ic2_data=ic2_data ^ _data;
    i2cWrite(ic2_data);  
}

void M5Photogrammetry::i2cOn(uint8_t _data)
{
    ic2_data=ic2_data & ~_data;
    i2cWrite(ic2_data); 
}

void M5Photogrammetry::i2cOff(uint8_t _data)
{
    ic2_data=ic2_data | _data;
    i2cWrite(ic2_data); 
}

void M5Photogrammetry::ledGreenToggle()
{
    i2cToggle(PHOTOGRAMMETRY_LED_GREEN);
}

void M5Photogrammetry::ledGreenOn()
{
    i2cOn(PHOTOGRAMMETRY_LED_GREEN);
}

void M5Photogrammetry::ledGreenOff()
{
    i2cOff(PHOTOGRAMMETRY_LED_GREEN);
}

void M5Photogrammetry::ledRedToggle()
{
    i2cToggle(PHOTOGRAMMETRY_LED_RED);
}

void M5Photogrammetry::ledRedOn()
{
    i2cOn(PHOTOGRAMMETRY_LED_RED);
}

void M5Photogrammetry::ledRedOff()
{
    i2cOff(PHOTOGRAMMETRY_LED_RED);
}

void M5Photogrammetry::jackFocusToggle()
{
  i2cToggle(PHOTOGRAMMETRY_JACK_FOCUS);
}

void M5Photogrammetry::jackFocusPress()
{
  i2cOn(PHOTOGRAMMETRY_JACK_FOCUS);
}

void M5Photogrammetry::jackFocusRelease()
{
  i2cOff(PHOTOGRAMMETRY_JACK_FOCUS);
}

void M5Photogrammetry::jackShootToggle()
{
  i2cToggle(PHOTOGRAMMETRY_JACK_SHOOT);
}

void M5Photogrammetry::jackShootPress()
{
  i2cOff(PHOTOGRAMMETRY_JACK_SHOOT);
}

void M5Photogrammetry::jackShootRelease()
{
  i2cOn(PHOTOGRAMMETRY_JACK_SHOOT);
}

void M5Photogrammetry::flucardStart()
{
  wifiMulti.addAP(PHOTOGRAMMETRY_FLUCARD_SSID, PHOTOGRAMMETRY_FLUCARD_PASSWORD);  
}

bool M5Photogrammetry::flucardIsConnected()
{
  return (wifiMulti.run() ==  WL_CONNECTED);
}

void M5Photogrammetry::flucardFocusToggle()
{
  httpClient.begin(PHOTOGRAMMETRY_FLUCARD_FOCUS);
  httpClient.GET();
  httpClient.end();
}

void M5Photogrammetry::flucardShootToggle()
{  
  httpClient.begin(PHOTOGRAMMETRY_FLUCARD_SHOOT);
  httpClient.GET();
  httpClient.end();
}

void M5Photogrammetry::flucardStop()
{
  
}

void M5Photogrammetry::begin()
{
  ledGreenOn(); 
  ledRedOff();
}

void M5Photogrammetry::motorForward()
{
  motor_forward=true;
}

void M5Photogrammetry::motorBackward()
{
  motor_forward=false;
}

void M5Photogrammetry::motorNextStep()
{  
  if (motor_forward)
  { 
    motor_current_step++;
    if (motor_current_step>=PHOTOGRAMMETRY_MOTOR_STEPS)
    {
      motor_current_step=0;
    }
  }
  else
  {
    if (motor_current_step==0)
    {
      motor_current_step=PHOTOGRAMMETRY_MOTOR_STEPS;
    }
    motor_current_step--;
  }
  ic2_data=motor_steps[motor_current_step];
  i2cWrite(ic2_data);
}

