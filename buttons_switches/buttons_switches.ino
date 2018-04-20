

//int BPIN = 2;
int BPIN = 2;
//int SPIN = 3;
int SPIN = 4;

void setup() {
    Serial.begin(9600);
    pinMode(BPIN, INPUT);
    pinMode(SPIN, INPUT);
    Serial.println("Beginning");
}

void loop() {
    if (digitalRead(BPIN) == HIGH) {
        //Serial.println("Button bushed");
        Serial.println("B");
    }
    if (digitalRead(SPIN) == HIGH) {
        //Serial.println("Switch on");
        Serial.println("S");
    }
    delay(50);
}
