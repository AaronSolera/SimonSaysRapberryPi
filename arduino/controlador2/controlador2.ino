long randNumber;
int confirmationBit=0;
String order;
int amount=1;
int flag=1;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  digitalWrite(2,LOW);
  digitalWrite(3,LOW);
  digitalWrite(4,LOW);
  digitalWrite(5,LOW);
}

void loop() {
  order="";
  //generates random number and turn on leds depending on pattern
  for (int i=0; i<amount; i++){
    randNumber = random(2, 6);
    order.concat(String(randNumber));
    digitalWrite(randNumber, HIGH);
    delay(1000);
    digitalWrite(randNumber, LOW);
    delay(100);
  }
  //Serial.write(order.toInt());
  Serial.println(order);
  //reads the serial and waits for the confirmation bit
  confirmationBit = Serial.read();
  flag=1;
  while(flag){
    if (Serial.available() > 0) {
    // read the incoming byte:
    confirmationBit = Serial.read();
    }    
    if(confirmationBit==49){//49 is 1
      flag=0;
      amount++;
    }
    else if(confirmationBit==50){//50 is 2
      amount=1;
      flag=0;
    }
    delay(10);
  }
  delay(1000);
  //clear the buffer from garbage values
  while(Serial.available() > 0) {
    char t = Serial.read();
  }
}
