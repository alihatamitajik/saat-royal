// States
#define S_CLOCK_INIT 0
#define S_CLOCK      1
#define S_DATE_INIT  2
#define S_DATE       3
#define S_BATRY_INIT 4
#define S_BATRY      5
#define S_RAINB_INIT 6
#define S_RAINB      7
#define S_PACKM_INIT 8
#define S_PACKM      9
#define S_TEMP_INIT  10
#define S_TEMP       11
#define S_IDLE       12
  
void goNextState() {
  switch(state) {
    case S_CLOCK_INIT : if(Prefrences.isShowClock)    {state = S_CLOCK; break;}
    case S_CLOCK      : if(Prefrences.isShowDate)     {state = S_DATE_INIT; break;}
    case S_DATE_INIT  : if(Prefrences.isShowDate)     {state = S_DATE; break;}
    case S_DATE       : if(Prefrences.isShowBattery)  {state = S_BATRY_INIT; break;}
    case S_BATRY_INIT : if(Prefrences.isShowBattery)  {state = S_BATRY; break;}
    case S_BATRY      : if(Prefrences.isTemprature)   {state = S_TEMP_INIT; break;}
    case S_TEMP_INIT  : if(Prefrences.isTemprature)   {state = S_TEMP; break;}
    case S_TEMP       : if(Prefrences.isShowRainbow)  {state = S_RAINB_INIT; break;}
    case S_RAINB_INIT : if(Prefrences.isShowRainbow)  {state = S_RAINB; break;}
    case S_RAINB      : if(Prefrences.isShowPackman)  {state = S_PACKM_INIT; break;}
    case S_PACKM_INIT : if(Prefrences.isShowPackman)  {state = S_PACKM; break;}
    case S_PACKM      : state = S_IDLE; break;
    default:
      state = S_CLOCK_INIT; break;
  }
}


void operate() {
  if (shouldLog) {
    log_temprature = getTemprature();
    log_battery = getBatteryPercentage();
    shouldLog = false;
  }
  // if we are not trigered and clock is not showing
  // capture clap
  if(!isTriggered && !isOperating){
    checkTriger();
  } else if (isTriggered){
    // else if we are trigered call the operation once
    // operation should     
    isTriggered = false;
    isOperating = true;
    // Show data once, next calls should be done by timer
    lastCheckpoint = millis();
    threshold = 200;
    state = 0;
  } else if (isOperating) {
    if (millis() - lastCheckpoint >  threshold){
      lastCheckpoint = millis();
      handleStates();
    }
  }

  if (millis() - alarmCheck > 1000) {
    alarmCheck = millis();
    handleAlarm();
  }
}

void handleStates() {
  /*
   * case i, i+1 is for one state
   * state i is for initialization and state i+1 is for main loop
   * each state at the end of its operation make isDone 1 to show
   * the controller to go for the next state;
   */
  switch(state) {
    case S_TEMP_INIT  : updateTemprature();
    case S_RAINB_INIT :
    case S_BATRY_INIT : updateBatteryPercentage();
    case S_DATE_INIT  :
    case S_PACKM_INIT : 
    case S_CLOCK_INIT : turnOff(); i_loc = 0; j=0; k=1; goNextState(); startState = millis(); break;
    case S_CLOCK      : updateTime(); displayTime(); break;
    case S_DATE       : displayDate(); break;
    case S_BATRY      : displayPercentage(); break;
    case S_TEMP       : displayTemprature(); break;
    case S_RAINB      : displayRainbow(); break;
    case S_PACKM      : displayPackman(); break;
    case S_IDLE       : state = S_CLOCK_INIT; turnOff(); isOperating = false;
    default:
      state = S_CLOCK_INIT; break;
  }
}
