/*
Simple F-Stop Timer by Andy
*/

#include <ShiftDisplay.h>
#include <SimpleRotary.h>
#include <Button.h>

const int DISPLAY_TYPE = COMMON_ANODE;  // COMMON_CATHODE or COMMON_ANODE
const int DISPLAY_SIZE = 4;             // number of digits on display
const int ROTA_PIN = 2;                 // rotary encoder A pin
const int ROTB_PIN = 3;                 // rotary encoder B pin
const int ROTBUTTON_PIN = 4;            // rotary encoder button pin
const int SBUTTON_PIN = 8;              // START button/pedal pin
const int LAMP_PIN = 9;                 //Lamp relay pin
const int BUZZER_PIN = 10;              // buzzer pin
const int t0ne = 523;                   //buzzer tome

byte encdir, encbut, menulevel, menuitem, itemcount, f_stop, strip;
double e_time, d_time;
float countdown;
byte f_stop_[6] = { 0, 1, 2, 3, 6, 12 };  // 1/1, 1/2, 1/3, 1/6, 1/12
byte strip_[5] = { 0, 3, 5, 7, 9 };       //strip count -/0/+ with f-stop step
String temp_str;
bool _10x;
float strip_expo_[10];  //store strip values

ShiftDisplay display(DISPLAY_TYPE, DISPLAY_SIZE);        // default pins used 5, 6, 7
SimpleRotary rotary(ROTA_PIN, ROTB_PIN, ROTBUTTON_PIN);  // Rotary Encoder Pin A, Pin B, Button Pin
Button pedal(SBUTTON_PIN);


void setup() {
  // Serial.begin(115200);   // for debug 
  // Set the trigger to be either a HIGH or LOW pin (Default: HIGH)
  // Note this sets all three pins to use the same state.
  rotary.setTrigger(HIGH);
  rotary.setDebounceDelay(5);  // Set the debounce delay in ms  (Default: 2)
  rotary.setErrorDelay(250);   // Set the error correction delay in ms  (Default: 200)
  pedal.begin();
  // pinMode(SBUTTON_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LAMP_PIN, OUTPUT);
  digitalWrite(LAMP_PIN, HIGH);  //turn OFF

  menulevel = 1;
  menuitem = 3;
  itemcount = 5;
  f_stop = f_stop_[2];
  strip = strip_[3];
  e_time = 10.0;
  d_time = 180;  // 180 sec - 3min for average paper
}

void loop() {
  encbut = rotary.pushType(1000);
  encdir = rotary.rotate();
  switch (encdir) {
    case 1:
      menuitem = menuitem + 1;
      if (menuitem > itemcount)
        menuitem = 1;
      break;
    case 2:
      menuitem = menuitem - 1;
      if (menuitem < 1)
        menuitem = itemcount;
      break;
  }
  if (menulevel == 1) {
    switch (menuitem) {
      case 1:  //set F-STOPs steps
        display.set("F-St", ALIGN_LEFT);
        display.setDot(3, true);
        if (encbut == 1) {
          menulevel = 11;
          encbut = 0;
          itemcount = 5;
          menuitem = binarySearch(f_stop_, 6, f_stop);
        }
        break;
      case 2:  //set strip count
        display.set("Stri", ALIGN_LEFT);
        display.setDot(3, true);
        if (encbut == 1) {
          menulevel = 21;
          encbut = 0;
          itemcount = 4;
          menuitem = binarySearch(strip_, 5, strip);
        }
        break;
      case 3:  // enlarger timer
        display.set("En-t", ALIGN_LEFT);
        display.setDot(1, true);
        display.setDot(3, true);
        if (encbut == 1) {
          menulevel = 31;
          encbut = 0;
          encdir = 0;
        }
        break;
      case 4:  //develop timer
        display.set("de-t", ALIGN_LEFT);
        display.setDot(1, true);
        display.setDot(3, true);
        if (encbut == 1) {
          menulevel = 41;
          encbut = 0;
          _10x = true;
        }
        break;
      case 5:  //Set initial time
        display.set("SEtt", ALIGN_LEFT);
        display.setDot(2, true);
        if (encbut == 1) {
          menulevel = 51;
          encbut = 0;
          itemcount = 10;
          menuitem = 10;
        }
        break;
    }
  }
  if (menulevel == 11) {  //set F-STOPs steps
    temp_str = "1-" + String(f_stop);
    display.set(temp_str, ALIGN_LEFT);
    f_stop = f_stop_[menuitem];
    if (encbut == 1) {
      menulevel = 1;
      menuitem = 1;
      itemcount = 5;
    }
  }

  if (menulevel == 21) {  //set strip count
    temp_str = "S-" + String(strip);
    display.set(temp_str, ALIGN_LEFT);
    display.setDot(0, true);
    strip = strip_[menuitem];
    if (encbut == 2)
      strip_expo();
    if (encbut == 1) {
      menulevel = 1;
      menuitem = 2;
      itemcount = 5;
    }
  }

  if (menulevel == 31) {  // enlarger timer
    switch (encdir) {
      case 1:
        e_time = e_time * pow(2.0, 1.0 / f_stop);
        if (e_time > 900)
          e_time = 900;
        break;
      case 2:
        e_time = e_time * pow(2.0, -1.0 / f_stop);
        if (e_time < 1)
          e_time = 1;
        break;
    }
    if (encdir != 0)
      countdown = 0;

    if (pedal.pressed())
      expo();
    if (countdown == 0)
      display.set(e_time, 1);
    else
      display.set(countdown, 1);
    if (encbut == 1) {
      menulevel = 1;
      menuitem = 3;
      countdown = 0;
    }
  }

  if (menulevel == 41) {  //develop timer
    switch (encdir) {
      case 1:
        if (_10x)
          d_time = d_time + 10;
        else
          d_time = d_time + 1;
        if (d_time > 900)
          d_time = 1;
        break;
      case 2:
        if (_10x)
          d_time = d_time - 10;
        else
          d_time = d_time - 1;
        if (d_time < 1)
          d_time = 900;
        break;
    }
    display.set(d_time, 1);
    if (encdir != 0)
      countdown = 0;

    if (encbut == 2)
      if (_10x) {
        _10x = false;
        display.show(1.0, 500, 1, ALIGN_LEFT);
      } else {
        _10x = true;
        display.show(10.0, 500, 1, ALIGN_LEFT);
      }

    if (pedal.pressed())
      develop_timer();
    if (countdown == 0)
      display.set(d_time, 1);
    else
      display.set(countdown, 1);
    if (encbut == 1) {
      menulevel = 1;
      menuitem = 4;
      countdown = 0;
    }
  }

  if (menulevel == 51) {  //Set initial time
    display.set(e_time, 1);
    if (encdir != 0)
      e_time = menuitem;
    if (encbut == 1) {
      menulevel = 1;
      menuitem = 5;
      itemcount = 5;
    }
  }
  display.show();
}


