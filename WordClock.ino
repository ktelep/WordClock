#include <FastLED.h>
#include <WiFiManager.h>
#include "time.h"

// NTP Info
const char* ntpServer = "0.pool.ntp.org";

// Time data
struct tm timeinfo;
String timezone ="EST5EDT,M3.2.0,M11.1.0";

// Number of items in strip
#define NUM_LEDS 256
#define DATA_PIN 16
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

// Text
const uint8_t ledITIS[] = {255,254,252,251,224,225,227,228};
const uint8_t ledITISs = ARRAY_SIZE(ledITIS);
const uint8_t ledPAST[] = {181,180,179,178,170,171,172,173};
const uint8_t ledPASTs = ARRAY_SIZE(ledPAST);
const uint8_t ledTO[] = {178,177,173,174};
const uint8_t ledTOs = ARRAY_SIZE(ledTO);
const uint8_t ledOCLOCK[] = {40,41,42,43,44,45,46,49,50,51,52,53,54,55};
const uint8_t ledOCLOCKs = ARRAY_SIZE(ledOCLOCK);

// MINUTES
const uint8_t ledHALF[]= {233,234,235,236,243,244,245,246};
const uint8_t ledHALFs = ARRAY_SIZE(ledHALF);
const uint8_t ledTOPTEN[] = {237,238,239,240,241,242};
const uint8_t ledTOPTENs = ARRAY_SIZE(ledTOPTEN);
const uint8_t ledTOPTWENTY[] = {223,222,221,220,219,218,192,193,194,195,196,197};
const uint8_t ledTOPTWENTYs = ARRAY_SIZE(ledTOPTWENTY);
const uint8_t ledTOPFIVE[] = {217,216,215,214,198,199,200,201};
const uint8_t ledTOPFIVEs = ARRAY_SIZE(ledTOPFIVE);
const uint8_t ledTOPQUARTER[] = {190,189,188,187,186,185,184,167,166,165,164,163,162,161};
const uint8_t ledTOPQUARTERs = ARRAY_SIZE(ledTOPQUARTER);

// HOURS
const uint8_t ledONE[] = {59,58,57,36,37,38};
const uint8_t ledONEs = ARRAY_SIZE(ledONE);
const uint8_t ledTWO[] = {155,154,153,132,133,134};
const uint8_t ledTWOs = ARRAY_SIZE(ledTWO);
const uint8_t ledTHREE[] = {151,150,149,148,147,136,137,138,139,140};
const uint8_t ledTHREEs = ARRAY_SIZE(ledTHREE);
const uint8_t ledFOUR[] = {115,114,113,112,111,110,109,108};
const uint8_t ledFOURs = ARRAY_SIZE(ledFOUR);
const uint8_t ledFIVE[] = {159,158,157,156,128,129,130,131};
const uint8_t ledFIVEs = ARRAY_SIZE(ledFIVE);
const uint8_t ledSIX[] = {85,84,83,76,75,74};
const uint8_t ledSIXs = ARRAY_SIZE(ledSIX);
const uint8_t ledSEVEN[] = {95,94,93,92,91,68,67,66,65,64};
const uint8_t ledSEVENs = ARRAY_SIZE(ledSEVEN);
const uint8_t ledEIGHT[] = {90,89,88,87,86,69,70,71,72,73};
const uint8_t ledEIGHTs = ARRAY_SIZE(ledEIGHT);
const uint8_t ledNINE[] = {63,62,61,60,32,33,34,35};
const uint8_t ledNINEs = ARRAY_SIZE(ledNINE);
const uint8_t ledTEN[] = {141,142,143,144,145,146};
const uint8_t ledTENs = ARRAY_SIZE(ledTEN);
const uint8_t ledELEVEN[] = {96,97,98,99,100,101,122,123,124,125,126,127};
const uint8_t ledELEVENs = ARRAY_SIZE(ledELEVEN);
const uint8_t ledTWELVE[] = {102,103,104,105,106,107,116,117,118,119,120,121};
const uint8_t ledTWELVEs = ARRAY_SIZE(ledTWELVE);

