/*
 * Photogrammetry 3D-Scanner controlled by M5Stack
 *
 * The current file contains the GUI based on M5ez
 *
 * M5Stack Core (Basic): use Partition scheme: Hudge APP 
 */

#include <M5Stack.h>
#include <M5ez.h>

#include "M5Photogrammetry.h"

M5Photogrammetry m5photogrammetry;

uint8_t nb_photos       = 0;
bool    photos_jack     = false;
bool    photos_bt       = false;
bool    photos_flucard  = false;

/*
 * Function to detect I2C port in used
 */
void menu_tests_i2c()
{
  int address;
  int error;
  ez.header.show("I2C scanning Address");
  ez.canvas.lmargin(5);
  ez.canvas.font(&FreeMono12pt7b);
  ez.canvas.println("");
  for (address = 1; address < 127; address++) {
      Wire.beginTransmission(address);  
      error = Wire.endTransmission(); 
      if (error == 0) {
          ez.canvas.print(address, HEX);
          ez.canvas.print(" ");
      } else
          ez.canvas.print(".");
  }
  ez.buttons.wait("OK");
}

/*
 * Function to test leds
 */
void menu_tests_leds()
{
  ez.header.show("Leds test");
  ez.canvas.lmargin(5);
  ez.canvas.font(&FreeMono12pt7b);
  ez.canvas.println("");
  ez.canvas.println("Press buttons");
  ez.canvas.println("to test leds");
  ez.buttons.show("OK # Green # Red");
  while (true) 
  {
    String btnpressed = ez.buttons.poll();
    if (btnpressed == "OK") break;
    
    if (btnpressed == "Green")
    {
      m5photogrammetry.ledGreenToggle();
    }
    if (btnpressed == "Red")
    {
      m5photogrammetry.ledRedToggle();
    }    
  }
}

/*
 * Function to test if photos can be taken thanks to jack connector
 */
void menu_tests_jack()
{
  ez.header.show("Jack test");
  ez.canvas.lmargin(5);
  ez.canvas.font(&FreeMono12pt7b);
  ez.canvas.println("");
  ez.canvas.println("Press buttons");
  ez.canvas.println("to test leds");
  ez.buttons.show("OK # Focus # Shoot");
  while (true) 
  {
    String btnpressed = ez.buttons.poll();
    if (btnpressed == "OK") break;
    
    if (btnpressed == "Focus")
    {
      m5photogrammetry.jackFocusToggle();
    }
    if (btnpressed == "Shoot")
    {
      m5photogrammetry.jackShootToggle();
    }    
  }
}

/*
 * Function to test if photos can be taken thanks Pentax Flucard
 */
void menu_tests_flucard()
{
  int cmpt=0;
  ez.header.show("Flucard test");
  ez.canvas.lmargin(5);
  ez.canvas.font(&FreeMono12pt7b);
  ez.canvas.println();
  ez.canvas.print("Trying to connect");
  m5photogrammetry.flucardStart();

  while ((cmpt < 20) && (!m5photogrammetry.flucardIsConnected()))  // Waiting for connection
  {
    ez.canvas.print(".");
    cmpt++;
  }
  bool flucard=m5photogrammetry.flucardIsConnected();

  ez.canvas.println();
  if (flucard)
  {    
    ez.canvas.println("Connected");
  }
  else
  {
    ez.canvas.println("Can't connect");
  }

  ez.buttons.show("OK # Focus # Shoot");
  while (true) 
  {
    String btnpressed = ez.buttons.poll();
    if (btnpressed == "OK") break;
    
    if (btnpressed == "Focus")
    {
      m5photogrammetry.flucardFocusToggle();
    }
    if (btnpressed == "Shoot")
    {
      m5photogrammetry.flucardShootToggle();
    }    
  }
}

/*
 * Function to test Bluetooth
 */
void menu_tests_bt()
{
  int cmpt=0;
  ez.header.show("Bluetooth Test");
  ez.canvas.lmargin(5);
  ez.canvas.font(&FreeMono12pt7b);
  ez.canvas.println();
  ez.canvas.print("Trying to connect");
  m5photogrammetry.bluetoothStart();

  while ((cmpt < 200) && (!m5photogrammetry.bluetoothIsConnected()))  // Waiting for connection
  {
    if (cmpt%2==0)
    {
      ez.canvas.print(".");
    }
    delay(100);
    cmpt++;
  }
  bool bluetooth=m5photogrammetry.bluetoothIsConnected();

  ez.canvas.println();
  if (bluetooth)
  {    
    ez.canvas.println("Connected");
  }
  else
  {
    ez.canvas.println("Can't connect");
  }  

  ez.buttons.show("OK # Shoot # OFF");
  while (true) 
  {
    String btnpressed = ez.buttons.poll();
    if (btnpressed == "OK") break;
    
    if (btnpressed == "Shoot") 
    {
      m5photogrammetry.bluetoothTakePhoto();
    }

    if (btnpressed == "OFF")
    {
      m5photogrammetry.bluetoothStop();
    }    
  }
}

