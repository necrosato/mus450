
#define ESP32MPU
#define ESP32

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

MPU6050 accelgyro;
//MPU6050 accelgyro(0x69); // <-- use for AD0 high
// default sda is 21 and scl is 22 on esp 32

#define MAP_MIN 0
#define MAP_MAX 127
#define AG_CONST
#define AG_MIN -32768
#define AG_MAX 32767
#define AX_CHANNEL 6
#define AY_CHANNEL 7
#define AZ_CHANNEL 8
#define GX_CHANNEL 9
#define GY_CHANNEL 10
#define GZ_CHANNEL 11
int16_t ax = 0;
int16_t ay = 0;
int16_t az = 0;
int16_t gx = 0;
int16_t gy = 0;
int16_t gz = 0;
int16_t ax_prev = 0;
int16_t ay_prev = 0;
int16_t az_prev = 0;
int16_t gx_prev = 0;
int16_t gy_prev = 0;
int16_t gz_prev = 0;

#ifdef AG_CONST
    int16_t ax_min = AG_MIN;
    int16_t ax_max = AG_MAX;
    int16_t ay_min = AG_MIN;
    int16_t ay_max = AG_MAX;
    int16_t az_min = AG_MIN;
    int16_t az_max = AG_MAX;
    int16_t gx_min = AG_MIN;
    int16_t gx_max = AG_MAX;
    int16_t gy_min = AG_MIN;
    int16_t gy_max = AG_MAX;
    int16_t gz_min = AG_MIN;
    int16_t gz_max = AG_MAX;
    int16_t ax_min_prev = AG_MIN;
    int16_t ax_max_prev = AG_MAX;
    int16_t ay_min_prev = AG_MIN;
    int16_t ay_max_prev = AG_MAX;
    int16_t az_min_prev = AG_MIN;
    int16_t az_max_prev = AG_MAX;
    int16_t gx_min_prev = AG_MIN;
    int16_t gx_max_prev = AG_MAX;
    int16_t gy_min_prev = AG_MIN;
    int16_t gy_max_prev = AG_MAX;
    int16_t gz_min_prev = AG_MIN;
    int16_t gz_max_prev = AG_MAX;
#else
    int16_t ax_min = 0;
    int16_t ax_max = 0;
    int16_t ay_min = 0;
    int16_t ay_max = 0;
    int16_t az_min = 0;
    int16_t az_max = 0;
    int16_t gx_min = 0;
    int16_t gx_max = 0;
    int16_t gy_min = 0;
    int16_t gy_max = 0;
    int16_t gz_min = 0;
    int16_t gz_max = 0;
    int16_t ax_min_prev = 0;
    int16_t ax_max_prev = 0;
    int16_t ay_min_prev = 0;
    int16_t ay_max_prev = 0;
    int16_t az_min_prev = 0;
    int16_t az_max_prev = 0;
    int16_t gx_min_prev = 0;
    int16_t gx_max_prev = 0;
    int16_t gy_min_prev = 0;
    int16_t gy_max_prev = 0;
    int16_t gz_min_prev = 0;
    int16_t gz_max_prev = 0;
#endif
int16_t ax_mapped = 0;
int16_t ay_mapped = 0;
int16_t az_mapped = 0;
int16_t gx_mapped = 0;
int16_t gy_mapped = 0;
int16_t gz_mapped = 0;
int16_t ax_mapped_prev = 0;
int16_t ay_mapped_prev = 0;
int16_t az_mapped_prev = 0;
int16_t gx_mapped_prev = 0;
int16_t gy_mapped_prev = 0;
int16_t gz_mapped_prev = 0;

#define ANALOG_MIN 0
#ifdef ESP32
    #define ANALOG_MAX 4095
#else
    #define ANALOG_MAX 1023
