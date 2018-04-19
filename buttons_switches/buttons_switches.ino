
int BPIN = 2;
int SPIN = 3;

void setup() {
    Serial.begin(9600);
    pinMode(BPIN, INPUT);
    pinMode(SPIN, INPUT);
    Serial.println("Beginning");
}

void loop() {
    if (digitalRead(BPIN) == HIGH) {
        Serial.println("Button bushed");
    }
    if (digitalRead(SPIN) == HIGH) {
        Serial.println("Switch on");
    }
    delay(50);
}
