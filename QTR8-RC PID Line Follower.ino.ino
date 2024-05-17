#include <QTRSensors.h>

#define r1 2 //IN1
#define r2 4 //IN2
#define enR 3
#define enL 5
#define l1 7 //IN3
#define l2 8 //IN4
 float t[2];
 int b[2];

 int d;
 int lastError = 0;
 int error=0;
 int power_difference=0;
//700
 int k=950;
 int j=500;


const int trigPin = 12;
const int echoPin = 11;

float dmin=7;

//0.1 0.1     0.8 0.5      0.6 0 6353   0.9 2 6355    1.4 2.5 7365   1.4 5 9078   1.6 10  9585   1.7 7 9080    1.4 20 8565 2 100   3 70 8565  2 60 9070   0.5 60 150 70     0.4 1 10070    0.2 1 110 65   0.3 5 110 65   0.3 5 120 75
#define SETPOINT    3500 
#define KP          0.3
#define KD        5
#define MAX_SPEED   110
#define SET_SPEED   65
#define MIN_SPEED   0


#define NUM_SENSORS             8  
#define NUM_SAMPLES_PER_SENSOR  2  // average 4 analog samples per sensor reading
#define EMITTER_PIN             10  // emitter is controlled by digital pin 2

QTRSensorsAnalog qtra((unsigned char[]) {A0,A1, A2, A3,A4,A5,A6,A7},NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS];




void setup()
{
 Serial.begin(9600);
 pinMode(r1,OUTPUT);
 pinMode(r2,OUTPUT);
 pinMode(l1,OUTPUT);
 pinMode(l2,OUTPUT);
 pinMode(enL,OUTPUT);
 pinMode(enR,OUTPUT);
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);



  for (int i = 0; i < 400; i++)  // make the calibration take about 10 seconds
  {
    qtra.calibrate();  
  }
   



}

void loop()

{     
  unsigned int position = qtra.readLine(sensorValues);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  float d = duration *0.034/ 2;
  int p;
  
if((d<dmin))
{
              digitalWrite(l1,HIGH);
              digitalWrite(l2,LOW);
              digitalWrite(r1,HIGH);
              digitalWrite(r2,LOW);
              analogWrite(enL,170);
              analogWrite(enR,166);
              p=1;
              delay(700);
              digitalWrite(l1,LOW);
              digitalWrite(l2,LOW);
              digitalWrite(r1,LOW);
              digitalWrite(r2,LOW);
              analogWrite(enL,0);
              analogWrite(enR,0);
              delay(600);
}else{






       p=0;
       if ((sensorValues[7]>k)&&(sensorValues[6]>k))
        {
          right();

        }else
        {
              if((sensorValues[1]>k)&&(sensorValues[0]>k))
              {

                left();
              }else
              { 
               
                  if((sensorValues[7]<j)&&(sensorValues[6]<j)&&(sensorValues[5]<j)&&(sensorValues[4]<j)&&(sensorValues[3]<j)&&(sensorValues[2]<j)&&(sensorValues[1]<j)&&(sensorValues[0]<j))
                  {
              digitalWrite(l1,HIGH);
              digitalWrite(l2,LOW);
              digitalWrite(r1,HIGH);
              digitalWrite(r2,LOW);
              analogWrite(enL,90);
              analogWrite(enR,86);
                  }else
                  {
                 
                   pid(); 
                    
                  }
                }
              }

          
        
  }
  for (unsigned char i = 0; i <NUM_SENSORS; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  Serial.print(b[0]);
  Serial.print("\t");
  Serial.print(b[1]);
  Serial.print("\t");
  Serial.print(position);
 Serial.print("\t");
   Serial.print(d);
 Serial.print("\t");
 Serial.println(p); // comment this line out if you are using raw values
  
}



void pid()
{

              unsigned int position = qtra.readLine(sensorValues);
              error = ((float)position - 3500);  
              d=error - lastError;
              lastError = error;
              power_difference = error*KP + d*KD;
              b[0]=constrain(SET_SPEED + power_difference, MIN_SPEED, MAX_SPEED);
              b[1]=constrain(SET_SPEED - power_difference-5, MIN_SPEED, MAX_SPEED);
          
              digitalWrite(l1,HIGH);
              digitalWrite(l2,LOW);
              digitalWrite(r1,HIGH);
              digitalWrite(r2,LOW);
              analogWrite(enL,b[0]);
              analogWrite(enR,b[1]);

          
}


void left()
{
 
    unsigned int position = qtra.readLine(sensorValues);
    while (sensorValues[5]<950) {
      position = qtra.readLine(sensorValues);
      digitalWrite(l1, LOW);
      digitalWrite(l2,LOW);
      digitalWrite(r1, HIGH);
      digitalWrite(r2, LOW);
      b[0]=0;
      b[1]=65;
      analogWrite(enR,b[1]);
      analogWrite(enL,b[0]);
  
    }

      pid();
}

void right()
{
  unsigned int position = qtra.readLine(sensorValues);
    while (sensorValues[2]<950) {
      position = qtra.readLine(sensorValues);
      digitalWrite(l1, HIGH);
      digitalWrite(l2, LOW);
      digitalWrite(r1, LOW);
      digitalWrite(r2,LOW);
      b[0]=70;
      b[1]=0;
      analogWrite(enR, b[1]);
      analogWrite(enL, b[0]);

    }

    pid();
}


//delay la pente avant stop pour determiner la largeur de la pente 
//fonction gauche et droite avec leurs seuilss
//



