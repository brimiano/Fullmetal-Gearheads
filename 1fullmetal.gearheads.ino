#define IN1 2
#define IN2 3
#define IN3 4
#define IN4 5
#define SWITCH 6
// Front sonar
#define SONAR1_T A0
#define SONAR1_E A1
// Back sonar
#define SONAR2_T A2
#define SONAR2_E A3

#define threshold_distance 15

void setup()
{
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(SWITCH, INPUT_PULLUP); // enable
  pinMode(13, OUTPUT);
  digitalWrite(13, 0);
}

bool Read_Sonar(byte triggerPin, byte echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  unsigned long time_ = pulseIn(echoPin, HIGH);
  int cm = time_ * 0.01723;
  if (cm <= threshold_distance)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

void obstacle_detect(bool& front, bool& rear)
{
  front = Read_Sonar(SONAR1_T, SONAR1_E);
  rear = Read_Sonar(SONAR2_T, SONAR2_E);
}

void run_mode()
{
  bool f, r;
  
  obstacle_detect(f, r);
  if (f == 0)
  {
    MotorControl(1); // forward
    digitalWrite(13, 0);
  }
  else if (r == 0)
  {
    MotorControl(2); // backward
    digitalWrite(13, 0);
  }
  else
  {
    MotorControl(3); // turn (rotate to another side)
    digitalWrite(13, 1);
  }
}

void MotorControl(byte Direction)
{
  switch (Direction)
  {
    case 1: // forward
      digitalWrite(IN1, 1);
      digitalWrite(IN2, 0);
      digitalWrite(IN3, 1);
      digitalWrite(IN4, 0);
      break;
    
    case 2: // backward
      digitalWrite(IN1, 0);
      digitalWrite(IN2, 1);
      digitalWrite(IN3, 0);
      digitalWrite(IN4, 1);
      break;

    case 3: // turn (rotate by moving one side forward, other side backward)
      digitalWrite(IN1, 1);  // Left wheels forward
      digitalWrite(IN2, 0);
      digitalWrite(IN3, 0);  // Right wheels backward
      digitalWrite(IN4, 1);
      delay(500);  // Adjust the delay to control turn duration
      MotorControl(0);  // stop after turn
      break;
    
    case 0: // stop
      digitalWrite(IN1, 0);
      digitalWrite(IN2, 0);
      digitalWrite(IN3, 0);
      digitalWrite(IN4, 0);
      break;
  }
}

void loop()
{
  bool x = digitalRead(SWITCH);
  if (x == 0)
  {  
    run_mode();
  }
  else
  {
    MotorControl(0); // stop
    digitalWrite(13, 0);
  }
}

