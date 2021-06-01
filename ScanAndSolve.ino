/***************************************************
  Vivin Kumar Loganathan | KIKFUC-17 | Class of 2021
  Bachelor Thesis - Vilnius Gediminas Technical University
  Faculty of Electronics :: Department of Computer Engineering
  Rubik's Cube Solver Robot
****************************************************/
//Necessary Libraries
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Pixy2.h>
#include <stdio.h>
//Declaring Variables
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
Pixy2 pixy;
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates
int x[3] = {130, 180, 230}; // X axis for the Pixy
int y[3] = {45, 95, 140}; // Y axis for the Pixy
int i = 0, j = 0; //Loop variables
int count = 0;// counter
char yellow_side[9]; //Down
char white_side[9]; //Up
char blue_side[9]; //Back
char red_side[9]; //Right
char green_side[9]; //Front
char orange_side[9]; //Left
//Servo pins
int servoRight = 12;
int servoLeft = 15;
int servoUp = 6;
int servoDown = 9;
int servoFront = 0;
int servoBack = 3;
char getColor[9];//Temporarily stores the colors detected by the Pixy
String cube;// Cube String gotten from each side of the cube
String solution;// Final solution gotten from the algorithm
String servoMove[30] = "";//stores the solution as array

void setup() {
  Serial.begin(115200); //Baud Rate of 115200
  Serial.print("Starting...\n");
  pixy.init(); // Start Pixy
  pixy.changeProg("video"); // change Pixy to Video mode
  //Setting up PCA9685
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);
  // set all servos to rest
  setServoPulse(servoLeft, 0.0015);
  setServoPulse(servoRight, 0.0015);
  setServoPulse(servoUp, 0.0015);
  setServoPulse(servoDown, 0.0015);
  setServoPulse(servoFront, 0.0015);
  setServoPulse(servoBack, 0.0015);
  delay(3000);// Wait 3 seconds
  //Program Start
  //Scan white side
  Serial.println("Scanning White side...");
  delay(5000);
  ReadCube();
  for (int a = 0; a < 9; a++) {
    white_side[a] = getColor[a];
    white_side[4] = 'U';
    Serial.print(white_side[a]);
    Serial.print(" ");
  }
  
  Serial.println();
  //Scan Red side
  Serial.println("Scanning Red side...");
  delay(5000);
  ReadCube();
  for (int a = 0; a < 9; a++) {
    red_side[a] = getColor[a];
    red_side[4] = 'R';
    Serial.print(red_side[a]);
    Serial.print(" ");
  }
  
  Serial.println();
  //Scan Green side
  Serial.println("Scanning Green side...");
  delay(5000);
  ReadCube();
  for (int a = 0; a < 9; a++) {
    green_side[a] = getColor[a];
    green_side[4] = 'F';
    Serial.print(green_side[a]);
    Serial.print(" ");
  }
  
  Serial.println();
  //Scan Yellow side
  Serial.println("Scanning Yellow side...");
  delay(5000);
  ReadCube();
  for (int a = 0; a < 9; a++) {
    yellow_side[a] = getColor[a];
    yellow_side[4] = 'D';
    Serial.print(yellow_side[a]);
    Serial.print(" ");
  }
  
  Serial.println();
  //Scan Orange side
  Serial.println("Scanning Orange side...");
  delay(5000);
  ReadCube();
  for (int a = 0; a < 9; a++) {
    orange_side[a] = getColor[a];
    orange_side[4] = 'L';
    Serial.print(orange_side[a]);
    Serial.print(" ");
  }
  
  Serial.println();
  //Scan Blue side
  Serial.println("Scanning Blue side...");
  delay(5000);
  ReadCube();
  for (int a = 0; a < 9; a++) {
    blue_side[a] = getColor[a];
    blue_side[4] = 'B';
    Serial.print(blue_side[a]);
    Serial.print(" ");
  }
  
  Serial.println();
  //End Scannig phase
  getCubeDefinitionString(); //Concat all the cube colors in to a single string
  Serial.print(cube);//print cube string
  delay(1000);
  getSolution();//Send the cube string to python for processing

  if (solution != "") {
    splitSolution();// split string to array
  }
  delay(2000);
}

