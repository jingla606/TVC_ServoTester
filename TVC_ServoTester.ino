#include <Servo.h>
#include <Math.h>

Servo x;
Servo y;
int nmos = 6;                             //NMOS transistor wired to ignition system
int led = 17;                             //LED for basic information

int pos = 0;
int xmid = 85;                            //X Servo angle at which motor is in normal position
int ymid = 90;                            //Y Servo angle at which motor is in normal position
int theta = 35;
int prei;
int posi;
int post;
bool repeat = false;
char prevcom;
char command;

void setup() {
  Serial.begin(9600);
  
  x.attach(12);
  y.attach(10);
  pinMode(led, OUTPUT);
  pinMode(nmos, OUTPUT);

  x.write(xmid);
  y.write(ymid);
  
  Serial.println("Center Position");
  Serial.println("Press any key to arm!");
  
  while (Serial.available() <= 0) {
    Serial.print(".");
    digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(led, LOW);
    delay(1000);
  }
  x.write(xmid);
  y.write(ymid);
  delay(theta);
  Serial.read();
  Serial.println();
  repeat = false;
}

void loop() {
  Serial.println("\nArmed! Input Command:");
  Serial.println("\tt = Control Systems Test (TVC Only)");
  Serial.println("\tn = Normal Test (TVC+Motor)");
  Serial.println("\th = Holddown Test (Motor Only)");
  if(repeat){
    Serial.println("\tr = Repeat Previous Command");
  }
  bool await = true;
  while(await){                                         //await command
    if(Serial.available() > 0) {
      command = Serial.read();
      if(command=='t'){
        Serial.println("Control Systems Test (TVC Only)");
      }
      else if(command=='n'){
        Serial.println("Normal Test (TVC+Motor)");
      }
      else if(command=='h'){
        Serial.println("Holddown Test (Motor Only)");
      }
      else if(command=='r'&&repeat){
        Serial.println("Repeating Previous Sequence");
        command = prevcom;
      }
      else{
        Serial.println("Invalid Command");
      }
      repeat = false;
      await = false;
    }
  }

  x.write(xmid);
  y.write(ymid);
  delay(theta);
  
  if(command=='n'||command=='h'){                       //ignite motor
    Serial.println("Initiate Ground-Fire Sequence");
    prei = millis();
    digitalWrite(nmos, HIGH);
    for(int i = 1; i<=15; i++){                         //Gives power to MOSFET for 1.5 seconds, hopefully igniting the ignitor
      digitalWrite(led, HIGH);
      delay(25);
      digitalWrite(led, LOW);
      delay(25);
      digitalWrite(led, HIGH);
      delay(25);
      digitalWrite(led, LOW);
      delay(25);
    }
    digitalWrite(nmos, LOW);
    posi = millis();
  }
  
  if(command=='t'||command=='n'){                       //TVC movements (the first parameter is servo delay per degree, second parameter is delay between movements)
    if(command=='t'){                                   //Slow Test
      xtwitch(15, 100);
      delay(500);
      ytwitch(15, 100);
      delay(500);
      xytwitch(15, 100);
      delay(500);
      circle(35);
      delay(1000);
    }
    xtwitch(2, 0);                                  //Quick Test
    delay(50);
    ytwitch(2, 0);
    delay(50);
    xytwitch(2, 0);
    delay(50);
    circle(5);
  }
  
  post = millis();
  for(int i = 1; i<=10; i++){
    digitalWrite(led, HIGH);
    delay(50);
    digitalWrite(led, LOW);
    delay(50);
  }
  Serial.println();

  if(command=='n'||command=='h'){                       //Calculate MET
    double preig = ((double)(post-prei))/1000;
    double posig = ((double)(post-posi))/1000;
    Serial.print("T since pre-ignition: ");
    Serial.println(preig);
    Serial.print("Sequence Time: ");
    Serial.println(posig);
  }

  prevcom = command;
  Serial.println();
  Serial.println("Press r to re-arm!");
  while(!repeat){
    if (Serial.available() > 0) {
      if(Serial.read()=='r'){
        repeat = true;
      }
    }
  }
}

