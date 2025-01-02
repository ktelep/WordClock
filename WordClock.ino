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
const uint8_t ledPAST[] = {181,180,179,178,170,171,172,173};
const uint8_t ledTO[] = {178,177,173,174};
const uint8_t ledOCLOCK[] = {40,41,42,43,44,45,46,49,50,51,52,53,54,55};
const uint8_t ledPLUS[] = {1,30};
const uint8_t ledMINUTES[] = {8,9,10,11,12,13,14,17,18,19,20,21,22,23,1,30};

// MINUTES
const uint8_t ledHALF[]= {233,234,235,236,243,244,245,246};
const uint8_t ledTOPTEN[] = {237,238,239,240,241,242};
const uint8_t ledTOPTWENTY[] = {223,222,221,220,219,218,192,193,194,195,196,197};
const uint8_t ledTOPFIVE[] = {217,216,215,214,198,199,200,201};
const uint8_t ledTOPQUARTER[] = {190,189,188,187,186,185,184,167,166,165,164,163,162,161};

// HOURS
const uint8_t ledONE[] = {59,58,57,36,37,38};
const uint8_t ledTWO[] = {155,154,153,132,133,134};
const uint8_t ledTHREE[] = {151,150,149,148,147,136,137,138,139,140};
const uint8_t ledFOUR[] = {115,114,113,112,111,110,109,108};
const uint8_t ledFIVE[] = {159,158,157,156,128,129,130,131};
const uint8_t ledSIX[] = {85,84,83,76,75,74};
const uint8_t ledSEVEN[] = {95,94,93,92,91,68,67,66,65,64};
const uint8_t ledEIGHT[] = {90,89,88,87,86,69,70,71,72,73};
const uint8_t ledNINE[] = {63,62,61,60,32,33,34,35};
const uint8_t ledTEN[] = {141,142,143,144,145,146};
const uint8_t ledELEVEN[] = {96,97,98,99,100,101,122,123,124,125,126,127};
const uint8_t ledTWELVE[] = {102,103,104,105,106,107,116,117,118,119,120,121};

// Minute Offset
const uint8_t offset_minutes[][2] = {
    {0,0},
    {3,28},
    {4,27},
    {5,26},
    {6,25}
};

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

    printIPAddress();

    // Connect to NTP and set our timezone
    configTime(0, 0, ntpServer);
    setenv("TZ",timezone.c_str(),1);
    tzset();
    
}

void addWord(const uint8_t word_to_add[], size_t word_size, int* cur_pos) {
    memcpy(timeleds+*cur_pos,word_to_add,word_size);
    *cur_pos += word_size;
}

void printIPAddress() {
  IPAddress myip = WiFi.localIP();
  char buff[50];
  int iplen = snprintf(buff, sizeof(buff), "%d.%d.%d.%d",myip[0],myip[1],myip[2],myip[3]);

  for (int i=0; i<iplen; i++) {
    int cur_pos = 0;
    memset(timeleds,0, ARRAY_SIZE(timeleds));
    fill_solid(leds,NUM_LEDS, 0);

    switch(buff[i]) {
    case 49:
      addWord(ledONE, ARRAY_SIZE(ledONE), &cur_pos);
      break;
    case 50:
      addWord(ledTWO, ARRAY_SIZE(ledTWO), &cur_pos);
      break;
    case 51:
      addWord(ledTHREE, ARRAY_SIZE(ledTHREE), &cur_pos);
      break;
    case 52:
      addWord(ledFOUR, ARRAY_SIZE(ledFOUR), &cur_pos);
      break;
    case 53:
      addWord(ledFIVE, ARRAY_SIZE(ledFIVE), &cur_pos);
      break;
    case 54:
      addWord(ledSIX, ARRAY_SIZE(ledSIX), &cur_pos);
      break;
    case 55:
      addWord(ledSEVEN, ARRAY_SIZE(ledSEVEN), &cur_pos);
      break;
    case 56:
      addWord(ledEIGHT, ARRAY_SIZE(ledEIGHT), &cur_pos);
      break;
    case 57:
      addWord(ledNINE, ARRAY_SIZE(ledNINE), &cur_pos);
      break;
    case 46:
      addWord(ledPLUS, ARRAY_SIZE(ledPLUS), &cur_pos);
      break;
    }

    timeleds[cur_pos] = 0;

    uint8_t j = 0;

    while(timeleds[j] > 0) {
      leds[timeleds[j]] = CHSV(123,230,255);
      j++;
      Serial.print(j);
      Serial.print(" - ");
      Serial.println(timeleds[j]);
    }

    FastLED.delay(1000);
  }
}

