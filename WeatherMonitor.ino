//#include <TimerOne.h>

const int button1Pin = D1;  // pushbutton 1 pin
const int button2Pin = D0;  // pushbutton 2 pin
const int RED_PIN =  D2;    // LED1 pin
const int GREEN_PIN =  D3;    // LED2 pin
const int BLUE_PIN =  D4;    // LED2 pin
//const int RED_PIN2 =  A2;    // LED1 pin
//const int GREEN_PIN2 =  A3;    // LED2 pin
//const int BLUE_PIN2 =  A4;    // LED2 pin

const int BuzzerPin = D5;  // Piezo

int m_hightemp = 0;
int m_maxwind = 0;
int m_pop = 0;
bool weatherGood = true;
bool weatherAlert = false;

void setup() {
    
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
    //pinMode(RED_PIN2, OUTPUT);
    //pinMode(GREEN_PIN2, OUTPUT);
    //pinMode(BLUE_PIN2, OUTPUT);
    
    pinMode(BuzzerPin, OUTPUT);

    //Publish weatherAlert variable for use with IFTTT   
    Particle.variable("weatherAlert", weatherAlert);
    // Subscribing to hook responses...
    //Particle.subscribe("hook-response/get_weather", gotWeatherData, MY_DEVICES);
    //Particle.subscribe("hook-response/WU_get_forecast", gotWeatherforecastData, MY_DEVICES);
    Particle.subscribe("hook-response/WUconditions_hook", gotWeatherData, MY_DEVICES);
    Particle.subscribe("hook-response/WU4dayforecast_hook", gotWeatherforecastData, MY_DEVICES);
    Particle.subscribe("hook-response/WUastronomy_hook", gotWeatherAstronomyData, MY_DEVICES);
    Particle.subscribe("hook-response/WUalerts_hook", gotWeatherAlertsData, MY_DEVICES);
    
    Serial.println("starting...");
    // Lets give ourselves 7 seconds before we actually start the program.
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(BLUE_PIN, HIGH);
    //analogWrite(RED_PIN2, 255);
    //analogWrite(GREEN_PIN2, 255);
    //analogWrite(BLUE_PIN2, 255);
    
    delay(7000);  // Turn on LEDs for 7 seconds...
    TurnOffRGBLED();
        
}


// called forever really fast
void loop() {

    int button1State, button2State;  // variables to hold the pushbutton states
    
    button1State = digitalRead(button1Pin);
    button2State = digitalRead(button2Pin);
  
    // ----------------- Weather alert! -----------------------------
    if (weatherAlert == true)
    {
        // Do this while a button is not pressed...
        delay(500);
        TurnOffRGBLED();  // blink the LED red  
        //noTone(BuzzerPin);  // sequence the piezo
        delay(500);
        //tone(BuzzerPin, 262, 150);
        //tone(BuzzerPin, 262);  // sequence the piezo
        digitalWrite(RED_PIN, HIGH);
        digitalWrite(GREEN_PIN, LOW);
        digitalWrite(BLUE_PIN, LOW);
        //analogWrite(RED_PIN2, 255);
        //analogWrite(GREEN_PIN2, 0);
        //analogWrite(BLUE_PIN2, 0);
    }
    // ---------------------------------------------------------------
    
    if (button1State == LOW)
    {
        weatherAlert = false;
        
        delay(1000);
        button1State = digitalRead(button1Pin);  // Read this value again to test for a long press...
        if (button1State == LOW)  // button state is still low (long press), so publish an alternate webhook
        {
            Serial.println("blue button - long press");
            // publish the event that will trigger our Webhook
            Particle.publish("WUalerts_hook");
            digitalWrite(RED_PIN, LOW);
            digitalWrite(GREEN_PIN, LOW);
            digitalWrite(BLUE_PIN, HIGH);
            //analogWrite(RED_PIN2, 0);
            //analogWrite(GREEN_PIN2, 0);
            //analogWrite(BLUE_PIN2, 255);
            delay(500);
        }
        else
        {
            Serial.println("blue button - short press");
            // publish the event that will trigger our Webhook
            Particle.publish("WUconditions_hook");
            digitalWrite(RED_PIN, LOW);
            digitalWrite(GREEN_PIN, LOW);
            digitalWrite(BLUE_PIN, HIGH);
            //analogWrite(RED_PIN2, 0);
            //analogWrite(GREEN_PIN2, 0);
            //analogWrite(BLUE_PIN2, 255);
            delay(500);
        }
        TurnOffRGBLED();
    }
    else if (button2State == LOW)
    {
       weatherAlert = false;
       
        delay(1000);
        button2State = digitalRead(button2Pin);  // Read this value again to test for a long press...
        if (button2State == LOW)  // button state is still low (long press), so publish an alternate webhook
        {
            Serial.println("yellow button - long press");
            // publish the event that will trigger our Webhook
            Particle.publish("WUastronomy_hook");
            digitalWrite(RED_PIN, HIGH);
            digitalWrite(GREEN_PIN, HIGH);
            digitalWrite(BLUE_PIN, LOW);
            delay(500);
        }
        else
        {
            Serial.println("yellow button - short press");
            // publish the event that will trigger our Webhook
            Particle.publish("WU4dayforecast_hook");
            digitalWrite(RED_PIN, HIGH);
            digitalWrite(GREEN_PIN, HIGH);
            digitalWrite(BLUE_PIN, LOW);
            delay(500);
        }
        TurnOffRGBLED();
    }
    
    
}