void setServoPulse(uint8_t n, double pulse) {
  double pulselength;
  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= SERVO_FREQ;   // Analog servos run at ~60 Hz updates
  pulselength /= 4096;  // 12 bits of resolution
  pulse *= 1000000;  // convert input seconds to us
  pulse /= pulselength;
  pwm.setPWM(n, 0, pulse);
}

void robot() {
  for (int i = 0 ; i < 29; i++) {
    if ((servoMove[i]) == "U") {
      Serial.println((servoMove[i]));
      setServoPulse(servoUp, 0.00187);//20
      delay(125);
      setServoPulse(servoUp, 0.0015);
      delay(1000);
    }
    else if ((servoMove[i]) == "U'") {
      Serial.println((servoMove[i]));
      setServoPulse(servoUp, 0.0009);
      delay(100);
      setServoPulse(servoUp, 0.0015);
      delay(1000);
    }
    else if ((servoMove[i]) == "U2") {
      Serial.println((servoMove[i]));
      setServoPulse(servoUp, 0.00187);
      delay(125);
      setServoPulse(servoUp, 0.0015);
      delay(1000);
      setServoPulse(servoUp, 0.00187);
      delay(125);
      setServoPulse(servoUp, 0.0015);
      delay(1000);
    }
    else if ((servoMove[i]) == "U2'") {
      Serial.println((servoMove[i]));
      setServoPulse(servoUp, 0.0009);
      delay(100);
      setServoPulse(servoUp, 0.0015);
      delay(1000);
      setServoPulse(servoUp, 0.0009);
      delay(100);
      setServoPulse(servoUp, 0.0015);
      delay(1000);
    }
    //2
    else if ((servoMove[i]) == "F") {
      Serial.println((servoMove[i]));
      setServoPulse(servoFront, 0.00187);
      delay(125);
      setServoPulse(servoFront, 0.0015);
      delay(1000);
    }
    else if ((servoMove[i]) == "F'") {
      Serial.println((servoMove[i]));
      setServoPulse(servoFront, 0.0009);
      delay(100);
      setServoPulse(servoFront, 0.0015);
      delay(1000);
    }
    else if ((servoMove[i]) == "F2") {
      Serial.println((servoMove[i]));
      setServoPulse(servoFront, 0.00187);
      delay(125);
      setServoPulse(servoFront, 0.0015);
      delay(1000);
      setServoPulse(servoFront, 0.00187);
      delay(125);
      setServoPulse(servoFront, 0.0015);
      delay(1000);
    }
    else if ((servoMove[i]) == "F2'") {
      Serial.println((servoMove[i]));
      setServoPulse(servoFront, 0.0009);
      delay(100);
      setServoPulse(servoFront, 0.0015);
      delay(1000);
      setServoPulse(servoFront, 0.0009);
      delay(100);
      setServoPulse(servoFront, 0.0015);
      delay(1000);
    }
    //3
    else if ((servoMove[i]) == "B") {
      Serial.println((servoMove[i]));
      setServoPulse(servoBack, 0.00187);
      delay(125);
      setServoPulse(servoBack, 0.0015);
      delay(1000);
    }
    else if ((servoMove[i]) == "B'") {
      Serial.println((servoMove[i]));
      setServoPulse(servoBack, 0.0009);
      delay(100);
      setServoPulse(servoBack, 0.0015);
      delay(1000);
    }
    else if ((servoMove[i]) == "B2") {
      Serial.println((servoMove[i]));
      setServoPulse(servoBack, 0.00187);
      delay(125);
      setServoPulse(servoBack, 0.0015);
      delay(1000);
      setServoPulse(servoBack, 0.00187);
      delay(125);
      setServoPulse(servoBack, 0.0015);
      delay(1000);
    }
    else if ((servoMove[i]) == "B2'") {
      Serial.println((servoMove[i]));
      setServoPulse(servoBack, 0.0009);
      delay(100);
      setServoPulse(servoBack, 0.0015);
      delay(1000);
      setServoPulse(servoBack, 0.0009);
      delay(100);
      setServoPulse(servoBack, 0.0015);
      delay(1000);
    }
    //4
    else if ((servoMove[i]) == "R") {
      Serial.println((servoMove[i]));
      setServoPulse(servoRight, 0.00187);
      delay(125);
      setServoPulse(servoRight, 0.0015);
      delay(1000);
    }
    else if ((servoMove[i]) == "R'") {
      Serial.println((servoMove[i]));
      setServoPulse(servoRight, 0.0009);
      delay(100);
      setServoPulse(servoRight, 0.0015);
      delay(1000);
    }
    else if ((servoMove[i]) == "R2") {
      Serial.println((servoMove[i]));
      setServoPulse(servoRight, 0.00187);
      delay(125);
      setServoPulse(servoRight, 0.0015);
      delay(1000);
      setServoPulse(servoRight, 0.00187);
      delay(125);
      setServoPulse(servoRight, 0.0015);
      delay(1000);
    }
    else if ((servoMove[i]) == "R2'") {
      Serial.println((servoMove[i]));
      setServoPulse(servoRight, 0.0009);
      delay(100);
      setServoPulse(servoRight, 0.0015);
      delay(1000);
      setServoPulse(servoRight, 0.0009);
      delay(100);
      setServoPulse(servoRight, 0.0015);
      delay(1000);
    }
    //5
    else if ((servoMove[i]) == "L") {
      Serial.println((servoMove[i]));
      setServoPulse(servoLeft, 0.00187);
      delay(125);
      setServoPulse(servoLeft, 0.0015);
      delay(1000);
    }
    else if ((servoMove[i]) == "L'") {
      Serial.println((servoMove[i]));
      setServoPulse(servoLeft, 0.0009);
      delay(100);
      setServoPulse(servoLeft, 0.0015);
      delay(1000);
    }
    else if ((servoMove[i]) == "L2") {
      Serial.println((servoMove[i]));
      setServoPulse(servoLeft, 0.00187);
      delay(125);
      setServoPulse(servoLeft, 0.0015);
      delay(1000);
      setServoPulse(servoLeft, 0.00187);
      delay(125);
      setServoPulse(servoLeft, 0.0015);
      delay(1000);
    }
    else if ((servoMove[i]) == "L2'") {
      Serial.println((servoMove[i]));
      setServoPulse(servoLeft, 0.0009);
      delay(100);
      setServoPulse(servoLeft, 0.0015);
      delay(1000);
      setServoPulse(servoLeft, 0.0009);
      delay(100);
      setServoPulse(servoLeft, 0.0015);
      delay(1000);
    }
    //6
    else if ((servoMove[i]) == "D") {
      Serial.println((servoMove[i]));
      setServoPulse(servoDown, 0.00187);
      delay(125);
      setServoPulse(servoDown, 0.0015);
      delay(1000);
    }
    else if ((servoMove[i]) == "D'") {
      Serial.println((servoMove[i]));
      setServoPulse(servoDown, 0.0009);
      delay(100);
      setServoPulse(servoDown, 0.0015);
      delay(1000);
    }
    else if ((servoMove[i]) == "D2") {
      Serial.println((servoMove[i]));
      setServoPulse(servoDown, 0.00187);
      delay(125);
      setServoPulse(servoDown, 0.0015);
      delay(1000);
      setServoPulse(servoDown, 0.00187);
      delay(125);
      setServoPulse(servoDown, 0.0015);
      delay(1000);
    }
    else if ((servoMove[i]) == "D2'") {
      Serial.println((servoMove[i]));
      setServoPulse(servoDown, 0.0009);
      delay(100);
      setServoPulse(servoDown, 0.0015);
      delay(1000);
      setServoPulse(servoDown, 0.0009);
      delay(100);
      setServoPulse(servoDown, 0.0015);
      delay(1000);
    }
    else {
      setServoPulse(servoLeft, 0.0015);
      setServoPulse(servoRight, 0.0015);
      setServoPulse(servoUp, 0.0015);
      setServoPulse(servoDown, 0.0015);
      setServoPulse(servoFront, 0.0015);
      setServoPulse(servoBack, 0.0015);
      Serial.println("End of Program");
      Serial.println("Exit");
      break;
    }
    delay(500);
  }
}

