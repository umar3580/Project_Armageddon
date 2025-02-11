/***********************************************************
File name: AdeeptArmInitializationCode.ino
Description: Power on, the servo rotates 90 to straighten the robotic arm
Website: www.adeept.com
E-mail: support@adeept.com
Author: Tom
Date: 2019/04/26
***********************************************************/
#include <Servo.h>。
#include <stdio.h>
#define noServes 5

int servopin0 = 9;    //Define servo interface digital interface 9
int servopin1 = 6;    //Define servo interface digital interface 6
int servopin2 = 5;    //Define servo interface digital interface 5
int servopin3 = 3;    //Define servo interface digital interface 3
int servopin4 = 11;   //Define servo interface digital interface 11

Servo servo0;
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;



int setupAngle = 90;        //setupAngle of rotation of the servo


typedef struct 
{
  int servoIndex;
  Servo serve;
  int curPos;
  int instruction;
  bool detached;
  int servoPin;
  int turnolodgy;


}
servoInfo;

bool started = false;
const byte numChars = 32;
bool dataRec = false;
char receivedChars[numChars];

servoInfo servos[noServes];
int wrapInput(char in);

int makeInt(char value[numChars]);
void writeServo(servoInfo selected);
int readServo(Servo selected);
void attachDetach(servoInfo selected, bool attach);
void debug(int intx = 699, char charx = 'x', char churn[1] = {"n"});

void setup() {
  // put your setup code here, to run once:
  pinMode(servopin0,OUTPUT);//Set the servo interface as the output interface
  pinMode(servopin1,OUTPUT);//Set the servo interface as the output interface
  pinMode(servopin2,OUTPUT);//Set the servo interface as the output interface
  pinMode(servopin3,OUTPUT);//Set the servo interface as the output interface
  pinMode(servopin4,OUTPUT);//Set the servo interface as the output interface
  Serial.begin(9600);
}




void loop()
{
  if (started == false)
  {
    startUp();
    delay(200);
  }


  takeCmd();
  
  if (dataRec == true)
  {
    int serveIndex = wrapInput(receivedChars);
    writeServo(servos[serveIndex]);
  }

}


void startUp()
{
  Serial.println("starting up");

  servos[0] = {0, servo0, 0, setupAngle, true, servopin0, 0};
  servos[1] = {1, servo1, 0, setupAngle, true, servopin1, 0};
  servos[2] = {2, servo2, 0, setupAngle, true, servopin2, 0};
  servos[3] = {3, servo3, 0, setupAngle, true, servopin3, 0}; 
  servos[4] = {4, servo4, 0, setupAngle, true, servopin4, 0};


  for (int iu = 0; iu < noServes; iu++)
  {
    attachDetach(servos[iu], true);
    writeServo(servos[iu]);
  }
  delay(20);
  
  started = true;
  Serial.println("----Setup Complete----");

}


void takeCmd()
{
  static boolean cmdInTransmit = false;
  static byte byteIndex = 0;
  char cmdStart = '<';
  char cmdEnd = '>';
  char recInput;


  while ((Serial.available() > 0) && (dataRec == false))
  {
    recInput = Serial.read();

    if (cmdInTransmit == true)
    {
      if (recInput != cmdEnd)
      {
        receivedChars[byteIndex] = recInput;
        byteIndex++;

        //if we've reached max chars, overwrite last
        if (byteIndex >= numChars)
        {
          byteIndex = numChars - 1;
        } 
      }
      else
      {
        receivedChars[byteIndex] = '\0';
        cmdInTransmit = false;
        byteIndex = 0;
        dataRec = true;
      }

    }
    else if (recInput == cmdStart)
    {
      cmdInTransmit = true;
    }

  }
}


int wrapInput(char in)
{
  Serial.println(receivedChars);

  int iu = 0;
  int servInt = 0;
  int moveCount=0;
  char serve[numChars];
  char moveInt[numChars];

  for (int i=0; receivedChars[i]; i++)
  {
      if (i == servInt)
      {
          if (receivedChars[i] == 's')
          {
            setup();
          }
          else if (receivedChars[i] != '-')
          {
              serve[0] = receivedChars[i];
              servInt++;
          }

      }

      else if (isdigit(receivedChars[i]))
      {
          moveInt[moveCount] = receivedChars[i];
          moveCount++;
      }
      iu++;

  }
  moveInt[moveCount] = '\0';
  serve[1] = '\0';

  int serveIndex;
  serveIndex = makeInt(serve);
  
  if (serveIndex < noServes)
  {
    servos[serveIndex].instruction = makeInt(moveInt);
    dataRec = false;
    return serveIndex;
  }
  else
  {
    Serial.println("Chosen wrong Servo");
    servos[0].instruction = makeInt(moveInt);
    dataRec = false;
    return 0;

  }


}

int makeInt(char value[numChars])
{
  //char* pValue, int lenValue
  // 0x5, 0x6
  // 0x56
  // 0x50+0x6
  // (int)value[0] << 31 + ((int)value[1] << 30)
  int valueInt = 0;
  sscanf(value, "%d", &valueInt);
  
  return valueInt;
}

void attachDetach(servoInfo selected, bool attach)
{
  if (attach == true)
  {
    if (selected.serve.attached() != true)
    {
      selected.serve.attach(selected.servoPin);
    }

  }
  else
  {
    selected.serve.detach();
  }
  
  if (selected.serve.attached() != true)
  {
    selected.detached = true;
  }
  else
  {
    selected.detached = false;
  }
}
void writeServo(servoInfo selected)
{
  selected.serve.write(selected.instruction);
  selected.turnolodgy++;
  selected.curPos = selected.instruction;
}

int readServo(Servo selected)
{
  return selected.read();
}