#include <Wire.h>
#include <SoftwareSerial.h>

SoftwareSerial BTSerial(10, 11);
int reading = 0;
String header="NODE1:";
String message;

void setup()
{
  pinMode(9, OUTPUT);  // this pin will pull the HC-05 pin 34 (key pin) HIGH to switch module to AT mode
  digitalWrite(9, HIGH);
  Wire.begin();                
  Serial.begin(9600);    
  BTSerial.begin(38400);  // HC-05 default speed in AT command more
  
  BTSerial.println("AT");
  delay(1000);
  BTSerial.println("AT+RESET");
  delay(1000);
  BTSerial.println("AT+RMAAD");
  delay(1000);
  BTSerial.println("AT+INIT");
  delay(1000);
  BTSerial.println("AT+LINK=98D3,31,2018A1");
  delay(1000);  
}

void loop()
{
  
  
  Wire.beginTransmission(112); // 
  
  Wire.write(byte(0x00));
  Wire.write(byte(0x51));     
  
  Wire.endTransmission();     
  
  delay(70);                   
  
  Wire.beginTransmission(112); 
  Wire.write(byte(0x02));
  Wire.endTransmission();
  
  Wire.requestFrom(112, 2);    
  
  if (2 <= Wire.available())   
  {
    reading = Wire.read();  
    reading = reading << 8;    
    reading |= Wire.read();
    Serial.println(reading);   
    
    message = header + reading;
    BTSerial.println(message);
    //Serial.println("cm");
  }

  delay(5000);                  
}
