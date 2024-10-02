/* Photogrammetry 3D-scanner controlled by M5Stack / M5StickC /M5Atom
 *
 * Connect the M5Stack to the electronic card thanks to port A (I2C)
 * Configure the I2C address, by default 0x20, you can configure the address thanks to microswitch on the electronic card
 * if all micro-switches are on address will be 0x20
 * You can foncigure I2C address thanks to
 * #define PHOGRAMMETRY_I2C_ADDR 0x20
 *
 * 8 bit will be sent to the electronic card:
 * - bit 0 => Focus   on external camera (set to 0 to activate)
 * - bit 1 => shutter on external camera (set to 0 to activate)
 * - bit 2 => Green Led (set to 0 to activate)
 * - bit 3 => Red   Led (set to 0 to activate) 
 * - bit 4 to 7 => stepper controller (set to 1 to activate)
 *
 *
 */

// Library to use for bluetooth: https://github.com/T-vK/ESP32-BLE-Keyboard
// M5ez https://community.m5stack.com/topic/4578/cant-compile-m5ez-h/6

#ifndef M5Photogrammetry_h
#define M5Photogrammetry_h

#include <BleKeyboard.h>
#include <Wire.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>

#define PHOTOGRAMMETRY_PROJECT        "Photogrammetry"
#define PHOTOGRAMMETRY_VERSION        "1.0.0"
#define PHOTOGRAMMETRY_AUTHOR         "Sebastien Colas"

#define PHOTOGRAMMETRY_MOTOR_STEPS 8
#define PHOTOGRAMMETRY_MOTOR_ROTATION 28712


  #ifndef PHOTOGRAMMETRY_I2C_ADDR
    #define PHOTOGRAMMETRY_I2C_ADDR            0x20  
  #endif

class M5Photogrammetry
{
  public:
    // Constructor
    M5Photogrammetry();

    void begin();

    // Functions for Bluetooth
    void bluetoothStart();
    bool bluetoothIsConnected();
    void bluetoothTakePhoto();
    void bluetoothStop();

    // Functions for Pentax Flucard
    void flucardStart();
    bool flucardIsConnected();
    void flucardFocusToggle();
    void flucardShootToggle();
    void flucardStop();

    // functions for Leds
    void ledGreenToggle();
    void ledGreenOn();
    void ledGreenOff();
    void ledRedToggle();
    void ledRedOn();
    void ledRedOff();

    // functions for jack
    void jackFocusToggle();
    void jackFocusPress();
    void jackFocusRelease();
    void jackShootToggle();    
    void jackShootPress();
    void jackShootRelease();

    // functions for stepper motor
    void motorForward();
    void motorBackward();
    void motorNextStep();

  private:
    static const uint8_t PHOTOGRAMMETRY_JACK_FOCUS = 1; // B00000001  
    static const uint8_t PHOTOGRAMMETRY_JACK_SHOOT = 2; // B00000010
    static const uint8_t PHOTOGRAMMETRY_LED_GREEN  = 4; // B00000100
    static const uint8_t PHOTOGRAMMETRY_LED_RED    = 8; // B00001000

    const char *PHOTOGRAMMETRY_FLUCARD_SSID      = "FLUCARD_for_PENTAX";
    const char *PHOTOGRAMMETRY_FLUCARD_PASSWORD  = "12345678";
    const char *PHOTOGRAMMETRY_FLUCARD_SHOOT     = "http://192.168.1.1/cgi-bin/host_operation?action=shutter&seq=00000000";
    const char *PHOTOGRAMMETRY_FLUCARD_FOCUS     = "http://192.168.1.1/cgi-bin/host_operation?action=af&x=50&y=50&seq=00000000";

    uint8_t ic2_data=0;
    BleKeyboard bleKeyboard;
    WiFiMulti wifiMulti;
    HTTPClient httpClient;

    bool      motor_forward=true;
    uint32_t  motor_current_step=0;

    void i2cWrite(uint8_t _data);
    void i2cToggle(uint8_t _data);
    void i2cOn(uint8_t _data);
    void i2cOff(uint8_t _data);  

    uint8_t motor_steps[PHOTOGRAMMETRY_MOTOR_STEPS]=
    {
      132, // B10000100,
      196, // B11000100,
      68,  // B01000100,
      100, // B01100100,
      36,  // B00100100,
      52,  // B00110100,
      20,  // B00010100,
      148  // B10010100  
    };

};


#endif
