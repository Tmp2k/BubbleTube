//changed by wilfy again

const int soundFreq = 100;  //sample every x milliseconds
const int soundTimeLong = 60; //seconds
const int soundTimeShort = 2; //seconds
const int soundTriggerLength = 3; //seconds
const float soundTriggerLevel = 1.1; //times louder than average
const int soundCutoff = 30;




const int rPin = 9;
const int gPin = 10;
const int bPin = 11;
const int buzzPin = 13;
const int micPin = A3;
const int lightPin = 3;
const int pumpPin = 12;


const unsigned long delayOff = 2400; //seconds to wait before turning off



const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
const byte rowPins[ROWS] = {0, 8, 7, 5};
const byte colPins[COLS] = {4, 2, 6};
const byte keys[ROWS][COLS] = {
  {1,2,3},
  {4,5,6},
  {7,8,9},
  {10,0,11}
};




void setup() {
  
  pinMode(buzzPin, OUTPUT);  
  pinMode(lightPin, OUTPUT); 
  pinMode(pumpPin, OUTPUT); 
  digitalWrite(pumpPin,LOW);
  
  
  for (int i = 0; i < ROWS; i++)  {
    pinMode(rowPins[i], OUTPUT);      
    digitalWrite(rowPins[i],HIGH);
  }
  
  for (int i = 0; i < COLS; i++)  {
    pinMode(colPins[i], INPUT);      
    digitalWrite(colPins[i],HIGH);
  }
  
  
   //Serial.begin(9600);
   
   
   
   powerOn();
  
}


int lastKeyPress = -1;


unsigned long sleepTime = 0;
boolean waitingToSleep = false;
int colorRotatePos = 0;
byte red = 255;
byte green = 255;
byte blue = 255;
byte brightness = 255;
boolean colorRotating = true;
boolean buttonFading = true;
byte buttonFadeVal = 0;

boolean buttonFadeUp = true;
boolean sleep = false;
byte shutter = 255;


boolean soundActivated = false;
int soundTriggered = 0;
int soundLevel = 0;
float aveSoundLevel = 0;
float longAveSoundLevel = 100;


void loop() {
    
  
  //keypress stuff...
  
  int keyPress = checkForKeyPress();
  if(keyPress != lastKeyPress) {
    //a key was pressed - do something!
   // Serial.println(keyPress,DEC);
    
    switch (keyPress) {
      
      
      case -1:
        
        break;
      case 0:
        if(!sleep) {
          brightness = brightness + 64;
          if (brightness > 255) brightness = 63;
          //tone(buzzPin,800+(brightness*2));
          //delay(50);
          //noTone(buzzPin);
        }
        break;
      case 1:
        randomColor();
        break;
      case 2:
        randomColor();
        break;
      case 3:
        randomColor();
        break;
      case 4:
        randomColor();
        break;
      case 5:
        randomColor();
        break;
      case 6:
        randomColor();
        break;
      case 7:
        randomColor();
        break;
      case 8:
        randomColor();
        break;
      case 9:
        randomColor();
        break;
      case 10:
        if(sleep) {
          powerOn();
        } else {
          powerOff();
        }
        /*  OLD SOUND STUFF
        if(soundActivated) {
          soundActivated = false;
          tone(buzzPin,1500);
          delay(50);
          tone(buzzPin,1200);
          delay(50);
          tone(buzzPin,900);
          delay(50);
          noTone(buzzPin);
        } else {
          soundActivated = true;
          tone(buzzPin,900);
          delay(50);
          tone(buzzPin,1200);
          delay(50);
          tone(buzzPin,1500);
          delay(50);
          noTone(buzzPin);
        }
        */
        break;
      case 11:
        startDelayOff();
        break;
     
    }
    
    
  
  }
  lastKeyPress = keyPress;
  
  
  
  
  
  //sound monitor stuff...
  /*  OLD SOUND STUFF
  int sensorValue;
  sensorValue = analogRead(micPin);  
  sensorValue = abs(sensorValue-512);
  if(millis() % 2 == 0) soundLevel = soundLevel * 0.95;
  if(sensorValue > soundLevel) soundLevel = sensorValue;
  
  if(millis() % soundFreq == 0) {
    aveSoundLevel = ((aveSoundLevel * ((soundTimeShort * (1000 / soundFreq))-1)) + soundLevel) / (soundTimeShort * (1000 / soundFreq));
    longAveSoundLevel = ((longAveSoundLevel * ((soundTimeLong * (1000 / soundFreq))-1)) + soundLevel) / (soundTimeLong * (1000 / soundFreq));
    
    if(soundActivated) {
      Serial.print(aveSoundLevel,DEC);
      Serial.print(" : ");
      Serial.print(soundTriggered,DEC);
      Serial.print(" : ");
      
      Serial.println(longAveSoundLevel,DEC);
      
      
      if(aveSoundLevel > longAveSoundLevel * soundTriggerLevel){
        soundTriggered++;
        //sound level over trigger
        if(soundTriggered > (soundTriggerLength * (1000/soundFreq))) {
          //sound level over trigger and time
          if(sleep && soundActivated) startDelayOff();
        }
        
      } else {
        //normal sound levels
        soundTriggered = 0;
      }
    }
  }
  */
  
  
  //check to see if it's time to sleep...
  if (millis() > sleepTime && waitingToSleep) powerOff();
    
  
  //colour rotate
  if (colorRotating && millis() % 100 == 0) colorRotate();
  
  
  //fadebuttons
  if (buttonFading && millis() % 10 == 0) buttonFade();
  
  
  //RGB LED stuff...
  setRgbLed();
  
  
  
}

