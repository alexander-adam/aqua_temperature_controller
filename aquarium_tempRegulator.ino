#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

DeviceAddress insideThermometer;


// Raws 
//#define R1 2
#define R1 7  //NEW
#define R2 3
#define R3  4
#define R4 5
#define R5 6
//#define R6 7  //error
#define R7 8
#define R8  9
//Columns
#define C3 10
#define C2 11
#define C1 12
//#define C4  13  //error
#define C5 A0
#define C6 A1
#define C7 A2
#define C8 A3


int temperatur=0;

int b=0;

int delTime=10000; // 10 seconds


//output for rele
int rele=A5;

//max temp before opening rele
int kippunkt = 18;
bool coolerStopped = true;

void  setup() {
  Serial.begin(9600);
  sensors.begin();
  if (!sensors.getAddress(insideThermometer, 0)) {
    Serial.println("Unable to find address for Device 0");
  } 
  sensors.setResolution(insideThermometer, 9);

  int waterTempInt=printTemperature(insideThermometer);

  temperatur=waterTempInt;
  
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3,  OUTPUT);
  pinMode(R4, OUTPUT);
  pinMode(R5, OUTPUT);
  //pinMode(R6,  OUTPUT);
  pinMode(R7, OUTPUT);
  pinMode(R8, OUTPUT);
  pinMode(C1,  OUTPUT);
  pinMode(C2, OUTPUT);
  pinMode(C3, OUTPUT);
  //pinMode(C4,  OUTPUT);
  pinMode(C5, OUTPUT);
  pinMode(C6, OUTPUT);
  pinMode(C7,  OUTPUT);
  pinMode(C8, OUTPUT);
  //Turning all the LEDs off at the start  of the sketch
  digitalWrite(R1,HIGH);
  digitalWrite(R2,HIGH);
  digitalWrite(R3,HIGH);
  digitalWrite(R4,HIGH);
  digitalWrite(R5,HIGH);
  //digitalWrite(R6,HIGH);
  digitalWrite(R7,HIGH);
  digitalWrite(R8,HIGH);
  digitalWrite(C1,LOW);
  digitalWrite(C2,LOW);
  digitalWrite(C3,LOW);
  //digitalWrite(C4,LOW);
  digitalWrite(C5,LOW);
  digitalWrite(C6,LOW);
  digitalWrite(C7,LOW);
  digitalWrite(C8,LOW);

  pinMode(rele, OUTPUT);
}


void selectRow(int row){
  if (row==1)  digitalWrite(R1,LOW);
  if (row==2) digitalWrite(R2,LOW);
  if (row==3) digitalWrite(R3,LOW);
  if (row==4) digitalWrite(R4,LOW);
  if (row==5)  digitalWrite(R5,LOW);
  //if (row==6) digitalWrite(R6,LOW);
  if (row==7) digitalWrite(R7,LOW);
  if (row==8) digitalWrite(R8,LOW);
}

void  selectColumn(int column){
  if (column==1) digitalWrite(C1,HIGH);
  if (column==2) digitalWrite(C2,HIGH);
  if (column==3)  digitalWrite(C3,HIGH);
  //if (column==4) digitalWrite(C4,HIGH);
  if (column==5) digitalWrite(C5,HIGH);
  if (column==6) digitalWrite(C6,HIGH);
  if (column==7)  digitalWrite(C7,HIGH);
  if (column==8) digitalWrite(C8,HIGH);

}

void eraseScreen(){
  digitalWrite(C1,LOW);
  digitalWrite(C2,LOW);
  digitalWrite(C3,LOW);
  digitalWrite(C5,LOW);
  digitalWrite(C6,LOW);
  digitalWrite(C7,LOW);
  digitalWrite(C8,LOW);

  digitalWrite(R1,HIGH);
  digitalWrite(R2,HIGH);
  digitalWrite(R3,HIGH);
  digitalWrite(R4,HIGH);
  digitalWrite(R5,HIGH);
  digitalWrite(R7,HIGH);
  digitalWrite(R8,HIGH);
}


int printTemperature(DeviceAddress deviceAddress)
{
  sensors.requestTemperatures();
  float tempC = sensors.getTempC(deviceAddress);
  /*
  if(tempC == DEVICE_DISCONNECTED_C) 
  {
    return;
  }
  */
  return tempC;
}

void loop() {
  // Temperature is higher than 10 (second_digit != 0). That means, two digits should be displayed.
  if ((temperatur/10%10) != 0) {
    //Serial.println(temperatur);
    int nmb1 = temperatur%10;
    int nmb2 = (temperatur/10)%10;
    printNmbOnScreen(temperatur%10, true);
    printNmbOnScreen((temperatur/10)%10, false);
  }
  else {
    printNmbOnScreen(temperatur, false);
  }

  //wait till delTime is over to check the temp
  int a = millis();
  if (a-b > delTime) {
    int waterTempInt = printTemperature(insideThermometer);
    temperatur = waterTempInt;
    b += delTime;
  }


  //turn on/off the vent
  if (
    ((temperatur > kippunkt) && !coolerStopped) ||
    ((temperatur > (kippunkt + 1)) && coolerStopped)
  ) {  // error
    digitalWrite(rele, HIGH);
    coolerStopped = false;
    Serial.println("Rele is open");
  }
  else {
    digitalWrite(rele, LOW);
    coolerStopped = true;
    Serial.println("Rele is closed");
  }
}

