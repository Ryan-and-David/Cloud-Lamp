#include <Adafruit_NeoPixel.h>  // Arduino Neopixel library (used for timing LED commands)

// Pin Declarations
#define PIN1 2
#define PIN2 3
#define PIN3 4
#define PIN4 5
#define PIN5 6

// Static Variables
const int LEDs = 30;  // number of LEDs per strip
const int strips = 5;  // number of strips

// Dynamic Variables
String input;  // character read by serial comm
String mode;  // operation command, latched from input readings
uint8_t reset;  // when changing modes, used to initialize new mode
uint8_t i;  // counter
uint8_t j;  // counter
uint8_t p;  // matrix position
long s_time;  // start time
long e_time;  // elapsed time

float bright[5] = {1,1,1,1,1};  // initial brightness settings for each column
float min_bright = .5;  // minimum brightness
float decay = .05;  // brightness decay rate per update cycle

// Timed sequence, 2-position array: (duration from previous command in ms, column to activate)
int rm[][2] = {
{0,1},
{120,4},
{280,3},
{240,5},
{360,2},
{40,3},
{160,4},
{320,1},
{200,2},
{280,5},
{200,3},
{600,1},
{80,4},
{160,3},
{160,5},
{240,1},
{200,4},
{160,2},
{160,1},
{160,4},
{200,2},
{160,3},
{240,5},
{80,1},
{160,4},
{240,5},
{80,3},
{200,2},
{520,1},
{320,3},
{720,6},
{1120,0},
{680,1},
{160,4},
{160,2},
{240,5},
{160,3},
{600,2},
{40,3},
{320,4},
{200,1},
{160,5},
{160,2},
{160,3},
{200,4},
{240,5},
{160,1}
};

// LED Controls
uint8_t R[strips][LEDs];  // Red value for each LED within each strip
uint8_t G[strips][LEDs];  // Green value for each LED within each strip
uint8_t B[strips][LEDs];  // Blue value for each LED within each strip

// Strip Initializations
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(LEDs, PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(LEDs, PIN2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(LEDs, PIN3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip4 = Adafruit_NeoPixel(LEDs, PIN4, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip5 = Adafruit_NeoPixel(LEDs, PIN5, NEO_GRB + NEO_KHZ800);

void voice(uint8_t active){
  for (i = 0; i < strips; i++) {
    for (j = 0; j < LEDs; j++){
  
      uint8_t sector = check(i,j);
      
      if (sector == 0){
        if (active == 6){
          uint8_t rand = random(10);
          R[i][j] = 0;
          G[i][j] = floor(255/rand);
          B[i][j] = floor(255/rand);
        }
        else{
          R[i][j] = 0;
          G[i][j] = 25;
          B[i][j] = 25;
        }
      }
      else if (sector == 1){
        if (active == 1){
          bright[i] = 1;
        }
        else{
          bright[i] = max(bright[i] - decay,min_bright);
        }
        R[i][j] = floor(128*bright[i]);
        G[i][j] = 0;
        B[i][j] = floor(255*bright[i]);
      }
      else if (sector == 2){
        if (active == 2){
          bright[i] = 1;
        }
        else{
          bright[i] = max(bright[i] - decay,min_bright);
        }
        R[i][j] = 0;
        G[i][j] = 0;
        B[i][j] = floor(255*bright[i]);
      }
      else if (sector == 3){
        if (active == 3){
          bright[i] = 1;
        }
        else{
          bright[i] = max(bright[i] - decay,min_bright);
        }
        R[i][j] = 0;
        G[i][j] = floor(255*bright[i]);
        B[i][j] = 0;
      }
      else if (sector == 4){
        if (active == 4){
          bright[i] = 1;
        }
        else{
          bright[i] = max(bright[i] - decay,min_bright);
        }
        R[i][j] = floor(255*bright[i]);
        G[i][j] = 0;
        B[i][j] = floor(255*bright[i]);
      }
      else if (sector == 5){
        if (active == 5){
          bright[i] = 1;
        }
        else{
          bright[i] = max(bright[i] - decay,min_bright);
        }
        R[i][j] = floor(255*bright[i]);
        G[i][j] = floor(255*bright[i]);
        B[i][j] = 0;
      }
    }
  }
}

uint8_t check(uint8_t row, uint8_t col){
  if (row == 1 && col > 23) {
    return 1;
  }
  else if (row == 0 && col < 8) {
    return 2;
  }
  else if (row == 2 && (col > 10 && col < 18)) {
    return 3;
  }
  else if (row == 4 && col > 23) {
    return 4;
  }
  else if (row == 3 && col < 8) {
    return 5;
  }
  else{
    return 0;
  }
}
  
void setup() {
  
  // Initialize LED strips
  strip1.begin();
  strip1.show();
  strip2.begin();
  strip2.show();
  strip3.begin();
  strip3.show();
  strip4.begin();
  strip4.show();
  strip5.begin();
  strip5.show();
  
  // Initialize serial communication, wait until ready
  Serial.begin(115200);
  while(!Serial);
  Serial.println("ready");
}

void loop(){
  
  if (Serial.available()){
    input += char(Serial.read());
    delay(1);
  }
  else if (!Serial.available() && input != ""){
    Serial.println(input);
    mode = input;
    input = "";
    reset = 1;
  }
  
  else if (mode == "o"){
    if (reset){
      reset = 0;
      for (i = 0; i < strips; i++){
        for (j = 0; j < LEDs; j++){
          R[i][j] = 0;
          G[i][j] = 0;
          B[i][j] = 0;
        }
      }
    }
  }
  if (mode == "r"){  // Rick and Morty mode
    if (reset){
      reset = 0;
      p = 0;
      for (i = 0; i < strips; i++) {
        for (j = 0; j < LEDs; j++){
          voice(0);
        }
      }
      s_time = millis();
    }
    else{
      
      e_time = millis() - s_time;
      
      if (p < sizeof(rm)/sizeof(rm[0]) && (e_time >= rm[p][0])){
        voice(rm[p][1]);
        s_time = millis();
        p++;
      }
      else if (rm[p][1] == 6){
        voice(6);
      }
      else{
        voice(0);
      }
    }
  }
  
  for (i = 0; i < strips; i++) {
    for (j = 0; j < LEDs; j++){
      if (i == 0){
        strip1.setPixelColor(j,R[i][j], G[i][j], B[i][j]);
      }
      else if (i == 1){
        strip2.setPixelColor(j,R[i][j], G[i][j], B[i][j]);
      }
      else if (i == 2){
        strip3.setPixelColor(j,R[i][j], G[i][j], B[i][j]);
      }
      else if (i == 3){
        strip4.setPixelColor(j,R[i][j], G[i][j], B[i][j]);
      }
      else if (i == 4){
        strip5.setPixelColor(j,R[i][j], G[i][j], B[i][j]);
      }
    }
    strip1.show();
    strip2.show();
    strip3.show();
    strip4.show();
    strip5.show();
  }

}
