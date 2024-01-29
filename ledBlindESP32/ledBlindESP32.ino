#define INTERNAL_LED 2

void setup() {
  // put your setup code here, to run once:
  pinMode(INTERNAL_LED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(INTERNAL_LED, HIGH);
  delay(1000);
  digitalWrite(INTERNAL_LED, LOW);
  delay(1000);
}