//print a digit in one of the two spots on the display
void printNmbOnScreen(int nmb, boolean secNmb){
  int aditionalSpaceForNmb=0;
  if(secNmb==true){
    aditionalSpaceForNmb=4;
  }
  switch (nmb){
    case 0:
      selectRow(1);
      selectColumn(1+aditionalSpaceForNmb);
      selectColumn(2+aditionalSpaceForNmb);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      selectRow(2);
      selectColumn(1+aditionalSpaceForNmb);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      selectRow(3);
      selectColumn(1+aditionalSpaceForNmb);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      selectRow(4);
      selectColumn(1+aditionalSpaceForNmb);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      selectRow(5);
      selectColumn(1+aditionalSpaceForNmb);
      selectColumn(2+aditionalSpaceForNmb);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      break;
    case 1:
      selectRow(1);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      selectRow(2);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      selectRow(3);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      selectRow(4);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      selectRow(5);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      break;
    case 2:
      selectRow(1);
      selectColumn(1+aditionalSpaceForNmb);
      selectColumn(2+aditionalSpaceForNmb);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      
      selectRow(2);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      
      selectRow(3);
      selectColumn(1+aditionalSpaceForNmb);
      selectColumn(2+aditionalSpaceForNmb);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      
      selectRow(4);
      selectColumn(1+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      
      selectRow(5);
      selectColumn(1+aditionalSpaceForNmb);
      selectColumn(2+aditionalSpaceForNmb);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      break;
    case 3:
      selectRow(1);
      selectColumn(1+aditionalSpaceForNmb);
      selectColumn(2+aditionalSpaceForNmb);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      selectRow(2);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      selectRow(3);
      selectColumn(1+aditionalSpaceForNmb);
      selectColumn(2+aditionalSpaceForNmb);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      selectRow(4);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      selectRow(5);
      selectColumn(1+aditionalSpaceForNmb);
      selectColumn(2+aditionalSpaceForNmb);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      break;
    case 4:
      selectRow(1);
      selectColumn(1+aditionalSpaceForNmb);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      
      selectRow(2);
      selectColumn(1+aditionalSpaceForNmb);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      
      selectRow(3);
      selectColumn(1+aditionalSpaceForNmb);
      selectColumn(2+aditionalSpaceForNmb);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      
      selectRow(4);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      
      selectRow(5);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      break;
    case 5:
      selectRow(1);
      selectColumn(1+aditionalSpaceForNmb);
      selectColumn(2+aditionalSpaceForNmb);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      
      selectRow(2);
      selectColumn(1+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      
      selectRow(3);
      selectColumn(1+aditionalSpaceForNmb);
      selectColumn(2+aditionalSpaceForNmb);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      
      selectRow(4);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      
      selectRow(5);
      selectColumn(1+aditionalSpaceForNmb);
      selectColumn(2+aditionalSpaceForNmb);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      break;
    case 6:
      selectRow(1);
      selectColumn(1+aditionalSpaceForNmb);
      selectColumn(2+aditionalSpaceForNmb);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      
      selectRow(2);
      selectColumn(1+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      
      selectRow(3);
      selectColumn(1+aditionalSpaceForNmb);
      selectColumn(2+aditionalSpaceForNmb);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      
      selectRow(4);
      selectColumn(1+aditionalSpaceForNmb);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      
      selectRow(5);
      selectColumn(1+aditionalSpaceForNmb);
      selectColumn(2+aditionalSpaceForNmb);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      break;
    case 7:
      selectRow(1);
      selectColumn(1+aditionalSpaceForNmb);
      selectColumn(2+aditionalSpaceForNmb);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      
      selectRow(2);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      
      selectRow(3);
      selectColumn(2+aditionalSpaceForNmb);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      
      selectRow(4);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      
      selectRow(5);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      break;
    case 8:
      selectRow(1);
      selectColumn(1+aditionalSpaceForNmb);
      selectColumn(2+aditionalSpaceForNmb);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      
      selectRow(2);
      selectColumn(1+aditionalSpaceForNmb);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      
      selectRow(3);
      selectColumn(1+aditionalSpaceForNmb);
      selectColumn(2+aditionalSpaceForNmb);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      
      selectRow(4);
      selectColumn(1+aditionalSpaceForNmb);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      
      selectRow(5);
      selectColumn(1+aditionalSpaceForNmb);
      selectColumn(2+aditionalSpaceForNmb);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      break;
    case 9:
      selectRow(1);
      selectColumn(1+aditionalSpaceForNmb);
      selectColumn(2+aditionalSpaceForNmb);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      
      selectRow(2);
      selectColumn(1+aditionalSpaceForNmb);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      
      selectRow(3);
      selectColumn(1+aditionalSpaceForNmb);
      selectColumn(2+aditionalSpaceForNmb);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      
      selectRow(4);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      
      selectRow(5);
      selectColumn(1+aditionalSpaceForNmb);
      selectColumn(2+aditionalSpaceForNmb);
      selectColumn(3+aditionalSpaceForNmb);
      delay(1);
      eraseScreen();
      break;
  }
}