// Minute Offset
const uint8_t ledPLUS[] = {1,30};
const uint8_t ledPLUSs = ARRAY_SIZE(ledPLUS);
const uint8_t ledNUMONE[] = {3,28};
const uint8_t ledNUMONEs = ARRAY_SIZE(ledNUMONE);
const uint8_t ledNUMTWO[] = {4,27};
const uint8_t ledNUMTWOs = ARRAY_SIZE(ledNUMTWO);
const uint8_t ledNUMTHREE[] = {5,26};
const uint8_t ledNUMTHREEs = ARRAY_SIZE(ledNUMTHREE);
const uint8_t ledNUMFOUR[] = {6,25};
const uint8_t ledNUMFOURs = ARRAY_SIZE(ledNUMFOUR);
const uint8_t ledMINUTES[] = {8,9,10,11,12,13,14,17,18,19,20,21,22,23};
const uint8_t ledMINUTESs = ARRAY_SIZE(ledMINUTES);

uint8_t timeleds[256];

CRGBArray<NUM_LEDS> leds;

void setup() {
    delay(2000);
    FastLED.addLeds<WS2812B,DATA_PIN,RGB>(leds, NUM_LEDS);
    FastLED.setBrightness(70);
    FastLED.clear();

    fill_solid(leds,NUM_LEDS, 0);
    leds(250,247) = CRGB::DarkOrange;
    leds(229,232) = CRGB::DarkOrange;
    FastLED.show();

    Serial.begin(115200);

    WiFi.mode(WIFI_STA);
    WiFiManager wfm;

    bool res;

    // wfm.resetSettings();  // Uncomment for debugging WifiManager
    res = wfm.autoConnect("WordClock");

    if (!res) {
        Serial.println("Failed to connect");
    } 
    Serial.println("");
    Serial.println("WiFi connected.");

    // Connect to NTP and set our timezone
    configTime(0, 0, ntpServer);
    setenv("TZ",timezone.c_str(),1);
    tzset();
    
}

