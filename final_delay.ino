#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=133,198
AudioMixer4              dry_wet_mixer;         //xy=295.25,659
AudioEffectDelay         delay1;         //xy=335,279
AudioMixer4              feedback_mixer;         //xy=367,148
AudioSynthWaveformSineHires   sin_frequency_shifter;          //xy=476,368
AudioOutputI2S           i2s2;           //xy=503.5,657.2499694824219
AudioEffectMultiply      am_multiplier;      //xy=684,323
AudioConnection          patchCord1(i2s1, 1, feedback_mixer, 0);
AudioConnection          patchCord2(i2s1, 1, dry_wet_mixer, 0);
AudioConnection          patchCord3(dry_wet_mixer, 0, i2s2, 1);
AudioConnection          patchCord4(delay1, 0, am_multiplier, 0);
AudioConnection          patchCord5(feedback_mixer, delay1);
AudioConnection          patchCord6(sin_frequency_shifter, 0, am_multiplier, 1);
AudioConnection          patchCord7(am_multiplier, 0, dry_wet_mixer, 1);
AudioConnection          patchCord8(am_multiplier, 0, feedback_mixer, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=670,657
// GUItool: end automatically generated code

//Global variables (lol)
//These will be the parameters we can adjust
float dry_wet_amount;
float delay_amount;
float volume_ctrl;
float feedback_amount;
float mod_freq;

const int DW_PIN  = 3; //which pin to control dry 
const int D_AMNT_PIN = 2; //which pin to control delay amount
const int V_CTRL_PIN = 1; //volume control pin
const int FB_AMNT_PIN = 7; //feedback amount pin
const int MOD_FREQ_PIN = 6; //which pin controls frequency of sinusoid


void setup() {
  AudioMemory(170); //We will reduce this based on test results
  Serial.begin(9600);
  //while (!Serial) ;

  dry_wet_amount = .5;
  volume_ctrl = .6;
  delay_amount = 0;
  mod_freq = 0;
  feedback_amount = 0;

  //Initialize feedback mixer
  feedback_mixer.gain(0, 1);
  feedback_mixer.gain(1, feedback_amount);

  //initialize delay1
  delay1.delay(0, 0);
  //Disable other delays to save memory
  delay1.disable(1);
  delay1.disable(2);
  delay1.disable(3);
  delay1.disable(4);
  delay1.disable(5);
  delay1.disable(6);
  delay1.disable(7);
  
  //Initialize audio effect parameters
  sin_frequency_shifter.amplitude(0.8);
  sin_frequency_shifter.phase(0);
  sin_frequency_shifter.frequency(0);

  dry_wet_mixer.gain(0, dry_wet_amount);
  dry_wet_mixer.gain(1, 1-dry_wet_amount);
  
  sgtl5000_1.enable();
  sgtl5000_1.volume(1);
}

void loop() {

  /*
  Serial.print("Processor usage: ");
  Serial.print(AudioProcessorUsage());
  Serial.print(",");
  Serial.print(AudioProcessorUsageMax());
  Serial.print("    ");
  Serial.print("Memory: ");
  Serial.print(AudioMemoryUsage());
  Serial.print(",");
  Serial.println(AudioMemoryUsageMax());
  */
  
  //get the correct parameters
  dry_wet_amount = get_dw_amount();
  delay_amount = get_delay_amount();
  volume_ctrl = get_volume_ctrl();
  feedback_amount = get_fb_amnt();
  mod_freq = get_mod_freq();
  
  //set feedback amount
  feedback_mixer.gain(0, 1);
  feedback_mixer.gain(1, feedback_amount);

  //set delay 1
  delay1.delay(0, delay_amount);
  
  //set modulation frequency 
  sin_frequency_shifter.frequency(mod_freq);

  //set dry/wet & volume
  dry_wet_mixer.gain(0, (dry_wet_amount)*volume_ctrl);
  dry_wet_mixer.gain(1, (1-dry_wet_amount)*volume_ctrl);

  Serial.print("fb: ");
  Serial.println(feedback_amount);
  
}

/**
 * I seperated all of these into functions so if we decide to make make any of them
 * nonlinear we can edit it without impacting anything else
 */

/*
 * get_dw_amount()
 * Reads the dry wet amount from analog reading the get dw pin and then scales it from 0-1
 */
float get_dw_amount() {
  return analogRead(DW_PIN)/1023.0;
}

/*
 * get_delay_amount()
 * reads from the delay amount pin and then scales it to the maximum possible delay value
 * calculated by dividing the analog read by 1024, and multiplying that by the max delay val
 * for our hardware (teensy 3.2), 449ms
 * 
 * 449/1023.0 = 0.4389051808406647
 */
 float get_delay_amount() {
  return (1023-analogRead(D_AMNT_PIN))*0.3;
 }

/**
 * get_volume_ctrl()
 * reads the volume and scales from 0-1
 */
 float get_volume_ctrl() {
  return (1023-analogRead(V_CTRL_PIN))/1023.0;
 }

 /**
  * get_fb_amnt()
  * reads the feedback amount and scales from 0-1
  */
float get_fb_amnt() {
  //Serial.println(analogRead(FB_AMNT_PIN));
  return (1023-analogRead(FB_AMNT_PIN))/1023.0;
}

/**
 * get_mod_freq()
 * read the frequency of the sin (am) modulator
 * **This is the one we may make non-linear
 * should go between 0-150 (150/1024=0.1466275659824)
 */

float get_mod_freq() {
  Serial.println(analogRead(MOD_FREQ_PIN));
  return (1023-analogRead(MOD_FREQ_PIN))*0.5;
}

