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
const int colors = 1530;  // total number of colors in standard colorwheel (255 bits * 3 primary colors * 2 interactions per color)

// Dynamic Variables
String input;  // character read by serial comm
String sb;  // speed and brightness changes [+/- for faster/slower, b/d for brighter/darker]
String dir = "h";  // display direction [h/v for horizontal/vertical]
String mode;  // operation command, latched from input readings

// Mode Specific Variables
const uint8_t hot_range = ceil(255*.75);  // upper limit of sunset range
uint8_t cw_dir[strips][LEDs];  // sunset color direction
int acc[strips];  // aurora current color
int atc[strips];  // aurora target color
uint8_t acb[strips];  // aurora current brightness
uint8_t atb[strips];  // aurora target brightness
uint8_t lp;  // lightning position
uint8_t ls;  // lightning size
uint8_t lb;  // lightning brightness

// Iteration Variables
uint8_t reset;  // when changing modes, used to initialize new mode
uint8_t i;  // counter
uint8_t j;  // counter

// LED Controls
uint8_t cw_val[3];  // temporary storage for a single set of RGB values
uint8_t cw_speed = 5;  // color change speed
float bright = 0.5;  // master brightness, initially set to 50%
int cw_pos[strips][LEDs];  // current position within colorwheel
uint8_t R[strips][LEDs];  // Red value for each LED within each strip
uint8_t G[strips][LEDs];  // Green value for each LED within each strip
uint8_t B[strips][LEDs];  // Blue value for each LED within each strip