void buttonFade() {
  
  if(buttonFadeUp) {
    if(buttonFadeVal == 255) {
      buttonFadeUp = false;
    } else {
      buttonFadeVal++;
    }
  } else {
    if(buttonFadeVal == 10) {
      buttonFadeUp = true;
    } else {
      buttonFadeVal--;
    }
  }
  
  if(!sleep && !waitingToSleep) {
    analogWrite(lightPin,buttonFadeVal);
  } else {
    analogWrite(lightPin,0);
  }
    
}


void randomColor(){
  colorRotatePos = random(0, 1536);
  colorRotate();
}

void colorRotate() {
  colorRotatePos++;

  if (colorRotatePos == 1536) colorRotatePos = 0;
  
  if(colorRotatePos >= 0 && colorRotatePos < 256) {
    red = colorRotatePos;
    green = 0;
    blue = 255;
    
  } else if(colorRotatePos >= 256 && colorRotatePos < 512) {
    red = 255;
    green = 0;
    blue = 255 - (colorRotatePos - 256);
    
  } else if(colorRotatePos >= 512 && colorRotatePos < 768) {
    red = 255;
    green = colorRotatePos - 512;
    blue = 0;
    
  } else if(colorRotatePos >= 768 && colorRotatePos < 1024) {
    red = 255 - (colorRotatePos - 768);
    green = 255;
    blue = 0;
    
  } else if(colorRotatePos >= 1024 && colorRotatePos < 1280) {
    red = 0;
    green = 255;
    blue = colorRotatePos - 1024;
    
  } else if(colorRotatePos >= 1280 && colorRotatePos < 1536) {
    red = 0;
    green = 255 - (colorRotatePos - 1280);
    blue = 255;
    
  } 
  
  
}


void setRgbLed() {
  
  
  int r = 0;
  int g = 0;
  int b = 0;
  
  r = red;
  g = green;
  b = blue; 
  
  //apply shutter
  r = (r * ((shutter*100)/255))/100;
  g = (g * ((shutter*100)/255))/100;
  b = (b * ((shutter*100)/255))/100;
  
  //apply brightness
  r = (r * ((brightness*100)/255))/100;
  g = (g * ((brightness*100)/255))/100;
  b = (b * ((brightness*100)/255))/100;
  
  analogWrite(rPin,r);
  analogWrite(gPin,g);
  analogWrite(bPin,b);
  
}

void startDelayOff() {
  if (sleep) powerOn();
  sleepTime = millis() + (delayOff * 1000);
  waitingToSleep = true;
  tone(buzzPin,1500);
   delay(30);
   tone(buzzPin,1400);
   delay(30);
   tone(buzzPin,1300);
   delay(30);
   tone(buzzPin,1200);
   delay(30);
   noTone(buzzPin);
  
}

void powerOff() {
  waitingToSleep = false;
  sleep = true;
  shutter = 0;
  digitalWrite(pumpPin,LOW);
  tone(buzzPin,1500);
   delay(10);
   tone(buzzPin,1400);
   delay(10);
   tone(buzzPin,1300);
   delay(10);
   tone(buzzPin,1200);
   delay(10);
   tone(buzzPin,1000);
   delay(10);
   noTone(buzzPin);
}

void powerOn() {
  waitingToSleep = false;
  sleep = false;
  shutter = 255;
  digitalWrite(pumpPin,HIGH);
  tone(buzzPin,1000);
   delay(10);
   tone(buzzPin,1200);
   delay(10);
   tone(buzzPin,1300);
   delay(10);
   tone(buzzPin,1400);
   delay(10);
   tone(buzzPin,1500);
   delay(10);
   noTone(buzzPin);
}

int checkForKeyPress() {
  
  int keyVal = -1;
  
  for (int i = 0; i < ROWS; i++)  {
    digitalWrite(rowPins[i],LOW);
    
    for (int j = 0; j < COLS; j++)  {
      if (!digitalRead(colPins[j])) {
       
     
       keyVal = keys[i][j]; 
      }
    }
    
    digitalWrite(rowPins[i],HIGH);
  }
  
  return keyVal;
}