void menu_tests_motor()
{
  ez.header.show("Stepper motor test");
  ez.canvas.lmargin(5);
  ez.canvas.font(&FreeMono12pt7b);
  ez.canvas.println("");
  ez.canvas.println("Forward");

  m5photogrammetry.motorForward();
  for (int i=0;i<PHOTOGRAMMETRY_MOTOR_ROTATION;i++)
  {
    m5photogrammetry.motorNextStep();
    delay(1);
  }
  ez.canvas.println("Stop");
  m5photogrammetry.ledGreenOn();
  m5photogrammetry.ledRedOff();
  delay(4000);
  ez.canvas.println("Backward");
  m5photogrammetry.motorBackward();
  for (int i=0;i<PHOTOGRAMMETRY_MOTOR_ROTATION;i++)
  {
    m5photogrammetry.motorNextStep();
    delay(1);
  }
  m5photogrammetry.ledGreenOn();
  m5photogrammetry.ledRedOff();
  ez.buttons.wait("OK");
}

void setup() {
  M5.begin();                       // Init M5Stack(Initial I2C is also included).
  M5.Power.begin();                 // Init power
  m5photogrammetry.begin();         // Initialize the electronic card: green led => on

  #include <themes/default.h>
  #include <themes/dark.h>
  ezt::setDebug(INFO);
  ez.begin();    
}

void loop() {
  ezMenu mainmenu(PHOTOGRAMMETRY_PROJECT);
  mainmenu.txtSmall();
  mainmenu.addItem("Take photos", mainmenu_photos);
  mainmenu.addItem("Tests photogrammetry card", mainmenu_tests);
  mainmenu.addItem("Built-in wifi & other settings", ez.settings.menu);
  mainmenu.addItem("About", mainmenu_about);
  mainmenu.upOnFirst("last|up");
  mainmenu.downOnLast("first|down");
  mainmenu.run();
}

void mainmenu_tests()
{
  ezMenu submenu("Test");
  submenu.txtSmall();
  submenu.buttons("up#Back#select##down#");
  submenu.addItem("I2C",menu_tests_i2c);
  submenu.addItem("Leds",menu_tests_leds);
  submenu.addItem("Bluetooth",menu_tests_bt);
  submenu.addItem("Jack",menu_tests_jack);
  submenu.addItem("Pentax Flucard",menu_tests_flucard);
  submenu.addItem("Motor",menu_tests_motor);
  submenu.run();  
}

void mainmenu_photos()
{
  String nbSteps="Nb photos: ";
  ezMenu submenu("Photos");
  submenu.txtSmall();
  submenu.buttons("up#Back#select##down#");
  submenu.addItem("Set the number of photos",menu_photos_nb);
  submenu.addItem("Take photos via Jack",menu_photos_jack);
  submenu.addItem("Take photos via BlueTooth",menu_photos_bt);
  submenu.addItem("Take photos via Flucard",menu_photos_flucard);
  submenu.addItem("Start taking photos",menu_photos_start);
  submenu.run();  
}


void menu_photos_start_txt(uint8_t photo_id)
{
  ez.canvas.clear();
  ez.header.show("Photos");
  ez.canvas.lmargin(5);
  ez.canvas.font(&FreeMono12pt7b);  
  ez.canvas.println("");
  ez.canvas.println("Photo:");
  ez.canvas.println("");  
  ez.canvas.println("current:");  
  ez.canvas.println(photo_id);
  ez.canvas.println("total:");  
  ez.canvas.println(nb_photos);
}

void menu_photos_start()
{
  uint8_t photo_id=0;
  int nb_steps_todo=PHOTOGRAMMETRY_MOTOR_ROTATION/nb_photos;
  int current_photo=1;

  menu_photos_start_txt(current_photo);

  m5photogrammetry.motorForward();
  for (int i=0;i<PHOTOGRAMMETRY_MOTOR_ROTATION;i++)
  {
    if (i%nb_steps_todo==0)
    {
      delay(100);
      if (photos_jack)
      {
        m5photogrammetry.jackShootPress();
        delay(500);
        m5photogrammetry.jackShootRelease();
      }
      if (photos_bt)
      {
        m5photogrammetry.bluetoothTakePhoto();
      } 
      if (photos_flucard)
      {
        m5photogrammetry.flucardShootToggle();
      }
      menu_photos_start_txt(current_photo++);     
    }
    m5photogrammetry.motorNextStep();
    delay(1);
  }
  delay(1000);
  m5photogrammetry.motorBackward();
  for (int i=0;i<PHOTOGRAMMETRY_MOTOR_ROTATION;i++)
  {
    m5photogrammetry.motorNextStep();
    delay(1);
  }

  ez.buttons.show("OK");
  while (true) 
  {
    String btnpressed = ez.buttons.poll();
    if (btnpressed == "OK") break;
  }    
}

