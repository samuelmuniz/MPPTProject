
#include <LiquidCrystal_I2C.h>



#define currentSensor 25
#define inputVoltageSensor 26
#define dutyCyclePWM 27

/*
current sensor model  = ACS712x20A
 - current range = +- 20 A
 - 
Boost DC-DC Converter
- 380 V output
*/

const int freq = 5000; // PWM frequency
const int ledChannel = 0;
const int resolution = 13; // resolution used for the PWM sugnal
int dutyCycleMain = 2048;
int dutyCycleTest = 2048;
float inputVolt = 0;
float current;
float power[3]={0};

unsigned long t = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2); 



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

  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
  lcd.setCursor(0, 0);
  // print message
  lcd.print("POWER : ");
}

void loop() {


  dutyCycleTest = dutyCycleMain - 10;
  ledcWrite(ledChannel, dutyCycleTest);
  // rotine used for waiting 400 ms until the stationary state
  while(millis()-t <= 400){ 
  }
  t = millis();
  // rotine used for waiting 400 ms until the stationary state    current = map(analogRead(currentSensor),0,4095,-2000,2000); // resolution ADC ESP32 = 12 bits
  current = current/100.00; // Ip = [-20 A,20 A]
  inputVolt = map(analogRead(inputVoltageSensor),0,4095,0,1000); // resolution ADC ESP32 = 12 bits
  inputVolt = inputVolt/100.00; // V = [0 V,10 V]
  power[0] = current*inputVolt;

  dutyCycleTest = dutyCycleMain + 10;
  ledcWrite(ledChannel, dutyCycleTest);
  // rotine used for waiting 400 ms until the stationary state
  while(millis()-t <= 400){ 
  }
  t = millis();
  // rotine used for waiting 400 ms until the stationary state    current = map(analogRead(currentSensor),0,4095,-2000,2000); // resolution ADC ESP32 = 12 bits
  current = current/100.00; // Ip = [-20 A,20 A]
  inputVolt = map(analogRead(inputVoltageSensor),0,4095,0,1000); // resolution ADC ESP32 = 12 bits
  inputVolt = inputVolt/100.00; // V = [0 V,10 V]
  power[2] = current*inputVolt;
  

  if(power[1] <= power[0] && power[1] >= power[2]){
    dutyCycleMain -= 10;  
  }
  else if(power[1] >= power[0] && power[1] <= power[2]){
    dutyCycleMain += 10;  
  }

  ledcWrite(ledChannel, dutyCycleMain);
  // rotine used for waiting 400 ms until the stationary state
  while(millis()-t <= 400){ 
  }
  t = millis();
  // rotine used for waiting 400 ms until the stationary state    current = map(analogRead(currentSensor),0,4095,-2000,2000); // resolution ADC ESP32 = 12 bits
  current = current/100.00; // Ip = [-20 A,20 A]
  inputVolt = map(analogRead(inputVoltageSensor),0,4095,0,1000); // resolution ADC ESP32 = 12 bits
  inputVolt = inputVolt/100.00; // V = [0 V,10 V]
  power[1] = current*inputVolt;

  // set cursor to first column, first row
  lcd.setCursor(0, 1);
  // print message
  lcd.print(power[1]);


  ledcWrite(ledChannel, dutyCycleMain);
  // rotine used for waiting 5 s until the stationary state
  while(millis()-t <= 5000){ 
  }
  t = millis();
  // rotine used for waiting 5 s until the stationary state




  


}