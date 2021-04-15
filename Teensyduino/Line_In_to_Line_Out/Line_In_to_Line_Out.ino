// This is a modified version of the below noted program
// I think this program should take in the audio via line in and output it 1s later from the pre-installed audio jack



// Advanced Microcontroller-based Audio Workshop
//
// http://www.pjrc.com/store/audio_tutorial_kit.html
// https://hackaday.io/project/8292-microcontroller-audio-workshop-had-supercon-2015
// 
// Part 2-4: Using The Microphone


///////////////////////////////////
// copy the Design Tool code here
///////////////////////////////////

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=370.88890075683594,327.8888759613037
AudioOutputI2S           i2s2;           //xy=608.8888702392578,328.8888826370239
AudioConnection          patchCord1(i2s1, 0, i2s2, 1);                                  //bc John was a dummy and told Garrett to wire the channels wrong :), these two lines should switch them.
AudioConnection          patchCord2(i2s1, 1, i2s2, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=492.88890838623047,408.00000381469727
// GUItool: end automatically generated code



void setup() {
  Serial.begin(9600);
  AudioMemory(8);                               //Allocates Teensy RAM for use by the audio library.
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);                       //Range is 0 to 1 but 0.8 is maximum undistorted output for a full scale signal, we may have to play around with this a bit
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);   //It was very important to change this line so that the audio is received from the LineIn versus the microphone as it was by default
  // sgtl5000_1.micGain(36);                    //This line shouldn't do anything because we are using LineIn
  delay(1000);
}

elapsedMillis msecs;

void loop() {
    delay(10); // wait for library to parse WAV info (maybe?)
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
