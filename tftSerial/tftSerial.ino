#include <ArduinoJson.h>

#define WAIT 2000 // Delay between tests, set to 0 to demo speed, 2000 to see what it does!
int incomingByte = 0;
#include <TFT_HX8357.h> // Hardware-specific library
#include <string.h>
TFT_HX8357 gTFT = TFT_HX8357();       // Invoke custom library

#define TFT_GREY 0x7BEF
#define TFT_WIN_BLUE 0x4da9ff//0x77DB
//StaticJsonBuffer<200> jsonBuffer;

#include "win10.h"
#include "mac.h"

void setup() {
  // put your setup code here, to run once:
  gTFT.init();
  gTFT.setRotation(1);
  writeCeremony();
  Serial.begin(115200); 
//  drawIcon(win10, 370, 210, 96,  96);
  gTFT.fillScreen(TFT_BLACK);
  gTFT.setTextColor(TFT_BLUE);
  gTFT.setTextColor(TFT_RED);
  drawIcon(win10, 370, 210, 96,  96);
  gTFT.drawString("Disconnected", 5, 290, 2);
  //gTFT.drawLine(5,285, 320, 285, TFT_RED);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
        // read the incoming byte:
        //incomingByte = Serial.read();

        // say what you got:
//        Serial.print("I received: ");
//        Serial.println(incomingByte, DEC);
        String myString = Serial.readString();//String(incomingByte);
        char ch[myString.length()];
        myString.toCharArray(ch, myString.length());
        Serial.print("I received: ");
        Serial.println(myString);
        DynamicJsonBuffer  jsonBuffer(320);
        JsonObject& root = jsonBuffer.parseObject(ch);

        const char* cpuName = root["cpuName"];
        const char* cpuSpeed = root["cpuSpeed"];
        const char* cpuUsage = root["cpuUsage"];
        const char* amountRam = root["amountRam"];
        const char* ramUse = root["ramUse"];
        const char* batterySpecs = root["batterySpecs"];
        const char* pcName = root["pcName"];
        const char* osName = root["osName"];
        boolean connectionStatus = root["connectionStatus"];
        const char* tm = root["time"];
        const char* dt = root["date"];
        const char* at = root["author"];

         if (!root.success()) {
          Serial.println("parseObject() failed");
          return;
        }

        gTFT.fillScreen(TFT_BLACK);

        if(connectionStatus){
          gTFT.setTextColor(TFT_GREEN);
  
          drawIcon(win10, 370, 210, 96,  96);
//          gTFT.drawString("Connected", 5, 290, 2);
  
          gTFT.setTextColor(TFT_BLUE);
          //Start WRiting the data!?
          gTFT.drawString("PC Name ", 5, 5, 4);
          gTFT.drawString(pcName, 180, 5, 4);
  
          gTFT.drawString("CPU Speed ", 5, 30, 4);
          gTFT.drawString(cpuSpeed, 180, 30, 4);
  
          gTFT.drawString("CPU % ", 5, 55, 4);
          gTFT.drawString(cpuUsage, 180, 55, 4);
  
          gTFT.drawString("Amount RAM ", 5, 80, 4);
          gTFT.drawString(amountRam, 180, 80, 4);
  
          gTFT.drawString("RAM % ", 5, 105, 4);
          gTFT.drawString(ramUse, 180, 105, 4);
  
          gTFT.drawString("Battery % ", 5, 130, 4);
          gTFT.drawString(batterySpecs, 180, 130, 4);
  
          gTFT.drawString("OS ", 5, 155, 4);
          gTFT.drawString(osName, 180, 155, 4);
          
          gTFT.drawLine(5,190, 320, 190, TFT_RED);
          
          gTFT.setTextColor(TFT_LIGHTGREY);
          gTFT.drawString("Date ", 5, 195, 4);
          gTFT.drawString(dt, 180, 195, 4);

          gTFT.drawString("Time ", 5, 220, 4);
          gTFT.drawString(tm, 180, 220, 4);

          gTFT.drawLine(5,245, 320, 245, TFT_RED);
//          myGLCD.drawLine(i, 117, i, 121,TFT_BLUE)
        }else{
          gTFT.fillScreen(TFT_BLACK);
          gTFT.setTextColor(TFT_BLUE);
          gTFT.setTextColor(TFT_RED);
          drawIcon(win10, 370, 210, 96,  96);
          gTFT.drawString("Disconnected", 5, 290, 2);
        }

//        gTFT.drawString("CPU Speed ", 15, 5, 4);
//        gTFT.drawString(cpuName, 15, 40, 4);
//        drawIcon(win10, 370, 210, 96,  96);
    }
    else{
      
    }
}

void writeCeremony(){
  gTFT.fillScreen(TFT_BLACK);
  gTFT.setTextColor(TFT_DARKGREEN);
}


#define BUFF_SIZE 128

// Draw array "icon" of defined width and height at coordinate x,y
// Maximum icon size is 255x255 pixels to avoid integer overflow

void drawIcon(const unsigned short* icon, int16_t x, int16_t y, int8_t width, int8_t height) {

  uint16_t  pix_buffer[BUFF_SIZE];   // Pixel buffer (16 bits per pixel)

  // Set up a window the right size to stream pixels into
  gTFT.setWindow(x, y, x + width - 1, y + height - 1);

  // Work out the number whole buffers to send
  uint16_t nb = ((uint16_t)height * width) / BUFF_SIZE;

  // Fill and send "nb" buffers to TFT
  for (int i = 0; i < nb; i++) {
    for (int j = 0; j < BUFF_SIZE; j++) {
      pix_buffer[j] = pgm_read_word(&icon[i * BUFF_SIZE + j]);
    }
    gTFT.pushColors(pix_buffer, BUFF_SIZE);
  }

  // Work out number of pixels not yet sent
  uint16_t np = ((uint16_t)height * width) % BUFF_SIZE;

  // Send any partial buffer left over
  if (np) {
    for (int i = 0; i < np; i++) pix_buffer[i] = pgm_read_word(&icon[nb * BUFF_SIZE + i]);
    gTFT.pushColors(pix_buffer, np);
  }
}

