#include <SoftwareSerial.h>

SoftwareSerial BTSerial(10, 11); // RX | TX
String message="message from node1..!";


void setup()
{
  pinMode(9, OUTPUT);  // this pin will pull the HC-05 pin 34 (key pin) HIGH to switch module to AT mode
  digitalWrite(9, HIGH);
  Serial.begin(9600);
//  Serial.println("Enter AT commands:");
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
    
  // Keep reading from HC-05 and send to Arduino Serial Monitor
//  if (BTSerial.available())
//    Serial.write(BTSerial.read());

  // Keep reading from Arduino Serial Monitor and send to HC-05
//  if (Serial.available())
    BTSerial.println(message);
    delay(5000);
}
