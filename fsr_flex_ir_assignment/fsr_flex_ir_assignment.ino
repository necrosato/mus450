#define ESP32

#ifdef ESP32
    #define FLEX_PIN 12
    #define FSR_PIN 27
    #define IR_PIN 14
    #define ADC_MAX 4095
#else
    #define FLEX_PIN A0
    #define FSR_PIN A1
    #define IR_PIN A2
    #define ADC_MAX 1023
#endif

const int GRAPH_LENGTH = 127; // length of line graph (Max MIDI value)

void setup() {
    //Serial.begin(115200);
    Serial.begin(9600);
    delay(1000);
    Serial.println("Initializing...");
}

int flex = 0, fsr = 0, ir = 0;
int flex_prev = 0, fsr_prev = 0, ir_prev = 0;
int flex_midi = 0, fsr_midi = 0, ir_midi = 0;
int flex_midi_prev = 0, fsr_midi_prev = 0, ir_midi_prev = 0;

void send_flex_data() {
    Serial.print((char)FLEX_PIN);
    Serial.print((char)flex_midi);
    Serial.println();
}

void send_fsr_data() {
    Serial.print((char)FSR_PIN);
    Serial.print((char)fsr_midi);
    Serial.println();
}

void send_ir_data() {
    Serial.print((char)IR_PIN);
    Serial.print((char)ir_midi);
    Serial.println();
}

void loop() {
    flex = analogRead(FLEX_PIN);
    fsr = analogRead(FSR_PIN);
    ir = analogRead(IR_PIN);

    flex_midi = map(flex, 0, ADC_MAX, 0, GRAPH_LENGTH);
    fsr_midi = map(fsr, 0, ADC_MAX, 0, GRAPH_LENGTH);
    ir_midi = map(ir, 0, ADC_MAX, 0, GRAPH_LENGTH);

    if (flex_midi != flex_midi_prev) {
        send_flex_data();
    }
    if (fsr_midi != fsr_midi_prev) {
        send_fsr_data();
    }
    if (ir_midi != ir_midi_prev) {
        send_ir_data();
    }

    flex_prev = flex;
    fsr_prev = fsr;
    ir_prev = ir;
    flex_midi_prev = flex_midi;
    fsr_midi_prev = fsr_midi;
    ir_midi_prev = ir_midi;
    delay(100);
}

