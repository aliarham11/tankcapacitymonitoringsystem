#include <SoftwareSerial.h>

SoftwareSerial incomingData(10, 11); // RX, TX
//SoftwareSerial incomingData2(6, 7);

int BluetoothData; 
String message;
String bundle;
float data1;
float data2;
float tempData;
float averageHeight;
double lat;
double lon;
double alt;

double Datatransfer(char *data_buf,char num)
{                                                                           
  double temp=0.0;                           
  unsigned char i,j;
 
  if(data_buf[0]=='-')
  {
    i=1;
    //process the data array
    while(data_buf[i]!='.')
      temp=temp*10+(data_buf[i++]-0x30);
    for(j=0;j<num;j++)
      temp=temp*10+(data_buf[++i]-0x30);
    //convert the int type to the float type
    for(j=0;j<num;j++)
      temp=temp/10;
    //convert to the negative numbe
    temp=0-temp;
  }
  else//for the positive number
  {
    i=0;
    while(data_buf[i]!='.')
      temp=temp*10+(data_buf[i++]-0x30);
    for(j=0;j<num;j++)
      temp=temp*10+(data_buf[++i]-0x30);
    for(j=0;j<num;j++)
      temp=temp/10 ;
  }
  return temp;
}
 
char ID()//Match the ID commands
{
  char i=0;
  char value[6]={
    '$','G','P','G','G','A'      };//match the gps protocol
  char val[6]={
    '0','0','0','0','0','0'      };
 
  while(1)
  {
    if(Serial.available())
    {
      val[i] = Serial.read();//get the data from the serial interface
      if(val[i]==value[i]) //Match the protocol
      {   
        i++;
        if(i==6)
        {
          i=0;
          return 1;//break out after get the command
        }
      }
      else
        i=0;
    }
  }
}
 
void comma(char num)//get ','
{  
  char val;
  char count=0;//count the number of ','
 
  while(1)
  {
    if(Serial.available())
    {
      val = Serial.read();
      if(val==',')
        count++;
    }
    if(count==num)//if the command is right, run return
      return;
  }
 
}

double latitude()//get latitude
{
  char i;
  char lat[10]={
    '0','0','0','0','0','0','0','0','0','0'
  };
 
 
  if( ID())
  {
    comma(2);
    while(1)
    {
      if(Serial.available())
      {
        lat[i] = Serial.read();
        i++;
      }
      if(i==10)
      {
        i=0;
//        Serial.println(Datatransfer(lat,5)/100.0,7);//print latitude
        return Datatransfer(lat,5)/100.0,7;
      } 
    }
  }
}

double longitude()//get longitude
{
  char i;
  char lon[11]={
    '0','0','0','0','0','0','0','0','0','0','0'
  };
 
  if( ID())
  {
    comma(4);
    while(1)
    {
      if(Serial.available())
      {
        lon[i] = Serial.read();
        i++;
      }
      if(i==11)
      {
        i=0;
//        Serial.println(Datatransfer(lon,5)/100.0,7);
        return Datatransfer(lon,5)/100.0,7;
      } 
    }
  }
}

double altitude()//get altitude data
{
  char i,flag=0;
  char alt[8]={
    '0','0','0','0','0','0','0','0'
  };
 
  if( ID())
  {
    comma(9);
    while(1)
    {
      if(Serial.available())
      {
        alt[i] = Serial.read();
        if(alt[i]==',')
          flag=1;
        else
          i++;
      }
      if(flag)
      {
        i=0;
//        Serial.println(Datatransfer(alt,1),1);//print altitude data
        return Datatransfer(alt,1),1;
      } 
    }
  }
}

float getHeight(String message, String nodeSource, String endData)
{
  int indexStart;
  int indexEnd;

  indexStart = message.indexOf(nodeSource) + 6;
  indexEnd = message.indexOf(endData);
  return message.substring(indexStart, indexEnd).toFloat();
  
}

void setup() {
  
  pinMode(3,OUTPUT);//The default digital driver pins for the GSM and GPS mode
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  digitalWrite(5,HIGH);
  delay(1500);
  digitalWrite(5,LOW);
 
  digitalWrite(3,LOW);//Enable GSM mode
  digitalWrite(4,HIGH);//Disable GPS mode
  delay(2000);
  
  
  incomingData.begin(9600);
// incomingData2.begin(9600);
  //incomingData1.println("Hello from Center Node! type some string..");
  Serial.begin(9600);
  delay(5000);//GPS ready
 
  Serial.println("AT");  
  delay(2000);
  //turn on GPS power supply
  Serial.println("AT+CGPSPWR=1");
  delay(1000);
  //reset GPS in autonomy mode
  Serial.println("AT+CGPSRST=1");
  delay(1000);
 
  digitalWrite(4,LOW);//Enable GPS mode
  digitalWrite(3,HIGH);//Disable GSM mode
  delay(2000);
  
  Serial.println("Listening data from node1 and node2..");
  
}

void loop() {
  while(incomingData.available())
  {
      message+=char(incomingData.read());
  }
  
  if(!incomingData.available() && message.indexOf("NODE1") > -1 && message.indexOf("NODE2") > -1)
  {
    data1 = getHeight(message, "NODE1", "en1");
    data2 = getHeight(message, "NODE2", "en2");
    averageHeight = (data1 + data2)/2;
    lat = latitude();
    lon = longitude();
    alt = altitude();
    bundle = String("height=" + (String)averageHeight + "&latidude=" + (String)lat  + "&longitude=" + (String)lon  + "&altitude=" + (String)alt);
    Serial.println(bundle);
    message = "";

  }  
  delay(5000);
}

