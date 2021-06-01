/*
   Proyecto Investigación
   Agro OK Agricultura de Precisión
   Integrantes:
   Bryan Daza
   Johnny Tinjaca
   Christian Huerfano
   
   Sistemas Digitales
   Noviembre 2020
 
*/

#include <TinyGPS++.h>
#include <SoftwareSerial.h>
//AltSoftSerial emulates an additional serial port, allowing you to communicate with another serial device. 
#include <AltSoftSerial.h>


// Includes DHT humidity sensor library
#include "DHT.h"
#define DHTPIN A0     
#define DHTTYPE DHT11  

// Include lcd libraries
#include <Wire.h>
#include <LiquidCrystal_I2C.h>




static const int RXPin = 2, TXPin = 3;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

//Humidity sensor object
DHT dht(DHTPIN, DHTTYPE);

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

// establish connection with lcd
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);

//initialize altsoft serial
AltSoftSerial altSerial;

//Setup///////////////////////////////////////////////////////////////

void setup()
{
  Serial.begin(9600);
  ss.begin(9600);
  //Altsoft serial
  altSerial.begin(9600);
  //Initialize gps
  ss.begin(GPSBaud);
  //Initialize dht sensor
  dht.begin();

  
  
  Serial.println(F("Sats   Latitude   Longitude     Date       Time      Alt      Humidity       Temperature       Heat Index      "));
  Serial.println(F("           (deg)      (deg)                          (m)                                                       "));
  Serial.println(F("----------------------------------------------------------------------------------------------------------------------------------------"));

  //initialize lcd screen
  lcd.begin(16,2);
  lcd.clear();
  
}


