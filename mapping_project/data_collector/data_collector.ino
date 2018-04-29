#define ESP32

void serial_test_0() {
    delay(1000);
    char channel[2] = {0};
    char data[4] = {0};
    for (int i = 0; i < 40; i++) {
        channel[0] = (char) (i / 10);
        channel[1] = (char) (i % 10);
        Serial.print(channel[0]);
        Serial.print(channel[1]);
        Serial.print((char)0);
        Serial.print((char)0);
        Serial.print((char)0);
        Serial.print((char)0);
        Serial.println();
    }
    delay(1000);
    for (int i = 0; i < 40; i++) {
        channel[0] = (char) (i / 10);
        channel[1] = (char) (i % 10);
        for (int j = 0; j < 127; j++) {
            data[1] = j / 100;
            data[2] = (j % 100) / 10;
            data[3] = j % 10;
            Serial.print(channel[0]);
            Serial.print(channel[1]);
            Serial.print(data[0]);
            Serial.print(data[1]);
            Serial.print(data[2]);
            Serial.print(data[3]);
            Serial.println();
        }
        Serial.print(channel[0]);
        Serial.print(channel[1]);
        Serial.print((char)0);
        Serial.print((char)0);
        Serial.print((char)0);
        Serial.print((char)0);
        Serial.println();
    }
    delay(1000);
    for (int i = 0; i < 40; i++) {
        channel[0] = (char) (i / 10);
        channel[1] = (char) (i % 10);
        Serial.print(channel[0]);
        Serial.print(channel[1]);
        Serial.print((char)0);
        Serial.print((char)0);
        Serial.print((char)0);
        Serial.print((char)0);
        Serial.println();
    }
}

void serial_test_1() {
    delay(1000);
    char channel[2] = {0};
    char data[4] = {0};
    for (int i = 0; i < 40; i++) {
        channel[0] = (char) (i / 10);
        channel[1] = (char) (i % 10);
        Serial.print(channel[0]);
        Serial.print(channel[1]);
        Serial.print((char)0);
        Serial.print((char)0);
        Serial.print((char)0);
        Serial.print((char)0);
        Serial.println();
    }
    delay(1000);
    for (int j = 0; j < 127; j++) {
        data[1] = j / 100;
        data[2] = (j % 100) / 10;
        data[3] = j % 10;
        for (int i = 0; i < 40; i++) {
            channel[0] = (char) (i / 10);
            channel[1] = (char) (i % 10);
            Serial.print(channel[0]);
            Serial.print(channel[1]);
            Serial.print(data[0]);
            Serial.print(data[1]);
            Serial.print(data[2]);
            Serial.print(data[3]);
            Serial.println();
        }
    }
    delay(1000);
    for (int i = 0; i < 40; i++) {
        channel[0] = (char) (i / 10);
        channel[1] = (char) (i % 10);
        Serial.print(channel[0]);
        Serial.print(channel[1]);
        Serial.print((char)0);
        Serial.print((char)0);
        Serial.print((char)0);
        Serial.print((char)0);
        Serial.println();
    }
    delay(1000);
}

void setup() {
    Serial.begin(9600);
    //Serial.begin(115200);
    delay(1000);
}

void loop() {
    serial_test_0();
    serial_test_1();
    delay(3000);
}
