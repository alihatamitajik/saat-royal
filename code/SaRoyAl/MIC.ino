#define MIC D3


uint8_t numClap = 0; // keep number of claps for double clapping
long firstClap;      // Time of the first clap

// This function captures double flag
// it is called inside the loop and has two states
// 1. First clap not captured
// 2. First clap captured and waiting for second
//    If not arived discard first clap
void checkTriger() {
  int micVal = digitalRead(MIC);
  if (micVal == LOW) {
    if (!numClap){
      // first clap captured
      // wait for the second clap
      numClap = 1;
      firstClap = millis();
    } else {
      long noww = millis();
      // if new clap comes betwean 350 and 1300 millisecond
      // interperet that as double clap
      if (noww - firstClap > 350 && noww - firstClap < 1300 ) {
        isTriggered = true;
        numClap = 0;
      } else if (noww - firstClap > 1500 ){
        // else discard first clap
        numClap = 0;
      }
    }
  }
}
