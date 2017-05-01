
/// @dir pingPong
/// Demo of a sketch which sends and receives packets.
// 2010-05-17 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

// with thanks to Peter G for creating a test sketch and pointing out the issue
// see http://jeelabs.org/2010/05/20/a-subtle-rf12-detail/


#include <JeeLib.h>

#include <Adafruit_DotStar.h>
#include <SPI.h>   

#define outPin 8
#define outlet1 4

#define NUMPIXELS 3 // Number of LEDs in strip

// Here's how to control the LEDs from any two pins:
#define DATAPIN    7
#define CLOCKPIN   6
Adafruit_DotStar strip = Adafruit_DotStar(
  NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);
  
char openHours[] = "1";
char closedHours[] = "0";


bool businessHours = true;

unsigned long lastClockIn = 0;
unsigned long timePassed =0;


void setup () {
   strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP
  Serial.begin(9600);
  Serial.println(9600);
  Serial.println("Send and Receive");
  rf12_initialize(1, RF12_915MHZ, 33);
  pinMode(outPin, INPUT_PULLUP);
  pinMode(outlet1, OUTPUT);
  digitalWrite(outlet1, HIGH);
  setRed();
}

void loop () {
  if (rf12_recvDone() && rf12_crc == 0) {
    if (rf12_data[0] == '2')
    {
      businessHours = false;
      Serial.println("Off");

    }
    else if (rf12_data[0] == '0')
    {
      businessHours = true;
      setGreen();
      digitalWrite(outlet1, LOW);
      Serial.println("ON");
    }
    
    for (byte i = 0; i < rf12_len; ++i)
      Serial.print((char) rf12_data[i]);
    Serial.println();
  }
  

  if (!businessHours) {
    Serial.println(digitalRead(outPin));
    if (digitalRead(outPin) == LOW)
    {
      digitalWrite(outlet1, LOW);
      setGreen();
      lastClockIn = millis();
      delay(100);

    }
    timePassed = millis() - lastClockIn > 6000;
    if (millis() - lastClockIn > 6000 && !businessHours)
    {
     setRed();
      digitalWrite(outlet1, HIGH);
    }

    else if ( millis() - lastClockIn > 5000 && !businessHours )
    {
      setWarning();
    }

    else if (millis() - lastClockIn > 3000 && !businessHours)
    {
      setYellow();
    }
  }

}

void setRed()
{
  for(int i = 0; i < 3; i++)
    strip.setPixelColor(i, 0x00FF00);
    strip.show();
}

void setGreen()
{
  for(int i = 0; i < 3; i++)
    strip.setPixelColor(i, 0xFF0000);
    strip.show();
}

void setYellow()
{
  for(int i = 0; i < 3; i++)
    strip.setPixelColor(i, 0xFFFF00);
    strip.show();
}

void setWarning()
{
  for(int i = 0; i < 3; i++)
    strip.setPixelColor(i, 0x43ff00);
    strip.show();
}



