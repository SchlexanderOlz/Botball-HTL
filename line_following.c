// Copyright Jonas Grubbauer 2023

#include <kipr/wombat.h> int main()
{
    //wait_for_light(4);
    while(!digital(0)){};
    int normal_speed = 30;
    int left_speed = normal_speed;
    int right_speed = normal_speed;
    int isSmall = 0;
    int handfull = 0;
    enable_servos();
    // Initialize Grabber
    set_servo_position(2, 1000);
    set_servo_position(3, 500);
    while(!digital(1)){
        int right_sensor = analog(2);
        int left_sensor = analog(1);
        if(isSmall){
            printf("WHITE MODE");
            normal_speed = 15;
            // If both sensors white
            if(right_sensor > 3000 && left_sensor > 3000){
                isSmall = 0;
            }
            if (right_sensor > 3000){
                // Search for re-entry. Turn left and right and try to find black
                left_speed /= 1.01;
                right_speed *= 1.02;
            }
            else if(left_sensor > 3000){
                right_speed /= 1.01;
                left_speed *= 1.02;
            }
            else{
                right_speed = normal_speed;
                left_speed = normal_speed;
            }
        }
        else{
        if(right_sensor < 3000 && left_sensor < 3000){
            isSmall = 1;
        }
        else if (right_sensor < 3000){
            // On white
            // Search for re-entry. Turn left and right and try to find black
            left_speed *= 1.04;
            right_speed /= 1.02;
        }
        else if(left_sensor < 3000){
            right_speed *= 1.04;
            left_speed /= 1.02;
        }
        else{
            right_speed = normal_speed;
            left_speed = normal_speed;
        }
        }
        msleep(10);
        // Normalize Speed
        if(left_speed > 100){
            left_speed = 100;
        }
        if (right_speed > 100){
            right_speed = 100;
        }
        // IF DETECTION OF A OBJECT
        if(analog(3) > 2600 && handfull == 0){
            motor(0, -50);
            motor(1, -50);
            msleep(500);
            motor(0, 10);
            motor(1, 10);
            printf("Detected our cool thingy");
            // GRAB AND LIFT OBJECT
            set_servo_position(2, 1900);
            msleep(2000);
            handfull = 1;
            set_servo_position(3, 1700);
            motor(0, -10);
            motor(1, -10);
            msleep(2000);
            set_servo_position(2, 1100);
            motor(0, 0);
            motor(1, 0);
            msleep(500);
        }
        // Actually Drive the Motors
        motor(0, right_speed);
        motor(1, left_speed);
    }
    disable_servos();
    ao();
    return 0;
}
