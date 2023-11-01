#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

struct Motor{
    bool move_forward;
    bool move_backward;
};

struct Cleaner{
    bool on_off;
    bool power_up;
};

struct RVC {
    bool is_front_obs;
    bool is_left_obs;
    bool is_right_obs;
    bool is_dust;
    int pos_x;
    int pos_y;
    struct Motor motor;
    struct Cleaner cleaner;
};

void rvc_init(struct RVC *rvc) {
    rvc->is_front_obs = false;
    rvc->is_left_obs = false;
    rvc->is_right_obs = false;
    rvc->is_dust = false;

    rvc->motor.move_forward = false;
    rvc->motor.move_backward = false;
    
    rvc->cleaner.on_off = false;
    rvc->cleaner.power_up = false;
    //시작 pos 설정
}

void move_forward(struct RVC *rvc, bool enable) {
    if(enable){
        rvc->motor.move_forward = true;
    }
    else{
        rvc->motor.move_forward = false;
    }
    printf("move forward\n");
}

void move_backward(struct RVC *rvc, bool enable){
    if(enable){
        rvc->motor.move_backward = true;
    }
    else{
        rvc->motor.move_backward = false;
    }
    printf("move backward\n");    
}

void turn_left(struct RVC *rvc) {
    // 전진하고 있을 때 좌회전
    if(rvc->motor.move_forward){
        //지평좌표계를 왼쪽으로 어떻게 틀으신거죠?
        move_forward(rvc, false);
    }else if(rvc->motor.move_backward){
        move_backward(rvc, false);
    }
}


void rvc_detect(struct RVC *rvc, char *data) {
    rvc->is_front_obs = strchr(data, 'F') != NULL;
    rvc->is_left_obs = strchr(data, 'L') != NULL;
    rvc->is_right_obs = strchr(data, 'R') != NULL;
    rvc->is_dust = strchr(data, 'D') != NULL;

    if(rvc->is_front_obs && !rvc->is_left_obs){
        turn_left(rvc);
    } else if(rvc->is_front_obs && rvc->is_left_obs && !rvc->is_right_obs){
        // turn right logic here
    } else if(!rvc->is_front_obs && !rvc->is_left_obs && !rvc->is_right_obs){
        move_backward(rvc, true);
    } else {
        move_forward(rvc, true);
    }
    // add cleaner logic here if needed
}

int main(void) {
    struct RVC rvc;
    rvc_init(&rvc);
    FILE *fp = fopen("obstacle.txt", "r");
    char data[10]; 
    if(fp == NULL){
        printf("File open error\n");
        exit(1);
    }

    while(fgets(data, sizeof(data), fp) != NULL){
        data[strcspn(data, "\n")] = 0; 
        rvc_detect(&rvc, data);
        if(rvc.motor.move_forward){
            rvc.pos_y++;
        }
        if(rvc.motor.move_backward){
            rvc.pos_y--;
        }
        sleep(1); //tick = 1
    }

    fclose(fp);
}