#endif
#define BUTTON_CHANNEL 23   // slide switch or pushbutton
#define FSR0_CHANNEL 13     // nut fsr
#define FSR1_CHANNEL 12     // pick fsr
#define FSR2_CHANNEL 14     // neck fsr
#define HOTPOT_CHANNEL 27   // HotPot
#define SOFTPOT0_CHANNEL 26 // SoftPot leads to head
#define SOFTPOT1_CHANNEL 25 // SoftPot leads to body
int16_t button = LOW;
int16_t fsr0 = 0;
int16_t fsr1 = 0;
int16_t fsr2 = 0;
int16_t hotpot = 0;
int16_t softpot0 = 0;
int16_t softpot1 = 0;
int16_t fsr0_min = ANALOG_MIN;
int16_t fsr1_min = ANALOG_MIN;
int16_t fsr2_min = ANALOG_MIN;
int16_t hotpot_min = ANALOG_MIN;
int16_t softpot0_min = ANALOG_MIN;
int16_t softpot1_min = ANALOG_MIN;
int16_t fsr0_max = ANALOG_MAX;
int16_t fsr1_max = ANALOG_MAX;
int16_t fsr2_max = ANALOG_MAX;
int16_t hotpot_max = ANALOG_MAX;
int16_t softpot0_max = ANALOG_MAX;
int16_t softpot1_max = ANALOG_MAX;
int16_t fsr0_mapped = 0;
int16_t fsr1_mapped = 0;
int16_t fsr2_mapped = 0;
int16_t hotpot_mapped = 0;
int16_t softpot0_mapped = 0;
int16_t softpot1_mapped = 0;
int16_t button_prev = LOW;
int16_t fsr0_prev = 0;
int16_t fsr1_prev = 0;
int16_t fsr2_prev = 0;
int16_t hotpot_prev = 0;
int16_t softpot0_prev = 0;
int16_t softpot1_prev = 0;
int16_t fsr0_min_prev = ANALOG_MIN;
int16_t fsr1_min_prev = ANALOG_MIN;
int16_t fsr2_min_prev = ANALOG_MIN;
int16_t hotpot_min_prev = ANALOG_MIN;
int16_t softpot0_min_prev = ANALOG_MIN;
int16_t softpot1_min_prev = ANALOG_MIN;
int16_t fsr0_max_prev = ANALOG_MAX;
int16_t fsr1_max_prev = ANALOG_MAX;
int16_t fsr2_max_prev = ANALOG_MAX;
int16_t hotpot_max_prev = ANALOG_MAX;
int16_t softpot0_max_prev = ANALOG_MAX;
int16_t softpot1_max_prev = ANALOG_MAX;
int16_t fsr0_mapped_prev = 0;
int16_t fsr1_mapped_prev = 0;
int16_t fsr2_mapped_prev = 0;
int16_t hotpot_mapped_prev = 0;
int16_t softpot0_mapped_prev = 0;
int16_t softpot1_mapped_prev = 0;

char channel[2] = {0};
char data[4] = {0};

void read_accelgyro() {
    // read raw accel/gyro measurements from device
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    get_min_max_accel();
    get_min_max_gyro();
    // map accel and gyro
    map_accel();
    map_gyro();
}

void read_sensors() {
    // read digital button
    button = digitalRead(BUTTON_CHANNEL);

    // read raw analog measurements from device
    fsr0 = analogRead(FSR0_CHANNEL);
    fsr1 = analogRead(FSR1_CHANNEL);
    fsr2 = analogRead(FSR2_CHANNEL);
    hotpot = analogRead(HOTPOT_CHANNEL);
    softpot0 = analogRead(SOFTPOT0_CHANNEL);
    softpot1 = analogRead(SOFTPOT1_CHANNEL);
    get_min_max_analogs();
    // map analogs
    map_analogs();
}

void get_min_max_accel() {
    ax_min = min(ax, ax_min);
    ay_min = min(ay, ay_min);
    az_min = min(az, az_min);
    ax_max = max(ax, ax_max);
    ay_max = max(ay, ay_max);
    az_max = max(az, az_max);
}

void get_min_max_gyro() {
    gx_min = min(gx, gx_min);
    gy_min = min(gy, gy_min);
    gz_min = min(gz, gz_min);
    gx_max = max(gx, gx_max);
    gy_max = max(gy, gy_max);
    gz_max = max(gz, gz_max);
}

void get_min_max_analogs() {
    fsr0_min = min(fsr0, fsr0_min);
    fsr1_min = min(fsr1, fsr1_min);
    fsr2_min = min(fsr2, fsr2_min);
    hotpot_min = min(hotpot, hotpot_min);
    softpot0_min = min(softpot0, softpot0_min);
    softpot1_min = min(softpot1, softpot1_min);
    fsr0_max = max(fsr0, fsr0_max);
    fsr1_max = max(fsr1, fsr1_max);
    fsr2_max = max(fsr2, fsr2_max);
    hotpot_max = max(hotpot, hotpot_max);
    softpot0_max = max(softpot0, softpot0_max);
    softpot1_max = max(softpot1, softpot1_max);
}

void map_accel() {
    if (ax_min < ax_max) {
        ax_mapped = map(ax, ax_min, ax_max, MAP_MIN, MAP_MAX);
    }
    if (ay_min < ay_max) {
        ay_mapped = map(ay, ay_min, ay_max, MAP_MIN, MAP_MAX);
    }
    if (gz_min < az_max) {
        az_mapped = map(az, az_min, az_max, MAP_MIN, MAP_MAX);
    }
}

