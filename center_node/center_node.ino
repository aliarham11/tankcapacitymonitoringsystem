#include <SoftwareSerial.h>

SoftwareSerial incomingData(10, 11); // RX, TX
//SoftwareSerial incomingData2(6, 7);

int BluetoothData; 
String message;
float data1;
float data2;
float tempData;
float averageHeight;

void setup() {
  incomingData.begin(9600);
// incomingData2.begin(9600);
  //incomingData1.println("Hello from Center Node! type some string..");
  Serial.begin(9600);
  Serial.println("Listening data from node1 and node2..");
  
}

void loop() {
  while(incomingData.available())
  {
      message+=char(incomingData.read());
  }
  
  if(!incomingData.available())
  {
    if(message!="")
    {
      tempData = message.substring(6).toFloat();
      
      if(message.indexOf("NODE1") > -1)
        data1 = tempData;
      
      if(message.indexOf("NODE2") > -1)
        data2 = 72.0;
      
      if(data1!=0.0 && data2!=0.0)
      {
        averageHeight = (data1 + data2)/2;
        Serial.println("Liquid Level : " + (String)averageHeight);
      }
      message="";
      data1=0.0;
      data2=0.0;
      averageHeight = 0.0;
    }
  }  
  delay(5000);
}

