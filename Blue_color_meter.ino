#include <Adafruit_TCS34725.h>
#include <ArducamSSD1306.h>    // Modification of Adafruit_SSD1306 for ESP8266 compatibility
#include <Adafruit_GFX.h>   // Needs a little change in original Adafruit library (See README.txt file)
#include <Wire.h>           // For I2C comm, but needed for not getting compile error

/* SSD1306
D0  SCL
D1  SDA
RES  PIN7
VCC  3.3/5V
GND  GND
*/
#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif
// connect LED to digital 4 or GROUND for ambient light sensing
// connect SCL to analog 5
// connect SDA to analog 4
// connect Vin to 3.3-5V DC
// connect GROUND to common ground
// some magic numbers for this device from the DN40 application note
#define TCS34725_R_Coef 0.136
#define TCS34725_G_Coef 1.000
#define TCS34725_B_Coef -0.444
#define TCS34725_GA 1.0
#define TCS34725_DF 310.0
#define TCS34725_CT_Coef 3810.0
#define TCS34725_CT_Offset 1391.0
const int _LedPin = 13;

/* Initialise with default values (int time = 2.4ms, gain = 1x) */
// Adafruit_TCS34725 tcs = Adafruit_TCS34725();
/* Initialise with specific int time and gain values */
//Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);
//Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_101MS, TCS34725_GAIN_1X);
#define OLED_RESET  7  // Pin 7 -RESET digital signal
ArducamSSD1306 display(OLED_RESET); // FOR I2C

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels


void setup() {
// Start Serial
Serial.begin(115200);
pinMode(_LedPin, OUTPUT);
digitalWrite(_LedPin, LOW);
bool Sensor_Found = false;
//TCS34725 RGB sensor intialization
if (tcs.begin()) 
    {
    Serial.println("Found sensor");
    Sensor_Found = true;
    } 
if (!tcs.begin())  
    {
    Serial.println("No TCS34725 found ... check your connections");
    Sensor_Found = false;
    //delay(2000);
    }
tcs.setInterrupt(true);   
// SSD1306 Init
display.begin();  // Switch OLED
display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);

display.setCursor(15,2);
display.println("Cyanometer v.02");

display.setCursor(15,25);
if(Sensor_Found == true)
    {
  display.println("TCS34725 Ready");
    }
  else
    {
    display.println("TCS34725 Fault");
    }
    
display.setCursor(15,45);
display.println("Device Ready");
display.display();
delay(2000);

}

void loop() {
display.clearDisplay();
uint16_t r, g, b, c, colorTemp, lux;

tcs.getRawData(&r, &g, &b, &c);
// colorTemp = tcs.calculateColorTemperature(r, g, b);
colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
lux = tcs.calculateLux(r, g, b);

Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
Serial.println(" ");

display.setCursor(15,2);
display.print("Color Temp: ");
display.println(colorTemp, DEC);

display.setCursor(15,15);
display.print("Lux: ");
display.println(lux, DEC);

display.setCursor(15,25);
display.print("R: ");
display.println(r, DEC); 

display.setCursor(15,35);
display.print("G: "); 
display.println(g, DEC);

display.setCursor(15,45);
display.print("B: "); 
display.println(b, DEC); 

display.setCursor(15,55);
display.print("C: "); 
display.println(c, DEC); 

display.display();
delay(1000);
}
