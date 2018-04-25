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
    pmax = max(pmax, potval);
    phmax = max(phmax, photoval);
    int potvalmidi = map(potval, pmin, pmax, 0, 127);
    int photovalmidi = map(photoval, phmin, phmax, 0, 127); 
    Serial.print("Poteniometer(raw, mapped): \t");
    Serial.print(potval);
    Serial.print("\t");
    Serial.print(potvalmidi);
    Serial.print("\tMin:\t");
    Serial.print(pmin);
    Serial.print("\tMax:\t");
    Serial.print(pmax);
    Serial.println();
    Serial.print("Photocell(raw, mapped): \t");
    Serial.print(photoval);
    Serial.print("\t");
    Serial.print(photovalmidi);
    Serial.print("\tMin:\t");
    Serial.print(phmin);
    Serial.print("\tMax:\t");
    Serial.print(phmax);
    Serial.println();
    delay(100);
}
