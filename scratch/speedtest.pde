#define TIME 1
#define WRENABLE 3
#define TX 2
#define RX 4
    

void setup() {
  pinMode(TX, OUTPUT);     
  pinMode(RX, INPUT);
  pinMode(WRENABLE, OUTPUT);
  
  digitalWrite(WRENABLE, HIGH);
}

void loop() {
  digitalWrite(TX, HIGH);
  delayMicroseconds(TIME);              
  digitalWrite(TX, LOW); 
  delayMicroseconds(TIME);              
}
