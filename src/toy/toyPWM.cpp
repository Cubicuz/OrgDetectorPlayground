#include "toyPWM.h"
#include "preferencesManager.h"

const int PWM_CHANNEL = 0;    // ESP32 has 16 channels which can generate 16 independent waveforms
const int PWM_FREQ = 500;     // Recall that Arduino Uno is ~490 Hz. Official ESP32 example uses 5,000Hz
const int PWM_RESOLUTION = 8; // We'll use same resolution as Uno (8 bits, 0-255) but ESP32 can go up to 16 bits 

// The max duty cycle value based on PWM resolution (will be 255 if resolution is 8 bits)
const int MAX_DUTY_CYCLE = 200;
const int LOW_DUTY_CYCLE = 50;

const int LED_OUTPUT_PIN = 33;

void ToyPWM::init()
{
  // Sets up a channel (0-15), a PWM duty cycle frequency, and a PWM resolution (1 - 16 bits) 
  // ledcSetup(uint8_t channel, double freq, uint8_t resolution_bits);
  ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);

  // ledcAttachPin(uint8_t pin, uint8_t channel);
  ledcAttachPin(LED_OUTPUT_PIN, PWM_CHANNEL);
}

void ToyPWM::setIntensityInt(int16_t intensity)
{
  if (!PreferencesManager::instance.pwmToyEnabled()){
    ledcWrite(PWM_CHANNEL, 0);
    return;
  }
  if (intensity > MAX_DUTY_CYCLE){
    intensity = MAX_DUTY_CYCLE;
  } else if (intensity < 0){
    intensity = 0;
  }
  ledcWrite(PWM_CHANNEL, intensity);
}

void ToyPWM::setIntensity(Intensity intensity)
{
  switch (intensity)
  {
  case Off:
    setIntensityInt(0);
    break;
  case Low:
    setIntensityInt(LOW_DUTY_CYCLE);
    break;
  case High:
    setIntensityInt(MAX_DUTY_CYCLE);
    break;
  default:
    break;
  }
}
