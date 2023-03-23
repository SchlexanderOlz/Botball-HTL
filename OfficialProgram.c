#include <kipr/wombat.h>
#include <time.h>

#define true 1;
#define false 0;


const int right_wheel = 0;
const int left_wheel = 1;
const int grabber_base = 2;

const int grabber_up_down = 0; // Servo which is directly at the chain which move up and down
const int grabber_left_right = 1; // Servo which opens and closes the grabber

const int light_sensor = 2;

double map(double x, double in_min, double in_max, double out_min, double out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void drive(int distance_cm, int speed, int angle){
    float right_speed = speed;
    float left_speed = speed;
    double mp = map(speed, 50, 100, 2, 1);
    float direction = get_compass_angle();
    int start_seconds = (int)time(NULL);
    while(start_seconds + (62.5 * distance_cm * mp /1000)  > (int)time(NULL)){
        if (get_compass_angle() > direction){
            // Correct angle
            right_speed *= 1.1;
        }
        else{
            left_speed *= 1.1;
        }
        motor(right_wheel, right_speed);
        motor(left_wheel, left_speed);
    }
    printf("%f", mp);
    //msleep(62.5 * distance_cm * mp);
    motor(right_wheel, 0);
    motor(left_wheel, 0);
}
void turnDegrees(int degrees, int speed){
    double degs_start = fabs(get_compass_angle() * (180 / 3.1415));
    printf("%f\n", degs_start);
    motor(right_wheel, 10);
    motor(left_wheel, -10);
    while(degs_start + degrees > fabs(get_compass_angle() * (180 / 3.1415))){
        printf("%f\n", fabs(get_compass_angle() * (180 / 3.1415)));
        continue;
    };
    motor(right_wheel, 0);
    motor(left_wheel, 0);
}

void driveToBlackLine(int speed){
    while(analog(0) < 3300 && analog(1) < 3300){
        motor(right_wheel, speed);
        motor(left_wheel, speed);
    }
    motor(right_wheel, 0);
    motor(left_wheel, 0);
}

int main()
{
    //wait_for_light(light_sensor); // Wait on port 2
    //shut_down_in(119); // Stops the robot after 119 seconds
    motor(right_wheel, 20);
    motor(left_wheel, -20);
    calibrate_compass();
    drive(5, 50, 0);
    drive(5, 50, 90);
    drive(10, 50, 180);
    return 0;
}

void move_grabber()
{
	motor(grabber_base, 100);
    msleep(5000); // Time to move to top
    motor(grabber_base, 0);
    set_servo_position(grabber_left_right, 900);
    msleep(500);
    set_servo_position(grabber_up_down, 1100); // Adjust values so this actually puts all servos into a half opened mode
	msleep(300);
}