void printLocalTime() {
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }

  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

  // Clearout the timeleds array
  int cur_pos = 0;
  memset(timeleds,0, ARRAY_SIZE(timeleds));

  // Add our IT IS
  addWord(ledITIS, ARRAY_SIZE(ledITIS), &cur_pos);
  Serial.print("IT IS ");

  // Determine the minute information to display
  uint8_t ntp_minutes = timeinfo.tm_min;

  if ((ntp_minutes >= 30) && (ntp_minutes < 35)) {
    addWord(ledHALF, ARRAY_SIZE(ledHALF), &cur_pos);
    Serial.print("HALF");
  }

  if (((ntp_minutes >= 10) && (ntp_minutes < 15)) || ((ntp_minutes >=50) && (ntp_minutes <55))) {
    addWord(ledTOPTEN, ARRAY_SIZE(ledTOPTEN), &cur_pos);
    Serial.print("TEN");
  }

  if (((ntp_minutes >=20) && (ntp_minutes < 30)) || ((ntp_minutes >=35) && (ntp_minutes < 45))){
    addWord(ledTOPTWENTY, ARRAY_SIZE(ledTOPTWENTY),&cur_pos);
    Serial.print("TWENTY");
  }

  if (((ntp_minutes >= 5) && (ntp_minutes < 10)) || ((ntp_minutes >=25) && (ntp_minutes < 30)) ||
      ((ntp_minutes >= 35) && (ntp_minutes < 40)) || ((ntp_minutes >= 55))) {
        addWord(ledTOPFIVE, ARRAY_SIZE(ledTOPFIVE), &cur_pos);
        Serial.print("FIVE");
  } 

  if (((ntp_minutes >= 15) && (ntp_minutes < 20)) || ((ntp_minutes >=45) && (ntp_minutes < 50))) {
    addWord(ledTOPQUARTER, ARRAY_SIZE(ledTOPQUARTER), &cur_pos);
    Serial.print("QUARTER");
  }

  int shown_hour_offset = 0;   // If we flip to "TO" for the time, we need to show the next hour
  if (ntp_minutes > 4) {
    if (ntp_minutes >= 35) {
        addWord(ledTO, ARRAY_SIZE(ledTO), &cur_pos);
        Serial.print(" TO");
        shown_hour_offset = 1;

    } else {
        addWord(ledPAST, ARRAY_SIZE(ledPAST), &cur_pos);
        Serial.print(" PAST");
    }
  }

  // Map our hours

  uint8_t shown_hour = timeinfo.tm_hour % 12;  // Offset from 24 hours time
  if (shown_hour == 0)  { shown_hour = 12; } // Midnight is shown as 0, we want to show it as 12
  shown_hour += shown_hour_offset; // Add the hour offset it this is a "TO" time being spresented

  switch(shown_hour){
    case 1:
      addWord(ledONE, ARRAY_SIZE(ledONE), &cur_pos);
      Serial.print(" ONE");
      break;
    case 2:
      addWord(ledTWO, ARRAY_SIZE(ledTWO), &cur_pos);
      Serial.print(" TWO");
      break;
    case 3:
      addWord(ledTHREE, ARRAY_SIZE(ledTHREE), &cur_pos);
      Serial.print(" THREE");
      break;
    case 4:
      addWord(ledFOUR, ARRAY_SIZE(ledFOUR), &cur_pos);
      Serial.print(" FOUR");
      break;
    case 5:
      addWord(ledFIVE, ARRAY_SIZE(ledFIVE), &cur_pos);
      Serial.print(" FIVE");
      break;
    case 6:
      addWord(ledSIX, ARRAY_SIZE(ledSIX), &cur_pos);
      Serial.print(" SIX");
      break;
    case 7:
      addWord(ledSEVEN, ARRAY_SIZE(ledSEVEN), &cur_pos);
      Serial.print(" SEVEN");
      break;
    case 8:
      addWord(ledEIGHT, ARRAY_SIZE(ledEIGHT), &cur_pos);
      Serial.print(" EIGHT");
      break;
    case 9:
      addWord(ledNINE, ARRAY_SIZE(ledNINE), &cur_pos);
      Serial.print(" NINE");
      break;
    case 10:
      addWord(ledTEN, ARRAY_SIZE(ledTEN), &cur_pos);
      Serial.print(" TEN");
      break;
    case 11:
      addWord(ledELEVEN, ARRAY_SIZE(ledELEVEN), &cur_pos);
      Serial.print(" ELEVEN");
      break;
    case 12:
      addWord(ledTWELVE, ARRAY_SIZE(ledTWELVE), &cur_pos);
      Serial.print(" TWELVE");
      break;
  }

  // Add our O'Clock
  addWord(ledOCLOCK, ARRAY_SIZE(ledOCLOCK), &cur_pos);
  Serial.println(" O'CLOCK");
  
  // Figure out the additional offset (+1,2,3,4)
  int ntp_minutes_offset = ntp_minutes % 5;

  if (ntp_minutes_offset != 0) {
    addWord(ledMINUTES, ARRAY_SIZE(ledMINUTES), &cur_pos);
    Serial.print("+ ");

    addWord(offset_minutes[ntp_minutes_offset],2,&cur_pos);
    Serial.print(ntp_minutes_offset);
    Serial.print(" MINUTES");
  }

  timeleds[cur_pos] = 0;
  
}

void loop() {

    uint8_t hue = 224;
    fill_solid(leds,NUM_LEDS, 0);

    // Grab time and update the timeleds array    
    printLocalTime();

    // Set the HUE for the LEDS until we hit -1
    uint8_t j = 0;

    while(timeleds[j] > 0) {
      leds[timeleds[j]] = CHSV(hue,230,255);
      j++;
    }

    FastLED.delay(15000);
}