void printLocalTime(){
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }

  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

  // Clearout the timeleds array
  int cur_pos = 0;
  memset(timeleds,0, ARRAY_SIZE(timeleds));

  // Add our IT IS
  memcpy(timeleds+cur_pos,ledITIS,ledITISs);
  cur_pos += ledITISs;
  Serial.print("IT IS ");

  // Determine the minute information to display
  uint8_t ntp_minutes = timeinfo.tm_min;

  if ((ntp_minutes >= 30) && (ntp_minutes < 35)) {
    memcpy(timeleds+cur_pos,ledHALF,ledHALFs);
    cur_pos += ledHALFs;
    Serial.print("HALF");
  }

  if (((ntp_minutes >= 10) && (ntp_minutes < 15)) || ((ntp_minutes >=50) && (ntp_minutes <55))) {
    memcpy(timeleds+cur_pos,ledTOPTEN,ledTOPTENs);
    cur_pos += ledTOPTENs;
    Serial.print("TEN");
  }

  if (((ntp_minutes >=20) && (ntp_minutes < 30)) || ((ntp_minutes >=35) && (ntp_minutes < 45))){
    memcpy(timeleds+cur_pos,ledTOPTWENTY,ledTOPTWENTYs);
    cur_pos += ledTOPTWENTYs;
    Serial.print("TWENTY");
  }

  if (((ntp_minutes >= 5) && (ntp_minutes < 10)) || ((ntp_minutes >=25) && (ntp_minutes < 30)) ||
      ((ntp_minutes >= 35) && (ntp_minutes < 40)) || ((ntp_minutes >= 55))) {
        memcpy(timeleds+cur_pos,ledTOPFIVE,ledTOPFIVEs);
        cur_pos += ledTOPFIVEs;
        Serial.print("FIVE");
  } 

  if (((ntp_minutes >= 15) && (ntp_minutes < 20)) || ((ntp_minutes >=45) && (ntp_minutes < 50))) {
    memcpy(timeleds+cur_pos,ledTOPQUARTER,ledTOPQUARTERs);
    cur_pos += ledTOPQUARTERs;
    Serial.print("QUARTER");
  }

  int shown_hour_offset = 0;   // If we flip to "TO" for the time, we need to show the next hour
  if (ntp_minutes > 4) {
    if (ntp_minutes >= 35) {
        memcpy(timeleds+cur_pos, ledTO, ledTOs);
        cur_pos += ledTOs;
        Serial.print(" TO");
        shown_hour_offset = 1;

    } else {
        memcpy(timeleds+cur_pos, ledPAST, ledPASTs);
        cur_pos += ledPASTs;
        Serial.print(" PAST");
    }
  }

  // Map our hours

  uint8_t shown_hour = timeinfo.tm_hour % 12;  // Offset from 24 hours time
  if (shown_hour == 0)  { shown_hour = 12; } // Midnight is shown as 0, we want to show it as 12
  shown_hour += shown_hour_offset; // Add the hour offset it this is a "TO" time being spresented

  switch(shown_hour){
    case 1:
      memcpy(timeleds+cur_pos,ledONE,ledONEs);
      cur_pos+=ledONEs;
      Serial.print(" ONE");
      break;
    case 2:
      memcpy(timeleds+cur_pos,ledTWO,ledTWOs);
      cur_pos+=ledTWOs;
      Serial.print(" TWO");
      break;
    case 3:
      memcpy(timeleds+cur_pos,ledTHREE,ledTHREEs);
      cur_pos+=ledTHREEs;
      Serial.print(" THREE");
      break;
    case 4:
      memcpy(timeleds+cur_pos,ledFOUR,ledFOURs);
      cur_pos+=ledFOURs;
      Serial.print(" FOUR");
      break;
    case 5:
      memcpy(timeleds+cur_pos,ledFIVE,ledFIVEs);
      cur_pos+=ledFIVEs;
      Serial.print(" FIVE");
      break;
    case 6:
      memcpy(timeleds+cur_pos,ledSIX,ledSIXs);
      cur_pos+=ledSIXs;
      Serial.print(" SIX");
      break;
    case 7:
      memcpy(timeleds+cur_pos,ledSEVEN,ledSEVENs);
      cur_pos+=ledSEVENs;
      Serial.print(" SEVEN");
      break;
    case 8:
      memcpy(timeleds+cur_pos,ledEIGHT,ledEIGHTs);
      cur_pos+=ledEIGHTs;
      Serial.print(" EIGHT");
      break;
    case 9:
      memcpy(timeleds+cur_pos,ledNINE,ledNINEs);
      cur_pos+=ledNINEs;
      Serial.print(" NINE");
      break;
    case 10:
      memcpy(timeleds+cur_pos,ledTEN,ledTENs);
      cur_pos+=ledTENs;
      Serial.print(" TEN");
      break;
    case 11:
      memcpy(timeleds+cur_pos,ledELEVEN,ledELEVENs);
      cur_pos+=ledELEVENs;
      Serial.print(" ELEVEN");
      break;
    case 12:
      memcpy(timeleds+cur_pos,ledTWELVE,ledTWELVEs);
      cur_pos+=ledTWELVEs;
      Serial.print(" TWELVE");
      break;
  }

  // Add our O'Clock
  memcpy(timeleds+cur_pos,ledOCLOCK,ledOCLOCKs);
  cur_pos+=ledOCLOCKs; 
  Serial.println(" O'CLOCK");
  timeleds[cur_pos] = -1;
  
  // Figure out the additional offset (+1,2,3,4)
  int ntp_minutes_offset = ntp_minutes % 5;

  if (ntp_minutes_offset != 0) {
    memcpy(timeleds+cur_pos, ledPLUS, ledPLUSs);
    cur_pos += ledPLUSs;
    Serial.print("+");
    memcpy(timeleds+cur_pos,ledMINUTES, ledMINUTESs);
    cur_pos += ledMINUTESs;
  }
  switch (ntp_minutes_offset)
  {
  case 1:
    memcpy(timeleds+cur_pos, ledNUMONE, ledNUMONEs);
    cur_pos+=ledNUMONEs;
    Serial.println("1 MINUTES");
    break;
  case 2:
    memcpy(timeleds+cur_pos, ledNUMTWO, ledNUMTWOs);
    cur_pos+=ledNUMTWOs;
    Serial.println("2 MINUTES");
    break;
  case 3:
    memcpy(timeleds+cur_pos, ledNUMTHREE, ledNUMTHREEs);
    cur_pos+=ledNUMTHREEs;
    Serial.println("3 MINUTES");
    break;
  case 4:
    memcpy(timeleds+cur_pos, ledNUMFOUR, ledNUMFOURs);
    cur_pos+=ledNUMFOURs;
    Serial.println("4 MINUTES");
    break;
  }
  
}

void loop() {

    uint8_t hue = 224;

    fill_solid(leds,NUM_LEDS, 0);

    // Grab time and update the timeleds array    
    printLocalTime();

    // Set the HUE for the LEDS until we hit -1
    for (uint8_t j=0; timeleds[j] > 0; j++) {
        leds[timeleds[j]] = CHSV(hue,230,255);
    }

    FastLED.delay(15000);
    
    
}