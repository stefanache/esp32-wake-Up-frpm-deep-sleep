    /////////////////////////////////////////////////////////////////
   //             ESP32 Deep Sleep Example 1             v1.00    //
  //       Get the latest version of the code here:              //
 //       http://educ8s.tv/esp32-deep-sleep-tutorial            //
/////////////////////////////////////////////////////////////////

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  3        /* Time ESP32 will go to sleep (in seconds) */

RTC_DATA_ATTR int bootCount = 0;

int GREEN_LED_PIN = 16;
int YELLOW_LED_PIN = 17;

void setup(){
  Serial.begin(115200);
  //Print the wakeup reason for ESP32
  print_wakeup_reason();
  
  pinMode(GREEN_LED_PIN,OUTPUT);
  pinMode(YELLOW_LED_PIN,OUTPUT);
  delay(500);
  
  if(bootCount == 0) {                    //Run this only the first time
      digitalWrite(YELLOW_LED_PIN,HIGH);
      //Increment boot number and print it pn;y first because not need at every boot
      bootCount = bootCount+1;
      Serial.println("Boot number: " + String(bootCount));
  }else{
      digitalWrite(GREEN_LED_PIN,HIGH);
  }
  

  
  delay(3000);

  digitalWrite(GREEN_LED_PIN,LOW);
  digitalWrite(YELLOW_LED_PIN,LOW);

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}

void loop(){
  
}
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
}