////////////////////////////LOOP/////////////////////////////
void loop()
{
  //define variables for dht reading
  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  //Error check dht reading

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  //Compute heat index method
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);


  
  /* Print values in serial monitor
  printInt(gps.satellites.value(), gps.satellites.isValid(), 5);
  printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
  printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
  //printInt(gps.location.age(), gps.location.isValid(), 5);
  printDateTime(gps.date, gps.time);
  printFloat(gps.altitude.meters(), gps.altitude.isValid(), 7, 2);
  Serial.print(" ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(hic);
  Serial.print(" *C ");
  */
  
    

   //1. Checkings...

   //1. Battery Level///////////////////////////////////////////////////////////////
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Battery...");
    lcd.blink();
    smartDelay(7000);


   //Serial comm code///////////////////////////////////////////////////////////////
   if(Serial.available()){

        String a = Serial.readString();
       
        if (a.startsWith("80")){
          Serial.println("Battery 80% ");
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Battery 80%");
          smartDelay(5000);
          
        }
        else {
        lcd.clear();
        lcd.setCursor(0,0);  
        lcd.print("Recharge");
        smartDelay(5000);
        }
      }

      
   //Sensores///////////////////////////////////////////////////////////////
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Sensors...");
    lcd.blink();
    smartDelay(7000);
   
   if(Serial.available()){

        String b = Serial.readString();
       
        if (b.startsWith("ok")){
          Serial.println("Sensors Ok");
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Sensors Ok");
          smartDelay(5000);
          
        }
        else {
        lcd.clear();
        lcd.setCursor(0,0);  
        lcd.print("Check Sensors");
        smartDelay(5000);
        }
      }

    //Plan de vuelo///////////////////////////////////////////////////////////////
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Flight Plan...");
    lcd.blink();
    smartDelay(7000);
   
   if(Serial.available()){

        String c = Serial.readString();
       
        if (c.startsWith("ok")){
          Serial.println("Plan detected!");
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Plan detected!");
          smartDelay(5000);
          
        }
        else {
        lcd.clear();
        lcd.setCursor(0,0);  
        lcd.print("Check Plan");
        smartDelay(5000);
        }
      }

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("tests ok");
    lcd.blink();
    smartDelay(5000);

   //End test serial comm///////////////////////////////////////////////////////////////
    

   //2. Elevar///////////////////////////////////////////////////////////////

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Dron elevating");
    lcd.blink();
    smartDelay(7000);

    if(Serial.available()){

        String d = Serial.readString();
       
        if (d.startsWith("altmin")){
          Serial.println("Altitud: 1.50m");
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Altitud: 1.50m");
          smartDelay(5000);
         
          
        }
        else {
        lcd.clear();
        lcd.setCursor(0,0);  
        lcd.print("Waiting...");
        smartDelay(5000);
        }
      }
      
   //3. Recorrer///////////////////////////////////////////////////////////////
      //3.1 Calcular

   //Lat Lon actual
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Location:");
    smartDelay(5000);
    
    //Latitud
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("LT:");
    lcd.setCursor(3,0);
    lcd.print(gps.location.lat());
    Serial.println(gps.location.lat());
    //Longitud:
    lcd.setCursor(0,1);
    lcd.print("LN:");
    lcd.setCursor(3,1);
    lcd.print(gps.location.lng());
    smartDelay(5000);


    //3.2 ir///////////////////////////////////////////////////////////////
   
   

    for (int point = 1; point < 5; point++) {

      //Dron en curso
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Dron en curso...");
    smartDelay(10000);

    //Dron stop
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Dron stop.");
    smartDelay(5000);

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Punto ");
      lcd.print(point);
      smartDelay(5000);

      //Medir:
      //Collecting Data
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Collecting data");
      smartDelay(5000);

      //Show data
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Data:");
      smartDelay(2000);
    
    
      //Print values on lcd
      //Latitud
      
      lcd.setCursor(0,0);
      lcd.print("LT:");
      lcd.setCursor(3,0);
      lcd.print(gps.location.lat());

      //Temperatura:
      lcd.setCursor(10,0);
      lcd.print("T:");
      lcd.setCursor(12,0);
      lcd.print(t);

      //Longitud:
      lcd.setCursor(0,1);
      lcd.print("LN:");
      lcd.setCursor(3,1);
      lcd.print(gps.location.lng());

      //Humedad:
      lcd.setCursor(10,1);
      lcd.print("H:");
      lcd.setCursor(12,1);
      lcd.print(h);
      smartDelay(5000);

      // Print values in serial monitor
      printInt(gps.satellites.value(), gps.satellites.isValid(), 5);
      printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
      printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
      //printInt(gps.location.age(), gps.location.isValid(), 5);
      printDateTime(gps.date, gps.time);
      printFloat(gps.altitude.meters(), gps.altitude.isValid(), 7, 2);
      Serial.print(" ");
      Serial.print(h);
      Serial.print(" %\t");
      Serial.print(t);
      Serial.print(" *C ");
      Serial.print(hic);
      Serial.print(" *C ");
      Serial.println();

       
      
      
    
  }

    /*
  // Print values in serial monitor
      printInt(gps.satellites.value(), gps.satellites.isValid(), 5);
      printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
      printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
      //printInt(gps.location.age(), gps.location.isValid(), 5);
      printDateTime(gps.date, gps.time);
      printFloat(gps.altitude.meters(), gps.altitude.isValid(), 7, 2);
      Serial.print(" ");
      Serial.print(h);
      Serial.print(" %\t");
      Serial.print(t);
      Serial.print(" *C ");
      Serial.print(hic);
      Serial.print(" *C ");
      */


    //4 salir///////////////////////////////////////////////////////////////
    

    smartDelay(8000);
    //Data collected
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Data collected");
    smartDelay(5000); 
    //End course
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("End course");
    smartDelay(5000); 


    
    //Regresando
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Back to initial");
    lcd.setCursor(0,1);
    lcd.print("point");
    smartDelay(10000);

    //Arrived
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Arrived!");
    smartDelay(5000); 

    //Lat Lon actual
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Location:");
    smartDelay(5000);
    
    //Latitud
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("LT:");
    lcd.setCursor(3,0);
    lcd.print(gps.location.lat());
    Serial.println(gps.location.lat());
    //Longitud:
    lcd.setCursor(0,1);
    lcd.print("LN:");
    lcd.setCursor(3,1);
    lcd.print(gps.location.lng());
    smartDelay(5000); 

    //Descending
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Descending...");
    smartDelay(8000);

    //Dron stop
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Dron stop.");
    smartDelay(8000);


    

    

    

    
    
    
    
  /////check if data received from gps
  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));


   

    
    
}

//////////////////////////////////////////////////////////////////end loop


// This custom version of delay() ensures that the gps object
// is being "fed".
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}


static void printFloat(float val, bool valid, int len, int prec)
{
  if (!valid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartDelay(0);
}

static void printInt(unsigned long val, bool valid, int len)
{
  char sz[32] = "*****************";
  if (valid)
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
  Serial.print(sz);
  smartDelay(0);
}

static void printDateTime(TinyGPSDate &d, TinyGPSTime &t)
{
  if (!d.isValid())
  {
    Serial.print(F("********** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
    Serial.print(sz);
  }
  
  if (!t.isValid())
  {
    Serial.print(F("******** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
    Serial.print(sz);
  }

  //printInt(d.age(), d.isValid(), 5);
  smartDelay(0);
}

static void printStr(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
    Serial.print(i<slen ? str[i] : ' ');
  smartDelay(0);
}
