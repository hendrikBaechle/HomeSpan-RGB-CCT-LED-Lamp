bool RGB_Mode;
float combinedWhite;

#include "HomeSpan.h"

 #include <FastLED.h>

#include "extras/PwmPin.h"                          // library of various PWM functions

int TemperatureLED_cold = 166; // The Color Temperature of your colder LED, mine is 6024K (1000000/6024 =166)
int TemperatureLED_warm = 400; // The Color Temperature of your colder LED, mine is 2500K (1000000/2500 =400)

//////////////////////////////////

struct DEV_RgbLED : Service::LightBulb {       // RGB LED (Command Cathode)

  LedPin *redPin, *greenPin, *bluePin, *coldPin, *warmPin;

  float v, h, s, r, g, b, cw, ww, c, cw_2, ww_2;
  
  SpanCharacteristic *power;                   // reference to the On Characteristic
  SpanCharacteristic *H;                       // reference to the Hue Characteristic
  SpanCharacteristic *S;                       // reference to the Saturation Characteristic
  SpanCharacteristic *V;                       // reference to the Brightness Characteristic
  SpanCharacteristic *C;                       // reference to the Color Temperature Characteristic
  
  DEV_RgbLED(int red_pin, int green_pin, int blue_pin, int cold_Pin, int warm_Pin) : Service::LightBulb(){       // constructor() method

    power=new Characteristic::On();                    
    H=new Characteristic::Hue(0);              // instantiate the Hue Characteristic with an initial value of 0 out of 360
    S=new Characteristic::Saturation(0);       // instantiate the Saturation Characteristic with an initial value of 0%
    V=new Characteristic::Brightness(100);     // instantiate the Brightness Characteristic with an initial value of 100%
    V->setRange(0,100,1);                      // sets the range of the Brightness to be from a min of 5%, to a max of 100%, in steps of 1%
    C=new Characteristic::ColorTemperature(TemperatureLED_warm); // instantiate the ColorTemperature Characteristic with an initial value of "TemperatureLED_warm"
    C->setRange(TemperatureLED_cold, TemperatureLED_warm, 1);                  
    this->redPin=new LedPin(red_pin);        // configures a PWM LED for output to the RED pin
    this->greenPin=new LedPin(green_pin);    // configures a PWM LED for output to the GREEN pin
    this->bluePin=new LedPin(blue_pin);      // configures a PWM LED for output to the BLUE pin
    this->coldPin=new LedPin(cold_Pin);      // configures a PWM LED for output to the COLD WHITE pin
    this->warmPin=new LedPin(warm_Pin);      // configures a PWM LED for output to the WARM WHITE pin
    
 
    char cBuf[128];
    sprintf(cBuf,"Configuring RGB LED: Pins=(%d,%d,%d)\n",redPin->getPin(),greenPin->getPin(),bluePin->getPin());
    Serial.print(cBuf);
    
  } // end constructor

