/*Pomodoro Timer
   by Che-Wei Wang,  CW&T
   for Blinks by Move38

  single click: 25 min timer
  double click: 5 min timer
  
  double click while timer is active: force timer to reset

  longpress: turn blink off

  green glow: ready
  yellow glow: 25 timer active
  cyan glow: 5 timer active

  red: times up

*/

//for button press simulation
#include "shared/blinkbios_shared_button.h"
#include "shared/blinkbios_shared_functions.h"

enum pomodoroStates {WAKE, READY, COUNTING, ALARM, SLEEP};  

float pomodoroLength;
Timer pomodoro;

byte state = READY;

Timer readyAnimation;
Timer alarmAnimation;
Timer countAnimation;
Timer turnOffAnimation;

#define STEP_SIZE 10
#define STEP_TIME_MS 30
byte brightness = MAX_BRIGHTNESS;

bool alarmFlashOn = false;

int step = STEP_SIZE;
int smallStep = 5;

bool goToSleep = false;

void setup() {
  turnOffAnimation.never();
}

void loop() {
  //waking up from sleep///////////
  if (state == WAKE) {
    if(buttonSingleClicked()){
      state = READY;
    }
  }

  //ready///////////
  else if (state == READY)
    if (buttonSingleClicked()) { // 25 mins
      pomodoroLength = 1500000;
      pomodoro.set(pomodoroLength);
      state = COUNTING;
    }
    else if (buttonDoubleClicked()) { //5 mins
      pomodoroLength = 300000;
      pomodoro.set(pomodoroLength);
      state = COUNTING;
    }
    else {
      readyDisplay();
    }


  //counting///////////
  else if (state == COUNTING) {
    if (pomodoro.isExpired()) {
      state = ALARM;
    }
    //show time left
    if (buttonSingleClicked()) {
      countDisplayBright();
    }
    //reset///////////
    else if (buttonDoubleClicked()) {
      state = READY;
    }
    countDisplay();
  }

  //alarm///////////
  else if (state == ALARM) {
    alarmDisplay();
    //reset
    if (buttonSingleClicked()) {
      state = READY;
    }
  }


  //turn off///////////
  if (buttonLongPressed()) {
    turnOffAnimation.set(1800);
    goToSleep = true;
    state = SLEEP;
    brightness = MAX_BRIGHTNESS;
  }

  if (state == SLEEP) {
    brightness -= 4;
    
    setColor( dim( BLUE ,  constrain(brightness,0,MAX_BRIGHTNESS) ) );

    if (turnOffAnimation.isExpired()) {
      setColor( OFF);
      turnOffAnimation.never();
      state = WAKE;
      BLINKBIOS_SLEEP_NOW_VECTOR();
    }
  }




  if (state == COUNTING) {
    // These two lines will postpone sleeping. The first one postpones cold sleep and
    // the second one postpones warm sleep. Note that this will also keep any neighbooring
    // blinks from warm sleeping as well (they will still cold sleep).

    BLINKBIOS_POSTPONE_SLEEP_VECTOR();
    blinkbios_button_block.bitflags |= BUTTON_BITFLAG_PRESSED;
  }

}

void countDisplayBright() {
  brightness = MAX_BRIGHTNESS;
  step = -STEP_SIZE;
}

void countDisplay() {
  float timeLeft = float(pomodoro.getRemaining() / pomodoroLength) * 6.0;

  if (countAnimation.isExpired()) {
    if ( (brightness + step > MAX_BRIGHTNESS ) || (brightness + step < MAX_BRIGHTNESS * -3.0) ) {
      step = -step;
    }
    brightness += step;

    for (int i = 0; i < 6; i++) {
      if (i <= timeLeft) {
        if (pomodoroLength <= 300000) setColorOnFace(dim(CYAN, constrain(brightness, 0, MAX_BRIGHTNESS)), i); //5min
        else setColorOnFace(dim(YELLOW, constrain(brightness, 0, MAX_BRIGHTNESS)), i); //25min
      }
      else {
        setColorOnFace(OFF, i);
      }
    }
    countAnimation.set(40);
  }



}

void alarmDisplay() {
  if (alarmAnimation.isExpired()) {
      alarmFlashOn = !alarmFlashOn;
      alarmAnimation.set( 200 );
  }

  if(alarmFlashOn) {
    setColor(RED);
  }
  else {
    setColor(OFF);
  }
}

void readyDisplay() {
  setColor(dim(GREEN, sin8_C(millis()/10)));
}
