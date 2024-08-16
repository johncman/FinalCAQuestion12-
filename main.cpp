#include "mbed.h"
#include "LSM6DSLSensor.h"

// I2C pins for the LSM6DSL sensor 
static DevI2C devI2c(PB_11, PB_10);
static LSM6DSLSensor acc_gyro(&devI2c, 0xD4, D4, D5);

// PWM outputs for LEDs 
PwmOut led1(PA_5); // LED1 for X-axis
PwmOut led2(PA_6); // LED2 for Y-axis
PwmOut led3(PA_7); // LED3 for Z-axis

// Function to map accelerometer values to PWM duty cycle (0.0 to 1.0)
float map_accel_to_brightness(int axis_value) {
    // Define the range of accelerometer values 
    int min_val = -2000;  
    int max_val = 2000;
    
    // axis value range of 0 to 1
    float brightness = (float)(axis_value - min_val) / (float)(max_val - min_val);
    
    // brightness 0 to 1
    if (brightness < 0.0f) brightness = 0.0f;
    if (brightness > 1.0f) brightness = 1.0f;
    
    return brightness;
}

int main() {
    uint8_t id;
    int32_t axes[3];

    // Initialize the LSM6DSL sensor
    acc_gyro.init(NULL);
    acc_gyro.enable_x(); // Enable accelerometer
    

    printf("This is an accelerometer example running on Mbed OS %d.%d.%d.\n", 
           MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);

    // Read and print the sensor ID
    acc_gyro.read_id(&id);
    printf("LSM6DSL accelerometer & gyroscope = 0x%X\r\n", id);

    while (1) {
        // Accelerometer values for X, Y, and Z axes
        acc_gyro.get_x_axes(axes);

        // Map accelerometer values to LED brightness levels
        float brightness1 = map_accel_to_brightness(axes[0]); // X-axis
        float brightness2 = map_accel_to_brightness(axes[1]); // Y-axis
        float brightness3 = map_accel_to_brightness(axes[2]); // Z-axis

        // Set the brightness on LEDs
        led1.write(brightness1);
        led2.write(brightness2);
        led3.write(brightness3);

        // Print the accelerometer values and brightness levels
        printf("LSM6DSL: X=%6d, Y=%6d, Z=%6d, Brightness: LED1=%.2f, LED2=%.2f, LED3=%.2f\r\n", 
               axes[0], axes[1], axes[2], brightness1, brightness2, brightness3);

        // Delay to reduce the update rate
        thread_sleep_for(200);
    }
}
