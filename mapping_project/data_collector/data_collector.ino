
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
#define AX_CHANNEL 2
#define AY_CHANNEL 3
#define AZ_CHANNEL 4
#define GX_CHANNEL 5
#define GY_CHANNEL 6
#define GZ_CHANNEL 7
int16_t ax = 0;
int16_t ay = 0;
int16_t az = 0;
int16_t gx = 0;
int16_t gy = 0;
int16_t gz = 0;
#ifdef AG_CONST
    int16_t axmin = AG_MIN;
    int16_t axmax = AG_MAX;
    int16_t aymin = AG_MIN;
    int16_t aymax = AG_MAX;
    int16_t azmin = AG_MIN;
    int16_t azmax = AG_MAX;
    int16_t gxmin = AG_MIN;
    int16_t gxmax = AG_MAX;
    int16_t gymin = AG_MIN;
    int16_t gymax = AG_MAX;
    int16_t gzmin = AG_MIN;
    int16_t gzmax = AG_MAX;
#else
    int16_t axmin = 0;
    int16_t axmax = 0;
    int16_t aymin = 0;
    int16_t aymax = 0;
    int16_t azmin = 0;
    int16_t azmax = 0;
    int16_t gxmin = 0;
    int16_t gxmax = 0;
    int16_t gymin = 0;
    int16_t gymax = 0;
    int16_t gzmin = 0;
    int16_t gzmax = 0;
#endif
int16_t axmapped = 0;
int16_t aymapped = 0;
int16_t azmapped = 0;
int16_t gxmapped = 0;
int16_t gymapped = 0;
int16_t gzmapped = 0;

char channel[2] = {0};
char data[4] = {0};

void get_min_max_accel() {
    axmin = min(ax, axmin);
    aymin = min(ay, aymin);
    azmin = min(az, azmin);
    axmax = max(ax, axmax);
    aymax = max(ay, aymax);
    azmax = max(az, azmax);
}

void get_min_max_gyro() {
    gxmin = min(gx, gxmin);
    gymin = min(gy, gymin);
    gzmin = min(gz, gzmin);
    gxmax = max(gx, gxmax);
    gymax = max(gy, gymax);
    gzmax = max(gz, gzmax);
}

void map_accel() {
    if (axmin < axmax) {
        axmapped = map(ax, axmin, axmax, MAP_MIN, MAP_MAX);
    }
    if (aymin < aymax) {
        aymapped = map(ay, aymin, aymax, MAP_MIN, MAP_MAX);
    }
    if (gzmin < azmax) {
        azmapped = map(az, azmin, azmax, MAP_MIN, MAP_MAX);
    }
}

void map_gyro() {
    if (gxmin < gxmax) {
        gxmapped = map(gx, gxmin, gxmax, MAP_MIN, MAP_MAX);
    }
    if (gymin < gymax) {
        gymapped = map(gy, gymin, gymax, MAP_MIN, MAP_MAX);
    }
    if (gzmin < gzmax) {
        gzmapped = map(gz, gzmin, gzmax, MAP_MIN, MAP_MAX);
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
    Serial.print(axmin); Serial.print("\t");
    Serial.print(aymin); Serial.print("\t");
    Serial.print(azmin); Serial.print("\t");
    Serial.print(gxmin); Serial.print("\t");
    Serial.print(gymin); Serial.print("\t");
    Serial.print(gzmin); Serial.print("\t");
    Serial.print("maxs:\t");
    Serial.print(axmax); Serial.print("\t");
    Serial.print(aymax); Serial.print("\t");
    Serial.print(azmax); Serial.print("\t");
    Serial.print(gxmax); Serial.print("\t");
    Serial.print(gymax); Serial.print("\t");
    Serial.print(gzmax); Serial.print("\t");
    Serial.println();
}

void print_ag_mapped_all() {
    // display tab-separated accel/gyro x/y/z values
    Serial.print("a/g:\t");
    Serial.print(axmapped); Serial.print("\t");
    Serial.print(aymapped); Serial.print("\t");
    Serial.print(azmapped); Serial.print("\t");
    Serial.print(gxmapped); Serial.print("\t");
    Serial.print(gymapped); Serial.print("\t");
    Serial.print(gzmapped); Serial.print("\t");
    Serial.print("mins:\t");
    Serial.print(axmin); Serial.print("\t");
    Serial.print(aymin); Serial.print("\t");
    Serial.print(azmin); Serial.print("\t");
    Serial.print(gxmin); Serial.print("\t");
    Serial.print(gymin); Serial.print("\t");
    Serial.print(gzmin); Serial.print("\t");
    Serial.print("maxs:\t");
    Serial.print(axmax); Serial.print("\t");
    Serial.print(aymax); Serial.print("\t");
    Serial.print(azmax); Serial.print("\t");
    Serial.print(gxmax); Serial.print("\t");
    Serial.print(gymax); Serial.print("\t");
    Serial.print(gzmax); Serial.print("\t");
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
    set_data(axmapped);
    send_channel_data();
    set_channel(AY_CHANNEL);
    set_data(aymapped);
    send_channel_data();   
    set_channel(AZ_CHANNEL);
    set_data(azmapped);
    send_channel_data();
    set_channel(GX_CHANNEL);
    set_data(gxmapped);
    send_channel_data();
    set_channel(GY_CHANNEL);
    set_data(gymapped);
    send_channel_data();
    set_channel(GZ_CHANNEL);
    set_data(gzmapped);
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
    //accelgyro.getAcceleration(&ax, &ay, &az);
    //accelgyro.getRotation(&gx, &gy, &gz);
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
