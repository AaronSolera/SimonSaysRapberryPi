long randNumber;
int confirmationBit=0;
String order;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
}

void loop() {
  // print a random number
  order="";
  for (int i=0; i<4; i++){
    randNumber = random(2, 6);
    order.concat(String(randNumber));
    //Serial.println(randNumber);
    digitalWrite(randNumber, HIGH);
    delay(1000);
    digitalWrite(randNumber, LOW);
    delay(100);
  }
  Serial.println(order);
  confirmationBit = Serial.read();
  Serial.println(confirmationBit);
  // send data only when you receive data:
//  if (Serial.available() > 0) {
//    // read the incoming byte:
//    confirmationBit = Serial.read();
//  }
  while(confirmationBit==-1){
    confirmationBit = Serial.read();
    if(confirmationBit==1){
      break;
    }
    delay(1);
  }
  delay(1000);
  while(Serial.available() > 0) {
    char t = Serial.read();
  }
}
