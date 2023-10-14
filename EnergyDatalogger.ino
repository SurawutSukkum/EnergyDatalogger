#include <TinyGPSPlus.h>
#include <TimeLib.h>          // include Arduino time library
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h>

#include <PZEM004Tv30.h>
#include <SPI.h>
#include <SD.h>

const int chipSelect = 8;

#if !defined(PZEM_RX_PIN) && !defined(PZEM_TX_PIN)
#define PZEM_RX_PIN 2
#define PZEM_TX_PIN 3
#endif


SoftwareSerial pzemSWSerial(PZEM_RX_PIN, PZEM_TX_PIN);
PZEM004Tv30 pzem(pzemSWSerial);

#define SCREEN_WIDTH 128 // pixel ความกว้าง
#define SCREEN_HEIGHT 32 // pixel ความสูง 
 


static const int RXPin = 10, TXPin = 9;
static const uint32_t GPSBaud = 9600;

// The TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);


void setup()
{

   Serial.begin(115200);
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
   
  }
  Serial.println("card initialized.");


}


void loop()
{ 


    
    delay(100);  
    // Read the data from the sensor
    float voltage = pzem.voltage();
    float current = pzem.current();
    float power = pzem.power();
    float energy = pzem.energy();
    float frequency = pzem.frequency();
    float pf = pzem.pf();

    // Check if the data is valid
    if(isnan(voltage)){
        Serial.println("Error reading voltage");
    
    } else {


        //ss.begin(GPSBaud);
        //delay(100); 
        //char dateGps[32];
        //sprintf(dateGps, "%02d/%02d/%02d ", gps.date.day(), gps.date.month(), gps.date.year());
        //char timeGps[32];
        //sprintf(timeGps, "%02d/%02d/%02d ", gps.time.hour(), gps.time.minute(), gps.time.second());
        String data1 = "Voltage: " + String(voltage)  + " V "   + " Current: "   + String(current) +" A " + ", Power: " + String(power)+ " W " ;
        String data2 = "Energy: " + String(energy)   + " kWh " ;// 
        String data3 = "PF: " + String(pf)+ ", ";// + String(dateGps) + String(timeGps);
        String data4 = " Frequency: " + String((frequency))+" Hz " ;
        
        Serial.print(data1);
        Serial.print(data2);
        Serial.print(data3);
        Serial.println(data4);
        Serial.println("SAVE COMPLETE");
        sdcard(data1 + data2 + data3 +data4 );
    }
    

 delay(1000);
}


void sdcard(String dataString)
{


  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.csv", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}
