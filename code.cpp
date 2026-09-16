#include <cstdint>
#include <raylib.h>
#include <iostream>
#include <cmath>
#include <cstdlib>

typedef int32_t i32;
typedef float f32;

constexpr i32 WIDTH{700};
constexpr i32 HEIGHT{500};
constexpr i32 ITERATIONS{12};
constexpr f32 POWER{8.0};

struct vec3 {
    i32 x, y, z;
};

f32 mandelblub(vec3 &pos) {
    f32 dr = 1.0;
    f32 r = 0.0;
    for (i32 i = 0; i < ITERATIONS; ++i) {
        r = length(vec3.z);
    }
}

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
