#include <kipr/wombat.h>

const int right_wheel = 0;
const int left_wheel = 1;
const int grabber_base = 2;

const int grabber_up_down = 0; // Servo which is directly at the chain which move up and down
const int grabber_left_right = 1; // Servo which opens and closes the grabber

const int light_sensor = 2;

double map(double x, double in_min, double in_max, double out_min, double out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void drive(int distance_cm, int speed){
    cmpc(1);
    cmpc(right_wheel);
    motor(0, speed);
    motor(1, speed);
    while(1){
        if(get_motor_position_counter(0) >= 82*distance_cm)
            motor(0, 0);
        if(get_motor_position_counter(1) >= 82*distance_cm)
            motor(0, 0);
        if(get_motor_position_counter(1) >= 82*distance_cm && get_motor_position_counter(0) >= 82*distance_cm)
            break;
    }
    freeze(1);
    freeze(0);
    msleep(500);
}

void driveToBlackLine(int speed){
    while(analog(0) < 3300 && analog(1) < 3300){
        motor(right_wheel, speed);
        motor(left_wheel, speed);
    }
    motor(right_wheel, 0);
    motor(left_wheel, 0);
}

void rotate(int degrees, int speed){
    cmpc(1);
    cmpc(0);
	mtp(left_wheel, speed*0.8, degrees*12.5);
    mtp(right_wheel, speed*0.8, -degrees*12.5);
    bmd(left_wheel);
    bmd(right_wheel);
    freeze(left_wheel);
    freeze(right_wheel);
    msleep(500);
}

void FollowLine(int speed, int distance_cm){
    int normal_speed = 30;
    cmpc(0);
    int left_speed = normal_speed;
    int right_speed = normal_speed;
    while(gmpc(0) < distance_cm*82){
        int right_sensor = analog(2);
        int left_sensor = analog(1);
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
        // Actually Drive the Motors
        motor(0, right_speed);
        motor(1, left_speed);
    }
}

int main()
{
    // Range from 0 to 1500
    int speed = 1200;
    int normal_speed = map(speed, 0, 1500, 0, 100);
    //wait_for_light(light_sensor); // Wait on port 2
    //shut_down_in(119); // Stops the robot after 119 seconds
    drive(20, 
    return 0;
    driveToBlackLine(normal_speed);
    rotate(-10, speed);
    FollowLine(normal_speed, 50);
    return 0;
}