void map_gyro() {
    if (gx_min < gx_max) {
        gx_mapped = map(gx, gx_min, gx_max, MAP_MIN, MAP_MAX);
    }
    if (gy_min < gy_max) {
        gy_mapped = map(gy, gy_min, gy_max, MAP_MIN, MAP_MAX);
    }
    if (gz_min < gz_max) {
        gz_mapped = map(gz, gz_min, gz_max, MAP_MIN, MAP_MAX);
    }
}

void map_analogs() {
    if (fsr0_min < fsr0_max) {
        fsr0_mapped = map(fsr0, fsr0_min, fsr0_max, ANALOG_MIN, ANALOG_MAX);
    }
    if (fsr1_min < fsr1_max) {
        fsr1_mapped = map(fsr1, fsr1_min, fsr1_max, ANALOG_MIN, ANALOG_MAX);
    }
    if (fsr2_min < fsr2_max) {
        fsr2_mapped = map(fsr2, fsr2_min, fsr2_max, ANALOG_MIN, ANALOG_MAX);
    }
    if (hotpot_min < hotpot_max) {
        hotpot_mapped = map(hotpot, hotpot_min, hotpot_max, ANALOG_MIN, ANALOG_MAX);
    }
    if (softpot0_min < softpot0_max) {
        softpot0_mapped = map(softpot0, softpot0_min, softpot0_max, ANALOG_MIN, ANALOG_MAX);
    }
    if (softpot1_min < softpot1_max) {
        softpot1_mapped = map(softpot1, softpot1_min, softpot1_max, ANALOG_MIN, ANALOG_MAX);
    }
}

void print_ag_raw_all() {
    // display tab-separated accel/gyro x/y/z values
    Serial.print("a/g:\t");
    Serial.print(ax); Serial.print("\t");
    Serial.print(ay); Serial.print("\t");
    Serial.print(az); Serial.print("\t");
    Serial.print(gx); Serial.print("\t");
    Serial.print(gy); Serial.print("\t");
    Serial.print(gz); Serial.print("\t");
    Serial.print("mins:\t");
    Serial.print(ax_min); Serial.print("\t");
    Serial.print(ay_min); Serial.print("\t");
    Serial.print(az_min); Serial.print("\t");
    Serial.print(gx_min); Serial.print("\t");
    Serial.print(gy_min); Serial.print("\t");
    Serial.print(gz_min); Serial.print("\t");
    Serial.print("maxs:\t");
    Serial.print(ax_max); Serial.print("\t");
    Serial.print(ay_max); Serial.print("\t");
    Serial.print(az_max); Serial.print("\t");
    Serial.print(gx_max); Serial.print("\t");
    Serial.print(gy_max); Serial.print("\t");
    Serial.print(gz_max); Serial.print("\t");
    Serial.println();
}

void print_ag_mapped_all() {
    // display tab-separated accel/gyro x/y/z values
    Serial.print("a/g:\t");
    Serial.print(ax_mapped); Serial.print("\t");
    Serial.print(ay_mapped); Serial.print("\t");
    Serial.print(az_mapped); Serial.print("\t");
    Serial.print(gx_mapped); Serial.print("\t");
    Serial.print(gy_mapped); Serial.print("\t");
    Serial.print(gz_mapped); Serial.print("\t");
    Serial.print("mins:\t");
    Serial.print(ax_min); Serial.print("\t");
    Serial.print(ay_min); Serial.print("\t");
    Serial.print(az_min); Serial.print("\t");
    Serial.print(gx_min); Serial.print("\t");
    Serial.print(gy_min); Serial.print("\t");
    Serial.print(gz_min); Serial.print("\t");
    Serial.print("maxs:\t");
    Serial.print(ax_max); Serial.print("\t");
    Serial.print(ay_max); Serial.print("\t");
    Serial.print(az_max); Serial.print("\t");
    Serial.print(gx_max); Serial.print("\t");
    Serial.print(gy_max); Serial.print("\t");
    Serial.print(gz_max); Serial.print("\t");
    Serial.println();
}

