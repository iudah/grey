#include "raylib.h"

int main(void){
  InitWindow(800, 450, "Grey Engine: M0");
  SetTargetFPS(60);

  while(!WindowShouldClose()){
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("M0: Hello!", 100, 200, 20, RED);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}