void splitSolution() {
  while ( j < solution.length())
  {
    if (solution.charAt(j) == ' ')
    {
      i++;
    }
    else
    {
      servoMove[i] += solution.charAt(j);
    }
    j++;
  }
  for (int a = 0; a < 25; a++) {
    Serial.print(servoMove[a]);
    Serial.print(" ");
  }
  Serial.println();
  Serial.println("Exit");
}

void getSolution() {
  solution = Serial.readString();
}

char ReadCube() {
  count = 0;
  uint8_t r, g, b;
  for (int i = 0; i < 3; i++) {//Y Axis
    for (int j = 0; j < 3; j++) {//X Axis
      if (pixy.video.getRGB(x[j], y[i], &r, &g, &b) == 0)
      {
        Serial.print("Cubie:::");
        Serial.print("red:");
        Serial.print(r);
        Serial.print(" green:");
        Serial.print(g);
        Serial.print(" blue:");
        Serial.println(b);
        getColor[count] = GetCubieColor(r, g, b);
        count++;
      }
    }
  }
}

char GetCubieColor(uint8_t r, uint8_t g, uint8_t b) {
  if (r >= 220 && r <= 255 && g >= 200 && g <= 255 && b >= 80 && b <= 180)//Yellow
    return ('D');//D
  else if (r == 255 && g >= 30 && g <= 115 && b >= 55 && b <= 150)//Red
    return ('R');//R
  else if (r >= 195 && r <= 255 && g >= 210 && g <= 255 && b >= 195 && b <= 255) //White
    return ('U');//U
  else if (r == 255 && g >= 115 && g <= 180 && b >= 60 && b <= 135)//Orange
    return ('L');//L
  else if (r >= 105 && r <= 210 && g == 255 && b >= 85 && b <= 155)//Green
    return ('F');//F
  else if (r >= 35 && r <= 165 && g >= 185 && g <= 255 && b == 255) //Blue
    return ('B');//B
  else
    return ('X');//X
}

void print_cube(char cube_side[])
{
  // make a for loop for this
  for (int i = 0; i < 1; i++)
  {
    Serial.println('\r');
  }
  Serial.print("Face: ");
  Serial.println(cube_side[4]);
  for (int i = 0; i < 9; i = i + 3)
  {
    Serial.print("|");
    Serial.print(cube_side[i]);
    Serial.print("|");
    Serial.print(cube_side[i + 1]);
    Serial.print("|");
    Serial.print(cube_side[i + 2]);
    Serial.println("|");
  }
}

void getCubeDefinitionString() {
  //UP=RIGHT=FRONT=DOWN=LEFT=BACK
  //W>R>G>Y>O>B
  for (int i = 0; i < 9; i++) {
    cube += white_side[i];
  }
  for (int i = 0; i < 9; i++) {
    cube += red_side[i];
  }
  for (int i = 0; i < 9; i++) {
    cube += green_side[i];
  }
  for (int i = 0; i < 9; i++) {
    cube += yellow_side[i];
  }
  for (int i = 0; i < 9; i++) {
    cube += orange_side[i];
  }
  for (int i = 0; i < 9; i++) {
    cube += blue_side[i];
  }
}

void loop() {
}
