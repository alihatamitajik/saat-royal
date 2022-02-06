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
    case S_RAINB_INIT : if(Prefrences.isTemprature)   {state = S_RAINB; break;}
    case S_RAINB      : if(Prefrences.isShowRainbow)  {state = S_PACKM_INIT; break;}
    case S_PACKM_INIT : if(Prefrences.isShowRainbow)  {state = S_PACKM; break;}
    case S_PACKM      : if(Prefrences.isShowPackman)  {state = S_IDLE; break;}
    case S_IDLE       : if(Prefrences.isShowPackman)  {state = S_CLOCK_INIT; break;}
    default:
      state = S_CLOCK_INIT; break;
  }
}


void operate() {
  // if we are not trigered and clock is not showing
  // capture clap
  if(!isTriggered && !isOperating){
    checkTriger();
  } else if (isTriggered){
    // else if we are trigered call the operation once
    // operation should 
    Serial.println("Trigerred");    
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
}

void handleStates() {
  /*
   * case i, i+1 is for one state
   * state i is for initialization and state i+1 is for main loop
   * each state at the end of its operation make isDone 1 to show
   * the controller to go for the next state;
   */
  switch(state) {
    case S_RAINB_INIT :
    case S_BATRY_INIT :
    case S_DATE_INIT  :
    case S_PACKM_INIT : 
    case S_TEMP_INIT  :
    case S_CLOCK_INIT : i_loc = 0; goNextState(); startState = millis(); break;
    case S_CLOCK      : displayTime(); break;
    case S_DATE       : displayDate(); break;
    case S_BATRY      : if(Prefrences.isTemprature)   {state = S_TEMP_INIT; break;}
    case S_TEMP       : if(Prefrences.isShowRainbow)  {state = S_RAINB_INIT; break;}
    case S_RAINB      : if(Prefrences.isShowRainbow)  {state = S_PACKM_INIT; break;}
    case S_PACKM      : if(Prefrences.isShowPackman)  {state = S_IDLE; break;}
    case S_IDLE       : if(Prefrences.isShowPackman)  {state = S_CLOCK_INIT; isOperating = false; turnOff(); break;}
    default:
      state = S_CLOCK_INIT; break;
  }
}
