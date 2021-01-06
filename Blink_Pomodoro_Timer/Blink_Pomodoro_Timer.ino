/*Pomodoro Timer
   by Che-Wei Wang,  CW&T
   for Blinks by Move38

  single click: 25 min timer
  double click: 5 min timer
  longpress: force timer to reset anytime

  green glow: ready
  yellow glow: timer active
  red: times up

*/
#include "shared/blinkbios_shared_button.h"
#include "shared/blinkbios_shared_functions.h"

#define BLINKBIOS_SLEEP_NOW_VECTOR boot_vector12
// Calling BLINKBIOS_SLEEP_NOW_VECTOR() will immedeately put the blink into hardware sleep
// It can only wake up from a button press.
extern "C" void BLINKBIOS_SLEEP_NOW_VECTOR()  __attribute__((used)) __attribute__((noinline));

float pomodoroLength;
Timer pomodoro;

int state = 0;

Timer readyAnimation;
Timer alarmAnimation;
Timer countAnimation;
Timer turnOffAnimation;

#define STEP_SIZE 10
#define STEP_TIME_MS 30
int brightness = MAX_BRIGHTNESS;
;
int step = STEP_SIZE;
int smallStep = 5;




void setup() {
  turnOffAnimation.never();
}

void loop() {
  //ready///////////
  if (state == 0)
    if (buttonSingleClicked()) { // 25 mins
      pomodoroLength = 1500000;
      pomodoro.set(pomodoroLength);
      state = 1;
      brightness = MAX_BRIGHTNESS;
    }
    else if (buttonDoubleClicked()) { //5 mins
      pomodoroLength = 300000;
      pomodoro.set(pomodoroLength);
      state = 1;
      brightness = MAX_BRIGHTNESS;

    }
    else {
      readyDisplay();
    }


  //counting///////////
  else if (state == 1) {
    if (pomodoro.isExpired()) {
      state = 2;
      brightness = MAX_BRIGHTNESS;

    }
    if (buttonSingleClicked()) {
      countDisplayBright();
    }

    //reset///////////
    else if (buttonDoubleClicked()) {
      state = 0;
      brightness = MAX_BRIGHTNESS;
    }
    countDisplay();


  }

  //alarm///////////
  else if (state == 2) {
    alarmDisplay();

    if (buttonSingleClicked()) {
      state = 0;
      brightness = MAX_BRIGHTNESS;

    }
  }





  //turn off
  //  if (buttonLongPressed()) {
  //    turnOffAnimation.set( 500 );
  //    setColor( dim( RED ,  MAX_BRIGHTNESS  ) );
  //  }
  //
  //  if (turnOffAnimation.isExpired()) {
  //    setColor(OFF);
  //    BLINKBIOS_SLEEP_NOW_VECTOR();
  //  }
  //

  if (state == 1) {
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
    if ( (brightness + step > MAX_BRIGHTNESS ) || (brightness + step < 0 ) ) {
      step = -step;
    }
    brightness += step;
    setColor( dim( RED ,  brightness  ) );
    alarmAnimation.set( 1 );
  }
}

void readyDisplay() {
  if (readyAnimation.isExpired()) {
    if ( (brightness + step > MAX_BRIGHTNESS ) || (brightness + step < MAX_BRIGHTNESS * -1.0) ) {
      step = -step;
    }
    brightness += step;
    setColor( dim( GREEN ,  constrain(brightness, 0, MAX_BRIGHTNESS)  ) );
    readyAnimation.set( 40 );
  }
}
