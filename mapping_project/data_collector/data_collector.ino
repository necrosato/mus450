
// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

#define ESP32MPU
#define ESP32

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
#endif
int16_t ax_mapped = 0;
int16_t ay_mapped = 0;
int16_t az_mapped = 0;
int16_t gx_mapped = 0;
int16_t gy_mapped = 0;
int16_t gz_mapped = 0;

#define ANALOG_MIN 0
#ifdef ESP32
    #define ANALOG_MAX 4095
#else
    #define ANALOG_MAX 1023
#endif
#define FSR0_CHANNEL 15     // nut fsr
#define FSR1_CHANNEL 2      // pick fsr
#define FSR2_CHANNEL 4      // neck fsr
#define HOTPOT_CHANNEL 5    // HotPot
#define SOFTPOT0_CHANNEL 18 // SoftPot leads to head
#define SOFTPOT1_CHANNEL 18 // SoftPot leads to body
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

char channel[2] = {0};
char data[4] = {0};

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
    set_channel(AX_CHANNEL);
    set_data(ax_mapped);
    send_channel_data();
    set_channel(AY_CHANNEL);
    set_data(ay_mapped);
    send_channel_data();   
    set_channel(AZ_CHANNEL);
    set_data(az_mapped);
    send_channel_data();
    set_channel(GX_CHANNEL);
    set_data(gx_mapped);
    send_channel_data();
    set_channel(GY_CHANNEL);
    set_data(gy_mapped);
    send_channel_data();
    set_channel(GZ_CHANNEL);
    set_data(gz_mapped);
    send_channel_data();
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
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    // initialize serial communication
    Serial.begin(9600);
    //Serial.begin(115200);

    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    delay(1000);
}

void loop() {
    // read raw accel/gyro measurements from device
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    get_min_max_accel();
    get_min_max_gyro();
    map_accel();
    map_gyro();
    //print_ag_raw_all();
    print_ag_mapped_all();
    //send_ag_mapped_all();

    //int fsr_val = map(analogRead(A0), 0, 1024, 0, 127);
    //set_channel(1);
    //set_data(fsr_val);
    //send_channel_data();

    delay(50);
    //serial_test_0();
    //serial_test_1();
    //delay(3000);
}
