//https://lastminuteengineers.com/esp32-deep-sleep-wakeup-sources/
//https://components101.com/microcontrollers/esp32-devkitc
//codul culorilor la rezistente: https://eprofu.ro/cc/cc.html
//rezistenta=1k am pus
//
// 3.3V
//   |
//   |                     fir rosu
//   0 
//  \
//   0 --->GPIO36          fir verde
//   |                                     
//   1k
//   |
//   |                    fir negru
//  ___
//   -
//
//
//Remark: la rulare:
//apasa EN button daca la connecting apare .....-----.....-----
//pt ca s-ar putea sa fie in sleepeng status...
//
RTC_DATA_ATTR int bootCount = 0;
RTC_DATA_ATTR int led_pin = 4;   //led rosu
void setup(){
  Serial.begin(115200);
pinMode(led_pin, OUTPUT);
  digitalWrite (led_pin, HIGH); //led stay light until go to sleeping after 1 second:)  
  delay(1000); 

  //Increment boot number and print it every reboot
  ++bootCount;
  Serial.println("---------------");
  Serial.println("Boot number: " + String(bootCount));

  //Print the wakeup reason for ESP32
  print_wakeup_reason();

  //https://components101.com/microcontrollers/esp32-devkitc
  //Configure GPIO33 as ext0 wake up source for HIGH logic level
  //. Acest PIN poate fi unul dintre GPIO- urile RTC  0,2,4,12-15,25-27,32-39 .
  //aleg gpio36=phisical-pin-number=5
  //a se vedea buton negru mic cu firul verde
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_36,1);
  Serial.println("-will go to sleep for now");
  //delay(1000);
  //Go to sleep now
  digitalWrite (led_pin, LOW);   //light is now off because go to sleeping
  Serial.println("--now sleeping...");
  Serial.println("--click smallest black button to wakeup!--");
  Serial.flush();  
  esp_deep_sleep_start();
}

void loop(){}

//Function that prints the reason by which ESP32 has been awaken from sleep
void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  switch(wakeup_reason)
  {
    case 1  : Serial.println("R1.Wakeup caused by external signal using RTC_IO"); break;
    case 2  : Serial.println("R2.Wakeup caused by external signal using RTC_CNTL"); break;
    case 3  : Serial.println("R3.Wakeup caused by timer"); break;
    case 4  : Serial.println("R4.Wakeup caused by touchpad"); break;
    case 5  : Serial.println("R5.Wakeup caused by ULP program"); break;
    default : Serial.println("Rdefault.Wakeup was not caused by deep sleep"); break;
  }
  Serial.flush();
}
