// FFT Test

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 13
#define LED_COUNT 60

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

const int myInput = AUDIO_INPUT_LINEIN;
//const int myInput = AUDIO_INPUT_MIC;

// Create the Audio components.  These should be created in the
// order data flows, inputs/sources -> processing -> outputs
//
AudioInputI2S          audioInput;         // audio shield: mic or line-in
AudioSynthWaveformSine sinewave;
AudioAnalyzeFFT1024    myFFT;
AudioAnalyzeFFT1024    myFFT_R;
AudioOutputI2S         audioOutput;        // audio shield: headphones & line-out

// Connect either the live input or synthesized sine wave
AudioConnection patchCord1(audioInput, 0, myFFT, 0);
AudioConnection patchCord2(audioInput, 0, audioOutput, 0); //output
//AudioConnection patchCord3(audioInput, 1, myFFT_R, 1);
//AudioConnection patchCord4(audioInput, 1, audioOutput, 1); //output
//AudioConnection patchCord1(sinewave, 0, myFFT, 0);

AudioControlSGTL5000 audioShield;

struct Cuck{
  float subbass;
  float bass;
  float low_mid;
  float midrange;
  float high_mid;
  float presence;
  float brilliance;
  int middle_man;
};

typedef struct Cuck beta;
beta cuck;
void setup() {
  AudioMemory(12);

  // Enable the audio shield and set the output volume.
  audioShield.enable();
  audioShield.inputSelect(myInput);
  audioShield.volume(0.5);

  strip.begin();
  strip.show();
  strip.setBrightness(50);

  // Configure the window algorithm to use
  myFFT.windowFunction(AudioWindowHanning1024);
  //myFFT.windowFunction(NULL);

  // Create a synthetic sine wave, for testing
  // To use this, edit the connections above
  //sinewave.amplitude(0.8);
  //sinewave.frequency(1034.007);
}

void loop() {
  float n;
  int i;
  /////////////////////////////////////////////////////////FFT for the Left side
  if (myFFT.available()) {
    // each time new FFT data is available
    // print it all to the Arduino Serial Monitor
    Serial.print("FFT: ");
    for (i=0; i<501; i++) {
      n = myFFT.read(i);
      find_greatest_freq(n, i);
      if (n >= 0.01) {
        Serial.print(n);
        Serial.print(" ");
      } else {
        Serial.print("  -  "); // don't print "0.00"
      }
    }
    //int cum = value_return(x, true); this is actually where the lighting function call will go that is dependent on greatest frequency
    //twinkle(60,100);
    //bass_bright(100);
    eq_bar(50);
    Serial.println();
  }
  //////////////////////////////////////////////////////////FFT for the Right side
//    if (myFFT_R.available()) {
//    // each time new FFT data is available
//    // print it all to the Arduino Serial Monitor
//    Serial.print("FFT: ");
//    for (i=0; i<501; i++) {
//      n = myFFT_R.read(i);
//      find_greatest_freq(n, i);
//      if (n >= 0.01) {
//        Serial.print(n);
//        Serial.print(" ");
//      } else {
//        Serial.print("  -  "); // don't print "0.00"
//      }
//    }
//    //int cum = value_return(x, true); this is actually where the lighting function call will go that is dependent on greatest frequency
//    Serial.println();
//  }
}