void menu_photos_jack_txt()
{
  ez.canvas.clear();
  ez.header.show("Photos with Jack");
  ez.canvas.lmargin(5);
  ez.canvas.font(&FreeMono12pt7b);  
  ez.canvas.println("");
  ez.canvas.println("Take photos with");
  ez.canvas.println("Jack:");
  ez.canvas.println("");
  if (photos_jack)
  {
    ez.canvas.println("Enabled");
  }
  else
  {
    ez.canvas.println("Disabled");
  }  
}

void menu_photos_bt_txt()
{
  ez.canvas.clear();
  ez.header.show("Photos with BlueTooth");
  ez.canvas.lmargin(5);
  ez.canvas.font(&FreeMono12pt7b);  
  ez.canvas.println("");
  ez.canvas.println("Take photos with");
  ez.canvas.println("BlueTooth:");
  ez.canvas.println("");
  if (photos_bt)
  {
    ez.canvas.println("Enabled");
  }
  else
  {
    ez.canvas.println("Disabled");
  }  
}

void menu_photos_flucard_txt()
{
  ez.canvas.clear();
  ez.header.show("Photos with Flucard");
  ez.canvas.lmargin(5);
  ez.canvas.font(&FreeMono12pt7b);  
  ez.canvas.println("");
  ez.canvas.println("Take photos with");
  ez.canvas.println("FluCard:");
  ez.canvas.println("");
  if (photos_flucard)
  {
    ez.canvas.println("Enabled");
  }
  else
  {
    ez.canvas.println("Disabled");
  }  
}

void menu_photos_jack()
{
  menu_photos_jack_txt();
  ez.buttons.show("OK # ON # OFF");
  while (true) 
  {
    String btnpressed = ez.buttons.poll();
    if (btnpressed == "OK") break;
    
    if (btnpressed == "ON")
    {
      photos_jack=true;      
      menu_photos_jack_txt();
    }
    if (btnpressed == "OFF")
    {      
      photos_jack=false;
      menu_photos_jack_txt();
    }    
  }  
}

void menu_photos_bt()
{
  menu_photos_bt_txt();
  ez.buttons.show("OK # ON # OFF");
  while (true) 
  {
    String btnpressed = ez.buttons.poll();
    if (btnpressed == "OK") break;
    
    if (btnpressed == "ON")
    {
      int cmpt=0;
      m5photogrammetry.bluetoothStart();
      while ((cmpt < 200) && (!m5photogrammetry.bluetoothIsConnected()))  // Waiting for connection
      {
        if (cmpt%2==0)
        {
          ez.canvas.print(".");
        }
        delay(100);
        cmpt++;
      }
      photos_bt=m5photogrammetry.bluetoothIsConnected();;      
      menu_photos_bt_txt();
    }
    if (btnpressed == "OFF")
    {      
      photos_bt=false;
      menu_photos_bt_txt();
    }    
  }  
}

void menu_photos_flucard()
{
  menu_photos_flucard_txt();
  ez.buttons.show("OK # ON # OFF");
  while (true) 
  {
    String btnpressed = ez.buttons.poll();
    if (btnpressed == "OK") break;
    
    if (btnpressed == "ON")
    {
      int cmpt=0;
      m5photogrammetry.flucardStart();
      while ((cmpt < 20) && (!m5photogrammetry.flucardIsConnected()))  // Waiting for connection
      {
        ez.canvas.print(".");
        cmpt++;
      }
      photos_flucard=m5photogrammetry.flucardIsConnected();    
      menu_photos_flucard_txt();
    }
    if (btnpressed == "OFF")
    {      
      photos_flucard=false;
      menu_photos_flucard_txt();
    }    
  }  
}

void menu_photos_nb_txt()
{
  ez.canvas.clear();
  ez.header.show("Photos");
  ez.canvas.lmargin(5);
  ez.canvas.font(&FreeMono12pt7b);  
  ez.canvas.println("");
  ez.canvas.println("Number of photos:");
  ez.canvas.println("");
  ez.canvas.println(nb_photos);
  ez.canvas.println("");
}
/*
 * Function to test if photos can be taken thanks to jack connectoer
 */
void menu_photos_nb()
{  
  menu_photos_nb_txt();
  ez.buttons.show("OK # +1 # +10");
  while (true) 
  {
    String btnpressed = ez.buttons.poll();
    if (btnpressed == "OK") break;
    
    if (btnpressed == "+1")
    {
      nb_photos+=1;      
      menu_photos_nb_txt();
    }
    if (btnpressed == "+10")
    {      
      nb_photos+=10;
      menu_photos_nb_txt();
    }    
  }
}


void mainmenu_about() { 
  String s1=PHOTOGRAMMETRY_PROJECT;
  String s2=PHOTOGRAMMETRY_VERSION;
  String s3=PHOTOGRAMMETRY_AUTHOR;
  ez.msgBox(PHOTOGRAMMETRY_PROJECT, s1+" | "+s2+" | | "+s3);
}