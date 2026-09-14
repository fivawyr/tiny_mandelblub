#include <cstdint>
#include <raylib.h>
#include <iostream>
#include <cmath>
#include <cstdlib>

typedef int32_t i32;
typedef float f32;

constexpr i32 WIDTH{700};
constexpr i32 HEIGHT{500};


int main() {
    InitWindow(WIDTH, HEIGHT, "Perlin Noise");
    SetTargetFPS(60);
    
    while (!WindowShouldClose()) {
        
        BeginDrawing();
        ClearBackground(BLACK);



        EndDrawing();
    }
    CloseWindow();
    return 0;
}
