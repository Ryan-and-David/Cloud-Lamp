#include <Adafruit_NeoPixel.h>  // Arduino Neopixel library (used for timing LED commands)

// Pin Declarations
#define PIN1 2
#define PIN2 3
#define PIN3 4
#define PIN4 5
#define PIN5 6

// Static Variables
const uint8_t LEDs = 30;  // number of LEDs per strip
const uint8_t strips = 5;  // number of strips

// Dynamic Variables
String input;  // character read by serial comm
String mode;  // operation command, latched from input readings

// Flag Data (left to right, bottom to top)
uint8_t argentina[][3] = {{0,0,0},{117,170,219},{252,255,255},{117,170,219},{0,0,0}};
uint8_t bolivia[][3] = {{0,0,0},{0,121,52},{252,209,22},{213,43,30},{0,0,0}};
uint8_t costa_rica[][3] = {{0,0,255},{255,255,255},{255,0,0},{255,255,255},{0,0,255}};
uint8_t france[][3] = {{0,0,0},{0,85,164},{255,255,255},{239,65,53},{0,0,0}};
uint8_t ireland[][3] = {{0,0,0},{22,155,98},{255,255,255},{255,136,62},{0,0,0}};
uint8_t italy[][3] = {{0,0,0},{0,146,70},{241,242,241},{206,43,55},{0,0,0}};
uint8_t japan[][3] = {{0,0,0},{255,255,255},{255,0,0},{255,255,255},{0,0,0}};
uint8_t nigeria[][3] = {{0,0,0},{0,135,83},{255,255,255},{0,135,83},{0,0,0}};
uint8_t peru[][3] = {{0,0,0},{255,0,0},{255,255,255},{255,0,0},{0,0,0}};
uint8_t poland[][3] = {{0,0,0},{255,0,0},{255,0,0},{255,255,255},{255,255,255}};
uint8_t romania[][3] = {{0,0,0},{0,0,255},{255,255,0},{255,0,0},{0,0,0}};
uint8_t russia[][3] = {{0,0,0},{213,43,30},{0,57,166},{255,255,255},{0,0,0}};
uint8_t spain[][3] = {{0,0,0},{255,0,0},{255,255,0},{255,255,0},{255,0,0}};
uint8_t thailand[][3] = {{253,0,0},{255,255,255},{2,0,230},{255,255,255},{253,0,0}};
uint8_t ukraine[][3] = {{0,0,0},{255,255,0},{255,255,0},{0,0,255},{0,0,255}};

// Iteration Variables
uint8_t reset;  // when changing modes, used to initialize new mode
uint8_t i;  // counter
uint8_t j;  // counter

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
  
  // Read User Inputs
  if (Serial.available()){
    input += char(Serial.read());
    delay(1);
  }
  else if (!Serial.available() && input != ""){
    mode = input;
    reset = 1;
    input = "";
  }
  
  // Off Mode
  if (mode == "o"){
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
  
  else if (mode == "ar"){
    if (reset){
      reset = 0;
      for (i = 0; i < strips; i++){
        for (j = 0; j < LEDs; j++){
          R[i][j] = argentina[i][0];
          G[i][j] = argentina[i][1];
          B[i][j] = argentina[i][2];
        }
      }
    }
  }
  else if (mode == "bo"){
    if (reset){
      reset = 0;
      for (i = 0; i < strips; i++){
        for (j = 0; j < LEDs; j++){
          R[i][j] = bolivia[i][0];
          G[i][j] = bolivia[i][1];
          B[i][j] = bolivia[i][2];
        }
      }
    }
  }
  else if (mode == "cr"){
    if (reset){
      reset = 0;
      for (i = 0; i < strips; i++){
        for (j = 0; j < LEDs; j++){
          R[i][j] = costa_rica[i][0];
          G[i][j] = costa_rica[i][1];
          B[i][j] = costa_rica[i][2];
        }
      }
    }
  }
  else if (mode == "fr"){
    if (reset){
      reset = 0;
      for (i = 0; i < strips; i++){
        for (j = 0; j < LEDs; j++){
          R[i][j] = france[i][0];
          G[i][j] = france[i][1];
          B[i][j] = france[i][2];
        }
      }
    }
  }
  else if (mode == "ir"){
    if (reset){
      reset = 0;
      for (i = 0; i < strips; i++){
        for (j = 0; j < LEDs; j++){
          R[i][j] = ireland[i][0];
          G[i][j] = ireland[i][1];
          B[i][j] = ireland[i][2];
        }
      }
    }
  }
  else if (mode == "it"){
    if (reset){
      reset = 0;
      for (i = 0; i < strips; i++){
        for (j = 0; j < LEDs; j++){
          R[i][j] = italy[i][0];
          G[i][j] = italy[i][1];
          B[i][j] = italy[i][2];
        }
      }
    }
  }
  else if (mode == "ja"){
    if (reset){
      reset = 0;
      for (i = 0; i < strips; i++){
        for (j = 0; j < LEDs; j++){
          R[i][j] = japan[i][0];
          G[i][j] = japan[i][1];
          B[i][j] = japan[i][2];
        }
      }
    }
  }
  else if (mode == "ni"){
    if (reset){
      reset = 0;
      for (i = 0; i < strips; i++){
        for (j = 0; j < LEDs; j++){
          R[i][j] = nigeria[i][0];
          G[i][j] = nigeria[i][1];
          B[i][j] = nigeria[i][2];
        }
      }
    }
  }
  else if (mode == "pe"){
    if (reset){
      reset = 0;
      for (i = 0; i < strips; i++){
        for (j = 0; j < LEDs; j++){
          R[i][j] = peru[i][0];
          G[i][j] = peru[i][1];
          B[i][j] = peru[i][2];
        }
      }
    }
  }
  else if (mode == "po"){
    if (reset){
      reset = 0;
      for (i = 0; i < strips; i++){
        for (j = 0; j < LEDs; j++){
          R[i][j] = poland[i][0];
          G[i][j] = poland[i][1];
          B[i][j] = poland[i][2];
        }
      }
    }
  }
  else if (mode == "ro"){
    if (reset){
      reset = 0;
      for (i = 0; i < strips; i++){
        for (j = 0; j < LEDs; j++){
          R[i][j] = romania[i][0];
          G[i][j] = romania[i][1];
          B[i][j] = romania[i][2];
        }
      }
    }
  }
  else if (mode == "ru"){
    if (reset){
      reset = 0;
      for (i = 0; i < strips; i++){
        for (j = 0; j < LEDs; j++){
          R[i][j] = russia[i][0];
          G[i][j] = russia[i][1];
          B[i][j] = russia[i][2];
        }
      }
    }
  }
  else if (mode == "sp"){
    if (reset){
      reset = 0;
      for (i = 0; i < strips; i++){
        for (j = 0; j < LEDs; j++){
          R[i][j] = spain[i][0];
          G[i][j] = spain[i][1];
          B[i][j] = spain[i][2];
        }
      }
    }
  }
  else if (mode == "th"){
    if (reset){
      reset = 0;
      for (i = 0; i < strips; i++){
        for (j = 0; j < LEDs; j++){
          R[i][j] = thailand[i][0];
          G[i][j] = thailand[i][1];
          B[i][j] = thailand[i][2];
        }
      }
    }
  }
  else if (mode == "uk"){
    if (reset){
      reset = 0;
      for (i = 0; i < strips; i++){
        for (j = 0; j < LEDs; j++){
          R[i][j] = ukraine[i][0];
          G[i][j] = ukraine[i][1];
          B[i][j] = ukraine[i][2];
        }
      }
    }
  }
  
  // LED updates
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
