#include <AnalogReader.h>
#include <LiquidCrystal.h>



const int timerPotPin = 0;
const int brightnessPotPin = 1;
const int minReadableVal = 50;
const int maxReadableVal = 1000;
const int potNoiseThreshold = 10;
AnalogReader timerAnalogReader(0,1023,potNoiseThreshold,timerPotPin,minReadableVal,maxReadableVal);
AnalogReader brightnessAnalogReader(0,255,potNoiseThreshold,brightnessPotPin,minReadableVal,maxReadableVal);


const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


const int ledPin = 6;
const int pushButtonPin = 7;

 
const int timerTurnJump = 15; 
// min and max timer values in seconds (maxTimerVal - mibTimerVal) must be divisible by timerTurnJump
const int minTimerVal = 120; 
const int maxTimerVal = 1800;

const int possibleTimes = ( maxTimerVal - minTimerVal ) / timerTurnJump;


int timer;
long prevTimer = 0;
const int delaySecond = 500; // delay value to wait one second in milisecond

int state = 0; /*POSSIBLE STATES: 
  0 - Setting timer: Turning timer pot updates timer, pressing push button starts timer.
  1 - Timer running: Timer is counting, push button cancels timer. Canceling or finishing timer calls timerOverCallback function.
  2 - Timer over: Show timer finished message. push button returns to state 0.
*/


void setup() {
  Serial.begin(9600);
  // set up pin for turning on LEDs via relay
  pinMode(ledPin, OUTPUT);
  // set up pin for push button 
  pinMode(pushButtonPin, INPUT);
  // turn off LEDs
  analogWrite(ledPin, 0);
  //set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  timerAnalogReader.setOutputRange(0, possibleTimes);

}

void loop() {
  // setting timer
  if(state == 0) {
    
    // update timer display if new avlue is read from pot
    boolean differentValueRead = timerAnalogReader.update();
    if(differentValueRead) {
      updateTimerValue();
      displayTimer("Timer: ", timer);
    } 

    // start the timer and turn on LEDs on button press
    if(pushButtonPressed()) {
      state = 1;
      brightnessAnalogReader.update();
      analogWrite(ledPin, brightnessAnalogReader.getCurrentValue());
      displayTimer("Time left: ", timer);
      delay(delaySecond);
    }
  }

  // Timer running
  else if(state == 1) {

    // update brightness
    if(brightnessAnalogReader.update()){
      analogWrite(ledPin, brightnessAnalogReader.getCurrentValue());  
    }
    
    //wait one second
    long currTimer = millis();
    if(currTimer - prevTimer > 1000){
      timer -= 1;
      displayTimer("Time left: ", timer);
      prevTimer = currTimer;
    }
    // when timer reaches zero, turn off LEDs move to state 2 and display timer finished

    if (timer <= 0) {

      analogWrite(ledPin, 0);
      showLabel("Timer Over!");
      state = 2; 
      delay(delaySecond);

    // when push button (cancel) is pressed, turn off LEDs move to state 2 and display timer canceled
    } else if (pushButtonPressed()) {

      analogWrite(ledPin, 0);
      showLabel("Timer Canceled!");
      state = 2; 
      delay(delaySecond);

    // otherwise display current time
    }
  }

  else if(state == 2) {
    // when timer pot gets moved move to state 0
    if(timerAnalogReader.update()) {
      
      state = 0;
      
    }
    // when push button gets pressed move to state 0
    else if(pushButtonPressed()) { 

      // displaying timer
      timerAnalogReader.update();
      updateTimerValue();
      displayTimer("Timer: ", timer);

      // there has to be a pause between going to state two, so the button pressed doesn't trigger in state 0 
      state = 0;
      delay(delaySecond);
      
    }
  }

  // Error - invalid state
  else {
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Error: ");
    lcd.setCursor(0, 1);
    lcd.print("Unknown state.");
    
  }
}


// map analog value to correct time
void updateTimerValue() {
  timer = minTimerVal + timerTurnJump*timerAnalogReader.getCurrentValue();
}

// displays custom label on first row, and time in format "x min y sec" on second row
void displayTimer(String label, int timeToDisplay) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(label);
  lcd.setCursor(0, 1);
  int minsRemaining = timeToDisplay/60;
  if(minsRemaining){
    lcd.print(minsRemaining);
    lcd.print(" min ");
  }
  lcd.print(timeToDisplay - (minsRemaining)*60);
  lcd.print(" sec ");
}


void showLabel(String label) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(label);
}


boolean pushButtonPressed() {
  if(digitalRead(pushButtonPin) == HIGH ) return true;
  return false; 
}


