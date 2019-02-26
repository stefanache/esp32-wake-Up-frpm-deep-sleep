//Define touch sensitivity. Greater the value, more the sensitivity.
//https://www.instructables.com/id/IOT-Made-Simple-Playing-With-the-ESP32-on-Arduino-/
// vezi si https://lastminuteengineers.com/esp32-deep-sleep-wakeup-sources/
//https://components101.com/microcontrollers/esp32-devkitc
//
//Remark: la rulare:
//apasa EN button daca la connecting apare .....-----.....-----
//pt ca s-ar putea sa fie in sleepeng status...
//
#define Threshold 50

RTC_DATA_ATTR int bootCount = 0;//e salvat in memoria RTC ca sa nu dispara pe perioada de sleep
RTC_DATA_ATTR int led_pin = 4;   //led rosu

//pot fi si alte date salvate in memoria RTC - ex:
RTC_DATA_ATTR char* net_SSID="UPC1341388";
RTC_DATA_ATTR char* net_Password="WACEYGCG";

touch_pad_t touchPin;

void callback(){
  //placeholder callback function //nu lucreaza
  Serial.println("---callback");
  int touch_value = touchRead(T5);
  Serial.println("-~-Touch value=" + String(touch_value));  // get value using T5   
  Serial.flush();
}

void setup(){
  Serial.begin(115200);
  pinMode(led_pin, OUTPUT);
  digitalWrite (led_pin, HIGH); //led stay light until go to sleeping for 1 second .:)
  delay(1000);

  //Increment boot number and print it every reboot
  ++bootCount;
  Serial.println("---------------");
  Serial.println("* Boot number: #" + String(bootCount));
  Serial.println("* Network SSID: " + String(net_SSID));
  Serial.println("* Network PSW:  " + String(net_Password));
  
  //Print the wakeup reason for ESP32 and touchpad too
  print_wakeup_reason();
  print_wakeup_touchpad();

  //https://randomnerdtutorials.com/esp32-pinout-reference-gpios/
  //Setup interrupt on Touch Pad 5 -GPIO12- ESP32 Pin IO12(Touch 5=T5)-Pinnumber 18
  touchAttachInterrupt(T5, callback, Threshold);  //firul orange care are capatul in aer

  Serial.println("-will go to sleep for now");
  //Configure Touchpad as wakeup source
  esp_sleep_enable_touchpad_wakeup();

  //Go to sleep now- nu mergea fara pauza asta pt ca nu avea flush
  //Serial.println("-over 1 second go to sleep for now ");
  //delay(1000);
  digitalWrite (led_pin, LOW);   //light is now off because go to sleeping
  Serial.println("--now sleeping...");
  Serial.println("--touch orange free feed to wakeup!--");
  Serial.flush();
  esp_deep_sleep_start();
}

void loop(){}

//Function that prints the reason by which ESP32 has been awaken from sleep
void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  switch(wakeup_reason)  {
    case 1  : Serial.println("R1.Wakeup caused by external signal using RTC_IO"); break;
    case 2  : Serial.println("R2.Wakeup caused by external signal using RTC_CNTL"); break;
    case 3  : Serial.println("R3.Wakeup caused by timer"); break;
    case 4  : Serial.println("R4.Wakeup caused by touchpad"); break;
    case 5  : Serial.println("R5.Wakeup caused by ULP program"); break;
    default : Serial.println("Rdefault-Wakeup was not caused by deep sleep"); break;
  }
}


//Function that prints the touchpad by which ESP32 has been awaken from sleep
void print_wakeup_touchpad(){
  touch_pad_t pin;
  touchPin = esp_sleep_get_touchpad_wakeup_status();
  switch(touchPin)  {
    case 0  : Serial.println("T0.Touch detected on GPIO 4"); break;
    case 1  : Serial.println("T1.Touch detected on GPIO 0"); break;
    case 2  : Serial.println("T2.Touch detected on GPIO 2"); break;
    case 3  : Serial.println("T3.Touch detected on GPIO 15"); break;
    case 4  : Serial.println("T4.Touch detected on GPIO 13"); break;
    case 5  : Serial.println("~T5.Touch detected on GPIO 12"); break;
    case 6  : Serial.println("T6.Touch detected on GPIO 14"); break;
    case 7  : Serial.println("T7.Touch detected on GPIO 27"); break;
    case 8  : Serial.println("T8.Touch detected on GPIO 33"); break;
    case 9  : Serial.println("T9.Touch detected on GPIO 32"); break;
    default : Serial.println("Tdefault.Wakeup not by touchpad"); break;
  }
  Serial.flush();
}
