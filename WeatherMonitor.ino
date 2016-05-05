// This #include statement was automatically added by the Particle IDE.
#include "lib1.h"

const int button1Pin = D1;  // pushbutton 1 pin
const int button2Pin = D0;  // pushbutton 2 pin
//const int ledPin =  D4;    // LED1 pin
//const int ledPin2 =  D5;    // LED2 pin

const int RED_PIN =  D2;    // LED1 pin
const int GREEN_PIN =  D3;    // LED2 pin
const int BLUE_PIN =  D4;    // LED2 pin

int buttonval = 0;

void setup() {
    // For simplicity, we'll format our weather data as text, and pipe it to serial.
    // but you could just as easily display it in a webpage or pass the data to another system.

    // Learn more about the serial commands at https://docs.particle.io/reference/firmware/photon/#serial
    //  for the Photon, or https://docs.particle.io/reference/firmware/core/#serial for the Core
    // You can also watch what's sent over serial with the particle cli with
    //  particle serial monitor
    Serial.begin(9600);  //115200);
    // Set up the pushbutton pins to be an input:
    pinMode(button1Pin, INPUT);
    pinMode(button2Pin, INPUT);
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
    
    // Set up the LED pin to be an output:
    //pinMode(ledPin, OUTPUT);
    //pinMode(ledPin2, OUTPUT);  
    
    // Lets listen for the hook response
    //Particle.subscribe("hook-response/get_weather", gotWeatherData, MY_DEVICES);
    Particle.subscribe("hook-response/WU_get_conditions", gotWeatherData, MY_DEVICES);
    Particle.subscribe("hook-response/WU_get_forecast", gotWeatherforecastData, MY_DEVICES);
    Serial.println("starting...");
    // Lets give ourselves 10 seconds before we actually start the program.
    // That will just give us a chance to open the serial monitor before the program sends the request
    //for(int i=0;i<10;i++) {
    //    Serial.println("waiting " + String(10-i) + " seconds before we publish");
    //    delay(1000);
    //}
    //digitalWrite(ledPin, HIGH);
    //digitalWrite(ledPin2, HIGH);
    delay(10000);  // Turn on LEDs for 10 seconds...
    //digitalWrite(ledPin, LOW);
    //digitalWrite(ledPin2, LOW);
}


// called forever really fast
void loop() {

   
    //----------------------------------------------------------------------
    int button1State, button2State;  // variables to hold the pushbutton states
    
    button1State = digitalRead(button1Pin);
    button2State = digitalRead(button2Pin);
    /*if (button1State == LOW)
  {
    digitalWrite(ledPin, HIGH);  // turn the LED on
    digitalWrite(ledPin2, LOW);  // turn the LED off
    digitalWrite(ledOnboard, HIGH);
    Serial.println("LED On");// and wait 5 minutes before doing 12 again
    delay(1000);
  }
  else if (button2State == LOW)
  {
    digitalWrite(ledPin2, HIGH);  // turn the LED on
    digitalWrite(ledPin, LOW);  // turn the LED off
    digitalWrite(ledOnboard, LOW);
    Serial.println("LED Off");
    delay(1000);
  }
  else
  {
    digitalWrite(ledPin, LOW);  // turn the LED off
    digitalWrite(ledPin2, LOW);  // turn the LED off
    digitalWrite(ledOnboard, LOW);
    Serial.println("LED Off");
    delay(1000);
  }*/
   if (button1State == LOW)
    {
        buttonval = 1;
        
        // Let's request the weather, but no more than once every 60 seconds.
        Serial.println("Requesting WU Weather!");
    
        // publish the event that will trigger our Webhook
        //Particle.publish("get_weather");
        Particle.publish("WU_get_conditions");
        //digitalWrite(ledPin, HIGH);
        //digitalWrite(ledPin2, LOW);
        //digitalWrite(RED_PIN, HIGH);
        //digitalWrite(GREEN_PIN, LOW);
        //digitalWrite(BLUE_PIN, LOW);
        
        digitalWrite(RED_PIN, HIGH);
        digitalWrite(GREEN_PIN, LOW);
        digitalWrite(BLUE_PIN, LOW);
        delay(10000);
        digitalWrite(RED_PIN, LOW);
        digitalWrite(GREEN_PIN, LOW);
        digitalWrite(BLUE_PIN, LOW);
    }
    else if (button2State == LOW)
    {
       buttonval = 2;
       // Let's request the weather, but no more than once every 60 seconds.
       Serial.println("Requesting WU Weather!");

        // publish the event that will trigger our Webhook
        //Particle.publish("get_weather");
        Particle.publish("WU_get_forecast");
        //digitalWrite(ledPin, LOW);
        //digitalWrite(ledPin2, HIGH);
        //digitalWrite(RED_PIN, LOW);
        //digitalWrite(GREEN_PIN, HIGH);
        //digitalWrite(BLUE_PIN, LOW);
        
        digitalWrite(RED_PIN, LOW);
        digitalWrite(GREEN_PIN, HIGH);
        digitalWrite(BLUE_PIN, LOW);
        delay(10000);
        digitalWrite(RED_PIN, LOW);
        digitalWrite(GREEN_PIN, LOW);
        digitalWrite(BLUE_PIN, LOW);
    }
    else
    {
        //Serial.println("Requesting WU Weather!");
        //Particle.publish("WU_get_conditions");
        //digitalWrite(RED_PIN, LOW);
        //digitalWrite(GREEN_PIN, LOW);
        //digitalWrite(BLUE_PIN, HIGH);
        //delay(10000);    
        
    }
    
    //----------------------------------------------------------------------
        
}