// This function will get called when WUconditions_hook event is published
void gotWeatherData(const char *name, const char *data) {
    String str = String(data);
    //Serial.println(data);
    
    char strBuffer[125] = "";
    str.toCharArray(strBuffer, 125);
    char weather[100];
    char humidity[100];
    char Winddir[100];
    
    strcpy (weather, strtok(strBuffer, "\"~"));
    float temp_f = atof(strtok(NULL, "~"));
    strcpy (humidity, strtok(NULL, "~"));
    strcpy (Winddir, strtok(NULL, "~"));
    float windmph = atof(strtok(NULL, "~"));
    float pressure = atof(strtok(NULL, "~"));
    int dewpoint_f = atoi(strtok(NULL, "~"));
    
    bool weatherGood = true;
    Serial.println("current weather...");
    Serial.println("-------------------------");
    Serial.print("weather: ");
    Serial.println(weather);
    Serial.print("temperature: ");
    Serial.println(temp_f);
    Serial.print("humidity: ");
    Serial.println(humidity);
    Serial.print("wind dir.: ");
    Serial.println(Winddir);
    Serial.print("wind in mph: ");
    Serial.println(windmph);
    Serial.print("pressure: ");
    Serial.print(pressure);
    Serial.println(" in.");
    Serial.print("dewpoint: ");
    Serial.print(dewpoint_f);
    Serial.println("F");
    Serial.println("");
    
}

// This function will get called when WU4dayforecast_hook event is published
void gotWeatherforecastData(const char *name, const char *data) {
    // Important note!  -- Right now the response comes in 512 byte chunks.
    //  This code assumes we're getting the response in large chunks, and this
    //  assumption breaks down if a line happens to be split across response chunks.
    String str = String(data);
    
    char dayofweek1[100];
    char dayofweek2[100];
    char dayofweek3[100];
    char dayofweek4[100];
    char strBuffer[125] = "";
    str.toCharArray(strBuffer, 125);
    strcpy (dayofweek1, strtok(strBuffer, "\"~"));
    int highday1 = atoi(strtok(NULL, "~"));
    int lowday1 = atoi(strtok(NULL, "~"));
    int windday1 = atoi(strtok(NULL, "~"));
    int pop1 = atoi(strtok(NULL, "~"));
    /*float qpfday1 = atof(strtok(NULL, "~"));
    if (qpfday1 == NULL)
        strcpy (dayofweek2, strtok(strBuffer, "~~"));
    else
        strcpy (dayofweek2, strtok(strBuffer, "~"));
    */
    strcpy (dayofweek2, strtok(NULL, "~"));
    int highday2 = atoi(strtok(NULL, "~"));
    int lowday2 = atoi(strtok(NULL, "~"));
    int windday2 = atoi(strtok(NULL, "~"));
    //int qpfday2 = atof(strtok(NULL, "~"));
    int pop2 = atoi(strtok(NULL, "~"));
    
    strcpy (dayofweek3, strtok(NULL, "~"));
    int highday3 = atoi(strtok(NULL, "~"));
    int lowday3 = atoi(strtok(NULL, "~"));
    int windday3 = atoi(strtok(NULL, "~"));
    //int qpfday3 = atof(strtok(NULL, "~"));
    int pop3 = atoi(strtok(NULL, "~"));
    
    strcpy (dayofweek4, strtok(NULL, "~"));
    int highday4 = atoi(strtok(NULL, "~"));
    int lowday4 = atoi(strtok(NULL, "~"));
    int windday4 = atoi(strtok(NULL, "~"));
    //int qpfday4 = atof(strtok(NULL, "~"));
    int pop4 = atoi(strtok(NULL, "~"));
    
    
    // set global variables...
    m_hightemp = highday1;
    m_maxwind = windday1;
    m_pop = pop1;
    
    Serial.println("4-day weather forecast...");
    Serial.println("-------------------------");
    Serial.println(dayofweek1);
    Serial.print("high temp: ");
    Serial.println(highday1);
    Serial.print("low temp: ");
    Serial.println(lowday1);
    Serial.print("max wind: ");
    Serial.println(windday1);
    Serial.print("probability of precip: ");
    Serial.println(pop1);
    Serial.println("");
    
    Serial.println(dayofweek2);
    Serial.print("high temp: ");
    Serial.println(highday2);
    Serial.print("low temp: ");
    Serial.println(lowday2);
    Serial.print("max wind: ");
    Serial.println(windday2);
    Serial.print("probability of precip: ");
    Serial.println(pop2);
    Serial.println("");
    
    Serial.println(dayofweek3);
    Serial.print("high temp: ");
    Serial.println(highday3);
    Serial.print("low temp: ");
    Serial.println(lowday3);
    Serial.print("max wind.: ");
    Serial.println(windday3);
    Serial.print("probability of precip: ");
    Serial.println(pop3);
    Serial.println("");
    
    Serial.println(dayofweek4);
    Serial.print("high temp: ");
    Serial.println(highday4);
    Serial.print("low temp: ");
    Serial.println(lowday4);
    Serial.print("max wind.: ");
    Serial.println(windday4);
    Serial.print("probability of precip: ");
    Serial.println(pop4);
    Serial.println("");
    
    // Set LED to indicate fine or poor weather for biking to work
    setLEDIndicator();
}

