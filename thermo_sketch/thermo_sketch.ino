#include <SimpleDHT.h>
//#include <TroykaDHT.h>
#include <LiquidCrystal.h>

int heater = 5;
bool heaterOn = false;
int temp = 0;
int btnLess = 2;
int btnMode = 3;
int btnMore = 4;
int lcdBacklight = 13;
int setTemp = 18;
int tempRange = 5;
int step = 1;
int inputMode = 0;
int tempSensorConst = 10;
int lastTemp = 0;
int currentTemp = 80;
int modeReset = 100;
int i = 0;
int j = 0;
int k = 0;
byte temperature = 80;
byte humidity = 50;

LiquidCrystal lcd(11,10,9,8,7,6);
//DHT dht(12,DHT11);
SimpleDHT11 dht11(12);

void setup()
{
  pinMode(heater, OUTPUT);
  pinMode(lcdBacklight, OUTPUT);
  digitalWrite(lcdBacklight, HIGH);
  pinMode(btnLess, INPUT);
  pinMode(btnMode, INPUT);
  pinMode(btnMore, INPUT);
  lcd.begin(16,2);
  //dht.begin();
  Serial.begin(9600);
}

void loop()
{
  if (digitalRead(btnLess) == HIGH){
    decreaseSetting();
    printSetting();
    updateDisplay();
    digitalWrite(lcdBacklight, HIGH);
    k = 0;
  } else if (digitalRead(btnMode) == HIGH){
    changeMode();
    printSetting();
    updateDisplay();
    digitalWrite(lcdBacklight, HIGH);
    k = 0;
  } else if (digitalRead(btnMore) == HIGH){
    increaseSetting();
    printSetting();
    updateDisplay();
    digitalWrite(lcdBacklight, HIGH);
    k = 0;
  }

//  dht.read();
//
//  switch(dht.getState()){
//    case DHT_OK:
//      currentTemp = dht.getTemperatureC();
//    case DHT_ERROR_CHECKSUM:
//      currentTemp = 999;
//      Serial.println("DHT: Checksum error");
//      break;
//    case DHT_ERROR_TIMEOUT:
//      currentTemp = 999;
//      Serial.println("DHT: Timout error");
//      break;
//    case DHT_ERROR_NO_REPLY:
//      currentTemp = 999;
//      Serial.println("DHT: Sensor not connected");
//      break;
//  }

  if (j < 15){
    j = j+1;
  }else{
    int err = SimpleDHTErrSuccess;
    if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
      Serial.print("Read DHT11 failed, err="); Serial.print(SimpleDHTErrCode(err));
      Serial.print(","); Serial.println(SimpleDHTErrDuration(err));
      return;
    }
    j = 0;
  }

  currentTemp = temperature;
  int minTemp = setTemp - tempRange;
  
  if (currentTemp < minTemp && !heaterOn){
    digitalWrite(heater, HIGH);
    heaterOn = true;
    updateDisplay();
  }else if (currentTemp >= setTemp && heaterOn){
    digitalWrite(heater, LOW);
    heaterOn = false;
    updateDisplay();
  }
  
  if (lastTemp != currentTemp){
    updateDisplay();
    printTemp(currentTemp);
    lastTemp = currentTemp;
  }
  
  if (inputMode != 0 && i < modeReset){
    i = i+1;
  }else if (inputMode != 0 && i >= modeReset){
    changeMode();
    updateDisplay();
    i = 0;
  }

  
  if (k < 300){
    k = k+1;
  }else{
    digitalWrite(lcdBacklight, LOW);
  }
  
  delay(100);
}

void printSetting(){
  Serial.println((String)"Set temperature: "+setTemp);
  Serial.println((String)"  Set range: "+tempRange);
}

void printTemp(int temp){
  Serial.println((String)"Current temperature: "+temp);
}


void increaseSetting(){
  if(setTemp <= 79 && inputMode == 0){
    setTemp = setTemp + step;
  }
  if(tempRange <= 19 && inputMode == 1){
    tempRange = tempRange + 1;
    i = 0;
  }
}

void decreaseSetting(){
  if(setTemp >= 1 && inputMode == 0){
    setTemp = setTemp - step;
  }
  if(tempRange >= 1 && inputMode == 1){
    tempRange = tempRange - 1;
    i = 0;
  }
}

void changeMode(){
  if(inputMode == 0){
    inputMode = 1;
  }else{
    inputMode = 0;
  }
  i = 0;
}

void updateDisplay(){
  lcd.clear();
  lcd.setCursor(0,0);
  if (heaterOn){
    lcd.print((String)"Temp: " + currentTemp + " HEATING");
  }else{
    lcd.print((String)"Temp: " + currentTemp);
  }
  lcd.setCursor(0,1);
  if (inputMode == 0){
    lcd.print((String)"Set: " + setTemp);
  }else if (inputMode == 1){
    lcd.print((String)"Range: " + tempRange);
  }
}
