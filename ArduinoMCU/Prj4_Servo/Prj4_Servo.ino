/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

// Debug Flags
#define DEBUG_ALL 0
#define DEBUG_SERIAL 0
#define DEBUG_PROTOCOL 0

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

// Serial Comm
String inputString = " ";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup() {
  Serial.begin(115200);   // open serial connection to USB Serial port (connected to your computer)
  Serial1.begin(57600);   // open internal serial connection to MT7688AN
  
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  serialEvent();
  // myservo.write(90);
  // for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
  //   // in steps of 1 degree
  //   myservo.write(pos);              // tell servo to go to position in variable 'pos'
  //   delay(15);                       // waits 15ms for the servo to reach the position
  // }
  // for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
  //   myservo.write(pos);              // tell servo to go to position in variable 'pos'
  //   delay(15);                       // waits 15ms for the servo to reach the position
  // }
}


void serialEvent() {

  // Taget protocol:
  // M[X][DIR][PWM]D
  // [X]: motor ID
  // [DIR]: F/R for forward end reverse direction.
  // [PWM]: 0-255 for PWM value

  // digitalWrite(LED, HIGH);
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    // if (inChar == '\n') {
    if (inChar == 'D') {
      stringComplete = true;
    }
  }

  // while (Serial1.available()) {
  //   // get the new byte:
  //   char inChar = (char)Serial1.read();
  //   // add it to the inputString:
  //   inputString += inChar;
  //   // if the incoming character is a newline, set a flag
  //   // so the main loop can do something about it:
  //   // if (inChar == '\n') {
  //   if (inChar == 'D') {
  //     stringComplete = true;
  //   }
  // }

  if (stringComplete) {

    #if defined(DEBUG_SERIAL) && defined(DEBUG_ALL)
      Serial.println("");

      Serial.print("[SERIAL]");
      Serial.print("Get:");
      Serial.print(inputString);
      Serial.print(",LEN=");
      Serial.print(inputString.length());
      Serial.println(""); 
    #endif

    // Command decoder
    if (inputString.length()>7) {
      inputString = inputString.substring(inputString.length()-7,inputString.length());

      #if defined(DEBUG_SERIAL) && defined(DEBUG_ALL)
        Serial.print("[SERIAL]");
        Serial.print("inputString_Corrected=");
        Serial.print(inputString);  
        Serial.print(",LEN=");
        Serial.print(inputString.length());
        Serial.println(""); 
      #endif
    }

    #if defined(DEBUG_SERIAL) && defined(DEBUG_ALL)
      Serial.print("[SERIAL]");
      Serial.print("01:");Serial.print(inputString.substring(0,1)); Serial.print(". "); // M
      Serial.print("12:");Serial.print(inputString.substring(1,2)); Serial.print(". "); // X
      Serial.print("23:");Serial.print(inputString.substring(2,3)); Serial.print(". "); // F/R
      Serial.print("34:");Serial.print(inputString.substring(3,4)); Serial.print(". "); // PWM[0]
      Serial.print("45:");Serial.print(inputString.substring(4,5)); Serial.print(". "); // PWM[1]
      Serial.print("45:");Serial.print(inputString.substring(5,6)); Serial.print(". "); // PWM[2]
      Serial.print("67:");Serial.print(inputString.substring(6,7)); Serial.print(". "); // D
      Serial.println("");
    #endif


    if (inputString.startsWith("M")) {
      //switch
      if (inputString.endsWith("D")) {
        #if defined(DEBUG_SERIAL) && defined(DEBUG_ALL)
          Serial.println("[SERIAL] Valid protocol!");
        #endif

        // Decode protocol here
        motor_protocol_decoder();

      } else {
        #if defined(DEBUG_SERIAL) && defined(DEBUG_ALL)
          Serial.println("[SERIAL] Wrong Ending Byte!");
        #endif
      }
    } else if (inputString.startsWith("L")) {
      // LED/IO Mode
      if (inputString.endsWith("D")) {
        #if defined(DEBUG_SERIAL) && defined(DEBUG_ALL)
          Serial.println("[SERIAL] Valid protocol! LED Mode");
        #endif

        // Decode protocol here
        // led_protocol_decoder();

      } else {
        #if defined(DEBUG_SERIAL) && defined(DEBUG_ALL)
          Serial.println("[SERIAL] Wrong Ending Byte! LED Mode");
        #endif
      }
    } else {
      #if defined(DEBUG_SERIAL) && defined(DEBUG_ALL)
        Serial.println("[SERIAL] Wrong Starting Byte!");  
      #endif
    }

    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}


// Decode serial protocol
void motor_protocol_decoder(void) {
  // Taget protocol:
  // M[X][DIR][PWM]D
  // [X]: motor ID
  // [DIR]: F/R for forward end reverse direction.
  // [PWM]: 0-255 for PWM valuen.

  int proto_motor_id  = inputString.substring(1,2).toInt();
  int proto_motor_pwm = inputString.substring(3,6).toInt();
  int proto_motor_mode = -1;

  if (inputString.charAt(2) == 'F') { 
    proto_motor_mode = 1; 
    // send command to motor
    // driveMotor(proto_motor_id, proto_motor_mode, proto_motor_pwm);
  }
  else if (inputString.charAt(2) == 'R') {
    proto_motor_mode = 0;
    // send command to motor
    // driveMotor(proto_motor_id, proto_motor_mode, proto_motor_pwm); 
  }
  else if (inputString.charAt(2) == 'P') {
    //Posision Control Mode
    //positionMotor(proto_motor_id, proto_motor_mode, proto_motor_pwm);
    // updataTargetPosition(proto_motor_id, proto_motor_mode, proto_motor_pwm);
    myservo.write(proto_motor_pwm);

    #if defined(DEBUG_PROTOCOL) && defined(DEBUG_ALL)
      Serial.println("");  
      Serial.print("[PROTO]");  
      Serial.print("Update M");   
      Serial.print(proto_motor_id);   
      Serial.print("goal pos to ");   
      Serial.print(proto_motor_pwm);   
      // Serial.print(", M1_Goal = ");   
      // Serial.print(motor_1_goal_pos);   
      // Serial.print(", M2_Goal = ");   
      // Serial.print(motor_2_goal_pos);   
      Serial.println(".");  
    #endif
  } else {
    #if defined(DEBUG_PROTOCOL) && defined(DEBUG_ALL)
      Serial.println("");  
      Serial.print("[PROTO]");  
      Serial.print("Wrong Motor Mode");   
      Serial.println(".");  
    #endif
  }


  #if defined(DEBUG_PROTOCOL) && defined(DEBUG_ALL)
    Serial.println("");  
    Serial.print("[PROTO]");  
    Serial.print("motor_id = ");  
    Serial.print(proto_motor_id);  
    Serial.print(", motor_dir = ");  
    Serial.print(proto_motor_mode);  
    Serial.print(", motor_pwm = ");  
    Serial.print(proto_motor_pwm);  
    Serial.println(".");  

  #endif


}