void find_greatest_freq(float n, int i){
  //beta cuck;
  //light reaction: random LED @ x freq
  if(i == 0){
    cuck.subbass = 0;
    cuck.bass = 0;
    cuck.low_mid = 0;
    cuck.midrange = 0;
    cuck.high_mid = 0;
    cuck.presence = 0;
    cuck.brilliance = 0;
    cuck.middle_man = 0;
  }
  //if(n >= 0.01){
    if(i == 1 or i == 2){ //sub bass @ 20 to 60hz note: rounding up to 80hz
      cuck.subbass = cuck.subbass + n;
    }else if(i >= 3 and i < 7){ //bass @ 60 to 250hz note: starting @ 120hz going up to < 240hz
      cuck.bass = cuck.bass + n;
    }else if(i >= 7 and i < 13){ //low midrange @ 250hz to 500hz: starting @ 240hz up to < 520hz
      cuck.low_mid = cuck.low_mid + n;
    }else if(i >= 13 and i < 50){ //midrange @ 500hz to 2000hz: starting @ 520 up to < 2000hz
      cuck.midrange = cuck.midrange + n;
    }else if(i >= 50 and i < 100){ //high midrange @ 2000hz to 4000hz: starting @ 2000hz up to < 4000hz
      cuck.high_mid = cuck.high_mid + n;
    }else if(i >= 100 and i < 150){ //presence @ 4000hz to 6000hz: starting @ 4000hz up to < 6000hz
      cuck.presence = cuck.presence + n;
    }else if(i >= 150){ //brilliance @ 6000hz to 20000hz: starting @ 6000hz up to 20000hz
      cuck.brilliance = cuck.brilliance + n;
    //}
  }
  if(i == 500){
    if(cuck.subbass > cuck.bass &&  cuck.subbass > cuck.low_mid && cuck.subbass > cuck.midrange && cuck.subbass > cuck.high_mid && cuck.subbass > cuck.presence && cuck.subbass > cuck.brilliance){ //subbass is greatest
      if(cuck.subbass > 0.01){
        cuck.middle_man = 1;
      }
    }else if(cuck.bass > cuck.low_mid && cuck.bass > cuck.midrange && cuck.bass > cuck.high_mid && cuck.bass > cuck.presence && cuck.bass > cuck.brilliance){ //bass is greatest
      if(cuck.bass > 0.01){
        cuck.middle_man = 2;
      }
    }else if(cuck.low_mid > cuck.midrange && cuck.low_mid > cuck.high_mid && cuck.low_mid > cuck.presence && cuck.low_mid > cuck.brilliance){ //low_mid is greatest
      if(cuck.low_mid > 0.01){
        cuck.middle_man = 3;
      }
    }else if(cuck.midrange > cuck.high_mid && cuck.midrange > cuck.presence && cuck.midrange > cuck.brilliance){ //midrange is greatest
      if(cuck.midrange > 0.01){
        cuck.middle_man = 4;
      }
    }else if(cuck.high_mid > cuck.presence && cuck.high_mid > cuck.brilliance){ //high_mid is greatest
      if(cuck.high_mid > 0.01){
        cuck.middle_man = 5;
      }
    }else if(cuck.presence > cuck.brilliance){ //presence is greatest
      if(cuck.presence > 0.01){
        cuck.middle_man = 6;
      }
    }else{                           //brilliance is greatest
      if(cuck.brilliance >= 0.02){
        cuck.middle_man = 7;
      }else{
        cuck.middle_man = 0;
      }
    }
    if((cuck.subbass + cuck.bass + cuck.low_mid + cuck.midrange + cuck.high_mid + cuck.presence + cuck.brilliance) <= 0.001 ){
      cuck.middle_man = 0;
    }
  }
}
void bass_bright(int Delay){
  if(cuck.subbass <= .05){//no bass
    for(int i = 0; i < 60; ++i){
      strip.setPixelColor(i, 0, 0, 0); 
    }
  }else if(cuck.subbass > .05 and cuck.subbass < .21){//50 bright
    for(int i = 0; i < 60; ++i){
      strip.setPixelColor(i, strip.ColorHSV(0, 0, 50));
    }
  }else if(cuck.subbass >= .21 and cuck.subbass < .41){//100 bright
    for(int i = 0; i < 60; ++i){
      strip.setPixelColor(i, strip.ColorHSV(0, 0, 100));
    }
  }else if(cuck.subbass >= .41 and cuck.subbass < .61){//150 bright
    for(int i = 0; i < 60; ++i){
      strip.setPixelColor(i, strip.ColorHSV(0, 0, 150));
    }
  }else if(cuck.subbass >= .61 and cuck.subbass < .81){//200 bright
    for(int i = 0; i < 60; ++i){
      strip.setPixelColor(i, strip.ColorHSV(0, 0, 200));
    }
  }else if(cuck.subbass >= .81){//250 bright...
    for(int i = 0; i < 60; ++i){
      strip.setPixelColor(i, strip.ColorHSV(0, 0, 250));
    }
  }
  strip.show();
  delay(Delay);
  strip.clear();
}
void eq_bar(int Delay){
  if(cuck.middle_man == 0){ //no input so all lights are off
    //set 60 pixels 0,0,0
    for(int i = 0; i < 60; ++i){
      strip.setPixelColor(i, 0, 0, 0);
    }
  }
  else if(cuck.middle_man == 1){  //greatest freq is SUBBASS section, so color will be red
    if(cuck.subbass >= .01 and cuck.subbass < .11){//amplitude total .01 to .10
      //set 6 pixels to 255,0,0
      for(int i = 0; i < 6; ++i){
        strip.setPixelColor(i, 255, 0, 0);
        //strip.show();
      }

    }else if(cuck.subbass >= .11 and cuck.subbass < .21){//amplitude total .11 to .20
      //set 12 pixels to 255,0,0
      for(int i = 0; i < 12; ++i){
        strip.setPixelColor(i, 255, 0, 0);
        //strip.show();
      }

    }else if(cuck.subbass >= .21 and cuck.subbass < .31){//amplitude total .21 to .30
      //set 18 pixels to 255,0,0
      for(int i = 0; i < 18; ++i){
        strip.setPixelColor(i, 255, 0, 0);
        //strip.show();
      }

    }else if(cuck.subbass >= .31 and cuck.subbass < .36){//amplitude total .31 to .35
      //set 24 pixels to 255,0,0
      for(int i = 0; i < 24; ++i){
        strip.setPixelColor(i, 255, 0, 0);
        //strip.show();
      }

    }else if(cuck.subbass >= .36 and cuck.subbass < .41){//amplitude total .36 to .40
      //set 30 pixels to 255,0,0
      for(int i = 0; i < 30; ++i){
        strip.setPixelColor(i, 255, 0, 0);
        //strip.show();
      }

    }else if(cuck.subbass >= .41 and cuck.subbass < .46){//amplitude total .41 to .46
      //set 36 pixels to 255,0,0
      for(int i = 0; i < 36; ++i){
        strip.setPixelColor(i, 255, 0, 0);
       // strip.show();
      }

    }else if(cuck.subbass >= .46 and cuck.subbass < .51){//amplitude total .46 to .51
      //set 42 pixels to 255,0,0
      for(int i = 0; i < 42; ++i){
        strip.setPixelColor(i, 255, 0, 0);
        //strip.show();
      }

    }else if(cuck.subbass >= .51 and cuck.subbass < .56){//amplitude total .51 to .56
      //set 48 pixels to 255,0,0
      for(int i = 0; i < 48; ++i){
        strip.setPixelColor(i, 255, 0, 0);
        //strip.show();
      }

    }else if(cuck.subbass >= .56 and cuck.subbass < .61){//amplitude total .56 to .61
      //set 54 pixels to 255,0,0
      for(int i = 0; i < 54; ++i){
        strip.setPixelColor(i, 255, 0, 0);
        //strip.show();
      }

    }else if(cuck.subbass >= .61){//amplitude total .66 & up
      //set 60 pixels to 255,0,0
      for(int i = 0; i < 60; ++i){
        strip.setPixelColor(i, 255, 0, 0);
        //strip.show();
      }
    }
  }
  else if(cuck.middle_man == 2){  //greatest freq is BASS section, so color will be orange
    if(cuck.bass >= .01 and cuck.bass < .11){//amplitude total
      //set 6 pixels to 255, 165, 0
      for(int i = 0; i < 6; ++i){
        strip.setPixelColor(i, 255, 165, 0);
        //strip.show();
      }

    }else if(cuck.bass >= .11 and cuck.bass < .21){//amplitude total
      //set 12 pixels to 255, 165, 0
      for(int i = 0; i < 12; ++i){
        strip.setPixelColor(i, 255, 165, 0);
        //strip.show();
      }

    }else if(cuck.bass >= .21 and cuck.bass < .31){//amplitude total
      //set 18 pixels to 255, 165, 0
      for(int i = 0; i < 18; ++i){
        strip.setPixelColor(i, 255, 165, 0);
        //strip.show();
      }

    }else if(cuck.bass >= .31 and cuck.bass < .36){//amplitude total
      //set 24 pixels to 255, 165, 0
      for(int i = 0; i < 24; ++i){
        strip.setPixelColor(i, 255, 165, 0);
        //strip.show();
      }

    }else if(cuck.bass >= .36 and cuck.bass < .41){//amplitude total
      //set 30 pixels to 255, 165, 0
      for(int i = 0; i < 30; ++i){
        strip.setPixelColor(i, 255, 165, 0);
        //strip.show();
      }

    }else if(cuck.bass >= .41 and cuck.bass < .46){//amplitude total
      //set 36 pixels to 255, 165, 0
      for(int i = 0; i < 36; ++i){
        strip.setPixelColor(i, 255, 165, 0);
        //strip.show();
      }

    }else if(cuck.bass >= .46 and cuck.bass < .51){//amplitude total
      //set 42 pixels to 255, 165, 0
      for(int i = 0; i < 42; ++i){
        strip.setPixelColor(i, 255, 165, 0);
        //strip.show();
      }

    }else if(cuck.bass >= .51 and cuck.bass < .56){//amplitude total
      //set 48 pixels to 255, 165, 0
      for(int i = 0; i < 48; ++i){
        strip.setPixelColor(i, 255, 165, 0);
        //strip.show();
      }

    }else if(cuck.bass >= .56 and cuck.bass < .61){//amplitude total
      //set 54 pixels to 255, 165, 0
      for(int i = 0; i < 54; ++i){
        strip.setPixelColor(i, 255, 165, 0);
        //strip.show();
      }
    }else if(cuck.bass >= .61){//amplitude total
      //set 60 pixels to 255, 165, 0
      for(int i = 0; i < 60; ++i){
        strip.setPixelColor(i, 255, 165, 0);
        //strip.show();
      }
    }
  }
  else if(cuck.middle_man == 3){  //greatest freq is LOW_MID section, so color will be yellow
    if(cuck.low_mid >= .01 and cuck.low_mid < .11){//amplitude total
      //set 6 pixels to 255, 255, 0
      for(int i = 0; i < 6; ++i){
        strip.setPixelColor(i, 255, 255, 0);
        //strip.show();
      }

    }else if(cuck.low_mid >= .11 and cuck.low_mid < .21){//amplitude total
      //set 12 pixels to 255, 255, 0
      for(int i = 0; i < 12; ++i){
        strip.setPixelColor(i, 255, 255, 0);
        //strip.show();
      }

    }else if(cuck.low_mid >= .21 and cuck.low_mid < .31){//amplitude total
      //set 18 pixels to 255, 255, 0
      for(int i = 0; i < 18; ++i){
        strip.setPixelColor(i, 255, 255, 0);
        //strip.show();
      }

    }else if(cuck.low_mid >= .31 and cuck.low_mid < .36){//amplitude total .16 to .20
      //set 24 pixels to 255, 255, 0
      for(int i = 0; i < 24; ++i){
        strip.setPixelColor(i, 255, 255, 0);
        //strip.show();
      }

    }else if(cuck.low_mid >= .36 and cuck.low_mid < .41){//amplitude total .21 to .25
      //set 30 pixels to 255, 255, 0
      for(int i = 0; i < 30; ++i){
        strip.setPixelColor(i, 255, 255, 0);
        //strip.show();
      }

    }else if(cuck.low_mid >= .41 and cuck.low_mid < .46){//amplitude total .26 to .30
      //set 36 pixels to 255, 255, 0
      for(int i = 0; i < 36; ++i){
        strip.setPixelColor(i, 255, 255, 0);
        //strip.show();
      }

    }else if(cuck.low_mid >= .46 and cuck.low_mid < .51){//amplitude total .31 to .35
      //set 42 pixels to 255, 255, 0
      for(int i = 0; i < 42; ++i){
        strip.setPixelColor(i, 255, 255, 0);
        //strip.show();
      }

    }else if(cuck.low_mid >= .51 and cuck.low_mid < .56){//amplitude total .36 to .40
      //set 48 pixels to 255, 255, 0
      for(int i = 0; i < 48; ++i){
        strip.setPixelColor(i, 255, 255, 0);
        //strip.show();
      }
    }else if(cuck.low_mid >= .56 and cuck.low_mid < .61){//amplitude total .41 to .45
      //set 54 pixels to 255, 255, 0
      for(int i = 0; i < 54; ++i){
        strip.setPixelColor(i, 255, 255, 0);
        //strip.show();
      }
    }else if(cuck.low_mid >= .61){//amplitude total .46 & up
      //set 60 pixels to 255, 255, 0
      for(int i = 0; i < 60; ++i){
        strip.setPixelColor(i, 255, 255, 0);
        //strip.show();
      }
    }
  }
  else if(cuck.middle_man == 4){  //greatest freq is MIDRANGE section, so color will be green
    if(cuck.midrange >= .01 and cuck.midrange < .11){//amplitude total .01 to .05
      //set 6 pixels to 0, 255, 0
      for(int i = 0; i < 6; ++i){
        strip.setPixelColor(i, 0, 255, 0);
        //strip.show();
      }
    }else if(cuck.midrange >= .11 and cuck.midrange < .21){//amplitude total .06 to .10
      //set 12 pixels to 0, 255, 0
      for(int i = 0; i < 12; ++i){
        strip.setPixelColor(i, 0, 255, 0);
        //strip.show();
      }
    }else if(cuck.midrange >= .21 and cuck.midrange < .31){//amplitude total .11 to .15
      //set 18 pixels to 0, 255, 0
      for(int i = 0; i < 18; ++i){
        strip.setPixelColor(i, 0, 255, 0);
        //strip.show();
      }
    }else if(cuck.midrange >= .31 and cuck.midrange < .36){//amplitude total .16 to .20
      //set 24 pixels to 0, 255, 0
      for(int i = 0; i < 24; ++i){
        strip.setPixelColor(i, 0, 255, 0);
        //strip.show();
      }
    }else if(cuck.midrange >= .36 and cuck.midrange < .41){//amplitude total .21 to .25
      //set 30 pixels to 0, 255, 0
      for(int i = 0; i < 30; ++i){
        strip.setPixelColor(i, 0, 255, 0);
        //strip.show();
      }
    }else if(cuck.midrange >= .41 and cuck.midrange < .46){//amplitude total .26 to .30
      //set 36 pixels to 0, 255, 0
      for(int i = 0; i < 36; ++i){
        strip.setPixelColor(i, 0, 255, 0);
        //strip.show();
      }
    }else if(cuck.midrange >= .46 and cuck.midrange < .51){//amplitude total .31 to .35
      //set 42 pixels to 0, 255, 0
      for(int i = 0; i < 42; ++i){
        strip.setPixelColor(i, 0, 255, 0);
        //strip.show();
      }
    }else if(cuck.midrange >= .51 and cuck.midrange < .56){//amplitude total .36 to .40
      //set 48 pixels to 0, 255, 0
      for(int i = 0; i < 48; ++i){
        strip.setPixelColor(i, 0, 255, 0);
        //strip.show();
      }
    }else if(cuck.midrange >= .56 and cuck.midrange < .61){//amplitude total .41 to .45
      //set 54 pixels to 0, 255, 0
      for(int i = 0; i < 54; ++i){
        strip.setPixelColor(i, 0, 255, 0);
        //strip.show();
      }
    }else if(cuck.midrange >= .61){//amplitude total .46 & up
      //set 60 pixels to 0, 255, 0
      for(int i = 0; i < 60; ++i){
        strip.setPixelColor(i, 0, 255, 0);
        //strip.show();
      }
    }
  }
  else if(cuck.middle_man == 5){  //greatest freq is HIGH_MID section, so color will be blue
      if(cuck.high_mid >= .01 and cuck.high_mid < .11){//amplitude total .01 to .05
      //set 6 pixels to 0, 0, 255
      for(int i = 0; i < 6; ++i){
        strip.setPixelColor(i, 0, 0, 255);
        //strip.show();
      }

    }else if(cuck.high_mid >= .11 and cuck.high_mid < .21){//amplitude total .06 to .10
      //set 12 pixels to 0, 0, 255
      for(int i = 0; i < 12; ++i){
        strip.setPixelColor(i, 0, 0, 255);
        //strip.show();
      }

    }else if(cuck.high_mid >= .21 and cuck.high_mid < .31){//amplitude total .11 to .15
      //set 18 pixels to 0, 0, 255
      for(int i = 0; i < 18; ++i){
        strip.setPixelColor(i, 0, 0, 255);
        //strip.show();
      }

    }else if(cuck.high_mid >= .31 and cuck.high_mid < .36){//amplitude total .16 to .20
      //set 24 pixels to 0, 0, 255
      for(int i = 0; i < 24; ++i){
        strip.setPixelColor(i, 0, 0, 255);
        //strip.show();
      }

    }else if(cuck.high_mid >= .36 and cuck.high_mid < .41){//amplitude total .21 to .25
      //set 30 pixels to 0, 0, 255
      for(int i = 0; i < 30; ++i){
        strip.setPixelColor(i, 0, 0, 255);
        //strip.show();
      }

    }else if(cuck.high_mid >= .41 and cuck.high_mid < .46){//amplitude total .26 to .30
      //set 36 pixels to 0, 0, 255
      for(int i = 0; i < 36; ++i){
        strip.setPixelColor(i, 0, 0, 255);
        //strip.show();
      }

    }else if(cuck.high_mid >= .46 and cuck.high_mid < .51){//amplitude total .31 to .35
      //set 42 pixels to 0, 0, 255
      for(int i = 0; i < 42; ++i){
        strip.setPixelColor(i, 0, 0, 255);
        //strip.show();
      }

    }else if(cuck.high_mid >= .51 and cuck.high_mid < .56){//amplitude total .36 to .40
      //set 48 pixels to 0, 0, 255
      for(int i = 0; i < 48; ++i){
        strip.setPixelColor(i, 0, 0, 255);
        //strip.show();
      }

    }else if(cuck.high_mid >= .56 and cuck.high_mid < .61){//amplitude total .41 to .45
      //set 54 pixels to 0, 0, 255
      for(int i = 0; i < 54; ++i){
        strip.setPixelColor(i, 0, 0, 255);
        //strip.show();
      }

    }else if(cuck.high_mid >= .61){//amplitude total .46 & up
      //set 60 pixels to 0, 0, 255
      for(int i = 0; i < 60; ++i){
        strip.setPixelColor(i, 0, 0, 255);
        //strip.show();
      }
    }
  }
  else if(cuck.middle_man == 6){  //greatest freq is PRESENCE section, so color will be indigo
    if(cuck.presence >= .01 and cuck.high_mid < .11){//amplitude total .01 to .05
      //set 6 pixels to 75, 0, 130
      for(int i = 0; i < 6; ++i){
        strip.setPixelColor(i, 75, 0, 130);
        //strip.show();
      }

    }else if(cuck.presence >= .11 and cuck.presence < .21){//amplitude total .06 to .10
      //set 12 pixels to 75, 0, 130
      for(int i = 0; i < 12; ++i){
        strip.setPixelColor(i, 75, 0, 130);
        //strip.show();
      }

    }else if(cuck.presence >= .21 and cuck.presence < .31){//amplitude total .11 to .15
      //set 18 pixels to 75, 0, 130
      for(int i = 0; i < 18; ++i){
        strip.setPixelColor(i, 75, 0, 130);
        //strip.show();
      }

    }else if(cuck.presence >= .31 and cuck.presence < .36){//amplitude total .16 to .20
      //set 24 pixels to 75, 0, 130
      for(int i = 0; i < 24; ++i){
        strip.setPixelColor(i, 75, 0, 130);
        //strip.show();
      }

    }else if(cuck.presence >= .36 and cuck.presence < .41){//amplitude total .21 to .25
      //set 30 pixels to 75, 0, 130
      for(int i = 0; i < 30; ++i){
        strip.setPixelColor(i, 75, 0, 130);
        //strip.show();
      }

    }else if(cuck.presence >= .41 and cuck.presence < .46){//amplitude total .26 to .30
      //set 36 pixels to 75, 0, 130
      for(int i = 0; i < 36; ++i){
        strip.setPixelColor(i, 75, 0, 130);
        //strip.show();
      }

    }else if(cuck.presence >= .46 and cuck.presence < .51){//amplitude total .31 to .35
      //set 42 pixels to 75, 0, 130
      for(int i = 0; i < 42; ++i){
        strip.setPixelColor(i, 75, 0, 130);
        //strip.show();
      }

    }else if(cuck.presence >= .51 and cuck.presence < .56){//amplitude total .36 to .40
      //set 48 pixels to 75, 0, 130
      for(int i = 0; i < 48; ++i){
        strip.setPixelColor(i, 75, 0, 130);
        //strip.show();
      }

    }else if(cuck.presence >= .56 and cuck.presence < .61){//amplitude total .41 to .45
      //set 54 pixels to 75, 0, 130
      for(int i = 0; i < 54; ++i){
        strip.setPixelColor(i, 75, 0, 130);
        //strip.show();
      }

    }else if(cuck.presence >= .61){//amplitude total .46 & up
      //set 60 pixels to 75, 0, 130
      for(int i = 0; i < 60; ++i){
        strip.setPixelColor(i, 75, 0, 130);
        //strip.show();
      }
    }
  }
  else if(cuck.middle_man == 7){  //greatest freq is BRILLIANCE section, so color will be violet
    if(cuck.brilliance >= .02 and cuck.brilliance < .11){//amplitude total .02 to .05
      //set 6 pixels to 128, 0, 128
      for(int i = 0; i < 6; ++i){
        strip.setPixelColor(i, 128, 0, 128);
        //strip.show();
      }

    }else if(cuck.brilliance >= .11 and cuck.brilliance < .21){//amplitude total .06 to .10
      //set 12 pixels to 128, 0, 128
      for(int i = 0; i < 12; ++i){
        strip.setPixelColor(i, 128, 0, 128);
        //strip.show();
      }

    }else if(cuck.brilliance >= .21 and cuck.brilliance < .31){//amplitude total .11 to .15
      //set 18 pixels to 128, 0, 128
      for(int i = 0; i < 18; ++i){
        strip.setPixelColor(i, 128, 0, 128);
        //strip.show();
      }

    }else if(cuck.brilliance >= .31 and cuck.brilliance < .36){//amplitude total .16 to .20
      //set 24 pixels to 128, 0, 128
      for(int i = 0; i < 24; ++i){
        strip.setPixelColor(i, 128, 0, 128);
        //strip.show();
      }

    }else if(cuck.brilliance >= .36 and cuck.brilliance < .41){//amplitude total .21 to .25
      //set 30 pixels to 128, 0, 128
      for(int i = 0; i < 30; ++i){
        strip.setPixelColor(i, 128, 0, 128);
        //strip.show();
      }

    }else if(cuck.brilliance >= .41 and cuck.brilliance < .46){//amplitude total .26 to .30
      //set 36 pixels to 128, 0, 128
      for(int i = 0; i < 36; ++i){
        strip.setPixelColor(i, 128, 0, 128);
        //strip.show();
      }

    }else if(cuck.brilliance >= .46 and cuck.brilliance < .51){//amplitude total .31 to .35
      //set 42 pixels to 128, 0, 128
      for(int i = 0; i < 42; ++i){
        strip.setPixelColor(i, 128, 0, 128);
        //strip.show();
      }

    }else if(cuck.brilliance >= .51 and cuck.brilliance < .56){//amplitude total .36 to .40
      //set 48 pixels to 128, 0, 128
      for(int i = 0; i < 48; ++i){
        strip.setPixelColor(i, 128, 0, 128);
        //strip.show();
      }

    }else if(cuck.brilliance >= .56 and cuck.brilliance < .61){//amplitude total .41 to .45
      //set 54 pixels to 128, 0, 128
      for(int i = 0; i < 54; ++i){
        strip.setPixelColor(i, 128, 0, 128);
        //strip.show();
      }

    }else if(cuck.brilliance >= .61){//amplitude total .46 & up
      //set 60 pixels to 128, 0, 128
      for(int i = 0; i < 60; ++i){
        strip.setPixelColor(i, 128, 0, 128);
        //strip.show();
      }
    }
  }
  strip.show();
  delay(Delay);
  strip.clear();
}

