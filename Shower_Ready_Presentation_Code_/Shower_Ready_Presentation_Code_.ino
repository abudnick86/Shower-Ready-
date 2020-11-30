#include <DHT.h>
#include <Wire.h> // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD
#define BLYNK_PRINT Serial
// Include libraries required for Blynk and NodeMCU communication
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "PIthXgRLngQ_Kx0m5Pnd1xw0qWkF4UzZ";
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "AndroidAP";
char pass[] = "password";
#define DHT11_PIN 12
DHT DHT(DHT11_PIN, DHT11);
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 5, 16);
float user_input;

BLYNK_WRITE(V1)
{
  user_input = (float) param.asDouble();
  // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  Serial.print("V1 Slider value is: ");
  Serial.println(user_input);
}





float temperature;
float humidity;

String hs="Humidity: "+String((int)humidity)+" % ";
String ts="Temp: "+String((int)temperature)+" C ";

void printer() {
  lcd.setCursor(2, 0); // Set the cursor on the third column and first row.
  lcd.print(hs); // Print the string "Hello World!"
  lcd.setCursor(2, 1); //Set the cursor on the third column and the second row (counting starts at 0!).
  lcd.print(ts);
}

void myTimerEvent()
{
  temperature = DHT.readTemperature(); //float variables allow for decimals
  humidity = DHT.readHumidity();
  Blynk.virtualWrite(V4, temperature); // send data to app
  Blynk.virtualWrite(V5, humidity); // send data to app
  //these statements label and format the data for the serial monitor
  Serial.print( "T = " );
  Serial.print( temperature );
  Serial.print( " deg. C, H = " );
  Serial.print( humidity );
  Serial.println( "%" );

  delay(500);
  
}


void notify(){
  
  Blynk.notify("SHOWER READY");
  delay(5000);
  
  }

bool alert = true;
bool on = false;
bool fr = true;
int firstRead = 0; 



void checker(){
  
  if(fr){
  if (humidity != 0){
    firstRead = (int) humidity;
    fr = false;
  }else if(fr > (int) humidity){firstRead = humidity;}
  }
  Serial.print("First read is: ");
  Serial.println(firstRead);

  
  double function = function = user_input * 2;
  double differential = 0;
  
  if(!fr){
  differential = humidity - firstRead;
  }

  if (differential > 21) {function = differential;}
  boolean toggle1 = true;

    if (abs(function - differential + 0.1) < 1) {

    hs = "SHOWER READY!  ";
    ts = "                ";
    
    Serial.println( "CONDITIONAL TRUE" );
    
    notify();

    
    toggle1 = true;

      } else{
       
       Serial.println( "CONDITIONAL FALSE" );

       Serial.print("V1 Slider value is: ");
       Serial.println(user_input);
       
       
       if (toggle1){
        hs="Humidity: "+String((int)humidity)+" % ";
        ts="Temp: "+String((int)temperature)+" C ";
        toggle1 = false;}

       
        }
  }







void setup()
{
  
  Serial.begin(115200);
  DHT.begin();
  // Starts the connection with Blynk using the data provided at the top (Wi-Fi connection name, password, and auth token)
  Blynk.begin(auth, ssid, pass);
  lcd.init();
  lcd.backlight();
}
void loop()
{
  Blynk.run();
  myTimerEvent();
  checker();
  printer();
  
}