// Strip Initializations
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(LEDs, PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(LEDs, PIN2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(LEDs, PIN3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip4 = Adafruit_NeoPixel(LEDs, PIN4, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip5 = Adafruit_NeoPixel(LEDs, PIN5, NEO_GRB + NEO_KHZ800);

// Colorwheel reference (takes a 3-variable array and a position within the colorwheel, updates the array with RGB values for that position)
void cw(uint8_t c_temp[], int pos_temp){
    if (pos_temp <= 255){
      c_temp[0] = 255;
      c_temp[1] = pos_temp;
      c_temp[2] = 0;
    }
    else if (pos_temp <= 255*2){
      c_temp[0] = 255*2 - pos_temp;
      c_temp[1] = 255;
      c_temp[2] = 0;
    }
    else if (pos_temp <= 255*3){
      c_temp[0] = 0;
      c_temp[1] = 255;
      c_temp[2] = pos_temp - 255*2;
    }
    else if (pos_temp <= 255*4){
      c_temp[0] = 0;
      c_temp[1] = 255*4 - pos_temp;
      c_temp[2] = 255;
    }
    else if (pos_temp <= 255*5){
      c_temp[0] = pos_temp - 255*4;
      c_temp[1] = 0;
      c_temp[2] = 255;
    }
    else if (pos_temp <= 255*6){
      c_temp[0] = 255;
      c_temp[1] = 0;
      c_temp[2] = 255*6 - pos_temp;
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
  
  // Read User Inputs
  if (Serial.available()){
    input += char(Serial.read());
    delay(1);
  }
  else if (!Serial.available() && input != ""){ // once user is finished inputting
    if (input == "+" || input == "-" || input == "b" || input == "d"){  // brightness or speed change
      sb = input;
    }
    else if (input == "h" || input == "v"){  // direction change
      dir = input;
      reset = 1;
    }
    else if (input == "o" || input == "w" || input == "r" || input == "s" || input == "a" || input == "l"){  // mode change
      mode = input;
      reset = 1;
    }
    input = "";
  }
  
  // Slower
  if (sb == "-"){
    sb = "";
    if (cw_speed > 0){
      cw_speed--;
    }
    Serial.println(String(cw_speed));
  }
  // Faster
  else if (sb == "+"){
    sb = "";
    if (cw_speed < colors){
      cw_speed++;
    }
    Serial.println(String(cw_speed));
  }
  // Brighter
  else if (sb == "b"){
    sb = "";
    if (bright < 1){
      bright = bright + 0.1;
    }
    Serial.println(String(int(bright*10)));
  }
  // Darker
  else if (sb == "d"){
    sb = "";
    if (bright > 0){
      bright = bright - 0.1;
    }
    Serial.println(String(int(bright*10)));
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
  
  // White Light Mode
  if (mode == "w"){
    if (reset){
      reset = 0;
      for (i = 0; i < strips; i++){
        for (j = 0; j < LEDs; j++){
          R[i][j] = ceil(122*bright);
          G[i][j] = ceil(122*bright);
          B[i][j] = ceil(255*bright);
        }
      }
    }
  }
  
  // Rainbow Mode
  else if (mode == "r"){
    if (reset){
      reset = 0;
      for (i = 0; i < strips; i++){
        for (j = 0; j < LEDs; j++){
          if (dir == "h"){
            cw_pos[i][j] = i*100;
          }
          else if (dir == "v"){
            cw_pos[i][j] = j*10;
          }
          cw(cw_val,cw_pos[i][j]);
          R[i][j] = ceil(cw_val[0]*bright);
          G[i][j] = ceil(cw_val[1]*bright);
          B[i][j] = ceil(cw_val[2]*bright);
        }
      }
    }
    else{
      for (i = 0; i < strips; i++){
        for (j = 0; j < LEDs; j++){
          if (cw_pos[i][j] + cw_speed >= colors){
            cw_pos[i][j] = 0;
          }
          else{
            cw_pos[i][j] = cw_pos[i][j] + cw_speed;
          }
          cw(cw_val,cw_pos[i][j]);
          R[i][j] = ceil(cw_val[0]*bright);
          G[i][j] = ceil(cw_val[1]*bright);
          B[i][j] = ceil(cw_val[2]*bright);
        }
      }
    }
  }
  
  // Sunset Mode
  else if (mode == "s"){
    if (reset){
      reset = 0;
      for (i = 0; i < strips; i++){
        for (j = 0; j < LEDs; j++){
          
          if (dir == "h") cw_pos[i][j] = i*hot_range/strips;
          else if (dir == "v") cw_pos[i][j] = j*hot_range/LEDs;
          
          cw_dir[i][j] = 1;
          cw(cw_val,cw_pos[i][j]);
          R[i][j] = ceil(cw_val[0]*bright);
          G[i][j] = ceil(cw_val[1]*bright);
          B[i][j] = ceil(cw_val[2]*bright);
        }
      }
    }
    else{
      for (i = 0; i < strips; i++){
        for (j = 0; j < LEDs; j++){
          
          if (cw_pos[i][j] - cw_speed < 0 && !cw_dir[i][j]) cw_dir[i][j] = 1;
          else if (cw_pos[i][j] + cw_speed > hot_range && cw_dir[i][j]) cw_dir[i][j] = 0;
          else if (cw_dir[i][j]) cw_pos[i][j] = cw_pos[i][j] + cw_speed;
          else cw_pos[i][j] = cw_pos[i][j] - cw_speed;
          
          cw(cw_val,cw_pos[i][j]);
          R[i][j] = ceil(cw_val[0]*bright);
          G[i][j] = ceil(cw_val[1]*bright);
          B[i][j] = ceil(cw_val[2]*bright);
        }
      }
    }
  }
  
  // Aurora Borealis Mode
  else if (mode == "a"){
    delay(ceil(100/cw_speed));
    if (reset){
      reset = 0;
      for (i = 0; i < strips; i++){
        acc[i] = random(290+248*i,290+248*(i+1));
        atc[i] = acc[i];
        acb[i] = random(1,101);
        atb[i] = acb[i];
        cw(cw_val,acc[i]);
        for (j = 0; j < LEDs; j++){
          R[i][j] = ceil(cw_val[0]*acb[i]/100*bright);
          G[i][j] = ceil(cw_val[1]*acb[i]/100*bright);
          B[i][j] = ceil(cw_val[2]*acb[i]/100*bright);
        }
      }
    }
    else{
      for (i = 0; i < strips; i++){
        if (acc[i] == atc[i]) atc[i] = random(290+248*i,290+248*(i+1));
        else if (acc[i] < atc[i]) acc[i] = acc[i] + 1;
        else acc[i] = acc[i] - 1;
        
        if (atb[i] == acb[i]) atb[i] = random(1,101);
        else if (acb[i] < atb[i]) acb[i]++;
        else acb[i]--;
        
        cw(cw_val,acc[i]);
        for (j = 0; j < LEDs; j++){
          R[i][j] = ceil(cw_val[0]*acb[i]/100*bright);
          G[i][j] = ceil(cw_val[1]*acb[i]/100*bright);
          B[i][j] = ceil(cw_val[2]*acb[i]/100*bright);
        }
      }
    }
  }
  
  // Lightning Mode
  else if (mode == "l"){
    for (i = 0; i < strips; i++){
      lp = random(LEDs);
      ls = floor(random(LEDs/2));
      lb = random(256);
      for (j = 0; j < LEDs; j++){
        if (B[i][j] == 0 && ls > 5 && j > lp - floor(ls/2) && j < lp + floor(ls/2)){
          R[i][j] = 0;
          G[i][j] = 0;
          B[i][j] = ceil(lb*bright);
        }
        else{
          R[i][j] = 0;
          G[i][j] = 0;
          B[i][j] = max(B[i][j]-cw_speed,0);
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
