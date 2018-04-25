#define POT_PIN 12
#define PHOTO_PIN 14

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Initializing...");
}

int pmin = 10000;
int phmin = 10000;
int pmax = -1;
int phmax = -1;
void loop() {
    int potval = analogRead(POT_PIN);
    int photoval = analogRead(PHOTO_PIN);
    pmin = min(pmin, potval);
    phmin = min(phmin, photoval);
    pmax = max(pmin, potval);
    phmax = max(phmax, photoval);
    Serial.print("Poteniometer:\t");
    Serial.print(potval);
    Serial.print("\tMin:\t");
    Serial.print("\tMax:\t");
    Serial.println();
    Serial.print("Photocell:\t");
    Serial.print(photoval);
    Serial.print("\tMin:\t");
    Serial.print("\tMax:\t");
    Serial.println();
    delay(100);
}