void print_analogs_mapped_all() {
    // display tab-separated accel/gyro x/y/z values
    Serial.print("fsr(0, 1, 2), hotpot, softpot0, softpot1:\t");
    Serial.print(fsr0_mapped); Serial.print("\t");
    Serial.print(fsr1_mapped); Serial.print("\t");
    Serial.print(fsr2_mapped); Serial.print("\t");
    Serial.print(hotpot_mapped); Serial.print("\t");
    Serial.print(softpot0_mapped); Serial.print("\t");
    Serial.print(softpot1_mapped); Serial.print("\t");
    Serial.println();
}

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
    zero_data();
    data[0] = val / 1000;
    data[1] = (val % 1000) / 100;
    data[2] = (val % 100) / 10;
    data[3] = val % 10;
}

void send_ag_mapped_all() {
    if (ax_mapped_prev != ax_mapped) {
        set_channel(AX_CHANNEL);
        set_data(ax_mapped);
        send_channel_data();
    }
    ax_prev = ax;
    ax_mapped = ax_mapped;
    if (ay_mapped_prev != ay_mapped) {
        set_channel(AY_CHANNEL);
        set_data(ay_mapped);
        send_channel_data();   
    }
    ay_prev = ay;
    ay_mapped = ay_mapped;
    if (az_mapped_prev != az_mapped) {
        set_channel(AZ_CHANNEL);
        set_data(az_mapped);
        send_channel_data();
    }
    az_prev = az;
    az_mapped = az_mapped;
    if (gx_mapped_prev != gx_mapped) {
        set_channel(GX_CHANNEL);
        set_data(gx_mapped);
        send_channel_data();
    }
    gx_prev = gx;
    gx_mapped = gx_mapped;
    if (gy_mapped_prev != gy_mapped) {
        set_channel(GY_CHANNEL);
        set_data(gy_mapped);
        send_channel_data();
    }
    gy_prev = gy;
    gy_mapped = gy_mapped;
    if (gz_mapped_prev != gz_mapped) {
        set_channel(GZ_CHANNEL);
        set_data(gz_mapped);
        send_channel_data();
    }
    gz_prev = gz;
    gz_mapped = gz_mapped;
}

void send_button() {
    if (button_prev != button) {
        set_channel(BUTTON_CHANNEL);
        set_data(button);
        send_channel_data();
    }
    button_prev = button;
}

void send_analogs_mapped_all() {
    // FSR0
    if (fsr0_mapped_prev != fsr0_mapped) {
        set_channel(FSR0_CHANNEL);
        set_data(fsr0_mapped);
        send_channel_data();
    }
    fsr0_prev = fsr0;
    fsr0_prev_mapped = fsr0_mapped;
    // FSR1
    if (fsr1_mapped_prev != fsr1_mapped) {
        set_channel(FSR1_CHANNEL);
        set_data(fsr1_mapped);
        send_channel_data();   
    }
    fsr1_prev = fsr1;
    fsr1_prev_mapped = fsr1_mapped;
    // FSR2
    if (fsr2_mapped_prev != fsr2_mapped) {
        set_channel(FSR2_CHANNEL);
        set_data(fsr2_mapped);
        send_channel_data();
    }
    fsr2_prev = fsr2;
    fsr2_prev_mapped = fsr2_mapped;   
    // HOTPOT
    if (hotpot_mapped_prev != hotpot_mapped) {
        set_channel(HOTPOT_CHANNEL);
        set_data(hotpot_mapped);
        send_channel_data();
    }
    hotpot_prev = hotpot;
    hotpot_mapped_prev = hotpot_mapped;
    // SOFTPOT0
    if (softpot0_mapped_prev != softpot0_mapped) {
        set_channel(SOFTPOT0_CHANNEL);
        set_data(softpot0_mapped);
        send_channel_data();
    }
    softpot0_prev = softpot0;
    softpot0_mapped_prev = softpot0_mapped;
    // SOFTPOT1
    if (softpot1_mapped_prev != softpot1_mapped) {
        set_channel(SOFTPOT1_CHANNEL);
        set_data(softpot1_mapped);
        send_channel_data();
    softpot1_prev = softpot1;
    softpot1_mapped_prev = softpot1_mapped;
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
        channel[0] = (char) (i / 10); channel[1] = (char) (i % 10);
        zero_data();
        send_channel_data();
    }
    delay(1000);
}

void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    // initialize serial communication
    //Serial.begin(9600);
    Serial.begin(115200);

    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    pinMode(BUTTON_CHANNEL, INPUT);
    delay(1000);
}

void loop() {
    read_accelgyro();
    read_sensors();
    // send mapped data
    send_ag_mapped_all();
    send_analogs_mapped_all();

    // print mapped data
    //print_ag_mapped_all();
    //print_analogs_mapped_all();

    delay(50);
    //serial_test_0();
    //serial_test_1();
    //delay(3000);
}
/
