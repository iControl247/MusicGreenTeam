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
AudioSynthWaveform       waveform1;      //xy=735,431
AudioOutputI2S           i2s2;           //xy=786,273
AudioMixer4              mixer1;         //xy=911,406
AudioAnalyzeFFT1024      fft1024_1;      //xy=1070,404
AudioConnection          patchCord1(i2s1, 0, i2s2, 1);
AudioConnection          patchCord2(i2s1, 0, mixer1, 1);
AudioConnection          patchCord3(i2s1, 1, amp1, 0);
AudioConnection          patchCord4(amp1, 0, i2s2, 0);
AudioConnection          patchCord5(amp1, 0, mixer1, 0);
AudioConnection          patchCord6(waveform1, 0, mixer1, 2);
AudioConnection          patchCord7(mixer1, fft1024_1);
AudioControlSGTL5000     sgtl5000_1;     //xy=542,554
// GUItool: end automatically generated code




void setup() {
  Serial.begin(9600);
  AudioMemory(10);                               //Allocates Teensy RAM for use by the audio library, its 10 as seen in the peak example
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);                       //Range is 0 to 1 but 0.8 is maximum undistorted output for a full scale signal, we may have to play around with this a bit
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);   //It was very important to change this line so that the audio is received from the LineIn versus the microphone as it was by default
  // sgtl5000_1.micGain(36);                    //This line shouldn't do anything because we are using LineIn
  amp1.gain(8.0);                              //Calibration for left channel not being loud enough (this also makes hella noice)
  //delay(1000);
}

elapsedMillis msecs;

void loop() {
    // print Fourier Transform data to the Arduino Serial Monitor
  if (fft1024_1.available()) {
    
    Serial.print("FFT: ");
    for (int i=0; i<30; i++) {  // 0-25  -->  DC to 1.25 kHz
      float n = fft1024_1.read(i);
      printNumber(n);
    }
    Serial.println();
  }

  if (!fft1024_1.available()){
    Serial.println("Help");
  }
  /*
  if (fft1024_1.available()) {
    // each time new FFT data is available
    // print to the Arduino Serial Monitor
    Serial.print("FFT: ");
    printNumber(fft1024_1.read(0));
    printNumber(fft1024_1.read(1));
    printNumber(fft1024_1.read(2,3));
    printNumber(fft1024_1.read(4,6));
    printNumber(fft1024_1.read(7,10));
    printNumber(fft1024_1.read(11,15));
    printNumber(fft1024_1.read(16,22));
    printNumber(fft1024_1.read(23,32));
    printNumber(fft1024_1.read(33,46));
    printNumber(fft1024_1.read(47,66));
    printNumber(fft1024_1.read(67,93));
    printNumber(fft1024_1.read(94,131));
    printNumber(fft1024_1.read(132,184));
    printNumber(fft1024_1.read(185,257));
    printNumber(fft1024_1.read(258,359));
    printNumber(fft1024_1.read(360,511));
    Serial.println();
  }
*/

}


void printNumber(float n) {
  
  if (n >= 0.004) {
    Serial.print(n, 3);
    Serial.print(" ");
  } else {
    Serial.print("   -  "); // don't print "0.00"
  }
  
  /*
  if (n > 0.25) {
    Serial.print("***** ");
  } else if (n > 0.18) {
    Serial.print(" ***  ");
  } else if (n > 0.06) {
    Serial.print("  *   ");
  } else if (n > 0.005) {
    Serial.print("  .   ");
  }
  */
}
