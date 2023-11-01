#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#define LEFT 1
#define RIGHT 2
#define UP 3
#define DOWN 4

int house[10][20] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

struct Motor{
    bool move_forward;
    bool move_backward;
};

struct obstacleData 
{
    int left;
    int right;
    int front;
};

struct Cleaner{
    bool on_off;
    bool power_up;
};

struct RVC {
    bool is_front_obs;
    bool is_left_obs;
    bool is_right_obs;
    bool is_backing;
    bool is_dust;
    int pos_x;
    int pos_y;
    int direction;
    struct Motor motor;
    struct Cleaner cleaner;
};

void rvc_init(struct RVC *rvc) {
    rvc->is_front_obs = false;
    rvc->is_left_obs = false;
    rvc->is_right_obs = false;
    rvc->is_dust = false;
    rvc->is_backing = false;

    rvc->motor.move_forward = false;
    rvc->motor.move_backward = false;
    
    rvc->cleaner.on_off = false;
    rvc->cleaner.power_up = false;
    rvc->direction = UP; //일단 시작 방향 위로 설정
    rvc->pos_x = 8;
    rvc->pos_y = 1; //임의로 시작 지점 설정
}

void move_forward(struct RVC *rvc, bool enable) {
    if(enable){
        if(rvc->direction == UP){
            rvc->pos_x--;
        }
        else if(rvc->direction == DOWN){
            rvc->pos_x++;
        }
        else if(rvc->direction == LEFT){
            rvc->pos_y--;
        }
        else if(rvc->direction == RIGHT){
            rvc->pos_y++;
        }
    }
}

void move_backward(struct RVC *rvc, bool enable){
    if(enable){
       if(rvc->direction == UP){
            rvc->pos_x++;
        }
        else if(rvc->direction == DOWN){
            rvc->pos_x--;
        }
        else if(rvc->direction == LEFT){
            rvc->pos_y++;
        }
        else if(rvc->direction == RIGHT){
            rvc->pos_y--;
        }
    }
}

void turn_left(struct RVC *rvc) {
    if(rvc->motor.move_forward){
        rvc->motor.move_forward = false;
    }
    if(rvc->motor.move_backward){
        rvc->motor.move_backward = false;
    }
    if(rvc->direction == UP){
        rvc->direction = LEFT;
    }
    else if(rvc->direction == LEFT){
        rvc->direction = DOWN;
    }
    else if(rvc->direction == DOWN){
        rvc->direction = RIGHT;
    }
    else if(rvc->direction == RIGHT){
        rvc->direction = UP;
    }
    sleep(1); //tick * 5
}

void turn_right(struct RVC *rvc) {
    if(rvc->motor.move_forward){
        rvc->motor.move_forward = false;
    }
    if(rvc->motor.move_backward){
        rvc->motor.move_backward = false;
    }
    if(rvc->direction == UP){
        rvc->direction = RIGHT;
    }
    else if(rvc->direction == LEFT){
        rvc->direction = UP;
    }
    else if(rvc->direction == DOWN){
        rvc->direction = LEFT;
    }
    else if(rvc->direction == RIGHT){
        rvc->direction = DOWN;
    }
    sleep(1); //tick * 5
}

void set_obstacle(int direction, int pos_x, int pos_y, struct obstacleData *ob){
    if(direction == UP){
        ob->front = house[pos_x-1][pos_y];
        ob->left = house[pos_x][pos_y-1];
        ob->right = house[pos_x][pos_y+1];
    }
    else if(direction == DOWN){
        ob->front = house[pos_x+1][pos_y];
        ob->left = house[pos_x][pos_y+1];
        ob->right = house[pos_x][pos_y-1];
    }
    else if(direction == LEFT){
        ob->front = house[pos_x][pos_y-1];
        ob->left = house[pos_x+1][pos_y];
        ob->right = house[pos_x-1][pos_y];
    }
    else if(direction == RIGHT){
        ob->front = house[pos_x][pos_y+1];
        ob->left = house[pos_x-1][pos_y];
        ob->right = house[pos_x+1][pos_y];
    }
}

void rvc_detect(struct RVC *rvc, struct obstacleData *ob) {
    if(ob->front == 1){
        rvc->is_front_obs = true;
    }
    if(ob->left == 1){
        rvc->is_left_obs = true;
    }
    if(ob->right == 1){
        rvc->is_right_obs = true;
    }

    if(rvc->is_front_obs && !rvc->is_left_obs){
        turn_left(rvc);
        printf("turn left at : (%d, %d)\n", rvc->pos_x, rvc->pos_y);
    }
    else if(rvc->is_front_obs && rvc->is_left_obs && !rvc->is_right_obs){
        turn_right(rvc);
        printf("turn right at : (%d, %d)\n", rvc->pos_x, rvc->pos_y);
    }
    else if(rvc->is_front_obs && rvc->is_left_obs && rvc->is_right_obs){
        rvc->motor.move_backward = true;
        rvc->is_backing = true;
        move_backward(rvc, rvc->motor.move_backward);
        printf("back start at : (%d, %d)\n", rvc->pos_x, rvc->pos_y);
    }
    else if(rvc->is_backing){
        if (!rvc->is_left_obs) {
        // 왼쪽으로 회전하고 후진 상태를 종료합니다.
        turn_left(rvc);
        rvc->is_backing = false;
        } 
        else if (!rvc->is_right_obs) {
        // 오른쪽으로 회전하고 후진 상태를 종료합니다.
        turn_right(rvc);
        rvc->is_backing = false;
        } 
        else {
        // 계속 후진합니다.
        move_backward(rvc, rvc->motor.move_backward);
        }
    }
    else if(!rvc->is_front_obs){
        rvc->motor.move_forward = true;
        move_forward(rvc, rvc->motor.move_forward);
    }
}

int main(void) {
    struct RVC rvc;
    rvc_init(&rvc);
    struct obstacleData ob;
    int timeout = 500;
    while(timeout>0){
        set_obstacle(rvc.direction, rvc.pos_x, rvc.pos_y, &ob);
        rvc_detect(&rvc, &ob);
        rvc.is_front_obs = false;
        rvc.is_left_obs = false;
        rvc.is_right_obs = false;
        timeout -= 1;
        //sleep(1); //tick = 1
    }
}