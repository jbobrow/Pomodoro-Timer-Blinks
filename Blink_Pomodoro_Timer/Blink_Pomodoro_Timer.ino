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

float pomodoroLength;
Timer pomodoro;
Timer pomodoroAlarm;
int state = 0;

Timer readyAnimation;
Timer alarmAnimation;
Timer countAnimation;

int countBrightness;

#define STEP_SIZE 10
#define STEP_TIME_MS 30
int brightness = 1;
int step = STEP_SIZE;
int smallStep = 5;

void setup() {
  // No setup
}

void loop() {
  //ready///////////
  if (state == 0) {
    if (buttonSingleClicked()) { // 25 mins
      pomodoroLength = 1500000;
      pomodoro.set(pomodoroLength);
      state = 1;
    }
    else if (buttonDoubleClicked()) { //5 mins
      pomodoroLength = 300000;
      pomodoro.set(pomodoroLength);
      state = 1;
    }
    else {
      readyDisplay();
    }
  }

  //counting///////////
  else if (state == 1) {
    if (pomodoro.isExpired()) {
      state = 2;
    }
    countDisplay();
  }

  //alarm///////////
  else if (state == 2) {
    alarmDisplay();
    if (buttonSingleClicked()) {
      state = 0;
    }
  }

  //reset///////////
  if (buttonLongPressed()) {
    state = 0;
  }

}



void countDisplay() {
  float timeLeft = float(pomodoro.getRemaining() / pomodoroLength) * 6.0;

  if (countAnimation.isExpired()) {
    if ( (brightness + step > MAX_BRIGHTNESS ) || (brightness + step < 0) ) {
      step = -step;
    }
    brightness += step;

    for (int i = 0; i < 6; i++) {
      if (i <= timeLeft) {
        setColorOnFace(dim(YELLOW, brightness), i);
      }
      else {
        setColorOnFace(OFF, i);
      }
    }
    countAnimation.set(80);
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
    if ( (brightness + step > MAX_BRIGHTNESS ) || (brightness + step < 0 ) ) {
      step = -step;
    }
    brightness += step;
    setColor( dim( GREEN ,  brightness  ) );
    readyAnimation.set( 40 );
  }
}
