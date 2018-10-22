//Initialise Libraries, define constants

//incclude LCD Library
#include <LiquidCrystal.h>
// EmonLibrary examples openenergymonitor.org, Licence GNU GPL V3

#include "EmonLib.h" // Include Emon Library
#include <EEPROM.h>
EnergyMonitor emon1;             // Create an instance
//#define SAMPLE_TIME 5000  //The time between each EEPROM write function call in ms
  
int address = 0;      //EEPROM address counter /** the current address in the EEPROM (i.e. which byte we're going to write to next) **/
 
//unsigned long timer;

//float conv_coeff = 0.0;   //coefficient for converting from 0-1024 to 0-5 range
//void writebalance(); //declare function 
//void readbalance();
//void clearEEPROM();
int writecount = 0;


//Electricity Charge variables
float kwh;
float balance = 5000;
float value;
float block_track = 0;
float consumption;
float voltage;
float credit;
float owe;

//tariffing setup
float block_rate1 = 1.3516; //R/kwh
float block_rate2 = 2.1735; //R/kwh
float current_charge = 1.3516;


//initialise LCD Object
LiquidCrystal lcd(8,9,4,5,6,7);

//Pin setup for interrupt handling
int push1 = 2;
int push2 = 3;
int LED = 13;

int mode1;
int mode2;

void button();
void button2();
void tampering();
float metering();
void antenna();

int mode = 3;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(5000);
  pinMode(A6,INPUT);
  pinMode(11, INPUT);
  
  //Begin LCD Interface
  lcd.begin(16,2);
  
  //setup each module 

  emon1.voltage(2, 234.26, 1.7);  // Voltage: input pin, calibration, phase_shift
  emon1.current(1, 111.1);       // Current: input pin, calibration.
  //byte value = EEPROM.read(address);
  //balance = value;
  
  //float value = EEPROM.read(address);
  Serial.println("Value from EEzprom :");
  Serial.println(balance);

  delay(500);
  lcd.print("Kgomotso Display");
  
  //MOve Cursor
  delay(500);
  lcd.setCursor(0,1);
  lcd.print(balance);
  //Clear the Line 
  delay(500);

  //interrupt handling;
  pinMode(push1,INPUT);
  pinMode(push2, INPUT);
  pinMode(10,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(push1),button, RISING);
  attachInterrupt(digitalPinToInterrupt(push2),button2, RISING);
}

void loop() {
  // put your main code here, to run repeatedly:

  //Tamper Detection Module
  tampering();
    //check tampering module if any tampering has occured
    //if tampering has occured update tamper constant, lock and display tamper detected on screen-contact utility, break at the end

  //Metering Module Interaction
  metering();
  
    //update chare based on time-of-use
    //update balance 
    //display balance and time-of-use charge on screen 

  
  //Antenna Module 
    //send packets of data to data concentrator
  antenna();
    
  //customer interface module 
    //check buttons pressed by customer and modes
  switch (mode) {
     case 0:
        // statements
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Tamper Detected");
        lcd.setCursor(0,1);
        lcd.print("Locking Meter");
        digitalWrite(10,HIGH);
        //delay(1000);
        break;
      case 1:
        // statements
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Pwr Saving Mode");
        lcd.setCursor(0,1);
        lcd.print("Major loads off");
        digitalWrite(10,HIGH);
        creditmode();
        //delay(1000);
        break;
      case 2:
       // statements
        //pinMode(A7,INPUT);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Recharge Mode");
        lcd.setCursor(0,1);
        lcd.print("Scan Card");
        float recharge = analogRead(A7);
        Serial.println("Value read from a7 is:");
        Serial.println(recharge);
        delay(1000);
        // statements
        break;
    }


  
    
  //load control Module
    //check if balance is 0 and switch off load if balnce is 0
    //check remote control requests from utility and customer

  //wireless communication module
    //send consumtion information to cloud database
    //if tampering has occured send to utility 

  //card reader module 
    //allow custome rto upload credits if credit upload mode is pressed
    //if remote recharge has occured update balnce }
  }
  
float metering(){
  emon1.calcVI(20,2000);         // Calculate all. No.of half wavelengths (crossings), time-out
  emon1.serialprint();           // Print out all variables (realpower, apparent power, Vrms, Irms, power factor)
  
  float realPower       = emon1.realPower;        //extract Real Power into variable
  float apparentPower   = emon1.apparentPower;    //extract Apparent Power into variable
  float powerFactor     = emon1.powerFactor;      //extract Power Factor into Variable
  float supplyVoltage   = emon1.Vrms;             //extract Vrms into Variable
  float Irms            = emon1.Irms;             //extract Irms into Variable
  //delay(5000);
  
  delay(1000);
  //Calculate the consumption using c/kwh
  kwh= 1; //(realPower/1000)* (5/3600);
  consumption = kwh * current_charge; //update energy used
  balance = balance - consumption; //update the balance
  
  Serial.println(kwh);
  Serial.println(consumption);
  Serial.println(balance);
  Serial.println();
  delay(1000);
  
  lcd.setCursor(0,1);
  lcd.print("                                                                   ");
  delay(10);
  lcd.setCursor(0,0);
  lcd.print("Balance:");
  lcd.setCursor(0,1);
  lcd.print(balance);
  //pinMode(A7,INPUT);
    
  voltage = balance*(5.0/1023.0);
  Serial.println("Voltage:");
  Serial.println(voltage);

  return realPower;
  return apparentPower;
  return powerFactor; 
  return supplyVoltage;
  return Irms;
  return balance;
  }

//Function to handle tampering 
void tampering(){
    if(digitalRead(11)==HIGH){
      mode=0;
      }
    //check if input high, contunuosly polling this peripheral
    //if tamparing has occured get stuck in infinite loop and dislay on LCD
    
    }
    
//Functions to handle interrupts
void button(){
  mode = 1;
  digitalWrite(10, HIGH);
  }

void button2(){
  mode = 2;
  digitalWrite(13, HIGH);
  }
  
  
void antenna(){
  
  }

void creditmode(){
  
  emon1.calcVI(20,2000);         // Calculate all. No.of half wavelengths (crossings), time-out
  emon1.serialprint();           // Print out all variables (realpower, apparent power, Vrms, Irms, power factor)
  
  float realPower       = emon1.realPower;        //extract Real Power into variable
  float apparentPower   = emon1.apparentPower;    //extract Apparent Power into variable
  float powerFactor     = emon1.powerFactor;      //extract Power Factor into Variable
  float supplyVoltage   = emon1.Vrms;             //extract Vrms into Variable
  float Irms            = emon1.Irms;             //extract Irms into Variable
  //delay(5000);
  
  delay(1000);
  //Calculate the consumption using c/kwh
  kwh= 1; //(realPower/1000)* (5/3600);
  owe = kwh * current_charge; //update energy used
  credit = credit + owe; //update the balance
  
  Serial.println(kwh);
  Serial.println(owe);
  Serial.println(credit);
  Serial.println();
  delay(1000);
  
  lcd.setCursor(0,1);
  lcd.print("                                                                   ");
  delay(10);
  lcd.setCursor(0,0);
  lcd.print("Balance:");
  lcd.setCursor(0,1);
  lcd.print(balance);
  
  }
