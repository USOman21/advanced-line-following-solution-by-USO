#include <Wire.h>

#define IRS1 12
#define IRS2 14
#define IRS3 27
#define IRS4 26
#define IRS5 25
#define IRS6 33

int temp;
int count;
int sendInfo;

float servoAngle, diff1, diff2, diffDiff, incr;
float tempr = 0;

void setup()
{
  Serial.begin(9600);
  while (!Serial) {}
  Wire.begin();
  Wire.setClock(400000);
  count = 0;
}

void loop()
{
  // put your main code here, to run repeatedly:
  float a = analogRead(IRS1);
  float b = analogRead(IRS2);
  float c = analogRead(IRS3);
  float d = analogRead(IRS4);
  float e = analogRead(IRS5);
  float f = analogRead(IRS6);
  diff1 = (- (15*(4090-a)/2945) - (10*(4090-b)/1750) - (5*(4090-c)/3080) + (5*(4090-d)/2025) + (10*(4090-e)/1470) + (15*(4090-f)/2125));

  delay(1);

  a = analogRead(IRS1);
  b = analogRead(IRS2);
  c = analogRead(IRS3);
  d = analogRead(IRS4);
  e = analogRead(IRS5);
  f = analogRead(IRS6);
  diff2 = (- (15*(4090-a)/2945) - (10*(4090-b)/1750) - (5*(4090-c)/3080) + (5*(4090-d)/2025) + (10*(4090-e)/1470) + (15*(4090-f)/2125));
  
  incr = incr + 0.5*(diff1 + diff2)*0.001;
  diffDiff = (diff2 - diff1)/0.001;
  Serial.println(servoAngle);
  servoAngle = 3.2 * (15 - diff2) + 0.05 * incr + 0.005 * diffDiff;

  if (servoAngle < 0)
  {
    servoAngle = 0;
  }
  else if (servoAngle > 110)
  {
    servoAngle = 100;
  }
  else
  {
    sendInfo = servoAngle;
  }

  if (b > 4080 && c > 4080 && d > 4080 && e > 4080 )
  {
    count = count + 1;
    tempr = 0;
    delay(10);
  }
  else
  {
    count = 0;
    Serial.println(count);
    temp = servoAngle;
    delay(10);
  }

  if (count >= 30 && temp > 56)
  {
    sendInfo = 240;
    sendData();
  }

  if (count >= 30 && temp < 56)
  {
    sendInfo = 245;
    sendData();
  }
  sendData();
}

void sendData()
{

  Wire.beginTransmission(0x08);  // transmit to device #8
  Wire.write(sendInfo);          //sends speed value to arduino
  Wire.endTransmission();        // stop transmitting

}
