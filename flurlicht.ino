
#define ONTIME 30000

#define PINPIR 2
#define PINLED 10
#define BLED 13
#define PINLDRPWR 9
#define PINLDR A6

int sensorValue = 0;
int fade_state = 0;
int pir = 0;
unsigned long time_to_off = 0;

// Install the interrupt routine.
void pir_int_proc() {
  pir = digitalRead(PINPIR);
  time_to_off = millis() + ONTIME;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(PINLED, OUTPUT);
  pinMode(PINPIR, INPUT);
  pinMode(PINLDRPWR, OUTPUT);
  pinMode(BLED, OUTPUT);

  digitalWrite(PINLDRPWR, HIGH);

  while(digitalRead(PINPIR) == 1) {
    digitalWrite(BLED, 1);
    delay(100);
    digitalWrite(BLED, 0);
    delay(100);
  }

  attachInterrupt(digitalPinToInterrupt(PINPIR), pir_int_proc, RISING);
}

void fadein()
{
  if(!fade_state) {
    for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 5) {
      // sets the value (range from 0 to 255):
      analogWrite(PINLED, fadeValue);
      // wait for 30 milliseconds to see the dimming effect
      delay(30);
    }
    fade_state = 1;
  }
}

void fadeout()
{
  if(fade_state) {
    // fade out from max to min in increments of 5 points:
    for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 5) {
      // sets the value (range from 0 to 255):
      analogWrite(PINLED, fadeValue);
      // wait for 30 milliseconds to see the dimming effect
      delay(90);

      // falls ein neuer PIR int kommt wieder hell machen und abbrechen
      if(pir) {
        analogWrite(PINLED, 255);
        pir = 0;
        return;
      }
    }
    fade_state = 0;
  }
}



void loop() {

  sensorValue = analogRead(PINLDR);
  
  Serial.print("ldr = ");
  Serial.println(sensorValue);
  Serial.print("time_to_off = ");
  Serial.println(time_to_off);
  Serial.print("millis = ");
  Serial.println(millis());

  digitalWrite(BLED, digitalRead(PINPIR));
  
  if(sensorValue < 340) {
    if(pir) {
      //digitalWrite(PINLED, HIGH);
      Serial.println("pir");
      pir = 0;
      fadein();
    } 
//    else {
//      //digitalWrite(PINLED, LOW);
//      Serial.println("nopir");
//      fadeout();
//    }
  } 

  if(time_to_off < millis()) {
    Serial.println("off");
    fadeout();
  }
  
  //else 
  //  digitalWrite(PINLED, LOW);

  delay(200);
}
