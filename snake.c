#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <stdint.h>
#include <math.h>
#include <unistd.h>

#include "raylib.h"

Vector2 random_coords(int max_x, int max_y){
    Vector2 coords;
    coords.x = rand() % max_x;
    coords.y = rand() % max_y;

    return coords;
}

int main(){
    srand(time(NULL));

    int width = 1200;
    int height = 800;

    int grid_width = 40;
    int grid_height = 30;
    
    int cell_size = width / grid_width;
    while (grid_height*cell_size > height){
        cell_size--;
    }

    int top_bias = (height - cell_size*grid_height) / 2;
    int left_bias = (width - cell_size*grid_width) / 2;

    int start_snake_length = 3;
    int snake_length = start_snake_length;

    typedef struct {
        uint8_t type; //0 is body, 1 is head, 2 is tail
        int grid_x;
        int grid_y;
    } snake_piece;
    
    snake_piece snake[grid_width*grid_height]; //Preallocate on stack to dodge heap alloc

    snake[0].type = 2;
    snake[0].grid_x = grid_width / 2 - snake_length + 1;
    snake[0].grid_y = grid_height / 2;
    for (int index = 1; index < snake_length - 1; index++){
        snake[index].type = 0;
        snake[index].grid_x = grid_width / 2 - (snake_length - index - 1);
        snake[index].grid_y = grid_height / 2;
    }
    snake[snake_length - 1].type = 1;
    snake[snake_length - 1].grid_x = grid_width / 2;
    snake[snake_length - 1].grid_y = grid_height / 2;

    SetTraceLogLevel(LOG_NONE);
    InitWindow(width, height, "Snake");
  
    Vector2 apple;

    while (true){
        apple = random_coords(grid_width, grid_height);   

        bool good = true;
        for (int index = 0; index < snake_length; index++){
            if (apple.x == snake[index].grid_x && apple.y == snake[index].grid_y){
                good = false;
            }
        }

        if (good){
            break;
        }
    }
    
    Color bg = {25, 25, 25, 255};

    int last_xModify = 1;
    int last_yModify = 0;

    float timeAccumulate = 0;

    while (!WindowShouldClose()){
        BeginDrawing();

        ClearBackground(bg);
        for (int y = 0; y < grid_height; y++){
            for (int x = 0; x < grid_width; x++){
                Color colortouse = BLACK;
                
                if (apple.x == x && apple.y == y){
                    colortouse = RED;
                }
                
                for (int index = 0; index < snake_length; index++){
                    if (snake[index].grid_x == x && snake[index].grid_y == y){
                        colortouse = GREEN;
                    }
                }
                
                DrawRectangle(cell_size*x + left_bias, cell_size*y + top_bias, cell_size, cell_size, colortouse);

                char text[64];
                sprintf(text, "Score: %d", snake_length - 3);
                int pad = sqrtf(width*height) / 64;
                DrawText(text, pad + left_bias, pad + top_bias, sqrtf(width*height) / 25, WHITE);
            }
        }

        int xModify = 0;
        int yModify = 0;
        
        if (IsKeyDown(KEY_LEFT)){
           xModify--; 
        }
        else{
            if (IsKeyDown(KEY_RIGHT)){
                xModify++;
            }
            else{
                if (IsKeyDown(KEY_UP)){
                    yModify--;
                }
                else{
                    if (IsKeyDown(KEY_DOWN)){
                        yModify++;
                    }
                    else{
                        xModify = last_xModify;
                        yModify = last_yModify;
                    }
                }
            }
        }

        last_xModify = xModify;
        last_yModify = yModify;

        if (timeAccumulate > 0.25 - 0.025){ //Remove the usleep from the delay
            snake[snake_length].type = 1;
            snake[snake_length].grid_x = snake[snake_length - 1].grid_x + xModify;
            snake[snake_length].grid_y = snake[snake_length - 1].grid_y + yModify;
            snake[snake_length - 1].type = 0;
            
            if (!(snake[snake_length - 1].grid_x == apple.x && snake[snake_length - 1].grid_y == apple.y)){
                snake[1].type = 2;
                for (int index = 1; index < snake_length + 1; index++){
                    snake[index - 1] = snake[index];
                }
            }
            else{
                snake_length++;

                while (true){
                    apple = random_coords(grid_width, grid_height);   

                    bool good = true;
                    for (int index = 0; index < snake_length; index++){
                        if (apple.x == snake[index].grid_x && apple.y == snake[index].grid_y){
                            good = false;
                        }
                    }

                    if (good){
                        break;
                    }
                }
            }
            for (int index = 0; index < snake_length; index++){
                for (int subindex = 0; subindex < snake_length; subindex++){
                    if (subindex == index){
                        continue;
                    }
                    if (snake[index].grid_x == snake[subindex].grid_x && snake[index].grid_y == snake[subindex].grid_y){
                        goto gameover;
                    }
                }

                if (snake[index].grid_x > grid_width || snake[index].grid_x < 0 || snake[index].grid_y > grid_height || snake[index].grid_y < 0){
                    gameover:
                    printf("Game over, score=%d\n", snake_length-start_snake_length);
                    snake_length = start_snake_length;
                    
                    snake[0].type = 2;
                    snake[0].grid_x = grid_width / 2 - snake_length + 1;
                    snake[0].grid_y = grid_height / 2;
                    for (int index = 1; index < snake_length - 1; index++){
                        snake[index].type = 0;
                        snake[index].grid_x = grid_width / 2 - (snake_length - index - 1);
                        snake[index].grid_y = grid_height / 2;
                    }
                    snake[snake_length - 1].type = 1;
                    snake[snake_length - 1].grid_x = grid_width / 2;
                    snake[snake_length - 1].grid_y = grid_height / 2;
                    last_xModify = 1;
                    last_yModify = 0;
                    while (true){
                        apple = random_coords(grid_width, grid_height);   

                        bool good = true;
                        for (int index = 0; index < snake_length; index++){
                            if (apple.x == snake[index].grid_x && apple.y == snake[index].grid_y){
                                good = false;
                            }
                        }

                        if (good){
                            break;
                        }
                    }
                }
            }
            timeAccumulate = 0;
        }

        timeAccumulate += GetFrameTime();

        EndDrawing();
        
        usleep(25 * 1000); //25ms
    }

    CloseWindow();
    return 0;
}