void strip_expo() {  //to do!!!!
  encbut = 0;
  byte s = strip / 2;
  strip_expo_[s] = e_time;
  byte i = s;
  while (i != 0) {
    i--;
    strip_expo_[i] = strip_expo_[i + 1] * pow(2.0, -1.0 / f_stop);
  }
  i = s + 1;
  while (i < strip) {
    strip_expo_[i] = strip_expo_[i - 1] * pow(2.0, 1.0 / f_stop);
    i++;
  }
  /*for (i = 0; i < strip; i++)
    Serial.println(strip_expo_[i]);   //*/
  s = 1;
  while (s <= strip && rotary.pushType(1000) != 1) {

    temp_str = "S-" + String(s);
    display.show(temp_str, 250, ALIGN_LEFT);
    display.show("S-", 250, ALIGN_LEFT);
    if (pedal.pressed()) {
      if (countdown == 0)
        if (s == 1)
          countdown = strip_expo_[s - 1];
        else
          countdown = strip_expo_[s - 1] - strip_expo_[s - 2];
      lamp_on(true);
      tone(BUZZER_PIN, t0ne, 500);
      while (!pedal.pressed() && (countdown != 0)) {
        display.show(countdown, 100, 1);  // float with one decimal place, display 100ms
        countdown = countdown - 0.1;
        if (countdown < 0) {
          countdown = 0;
          s = s + 1;
        }
      }
      lamp_on(false);
      tone(BUZZER_PIN, t0ne, 500);
    }
  }
  countdown = 0;
}

void expo() {
  if (countdown == 0)
    countdown = e_time;
  lamp_on(true);
  tone(BUZZER_PIN, t0ne, 500);

  while (!pedal.pressed() && (countdown != 0)) {
    display.show(countdown, 100, 1);  // float with one decimal place, display 100ms
    countdown = countdown - 0.1;
    if (countdown < 0)
      countdown = 0;
  }
  lamp_on(false);
  tone(BUZZER_PIN, t0ne, 500);
}

void develop_timer() {
  if (countdown == 0)
    countdown = d_time;
  tone(BUZZER_PIN, t0ne, 500);

  while (!pedal.pressed() && (countdown != 0)) {
    display.show(countdown, 100, 1);  // float with one decimal place, display 100ms
    countdown = countdown - 0.1;
    if (countdown < 0)
      countdown = 0;
  }
  tone(BUZZER_PIN, t0ne, 500);
}

void lamp_on(bool turnon) {

  if (turnon)
    digitalWrite(LAMP_PIN, LOW);
  else
    digitalWrite(LAMP_PIN, HIGH);
}

byte binarySearch(byte array[], byte size, byte target) {
  int left = 0;
  int right = size - 1;

  while (left <= right) {
    int mid = left + (right - left) / 2;

    // Check if target is at mid
    if (array[mid] == target) {
      return mid;
    }

    // If target is greater, ignore left half
    if (array[mid] < target) {
      left = mid + 1;
    }
    // If target is smaller, ignore right half
    else {
      right = mid - 1;
    }
  }
  // Target is not present in array
  return -1;
}