void twinkle(int Count, int Delay){
    if(cuck.middle_man == 0){
      strip.setPixelColor(random(0,59), 0, 0, 0); //void
      strip.setPixelColor(random(0,59), 0, 0, 0); //void
    }
    else if(cuck.middle_man == 1){
      strip.setPixelColor(random(0,59), 255, 0, 0); //red
      strip.setPixelColor(random(0,59), 255, 0, 0); //red
      strip.setPixelColor(random(0,59), 255, 0, 0); //red
      strip.setPixelColor(random(0,59), 255, 0, 0); //red
    }
    else if(cuck.middle_man == 2){
      strip.setPixelColor(random(0,59), 255, 165, 0); //orange
      strip.setPixelColor(random(0,59), 255, 165, 0); //orange
      strip.setPixelColor(random(0,59), 255, 165, 0); //orange
      strip.setPixelColor(random(0,59), 255, 165, 0); //orange
    }
    else if(cuck.middle_man == 3){
      strip.setPixelColor(random(0,59), 255, 255, 0); //yellow
      strip.setPixelColor(random(0,59), 255, 255, 0); //yellow
      strip.setPixelColor(random(0,59), 255, 255, 0); //yellow
      strip.setPixelColor(random(0,59), 255, 255, 0); //yellow
    }
    else if(cuck.middle_man == 4){
      strip.setPixelColor(random(0,59), 0, 255, 0); //green
      strip.setPixelColor(random(0,59), 0, 255, 0); //green
      strip.setPixelColor(random(0,59), 0, 255, 0); //green
      strip.setPixelColor(random(0,59), 0, 255, 0); //green
    }
    else if(cuck.middle_man == 5){
      strip.setPixelColor(random(0,59), 0, 0, 255); //blue
      strip.setPixelColor(random(0,59), 0, 0, 255); //blue
      strip.setPixelColor(random(0,59), 0, 0, 255); //blue
      strip.setPixelColor(random(0,59), 0, 0, 255); //blue
    }
    else if(cuck.middle_man == 6){
      strip.setPixelColor(random(0,59), 75, 0, 130); //ingido
      strip.setPixelColor(random(0,59), 75, 0, 130); //ingido
      strip.setPixelColor(random(0,59), 75, 0, 130); //ingido
      strip.setPixelColor(random(0,59), 75, 0, 130); //ingido
    }
    else if(cuck.middle_man == 7){
      strip.setPixelColor(random(0,59), 128, 0, 128); //purple
      strip.setPixelColor(random(0,59), 128, 0, 128); //purple
      strip.setPixelColor(random(0,59), 128, 0, 128); //purple
      strip.setPixelColor(random(0,59), 128, 0, 128); //purple
    }
    strip.show();
    delay(Delay);
    strip.clear();
  }
