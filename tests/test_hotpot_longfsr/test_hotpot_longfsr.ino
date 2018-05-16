
const int HOT_POT_PIN = A0;
const int FSR_PIN = A1;

const int GRAPH_LENGTH = 127; // length of line graph (Max MIDI value)

void setup() {
    Serial.begin(115200);
    Serial.print("Setup Finished\n");
}

void loop() {
    int hotpot = analogRead(HOT_POT_PIN);
    int fsr = analogRead(FSR_PIN);

    Serial.print("Hotpot/FSR:\t\t");
    Serial.print(hotpot);
    Serial.print("\t");
    Serial.println(fsr);
    
    delay(100);
}

