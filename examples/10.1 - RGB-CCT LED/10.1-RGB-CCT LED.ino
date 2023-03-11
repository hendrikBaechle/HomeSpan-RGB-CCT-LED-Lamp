
// Example 10.1: Controlling a full-color RGB LED and 

#include "HomeSpan.h" 
#include "DEV_LED.h"  

void setup() {
  Serial.begin(115200);

// Initializing the pins of the leds

int red_pin = 32;
int green_pin = 33;
int blue_pin = 27;
int cold_pin = 25;
int warm_pin = 26;

// A test for the LEDs to check whether my code or my soldering doesn´t work

bool doLEDTest = true;

if (doLEDTest == true){
analogWrite(red_pin, 0);
analogWrite(green_pin, 0);
analogWrite(blue_pin, 0);
analogWrite(cold_pin, 0);
analogWrite(warm_pin, 0);

  analogWrite(red_pin, 1);
delay(1000);
analogWrite(red_pin, 0);

analogWrite(green_pin, 1);
delay(1000);
analogWrite(green_pin, 0);

analogWrite(blue_pin, 1);
delay(1000);
analogWrite(blue_pin, 0);

analogWrite(cold_pin, 1);
delay(1000);
analogWrite(cold_pin, 0);

analogWrite(warm_pin, 1);
delay(1000);
analogWrite(warm_pin, 0);

Serial.println("LED Test complete");
}
 
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  
 homeSpan.enableOTA();
  homeSpan.begin(Category::Lighting,"RGBCCT LED Panel_1");

  new SpanAccessory();                                                          
    new Service::AccessoryInformation();    
      new Characteristic::Identify();
      new Characteristic::FirmwareRevision("2.0.1");
      new Characteristic::HardwareRevision("1.1.0");           
      new Characteristic::Name("RGB LED"); 
    new DEV_RgbLED(red_pin,green_pin,blue_pin,cold_pin,warm_pin);                                                      // Create an RGB LED attached to pins 32,22,23 (for R, G, and B LED anodes)
      
} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();
  
} // end of loop()
