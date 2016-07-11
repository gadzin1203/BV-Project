#define intensitySlider 0
#define periodSlider 2
// #define pauseSlider 4
// #define discoSlider 6

#define onCheckbox 1
// #define greenCheckbox 3
// #define randCheckbox 5
// #define discoCheckbox 7

int intensity = 0;
int period = 0;
bool on = false;

int minPeriod = 300;
int maxPeriod = 1500;
int range = maxPeriod - minPeriod;

int PWMA = 1; //Speed control
int AIN1 = 0; //Direction
int AIN2 = 2; //Direction
int STBY = 3; //standby

void setup() {
  Serial.begin(57600);
  Serial.setTimeout(25);

  // Motor Controller Setup
  pinMode(STBY, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  digitalWrite(STBY, LOW); //enable standby
}

void loop() {
  char buffer[64];
  size_t length = 64;
  length = Serial.readBytes(buffer, length);
  if (length > 0) {
    //Check which checkbox/slider has been changed
    switch (buffer[0]) {
      case onCheckbox:
        on = buffer[1];
        break;
      case intensitySlider:
        intensity = (int)buffer[1];
        break;
      case periodSlider:
        period = minPeriod + (range * (int)buffer[1]) / 255;
        break;
      default:
        break;
    }
  }
  if (on) {
    vibrate(intensity, period);
    if (period > 0) {
      Bean.sleep(period);
    }
  }
  else {
    digitalWrite(STBY, LOW);
    Bean.sleep(0xFFFFFFFF); // Sleep until a serial message wakes us up
  }
}

void vibrate(int v, int T) {
  digitalWrite(STBY, HIGH); //disable standby

  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;
  digitalWrite(AIN1, inPin1);
  digitalWrite(AIN2, inPin2);
  analogWrite(PWMA, v);
  if (T > 0) {
    delay(T);
    analogWrite(PWMA, 0);
    digitalWrite(STBY, LOW);
  }

  digitalWrite(STBY, LOW); //enable standby
}

