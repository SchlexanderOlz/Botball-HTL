#include <kipr/wombat.h>
#include <time.h>
#include <unistd.h>

const float straightF = 1.010;

const int right_wheel = 0;
const int left_wheel = 1;
const int grabber_base = 2;

const int grabber_up_down = 0; // Servo which is directly at the chain which move up and down
const int grabber_left_right = 1; // Servo which opens and closes the grabber

const int light_sensor = 2;
const int touch_sensor = 0;

float sensor_avg = 3200;

int gmpc_left;
int gmpc_right;

double map(double x, double in_min, double in_max, double out_min, double out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void stopAtBlackLine(){
    while(analog(0) < sensor_avg && analog(1) < sensor_avg){
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

void rotateToB_L(){
    //cmpc(left_wheel);
    //cmpc(right_wheel);
    mav(left_wheel,300*straightF);
    mav(right_wheel,-300/straightF);
    while(analog(0) < sensor_avg){ //rechter Sensor
    }
    alloff();
    msleep(200);
}


void FollowLine(int speed, int distance_cm){
    cmpc(0);
    float left_speed = speed;
    float right_speed = speed;
    while(gmpc(0) < distance_cm*82){
        int left_sensor = analog(0);
        int right_sensor = analog(1);
        if (right_sensor < sensor_avg){
            // Search for re-entry. Turn left and right and try to find black
            left_speed += 0.75;
        }
        else if(left_sensor < sensor_avg){
            right_speed += 0.75;
        }
        else{
            right_speed = speed;
            left_speed = speed;
        }
        // Actually Drive the Motors
        motor(0, right_speed);
        motor(1, left_speed);
        msleep(100);
    }
}

void mavstraightf(int speed_incr){
    cmpc(left_wheel);
    cmpc(right_wheel);
    //float straight = 1.015;  //0.9->fC$hrt rechts, 1.1->fC$hrt links
    size_t i;
    for(i = 1; i < speed_incr+1; i++){
        mav(left_wheel,(100*straightF)*i);
        mav(right_wheel,(100/straightF)*i);
        msleep(100);
    }
}

void mavstraightb(int speed_incr){
    cmpc(left_wheel);
    cmpc(right_wheel);
    //float straight = 1.015;  //0.9->fC$hrt rechts, 1.1->fC$hrt links
    size_t i;
    for(i = 1; i < speed_incr+1; i++){
        mav(left_wheel,(-100*straightF)*i);
        mav(right_wheel,(-100/straightF)*i);
        msleep(100);
    };
}

void stopAt(int cm){
    while(abs((gmpc(left_wheel)+gmpc(left_wheel))/2.00)<(cm*82-82*3)){}
    if(gmpc(left_wheel)<0){
        mav(left_wheel,-500*straightF);
        mav(right_wheel,-500/straightF);
        msleep(200);
    }
    else{
        mav(left_wheel,500*straightF);
        mav(right_wheel,500/straightF);
        msleep(200);    
    }
    while(abs((gmpc(left_wheel)+gmpc(left_wheel))/2.00)<(cm*82)){}
    alloff();
}

void stopAtSensor(){
    while(digital(touch_sensor) == 0){};
    alloff();
}

void turnright90(){
    cmpc(left_wheel);
    cmpc(right_wheel);
    mav(left_wheel,-500*straightF);
    mav(right_wheel,500/straightF);
    while(gmpc(left_wheel)>(-13.42*82)){
    }
    alloff();
}

void turnleft90(){
    cmpc(left_wheel);
    cmpc(right_wheel);
    mav(left_wheel,500*straightF);
    mav(right_wheel,-500/straightF);
    while((-gmpc(left_wheel))>(-13.42*82)){
    }
    alloff();
}

int main()
{
    //while(analog(2)<1000){
    //printf("%f", analog(2));
    //}
    wait_for_light(light_sensor); // Wait on port 2
    shut_down_in(119); // Stops the robot after 119 seconds


    //sensor_avg = ((analog(0) + analog(1)) / 2) + 800;
    printf("%f", sensor_avg);
    // Enable and reset servos
    enable_servos();
    set_servo_position(2,1213);
    set_servo_position(grabber_up_down, 0);
    set_servo_position(1, 1800);
    cmpc(grabber_base);

    // Move out of starting box to new box
    mavstraightf(3);
    stopAt(5);
    turnleft90();
    msleep(1000);
    mavstraightf(30);
    stopAtBlackLine();
    mavstraightf(5);
    stopAt(32);
    msleep(1000);
    turnright90();
    msleep(1000);

    // Start To Align to schieb the balls
    mavstraightb(4);
    stopAt(7);
    // Foa den Collector owi
    set_servo_position(2, 105);
    msleep(500);

    mavstraightf(3);
    stopAtBlackLine();
    msleep(1000);
    mavstraightf(10);
    stopAt(16);
    msleep(1000);

    turnright90();



    // Vuigas
    FollowLine(50, 150);
    motor(0, 70);
    motor(1, 50);
    msleep(2000);
    motor(0, -70);
    motor(1, -50);
    msleep(2000);
    motor(0, 0);
    motor(1, 0);
    // Jez hat er alle bC$lle in die area geschoben
    set_servo_position(2,1213);
    msleep(700);
    turnleft90();
    msleep(500);
    turnleft90();
    // JEZ FAHRT ER RICHTUNG HOME AUF DER LINIE
    FollowLine(50, 74);
    turnleft90();
    mavstraightb(5);
    stopAt(34);
    // Jez steht er bissl vor dem tower
    move_to_position(grabber_base, 1000, 7650);
    bmd(grabber_base);
    // Vor und hinten herumheber
    set_servo_position(grabber_left_right, 600);
    msleep(1000);
    // Zuschnapper
    set_servo_position(grabber_up_down,956);
    msleep(700);
    mavstraightb(3);
    stopAt(8);
    // Greif zu
    set_servo_position(grabber_left_right, 1600);
    msleep(1000);
    // HEB AUF NACHM ANGREIFEN
    set_servo_position(grabber_up_down, 0);
    // FOA UMI UM DEN HINZULEGEN
    msleep(2000);
    mavstraightf(5);
    stopAt(20);
    turnleft90();
    msleep(500);
    turnleft90();
    msleep(500);
    rotate(45, 500);
    mavstraightb(5);
    stopAt(35);
    // Dann kann er wC<rfel absetzen
    move_to_position(grabber_base, -1000, 0);
    bmd(grabber_base);
    // Los den cube lous du doudl
    set_servo_position(grabber_up_down, 956);
    msleep(1000);
    set_servo_position(grabber_left_right, 1000);
    msleep(1000);
    set_servo_position(grabber_up_down, 0);
    msleep(1000);
    set_servo_position(1, 1800);
    msleep(1000);
    //mavstraightf(5); //wenn gebraucht
    //mavstraightb(5);
    //stopAt(20);
    ao();
    return 0;
    rotateToB_L();


    //new cube
    FollowLine(40, 30);
    msleep(700);
    turnleft90();
    msleep(700);
    mavstraightf(5);
    stopAt(8);
    msleep(700);
    turnleft90();
    mavstraightf(5);
    stopAtBlackLine();
    msleep(700);
    mavstraightf(5);
    stopAt(15);
    msleep(700);
    turnleft90();
    //new pos
    mavstraightb(5);
    stopAt(20);
    //before cube


    // Jez steht er bissl vor dem tower
    move_to_position(grabber_base, 1000, 7600);
    bmd(grabber_base);
    // Vor und hinten herumheber
    set_servo_position(grabber_left_right, 600);
    msleep(1000);
    // Zuschnapper
    set_servo_position(grabber_up_down,1000);
    msleep(700);
    mavstraightb(3);
    stopAt(8);
    // Greif zu
    set_servo_position(grabber_left_right, 1600);
    msleep(1000);
    // HEB AUF NACHM ANGREIFEN
    set_servo_position(grabber_up_down, 0);
    // FOA UMI UM DEN HINZULEGEN
    msleep(5000);
    mavstraightf(5);
    stopAt(20);
    turnleft90();
    msleep(500);
    turnleft90();
    move_to_position(grabber_base, -1000, 0);
    bmd(grabber_base);
    msleep(1000);
    //vll wieda zu box fahren




    //
    set_servo_position(grabber_up_down, 1000);
    msleep(1000);
    set_servo_position(grabber_left_right, 1000);
    msleep(1000);
    set_servo_position(grabber_up_down, 0);
    msleep(1000);
    set_servo_position(1, 1800);
    msleep(1000);
    alloff();
    return 0;
}
