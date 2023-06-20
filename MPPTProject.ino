
#include <LiquidCrystal_I2C.h>



#define currentSensor 25
#define inputVoltageSensor 26
#define dutyCyclePWM 27

/*
current sensor model  = ACS712x20A
 - current range = +- 20 A
 - 
Buck-Boost DC-DC Converter
- 380 V output
*/

// PWM frequency
const int freq = 5000; 
const int ledChannel = 0;
const int timeDelay = 500;
// resolution used for the PWM signal
//PWM max value = 4096 with resolution = 12
const int resolution = 12; 
int dutyCycle = 2048;
float inputVolt = 0;
float current = 0;
float power[2]={0};
float dpower = 0;

unsigned long t = 0;



void delay(){
  t = millis();
  while(millis() - t <= timeDelay){ 
  }
}

void measurement(){
  // Ip = [-20 A,20 A]
  current = map(analogRead(currentSensor),0,4095,-2000,2000);
  current = current/100.00; 
  // resolution ADC ESP32 = 12 bits
  inputVolt = map(analogRead(inputVoltageSensor),0,4095,0,1000); 
  // V = [0 V,10 V]
  inputVolt = inputVolt/100.00; 
}



void setup() {
  // put your setup code here, to rfun once:
  Serial.begin(115200);
  pinMode(dutyCyclePWM,OUTPUT);
  pinMode(currentSensor, INPUT_PULLUP);
  pinMode(inputVoltageSensor, INPUT_PULLUP);
  // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(dutyCyclePWM, ledChannel);
}

void loop() {

  measurement();
  power[0] = current*inputVolt;
  ledcWrite(ledChannel, dutyCycle + 10);
  
  delay();

  measurement();
  power[1] = current*inputVolt;
  ledcWrite(ledChannel, dutyCycle);

  dpower = power[1] - power[0];

  if(dpower > 0 && dutyCycle < 3000){
    dutyCycle += 10;  
  }
  else if(dpower < 0 && dutyCycle > 500){
    dutyCycle -= 10;  
  }

  ledcWrite(ledChannel, dutyCycle);
  delay();


}
