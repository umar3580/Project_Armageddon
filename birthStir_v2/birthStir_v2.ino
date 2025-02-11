#include <Servo.h>。
#include <stdio.h>
#define noServes 5

int servopin0 = 9;    //Define servo interface digital interface 9
int servopin1 = 6;    //Define servo interface digital interface 6
int servopin2 = 5;    //Define servo interface digital interface 5

int servopin3 = 3;    //Define servo interface digital interface 3
int servopin4 = 11;   //Define servo interface digital interface 11
int setupAngle = 90;        //setupAngle of rotation of the servo

Servo servo0;
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

typedef struct 
{
  int servoIndex;
  Servo serve;
  int curPos;
  int instruction;
  bool detached;
  int servoPin;
  int turnolodgy;
  char req;

}
servoInfo;

bool started = false;
bool dataRec = false;
char ignore = 'i';

const byte numChars = 32;
char receivedChars[numChars];
servoInfo servos[noServes];

int wrapInput(char in);
int makeInt(char value[numChars]);

int readServo(int selected);
int diff(int i, int iu);

void writeServo(int selected);
void attachDetach(int selected, bool attach);

void debug_c(char* pOut, char ex[] = "DEBUG: ");
void debug_i(int pOut, char ex[] = "DEBUG: ");
void callManager(int selected);

void debug_c(char* pOut, char ex[] = "DEBUG: ")
{
  if (ex != "DEBUG: ")
  {
    Serial.print(ex);
    Serial.println(&pOut[0]);
  }
  else
  {
    Serial.print("DEBUG: ");
    Serial.println(&pOut[0]);
  }
}

void debug_i(int pOut, char ex[] = "DEBUG: ")
{
  if (ex != "DEBUG: ")
  {
    Serial.print(ex);
    Serial.println(pOut);
  }
  else
  {
    Serial.print("DEBUG: ");
    Serial.println(pOut);
  }
}

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
  // debug_c("GOING FOR ANOTHER LOOP");
  // delay(1000);
  if (dataRec == true)
  {
    int serveIndex = wrapInput(receivedChars);
    callManager(serveIndex);
  }
}

void startUp()
{
  debug_c("starting up", "INFO: ");
  servos[0] = {0, servo0, 0, setupAngle, true, servopin0, 0, ignore};
  servos[1] = {1, servo1, 0, setupAngle, true, servopin1, 0, ignore};
  servos[2] = {2, servo2, 0, setupAngle, true, servopin2, 0, ignore};
  servos[3] = {3, servo3, 0, setupAngle, true, servopin3, 0, ignore}; 
  servos[4] = {4, servo4, 0, setupAngle, true, servopin4, 0, ignore};

  for (int iu = 0; iu < noServes; iu++)
  {
    attachDetach(iu, true);
    writeServo(iu);
  }
  
  delay(20);
  started = true;
  debug_c("Setup Complete", "INFO: ");
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

        if (byteIndex >= numChars) //If reached max values, overwrite the last
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
  debug_c(receivedChars, "INPUT RECEIVED: ");

  int iu = 0;
  int servInt = 0;
  int moveCount = 0;
  
  char serve[numChars];
  char moveInt[numChars];
  char requestType = ignore;

  for (int i=0; receivedChars[i]; i++)
  {
      if (i == servInt)
      {
          if (receivedChars[i] != '-')
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

      else if (isalpha(receivedChars[i]))
      {
        requestType = receivedChars[i];
      }

      iu++;
  }
  moveInt[moveCount] = '\0';

  int serveIndex;
  serveIndex = makeInt(serve);
  
  if (serveIndex < noServes)
  {
    servos[serveIndex].instruction = makeInt(moveInt);
    servos[serveIndex].req = requestType;
    dataRec = false;
    return serveIndex;
  }
  else
  {
    debug_c("Chosen wrong Servo", "INFO: ");
    servos[0].instruction = makeInt(moveInt);
    servos[0].req = ignore;
    dataRec = false;
    return 0;
  }
}

void callManager(int selected)
{
  switch(servos[selected].req)
  {
    case 'w':
      writeServo(selected);
      break;
    
    case 'r':
      debug_i(readServo(selected), "Read Position: ");
      break;
    
    case 'a':
      attachDetach(selected, true);
      break;
    
    case 'd':
      attachDetach(selected, false);
      break;
    
    default:
      debug_c("Incorrect Request", "WARNING: ");
      break;

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

void attachDetach(int selected, bool attach)
{
  if (attach == true)
  {
    if (servos[selected].serve.attached() != true)
    {
      servos[selected].serve.attach(servos[selected].servoPin);
    }
  }
  else
  {
    servos[selected].serve.detach();
  }
  
  if (servos[selected].serve.attached() != true)
  {
    servos[selected].detached = true;
  }
  else
  {
    servos[selected].detached = false;
  }
}

void writeServo(int selected)
{
  int instructMagnitude = diff(servos[selected].curPos, servos[selected].instruction);

  if ((instructMagnitude > 20) && (started == true))
  {
    int slowDown = servos[selected].instruction;
    
    if (servos[selected].curPos > servos[selected].instruction)
    {
      slowDown = servos[selected].curPos - 10;
    }
    else if (servos[selected].curPos < servos[selected].instruction)
    {
      slowDown = servos[selected].curPos + 10;
    }
    servos[selected].serve.write(slowDown);
    servos[selected].turnolodgy++;
    
    servos[selected].curPos = slowDown;
    delay(200);
    writeServo(selected);
  }
  else
  {
    servos[selected].serve.write(servos[selected].instruction);
    servos[selected].curPos = servos[selected].instruction;
  }
}

int readServo(int selected)
{
  servos[selected].curPos = servos[selected].serve.read();
  return servos[selected].serve.read();
}

int diff(int i, int iu)
{
  if (i >= iu)
  {
    return i-iu;
  }
  else 
  {
    return iu-i;
  }
}
