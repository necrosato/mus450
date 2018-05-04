
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

MPU6050 accelgyro;
//MPU6050 accelgyro(0x69); // <-- use for AD0 high

#define MAP_MIN 0
#define MAP_MAX 127
int16_t ax = 0, ay = 0, az = 0, axmin = 0, axmax = 0, aymin = 0, aymax = 0, azmin = 0, azmax = 0;
int16_t gx = 0, gy = 0, gz = 0, gxmin = 0, gxmax = 0, gymin = 0, gymax = 0, gzmin = 0, gzmax = 0;
int16_t axmapped = 0;
int16_t aymapped = 0;
int16_t azmapped = 0;
int16_t gxmapped = 0;
int16_t gymapped = 0;
int16_t gzmapped = 0;

void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    // initialize serial communication
    Serial.begin(9600);

    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    // use the code below to change accel/gyro offset values
    /*
    Serial.println("Updating internal sensor offsets...");
    // -76	-2359	1688	0	0	0
    Serial.print(accelgyro.getXAccelOffset()); Serial.print("\t"); // -76
    Serial.print(accelgyro.getYAccelOffset()); Serial.print("\t"); // -2359
    Serial.print(accelgyro.getZAccelOffset()); Serial.print("\t"); // 1688
    Serial.print(accelgyro.getXGyroOffset()); Serial.print("\t"); // 0
    Serial.print(accelgyro.getYGyroOffset()); Serial.print("\t"); // 0
    Serial.print(accelgyro.getZGyroOffset()); Serial.print("\t"); // 0
    Serial.print("\n");
    accelgyro.setXGyroOffset(220);
    accelgyro.setYGyroOffset(76);
    accelgyro.setZGyroOffset(-85);
    Serial.print(accelgyro.getXAccelOffset()); Serial.print("\t"); // -76
    Serial.print(accelgyro.getYAccelOffset()); Serial.print("\t"); // -2359
    Serial.print(accelgyro.getZAccelOffset()); Serial.print("\t"); // 1688
    Serial.print(accelgyro.getXGyroOffset()); Serial.print("\t"); // 0
    Serial.print(accelgyro.getYGyroOffset()); Serial.print("\t"); // 0
    Serial.print(accelgyro.getZGyroOffset()); Serial.print("\t"); // 0
    Serial.print("\n");
    */
}

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

void print_raw_all() {
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

void print_mapped_all() {
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
void loop() {
    // read raw accel/gyro measurements from device
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    get_min_max_accel();
    get_min_max_gyro();
    map_accel();
    map_gyro();
    //accelgyro.getAcceleration(&ax, &ay, &az);
    //accelgyro.getRotation(&gx, &gy, &gz);
    //print_raw_all();
    print_mapped_all();
    delay(100);
}