// This function will get called when weather data comes in
void gotWeatherData(const char *name, const char *data) {
    // Important note!  -- Right now the response comes in 512 byte chunks.
    //  This code assumes we're getting the response in large chunks, and this
    //  assumption breaks down if a line happens to be split across response chunks.
    //
    // Sample data:
    //  <location>Minneapolis, Minneapolis-St. Paul International Airport, MN</location>
    //  <weather>Overcast</weather>
    //  <temperature_string>26.0 F (-3.3 C)</temperature_string>
    //  <temp_f>26.0</temp_f>


    String str = String(data);
    //Serial.println(str);
    String locationStr = tryExtractString(str, "<location>", "</location>");
    String weatherStr = tryExtractString(str, "<weather>", "</weather>");
    String tempStr = tryExtractString(str, "<temp_f>", "</temp_f>");
    String windStr = tryExtractString(str, "<wind_string>", "</wind_string>");
    
    String pressureStr = tryExtractString(str, "<pressure_string>", "</pressure_string>");
    String dewpointStr = tryExtractString(str, "<dewpoint_f>", "</dewpoint_f>");
    String windchillStr = tryExtractString(str, "<windchill_string>", "</windchill_string>");
    String visibilityStr = tryExtractString(str, "<visibility_mi>", "</visibility_mi>");
    String fctStr = tryExtractString(str, "<fcttext>", "</fcttext>");
    
    if (buttonval == 1)  // conditions
    {
        if (locationStr != NULL) {
        Serial.println("At location: " + locationStr);
        }
    
        if (weatherStr != NULL) {
            Serial.println("The weather is: " + weatherStr);
        }

        if (tempStr != NULL) {
            Serial.println("The temp is: " + tempStr + String(" *F"));
        }
    
        if (windStr != NULL) {
            Serial.println("The wind is: " + windStr);
        }    
        
    }
    else if (buttonval == 2)  // forecast
    {
        if (pressureStr != NULL) {
        Serial.println("The pressure is: " + pressureStr);
        }
    
        if (dewpointStr != NULL) {
            Serial.println("The dewpoint is: " + dewpointStr);
        }

        if (windchillStr != NULL) {
            Serial.println("The windchill is: " + windchillStr);
        }
    
        if (visibilityStr != NULL) {
            Serial.println("The visibility is: " + visibilityStr);
        }
        if (fctStr != NULL) {
            Serial.println("The forecast is: " + visibilityStr);
        }
        
    }
}

// This function will get called when weather data comes in
void gotWeatherforecastData(const char *name, const char *data) {
    // Important note!  -- Right now the response comes in 512 byte chunks.
    //  This code assumes we're getting the response in large chunks, and this
    //  assumption breaks down if a line happens to be split across response chunks.
    //
    // Sample data:
    //  <location>Minneapolis, Minneapolis-St. Paul International Airport, MN</location>
    //  <weather>Overcast</weather>
    //  <temperature_string>26.0 F (-3.3 C)</temperature_string>
    //  <temp_f>26.0</temp_f>


    String str = String(data);
    Serial.println(str);
    String locationStr = tryExtractString(str, "<location>", "</location>");
    String weatherStr = tryExtractString(str, "<weather>", "</weather>");
    String tempStr = tryExtractString(str, "<temp_f>", "</temp_f>");
    String windStr = tryExtractString(str, "<wind_string>", "</wind_string>");
    
    String pressureStr = tryExtractString(str, "<pressure_string>", "</pressure_string>");
    String dewpointStr = tryExtractString(str, "<dewpoint_f>", "</dewpoint_f>");
    String windchillStr = tryExtractString(str, "<windchill_string>", "</windchill_string>");
    String visibilityStr = tryExtractString(str, "<visibility_mi>", "</visibility_mi>");
    
    if (buttonval == 1)
    {
        if (locationStr != NULL) {
        Serial.println("At location: " + locationStr);
        }
    
        if (weatherStr != NULL) {
            Serial.println("The weather is: " + weatherStr);
        }

        if (tempStr != NULL) {
            Serial.println("The temp is: " + tempStr + String(" *F"));
        }
    
        if (windStr != NULL) {
            Serial.println("The wind is: " + windStr);
        }    
        
    }
    else if (buttonval == 2)
    {
        if (pressureStr != NULL) {
        Serial.println("The pressure is: " + pressureStr);
        }
    
        if (dewpointStr != NULL) {
            Serial.println("The dewpoint is: " + dewpointStr);
        }

        if (windchillStr != NULL) {
            Serial.println("The windchill is: " + windchillStr);
        }
    
        if (visibilityStr != NULL) {
            Serial.println("The visibility is: " + visibilityStr);
        }
        
    }
}
// Returns any text found between a start and end string inside 'str'
// example: startfooend  -> returns foo
String tryExtractString(String str, const char* start, const char* end) {
    if (str == NULL) {
        return NULL;
    }

    int idx = str.indexOf(start);
    if (idx < 0) {
        return NULL;
    }

    int endIdx = str.indexOf(end);
    if (endIdx < 0) {
        return NULL;
    }

    return str.substring(idx + strlen(start), endIdx);
}