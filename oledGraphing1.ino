#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int values[100];          //The array used for storing points on the graph
double maxVal = 1;        //The maximum value (to be updated later on)


void setup(){
  Serial.begin(9600);
  while(!Serial);
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
	//Set up text display
	display.setTextSize(1);
	display.setTextColor(SSD1306_WHITE);
  delay(1000);
  Serial.println("Beginning Program");
}

void addValue(int newVal){
  //Set the maximum to zero
  int currentMax = 0;
  for(int i = 1; i < 100;i++){
    //move all array items down 1
    values[i-1] = values[i];
    if(abs(values[i]) > currentMax){
      //Find the maximum value
      currentMax = values[i];
    }
  }
  //Set maximum value
  maxVal = currentMax;
  //Set newest last value
  values[99] = newVal;
    
    
}
void drawScreen(){
  //display.drawLine(12,32,114,32,1);
  //Variable for controlling how big dash lines are
  int dashNumber = 0;

  for(int i = 0; i < 100; i++){
    dashNumber += 1;
    //Draws each item in the values array (resizing it by the max value to ensure the graph is always on screen)
    display.drawPixel(i,32-(values[i]*(25/maxVal)),1);
    //Draws graph line
    if (dashNumber < 5){
      display.drawPixel(i,32,1);
    }
    //Resets graph line number to continuously make lines.
    if (dashNumber >= 10){
      dashNumber = 0;
    }
  }
  //Show max value.
  display.setCursor(103,7);
  display.print(maxVal);
  display.display();
}

void loop(){
  display.clearDisplay();
  addValue(function());
  drawScreen();
  delay(50);
}

double function(){
  //Read input from a0
  int x = analogRead(A0);
  return(x);
}
