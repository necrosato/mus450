//#define ESP32

#ifdef ESP32
    #define POT_PIN 12
    #define PHOTO_PIN 14
#else
    #define POT_PIN A0
    #define PHOTO_PIN A1
#endif

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Initializing...");
}

int pmin = 10000;
int phmin = 10000;
int pmax = -1;
int phmax = -1;
int potvalmidi = 0;
int potvalmidi_prev = 0;
int photovalmidi = 0;
int photovalmidi_prev = 0;
int potval = 0;
int potval_prev = 0;
int photoval = 0;
int photoval_prev = 0;

void set_min_max() {
    pmin = min(potval,pmin);
    phmin = min(photoval,phmin);
    pmax = max(potval,pmax);
    phmax = max(photoval,phmax);
}

void serial_pot_log() {
    Serial.print("Potentiometer(raw, mapped): \t");
    Serial.print(potval);
    Serial.print("\t");
    Serial.print(potvalmidi);
    Serial.print("\tMin:\t");
    Serial.print(pmin);
    Serial.print("\tMax:\t");
    Serial.print(pmax);
    Serial.println();
}
void serial_photo_log() {
    Serial.print("Photocell(raw, mapped): \t");
    Serial.print(photoval);
    Serial.print("\t");
    Serial.print(photovalmidi);
    Serial.print("\tMin:\t");
    Serial.print(phmin);
    Serial.print("\tMax:\t");
    Serial.print(phmax);
    Serial.println();
}

void serial_pot_midi_data() {
    Serial.print("PO ");
    Serial.print(potvalmidi);
    Serial.println();
}
void serial_photo_midi_data() {
    Serial.print("PH ");
    Serial.print(photovalmidi);
    Serial.println();
}

void loop() {
    potval = analogRead(POT_PIN);
    photoval = analogRead(PHOTO_PIN);
    set_min_max();
    if (pmin < pmax) {
        potvalmidi  = map(potval, pmin, pmax, 0, 127);
    }
    if (phmin < phmax) {
        photovalmidi = map(photoval, phmin, phmax, 0, 127); 
    }
    //if (potval != potval_prev) {
    if (potvalmidi != potvalmidi_prev) {
        //serial_pot_log();
        serial_pot_midi_data();
    }
    //if (photoval != photoval_prev) {
    if (photovalmidi != photovalmidi_prev) {
        //serial_photo_log();
        serial_photo_midi_data();
    }
    potval_prev = potval;
    potvalmidi_prev = potvalmidi;
    photoval_prev = photoval;
    photovalmidi_prev = photovalmidi;
    delay(100);
}
