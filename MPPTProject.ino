#define currentSensor 25
#define dutyCyclePWM 26
#define inputVoltageSensor 27

/*
current sensor model  = ACS712x20A
 - current range = +- 20 A
 - 

Boost DC-DC Converter
- 380 V output

*/

const int sensitivity = 10; //10 A for 1 V
const int freq = 5000; // PWM frequency
const int ledChannel = 0;
const int resolution = 13; // resolution used for the PWM sugnal
int dutyCycleMain = 2048;
int dutyCycleTest = 2048;
float outputVolt = 0;
float inputVolt = 0;
float current;
float power[3]={0};

unsigned long t = 0;




void setup() {
  // put your setup code here, to rfun once:
  Serial.begin(115200);
  pinMode(currentSensor,OUTPUT);
  pinMode(dutyCyclePWM, INPUT_PULLUP);
    // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(currentSensor, ledChannel);
}

void loop() {

  for(int i=0;i<=2;i++){
    dutyCycleTest = (dutyCycleMain - 10) + i*10;
    ledcWrite(ledChannel, dutyCycleTest);
    while(millis()-t <= 10){ // rotine used for waiting the stationary state
    }
    t = millis();
    current = map(analogRead(currentSensor),0,4095,-250,250)/100.0; // resolution ADC ESP32 = 12 bits
    inputVolt = map(analogRead(inputVoltageSensor),0,4095,0,1000); // resolution ADC ESP32 = 12 bits
    power[i] = current*inputVolt;
  }

  if(power[1] <= power[0] && power[1] >= power[2]){
    dutyCycleMain -= 10;  
  }
  else if(power[1] >= power[0] && power[1] <= power[2]){
    dutyCycleMain += 10;  
  }




  


}
