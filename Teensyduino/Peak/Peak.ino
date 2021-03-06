// This is a modified version of the below noted programs
// I think this program could take in the line in input and output via the headphone jack AND ALSO give peak values for each channel with a graphical thingy via serial monitor
// I'm either popping off or just doing useless work idk



// Advanced Microcontroller-based Audio Workshop
//
// http://www.pjrc.com/store/audio_tutorial_kit.html
// https://hackaday.io/project/8292-microcontroller-audio-workshop-had-supercon-2015
// 
// Part 2-4: Using The Microphone

// Advanced Microcontroller-based Audio Workshop
//
// http://www.pjrc.com/store/audio_tutorial_kit.html
// https://hackaday.io/project/8292-microcontroller-audio-workshop-had-supercon-2015
// 
// Part 3-1: Peak Detection

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=242,289
AudioAmplifier           amp1;           //xy=537,319
AudioAnalyzePeak         peak1;          //xy=638,362
AudioAnalyzePeak         peak2;          //xy=641,416
AudioOutputI2S           i2s2;           //xy=743,292
AudioConnection          patchCord1(i2s1, 0, peak2, 0);
AudioConnection          patchCord2(i2s1, 0, i2s2, 1);
AudioConnection          patchCord3(i2s1, 1, amp1, 0);
AudioConnection          patchCord4(amp1, 0, i2s2, 0);
AudioConnection          patchCord5(amp1, peak1);
AudioControlSGTL5000     sgtl5000_1;     //xy=542,554
// GUItool: end automatically generated code


void setup() {
  Serial.begin(9600);
  AudioMemory(10);                               //Allocates Teensy RAM for use by the audio library, its 10 as seen in the peak example
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);                       //Range is 0 to 1 but 0.8 is maximum undistorted output for a full scale signal, we may have to play around with this a bit
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);   //It was very important to change this line so that the audio is received from the LineIn versus the microphone as it was by default
  // sgtl5000_1.micGain(36);                    //This line shouldn't do anything because we are using LineIn
  amp1.gain(6.0);                              //Calibration for left channel not being loud enough (this also makes hella noice)
  //delay(1000);
}

// Delays:
// 6 for Andrew's phone
// 8 for Nate's phone
// 39 for USB-C dongle


elapsedMillis msecs;

void loop() {
  //delay(10); // wait for library to parse WAV info (maybe?)
  if (msecs > 40) {                                               //This code is from the second cited program comments, it should find peaks and do a graphical waveform.
    if (peak1.available() && peak2.available()) {
      msecs = 0;
      float leftNumber = peak1.read();
      float rightNumber = peak2.read();
      int leftPeak = leftNumber * 30.0;
      int rightPeak = rightNumber * 30.0;
      int count;
      for (count=0; count < 30-leftPeak; count++) {
        Serial.print(" ");
      }
      while (count++ < 30) {
        Serial.print("<");
      }
      Serial.print("||");
      for (count=0; count < rightPeak; count++) {
        Serial.print(">");
      }
      while (count++ < 30) {
        Serial.print(" ");
      }
      Serial.print(leftNumber);
      Serial.print(", ");
      Serial.print(rightNumber);
      Serial.println();
    }
  }
}