// This function will get called when WUastronomy_hook event is published
void gotWeatherAstronomyData(const char *name, const char *data) {
    String str = String(data);
    char strBuffer[125] = "";
    str.toCharArray(strBuffer, 125); 

    int strPercentIlluminated = atoi(strtok(strBuffer, "~"));
    int strAgeOfMoon = atoi(strtok(NULL, "~"));
    int strSunsetHour = atoi(strtok(NULL, "~"));
    int strSunsetMinute = atoi(strtok(NULL, "~"));
    int strSunriseHour = atoi(strtok(NULL, "~"));
    int strSunriseMinute = atoi(strtok(NULL, "~"));
    int strMoonsetHour = atoi(strtok(NULL, "~"));
    int strMoonsetMinute = atoi(strtok(NULL, "~"));
    int strMoonriseHour = atoi(strtok(NULL, "~"));
    int strMoonriseMinute = atoi(strtok(NULL, "~"));

    Serial.println("current astronomy...");
    Serial.println("-------------------------");
    Serial.print("Moon illumination: ");
    Serial.print(strPercentIlluminated);
    Serial.println("%");
    Serial.print("age of moon: ");
    Serial.print(strAgeOfMoon);
    Serial.println(" days");
    Serial.print("Sunset: ");
    Serial.print(strSunsetHour);
    Serial.print(":");
    Serial.println(strSunsetMinute);
    Serial.print("Sunrise: ");
    Serial.print(strSunriseHour);
    Serial.print(":");
    Serial.println(strSunriseMinute);
    Serial.print("Moonset: ");
    Serial.print(strMoonsetHour);
    Serial.print(":");
    Serial.println(strMoonsetMinute);
    Serial.print("Moonrise: ");
    Serial.print(strMoonriseHour);
    Serial.print(":");
    Serial.println(strMoonriseMinute);
    Serial.println("");
    
    
}

// This function will get called when WUalerts_hook event is published
void gotWeatherAlertsData(const char *name, const char *data) {
    String str = String(data);
    
    Serial.println("Weather alerts...");
    Serial.println("-------------------------");
    String alertStr = tryExtractString(str, "<alerts>", "</alerts>");
    
    if (alertStr != "")
    {
        alertStr = "Tornado warning";
        Serial.println("Alert!... " + alertStr);  // I'm hard-coding this to simulate a warning (for demo purposes only)
        
        if (alertStr.trim().length() > 0)
            weatherAlert = true;
        else
            weatherAlert = false;
            
    }
    else
    {
        Serial.println("There are no current alerts... " + alertStr);
        weatherAlert = false;
    }
    Serial.println("");
    
}

// At-a-glance indication of how nice the current day is (e.g. for biking to work)
void setLEDIndicator()
{
    weatherGood = true;
    if (m_hightemp < 50)
        weatherGood = false;
    else if (m_maxwind > 19)
        weatherGood = false;
    else if (m_pop > 39)
        weatherGood = false;
    
    if (weatherGood == true)
    {
        // Display green (for "fair") for 7 seconds
        for (int i = 0; i < 7; i++)
        {
            digitalWrite(RED_PIN, LOW);
            digitalWrite(GREEN_PIN, HIGH);
            digitalWrite(BLUE_PIN, LOW);
            //analogWrite(RED_PIN2, 0);
            //analogWrite(GREEN_PIN2, 255);
            //analogWrite(BLUE_PIN2, 0);
            delay(900);
            TurnOffRGBLED();
            delay(100);
        }
    }
    else
    {
        // Display red (for "poor") for 7 seconds
        for (int i = 0; i < 7; i++)
        {
            digitalWrite(RED_PIN, HIGH);
            digitalWrite(GREEN_PIN, LOW);
            digitalWrite(BLUE_PIN, LOW);
            //analogWrite(RED_PIN2, 255);
            //analogWrite(GREEN_PIN2, 0);
            //analogWrite(BLUE_PIN2, 0);
            delay(900);
            TurnOffRGBLED();
            delay(100);
        }
    }
    TurnOffRGBLED();
    
}

void TurnOffRGBLED()
{
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, LOW);
    
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