void xtwitch(int del, int space){
  Serial.print("X:\tRight");
  for (pos = 0; pos <= theta; pos += 1) {
    x.write(xmid+pos);
    delay(del);
  }
  delay(space);
  
  Serial.print("\tCenter");
  for (pos = theta; pos >= 0; pos -= 1) {
    x.write(xmid+pos);
    delay(del);
  }
  delay(space);
  
  Serial.print("\tLeft");
  for (pos = 0; pos <= theta; pos += 1) {
    x.write(xmid-pos);
    delay(del);
  }
  delay(space);
  
  Serial.println("\tCenter");
  for (pos = theta; pos >= 0; pos -= 1) {
    x.write(xmid-pos);
    delay(del);
  }
  delay(space);
}

void ytwitch(int del, int space){
  Serial.print("Y:\tUp");
  for (pos = 0; pos <= theta; pos += 1) {
    y.write(ymid+pos);
    delay(del);
  }
  delay(space);
  
  Serial.print("\tCenter");
  for (pos = theta; pos >= 0; pos -= 1) {
    y.write(ymid+pos);
    delay(del);
  }
  delay(space);
  
  Serial.print("\tDown");
  for (pos = 0; pos <= theta; pos += 1) {
    y.write(ymid-pos);
    delay(del);
  }
  delay(space);
  
  Serial.println("\tCenter");
  for (pos = theta; pos >= 0; pos -= 1) {
    y.write(ymid-pos);
    delay(del);
  }
  delay(space);
}

void xytwitch(int del, int space){
  Serial.println("XY Rally");
  for (pos = 0; pos <= theta; pos += 1) {
    x.write(xmid+pos);
    y.write(ymid-pos);
    delay(del);
  }
  delay(space);
  
  for (pos = theta; pos >= 0; pos -= 1) {
    x.write(xmid+pos);
    y.write(ymid-pos);
    delay(del);
  }
  delay(space);
  
  for (pos = 0; pos <= theta; pos += 1) {
    x.write(xmid-pos);
    y.write(ymid+pos);
    delay(del);
  }
  delay(space);
  
  for (pos = theta; pos >= 0; pos -= 1) {
    x.write(xmid-pos);
    y.write(ymid+pos);
    delay(del);
  }
  delay(space);
  
  for (pos = 0; pos <= theta; pos += 1) {
    x.write(xmid-pos);
    y.write(ymid-pos);
    delay(del);
  }
  delay(space);
  
  for (pos = theta; pos >= 0; pos -= 1) {
    x.write(xmid-pos);
    y.write(ymid-pos);
    delay(del);
  }
  delay(space);
  
  for (pos = 0; pos <= theta; pos += 1) {
    x.write(xmid+pos);
    y.write(ymid+pos);
    delay(del);
  }
  delay(space);
  
  for (pos = theta; pos >= 0; pos -= 1) {
    x.write(xmid+pos);
    y.write(ymid+pos);
    delay(del);
  }
  delay(space);
}

void circle(int del){
  Serial.print("CW Circle\t");
  x.write(xmid+theta);
  delay(theta);
  for(float rad = 0; rad <=6.28; rad+=0.1){
    x.write(xmid+cos(rad)*theta);
    y.write(ymid+sin(rad)*theta);
    delay(del);
    Serial.print(".");
  }
  Serial.println(" Complete!");
  Serial.print("CCW Circle\t");
  for(float rad = 0; rad <=6.28; rad+=0.1){
    x.write(xmid+cos(rad)*theta);
    y.write(ymid-sin(rad)*theta);
    delay(del);
    Serial.print(".");
  }
  Serial.println(" Complete!");
  x.write(xmid);
  y.write(ymid);
  delay(theta);
}

