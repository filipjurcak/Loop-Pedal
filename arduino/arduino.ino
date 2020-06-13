#include <pitchToFrequency.h>
#include <pitchToNote.h>
#include <frequencyToNote.h>
#include <MIDIUSB_Defs.h>
#include <MIDIUSB.h>

#define NUM_BUTTONS 9

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
  btn_rec_play, btn_stop, btn_mode, btn_clear,
  btn_undo, btn_track1, btn_track2, btn_track3, btn_track4
};

const int minRetriggerTime = 60;  // Min millis between 2 button press
int longPressTime = 1000;
uint8_t btnBuffer[NUM_BUTTONS] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t btnEval[NUM_BUTTONS] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t priorBtnEval[NUM_BUTTONS] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned long lastCmdSendTimeStamp[NUM_BUTTONS+1] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

void setup() {
  for (int i = 0; i < NUM_BUTTONS; i++){
    pinMode(buttons[i], INPUT_PULLUP);
  }
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

void loop() {
  // save copy of last button evaluated values
  memcpy(priorBtnEval, btnEval, NUM_BUTTONS * sizeof(uint8_t));
    
  // Read the rest of the buttons
  for (int i = 0; i < NUM_BUTTONS; i++) {
    btnBuffer[i] = btnBuffer[i] << 1;  // shift buffer 1 bit left
    bitWrite(btnBuffer[i], 0, digitalRead(buttons[i]));  // add new sample to buffer
    btnEval[i] = btnBuffer[i] & 0x07;  // keep last 3 samples for evaluation    
    
    // Check for button presss and send correct note
    if (btnEval[i] == 0 && InNotARetrigger(i)){
      if(btnEval[i] != priorBtnEval[i]) {
        sendCmd(69 + i);
      }
    }
  }
}
