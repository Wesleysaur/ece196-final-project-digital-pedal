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
AudioSynthWaveformSine   sin_frequency_shifter;          //xy=476,368
AudioOutputI2S           i2s2;           //xy=503.5,657.2499694824219
AudioEffectMultiply      am_multiplier;      //xy=684,323
AudioConnection          patchCord1(i2s1, 0, feedback_mixer, 0);
AudioConnection          patchCord2(i2s1, 0, dry_wet_mixer, 0);
AudioConnection          patchCord3(dry_wet_mixer, 0, i2s2, 0);
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
float pitch_shift;


void setup() {
  AudioMemory(20); //We will reduce this based on test results
  Serial.begin(9600);
  while (!Serial) ;

  dry_wet_amount = .5;
  volume_ctrl = .6;
  delay_amount = 0;
  pitch_shift = 0;
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
  sin_frequency_shifter.amplitude(1.0);
  sin_frequency_shifter.phase(0);
  sin_frequency_shifter.frequency(0);

  dry_wet_mixer.gain(0, dry_wet_amount);
  dry_wet_mixer.gain(1, 1-dry_wet_amount);
  
  sgtl5000_1.enable();
  sgtl5000_1.volume(volume_ctrl);
}

void loop() {
  Serial.print("Processor usage: ");
  Serial.print(AudioProcessorUsage());
  Serial.print(",");
  Serial.print(AudioProcessorUsageMax());
  Serial.print("    ");
  Serial.print("Memory: ");
  Serial.print(AudioMemoryUsage());
  Serial.print(",");
  Serial.println(AudioMemoryUsageMax());


  /*
   * Set sine parameters
   */
  
  
}