  boolean update(){                         // update() method

    boolean p;
    // float v, h, s, r, g, b, cw, ww, c, cw_2, ww_2;

    h=H->getVal<float>();                      // get and store all current values.  Note the use of the <float> template to properly read the values
    s=S->getVal<float>();
    v=V->getVal<float>();                      // though H and S are defined as FLOAT in HAP, V (which is brightness) is defined as INT, but will be re-cast appropriately
    p=power->getVal();

    char cBuf[128];
    sprintf(cBuf,"Updating RGB LED: Pins=(%d,%d,%d): ",redPin->getPin(),greenPin->getPin(),bluePin->getPin());
    LOG1(cBuf);

    if(power->updated()){
      p=power->getNewVal();
      sprintf(cBuf,"Power=%s->%s, ",power->getVal()?"true":"false",p?"true":"false");
      Serial.print("Power was updated: "); Serial.print(p); Serial.print("\n");

    } else {
      sprintf(cBuf,"Power=%s, ",p?"true":"false");
    }
    LOG1(cBuf);
      
    if(H->updated()){
      h=H->getNewVal<float>();
      sprintf(cBuf,"H=%.0f->%.0f, ",H->getVal<float>(),h);
      RGB_Mode = true;
      Serial.print("Hue was updated: "); Serial.print(h);  Serial.print("\n");
  
    } else {
      sprintf(cBuf,"H=%.0f, ",h);
    }
    LOG1(cBuf);

    if(S->updated()){
      s=S->getNewVal<float>();
      sprintf(cBuf,"S=%.0f->%.0f, ",S->getVal<float>(),s);
      RGB_Mode = true;
      Serial.print("Saturation was updated: "); Serial.print(s); Serial.print("\n");

    } else {
      sprintf(cBuf,"S=%.0f, ",s);
    }
    LOG1(cBuf);

    if(V->updated()){
      v=V->getNewVal<float>();
      sprintf(cBuf,"V=%.0f->%.0f  ",V->getVal<float>(),v);
      Serial.print("Brightness was updated: "); Serial.print(v); Serial.print("\n");

    } else {
      sprintf(cBuf,"V=%.0f  ",v);
    }
    LOG1(cBuf);

    if(C->updated()){
      c=C->getNewVal<float>();
      Serial.print("Color Temperature was updated: "); Serial.print(c); Serial.print("\n")

    cw = map(c, TemperatureLED_cold, TemperatureLED_warm, 100, 0);
    ww = map(c, TemperatureLED_cold, TemperatureLED_warm, 0, 100);

     Serial.print("cw mapped: "); 
     Serial.print(cw);
     Serial.println();
     Serial.print("ww mapped: "); 
     Serial.print(ww);
     Serial.println();

     combinedWhite = cw + ww;
     
     cw = cw / 100;
     ww = ww / 100;

     Serial.print("cw adjusted: "); 
     Serial.print(cw);
     Serial.println();
     Serial.print("ww adjusted: "); 
     Serial.print(ww);
     Serial.println();

     Serial.print("combinedWhite: "); 
     Serial.print(combinedWhite);
     Serial.println();

     
      RGB_Mode = false;

      if (combinedWhite > 100){
        combinedWhite = combinedWhite / 100;
        cw = cw / combinedWhite;
        ww = ww / combinedWhite;
      }


Serial.print("cw final: "); 
Serial.print(cw);
Serial.println();
Serial.print("ww final: "); 
Serial.print(ww);
Serial.println();


if (c <= 250) {
KelvinSaturation = map(c,153,250,2,35);
KelvinHue = map(c,153,250,48,26);
} else {
KelvinSaturation = map(c,250,470,35,89);
KelvinHue = map(c,250,470,26,30);
}

H->setVal(KelvinHue);
S->setVal(KelvinSaturation);

      RGB_Mode = false;
    } 

    LedPin::HSVtoRGB(h,s/100.0,1.0,&r,&g,&b);   // since HomeKit provides S and V in percent, scale down by 100

    int R, G, B, CW, WW;

    sprintf(cBuf,"RGB=(%d,%d,%d)\n",R,G,B);
    LOG1(cBuf);

Serial.print("RGB Mode: "); Serial.println(RGB_Mode);

if (RGB_Mode == true) {
    R=p*r*v;                                      // since LedPin uses percent, scale back up by 100, and multiple by status fo power (either 0 or 1)
    G=p*g*v;
    B=p*b*v;
    redPin->set(R);                      // update the ledPin channels with new values
    greenPin->set(G);    
    bluePin->set(B);
    coldPin->set(0);
    warmPin->set(0);
} else if (RGB_Mode == false) {

       Serial.print("cw: "); Serial.println(cw);
     Serial.print("ww: "); Serial.println(ww); 

     cw_2 = p*cw*v;                                      // since LedPin uses percent, scale back up by 100, and multiple by status fo power (either 0 or 1)
     ww_2 = p*ww*v;

     Serial.print("cw_2: "); Serial.println(cw_2);
     Serial.print("ww_2: "); Serial.println(ww_2); 

     CW = cw_2;
     WW = ww_2;

Serial.print("CW: "); Serial.println(CW);
Serial.print("WW: "); Serial.println(WW); 

    redPin->set(0);                      // update the ledPin channels with new values
    greenPin->set(0);    
    bluePin->set(0);
    coldPin->set(CW);
    warmPin->set(WW);

KelvinSaturation = map(c,153,470,2,89);
KelvinHue = map(c,153,470,48,30);

}

     Serial.print("\n \n");

    return(true);                               // return true

  
  
  } // update
};
      
//////////////////////////////////
