#include <pitchToFrequency.h>
#include <pitchToNote.h>
#include <frequencyToNote.h>
#include <MIDIUSB_Defs.h>
#include <MIDIUSB.h>

#define NUM_BUTTONS 8

const uint8_t btn_rec_play = 2;
const uint8_t btn_stop = 3;
const uint8_t btn_mode = 4;
const uint8_t btn_clear = 5;
const uint8_t btn_undo = 6;
const uint8_t btn_track1 = 7;
const uint8_t btn_track2 = 8;
const uint8_t btn_track3 = 9;
const uint8_t btn_track4 = 10;

const uint8_t buttons[NUM_BUTTONS] = {
  btn_clear, btn_undo, btn_stop, btn_rec_play,
  btn_track1, btn_track2, btn_track3, btn_track4
};

const int ledPin = LED_BUILTIN; // the number of the LED pin

int mode = 0; // 0 = Play, 1 = Record
int modeValue = 1;
uint8_t modeBuffer = 0xFF;
uint8_t modeEval = 0xFF;
uint8_t priorModeEval = 0xFF;
const int btn_mode_idx = 8;

unsigned long clearLongPress = 0;
int longPressTime = 1000;
uint8_t btnBuffer[NUM_BUTTONS] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
uint8_t btnEval[NUM_BUTTONS] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
uint8_t priorBtnEval[NUM_BUTTONS] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

// Retrigger and debouce
const int minRetriggerTime = 60;  // Min millis between 2 button press
// NUM_BUTTON + 1 (Mode Button have different handler)
unsigned long lastCmdSendTimeStamp[NUM_BUTTONS+1] = {0,0,0,0,0,0,0,0,0};

void setup() {
  pinMode(ledPin, OUTPUT);
  
  for (int i = 0; i < NUM_BUTTONS; i++){
    pinMode(buttons[i], INPUT_PULLUP);
  }
  pinMode(btn_mode, INPUT_PULLUP);
}

/*
* Check if the current press is a valid and not a bounce of signal
* Ignore button press before minRetriggerTime ms
*/
bool InNotARetrigger(int ButtonNum) {
    unsigned long lastPressTime = lastCmdSendTimeStamp[ButtonNum];
    if(millis() - lastPressTime >= minRetriggerTime) {
        lastCmdSendTimeStamp[ButtonNum] = millis();
        return true;
    }
    return false;
}

void sendCmd(byte value) {
  midiEventPacket_t midi = {0x09, 0x90, value, 64};
  MidiUSB.sendMIDI(midi);
  MidiUSB.flush();
}

// 
void loop() {
   //check state of MODE button
  modeValue = digitalRead(btn_mode);      // read current input
  modeBuffer = modeBuffer << 1;           // shift buffer 1 bit left
  bitWrite(modeBuffer, 0, modeValue);     // add new sample to buffer
  
  priorModeEval = modeEval;               // update last evaluated value of MODE
  modeEval = modeBuffer & 0x07;           // keep last 3 samples for evaluation (0b00000111)
  
  if (modeEval == 0x00 && InNotARetrigger(btn_mode_idx)) {   // Evaluated input has at least 3 sequential 0s 
    if (modeEval != priorModeEval) {       // if evaluated MODE has changed from 1 to 0 for 3 consecutive samples (button pressed)
      // Toggle MODE betwee PLAY and REC
      if (mode == 0) {
        mode = 1;
        sendCmd(50);
      } else if (mode == 1) {
        mode = 0;
        sendCmd(51);
      }
    }
  }   
  
  // light record LED if in Record mode
  if (mode == 1) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else{
    digitalWrite(LED_BUILTIN, LOW);
  }

  // Read remaining buttons
  memcpy(priorBtnEval, btnEval, NUM_BUTTONS*sizeof(uint8_t));     //save copy of last button evaluated values
    
  // Read the rest of the buttons
  for (int i = 0; i < NUM_BUTTONS; i++) {
    btnBuffer[i] = btnBuffer[i] << 1;                      //shift buffer 1 bit left
    bitWrite(btnBuffer[i], 0, digitalRead(buttons[i]));     //add new sample to buffer
    btnEval[i] = btnBuffer[i] & 0x07;                      //keep last 3 samples for evaluation    
  }

  // Check button status and send correct MIDI Command
  // CLEAR, UNDO and STOP buttons do not depend on MODE. Evaulate them separately

  // Logic for CLEAR button
  if (btnEval[0] == 0 && InNotARetrigger(0)) {
    if(btnEval[0] != priorBtnEval[0]) {
      if((millis() - clearLongPress) >= longPressTime) { // handle short press
        sendCmd(54);
      } else { // handle long press
        sendCmd(55);
        clearLongPress = millis();
      }
    }
  }
//  } else {
//    // reset last time clear was pressed
//    clearLongPress = millis();
//  }

  // Logic for UNDO button
  if (btnEval[1] == 0 && InNotARetrigger(1)) {
      if(btnEval[1] != priorBtnEval[1]){                      
        sendCmd(56);
      }
  }

  // Logic for STOP button 
  if (btnEval[2] == 0 && InNotARetrigger(2)) {
    if (btnEval[2] != priorBtnEval[2]) {
      sendCmd(57);
    }
  }

  // Check for button presss and send correct note
  // Start at index 3 in the array since CLEAR, UNDO and STOP are handled outside the loop
  for(int i = 3; i < NUM_BUTTONS; i++){
    if (btnEval[i] == 0 && InNotARetrigger(i)){
      if(btnEval[i] != priorBtnEval[i]) {
        if (mode == 0) { // Play mode
          sendCmd(57 + i - 2); 
        } else if (mode == 1) { // Rec mode
          sendCmd(57 + i + 3);
        }
      }
    }
  }
}
