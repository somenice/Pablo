#include <SoftwareSerial.h>
#include <Servo.h> 
#include <Wire.h>

Servo jaw;
int opened = 140;
int closed = 185;

Servo yaw;
int straight = 90;
int quized = 110; //clockwise


String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete


#define rxPin 5    // Serial input (connects to Emic 2 SOUT)
#define txPin 6    // Serial output (connects to Emic 2 SIN)
#define ledPin 13  // Most Arduino boards have an on-board LED on this pin


#define I2C_SLAVE_ADDR  0x26            // i2c slave address (38)

// set up a new serial port
SoftwareSerial emicSerial =  SoftwareSerial(rxPin, txPin);

byte r,g,b = 0;
String redColor = "";
String greenColor = "";
String blueColor = "";

void setup()  // Set up code called once on start-up
{
  // define pin modes
  pinMode(ledPin, OUTPUT);
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  Serial.begin(19200);

  // set the data rate for the SoftwareSerial port
  emicSerial.begin(9600);

  Wire.begin();
  digitalWrite(ledPin, LOW);  // turn LED off

  jaw.attach(9);
  yaw.attach(10);
  /*
    When the Emic 2 powers on, it takes about 3 seconds for it to successfully
   intialize. It then sends a ":" character to indicate it's ready to accept
   commands. If the Emic 2 is already initialized, a CR will also cause it
   to send a ":"
   */
  emicSerial.print("\n");             // Send a CR in case the system is already up
  while (emicSerial.read() != ':');   // When the Emic 2 has initialized and is ready, it will send a single ':' character, so wait here until we receive it
  delay(10);                     // Short delay

  emicSerial.flush();                 // Flush the receive buffer
  

  emicSerial.print("P0"); // DECTalk processor
  //emicSerial.print('P1'); // Epson processor
  emicSerial.print("\n");
  emicSerial.print("N0"); // Voice 0: Perfect Paul (Paulo) 1: Huge Harry (Francisco) 2: Beautiful Betty 3: Uppity Ursula 4: Doctor Dennis (Enrique) 5: Kit the Kid 6: Frail Frank 7: Rough Rita 8: Whispering Wendy (Beatriz)
  emicSerial.print("\n");
  emicSerial.print("W245"); // Words per
  emicSerial.print("\n");
  //  emicSerial.print("V5"); //volume
 
  
  jaw.write(opened);
  delay(500);
  
  emicSerial.print('S');
//  emicSerial.print("[:rate 110][:dv pr 220 hs 110 ap 90] ALL SYStems FUNctional.");
  emicSerial.print("ALL systems functional.");
  emicSerial.print("\n");
  
  jaw.write(closed);
  delay(1000);
  
  yaw.write(quized);
  delay(1000);
  yaw.write(straight);
  delay(1000);
  
  Serial.println("Finished setup");
}

void loop() {

  if (stringComplete) {
    Serial.println(inputString);
    //    emicSerial.print('W100\n');
    if (inputString.startsWith("[:rgb")) { 
      int rgb = inputString.indexOf('b');
      redColor = inputString.substring(rgb + 1, rgb + 3);
      greenColor = inputString.substring(rgb + 3, rgb + 5);
      blueColor = inputString.substring(rgb + 5, rgb + 7);

      eyes(inputString);
      inputString = "";
      stringComplete = false;
      return;
    }
    if (inputString.startsWith("[:jaw0")) { 
      jaw.write(closed);
      stringComplete = false;
      return;
    }
    if (inputString.startsWith("[:jaw1")) { 
      jaw.write(opened);
      stringComplete = false;
      return;
    }
    if (inputString.startsWith("dunno")) { 
      yaw.write(quized);
      delay(1000);
      
      
    }
    jaw.write(opened);
    emicSerial.print('S');
    emicSerial.print(inputString);
    emicSerial.print('\n');
    delay(1000);
    
    while (emicSerial.read() != ':');
    jaw.write(closed);
    yaw.write(straight);
    delay(500);
    // clear the string:
    inputString = "";
    stringComplete = false;
    //    Serial.println("stringComplete");
  }
  delay(200);
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:

    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    
    if (inChar == '\n') {
      stringComplete = true;
    } 
  }
}
void eyes(String colour) { // Takes String R G B and writes bytes over I2C
  r = redColor.toInt();
  g = greenColor.toInt();
  b = blueColor.toInt();
  //    r = random(0, 30);
  //    g = random(0, 30);
  //    b = random(0, 30);
  Wire.beginTransmission(I2C_SLAVE_ADDR); // transmit to device #44 (0x2c)
  // device address is specified in datasheet
  Wire.write(r);  // sends value byte  
  Wire.write(g);
  Wire.write(b);
  Wire.endTransmission();     // stop transmitting
  Serial.print(r);
  Serial.print(" ");
  Serial.print(g);
  Serial.print(" ");
  Serial.println(b);
  //  x++;        // increment value  
  //  if(x == 64) // if reached 64th position (max)
  //  {
  //    x = 0;    // start over from lowest value
  //    Serial.println(x, HEX);
  //  }
  //  delay(500);
  Serial.println("talking");
  delay(100);
 
}
