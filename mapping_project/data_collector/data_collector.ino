#define ESP32

char channel[2] = {0};
char data[4] = {0};

void send_channel_data() {
    Serial.print(channel[0]);
    Serial.print(channel[1]);
    Serial.print(data[0]);
    Serial.print(data[1]);
    Serial.print(data[2]);
    Serial.print(data[3]);
    Serial.println();
}

void zero_data() {
    for (int i = 0; i < 4; i++) {
        data[i] = 0;
    }
}

void set_channel(int ch) {
    channel[0] = ch / 10;
    channel[1] = ch % 10;
}

void set_data(int val) {
    data[0] = val / 1000;
    data[1] = (val % 1000) / 100;
    data[2] = (val % 100) / 10;
    data[3] = val % 10;
}

void serial_test_0() {
    delay(1000);
    for (int i = 0; i < 40; i++) {
        channel[0] = (char) (i / 10);
        channel[1] = (char) (i % 10);
        zero_data();
        send_channel_data();
    }
    delay(1000);
    for (int i = 0; i < 40; i++) {
        channel[0] = (char) (i / 10);
        channel[1] = (char) (i % 10);
        for (int j = 0; j < 127; j++) {
            data[0] = j / 1000;
            data[1] = (j % 1000) / 100;
            data[2] = (j % 100) / 10;
            data[3] = j % 10;
            send_channel_data();
        }
        zero_data();
        send_channel_data();
    }
}

void serial_test_1() {
    for (int j = 0; j < 127; j++) {
        data[0] = j / 1000;
        data[1] = (j % 1000) / 100;
        data[2] = (j % 100) / 10;
        data[3] = j % 10;
        for (int i = 0; i < 40; i++) {
            channel[0] = (char) (i / 10);
            channel[1] = (char) (i % 10);
            send_channel_data();
        }
    }
    delay(1000);
    for (int i = 0; i < 40; i++) {
        channel[0] = (char) (i / 10);
        channel[1] = (char) (i % 10);
        zero_data();
        send_channel_data();
    }
    delay(1000);
}

void setup() {
    Serial.begin(9600);
    //Serial.begin(115200);
    delay(1000);
}

void loop() {
    int fsr_val = map(analogRead(A0), 0, 1024, 0, 127);
    //int fsr_val = map(analogRead(A0), 0, 1024, 0, 15);
    set_channel(A0);
    set_data(fsr_val);
    send_channel_data();
    
    delay(50);
    //serial_test_0();
    //serial_test_1();
    //delay(3000);
}
