#include "raylib.h"
#include <cstdint>

typedef int32_t i32; 
typedef float f32;

int main()
{
    const i32 WIDTH = 1000;
    const i32 HEIGHT = 700;
    InitWindow(WIDTH, HEIGHT, "Mandelbulb");
    SetTargetFPS(60);

    // for rendering mandelbulb on the gpu (which is required for the infite details, we write the OpenGL Shading Language Code in a Raw String Literal) -> first time for me using this 
const char *fragmentShader = R"(#version 330
in vec2 fragTexCoord;
out vec4 finalColor;
uniform vec2 resolution;
uniform float time;

float mandelbulb(vec3 pos)
{
    vec3 z = pos;
    float dr = 1.0;
    float r = 0.0;

    const int ITERATIONS = 12;
    const float POWER = 8.0;

    for (int i = 0; i < ITERATIONS; i++)
    {
        r = length(z);
        if (r > 2.0) break;
        if (r < 0.00001) continue; // Division durch Null verhindern
        float theta = acos(clamp(z.z / r, -1.0, 1.0));
        float phi = atan(z.y, z.x);
        dr = pow(r, POWER - 1.0) * POWER * dr + 1.0;
        float zr = pow(r, POWER);
        theta *= POWER;
        phi *= POWER;
        z = zr * vec3(sin(theta) * cos(phi),sin(phi) * sin(theta),cos(theta));
        z += pos;
    }
    if (r < 0.00001) return 0.0;
    return 0.5 * log(r) * r / dr;
}

float rayMarch(vec3 ro, vec3 rd)
{
    float totalDistance = 0.0;
    for (int i = 0; i < 150; i++)
    {
        vec3 p = ro + rd * totalDistance;
        float distance = mandelbulb(p);
        if (isnan(distance)) break;
        if (distance < 0.001) return totalDistance;
        totalDistance += distance;
        if (totalDistance > 20.0) break;
    }
    return -1.0;
}

vec3 getNormal(vec3 p)
{
    const float eps = 0.001;
    float dx = mandelbulb(p + vec3(eps, 0.0, 0.0)) - mandelbulb(p - vec3(eps, 0.0, 0.0));
    float dy = mandelbulb(p + vec3(0.0, eps, 0.0)) - mandelbulb(p - vec3(0.0, eps, 0.0));
    float dz = mandelbulb(p + vec3(0.0, 0.0, eps)) - mandelbulb(p - vec3(0.0, 0.0, eps));
    return normalize(vec3(dx, dy, dz));
}

void main()
{
    vec2 uv = (fragTexCoord * 2.0 - 1.0);
    uv.x *= resolution.x / resolution.y;
    float angle = time * 0.25;
    vec3 ro = vec3(4.0 * sin(angle), 2.0, 4.0 * cos(angle));
    vec3 target = vec3(0.0);
    vec3 forward = normalize(target - ro);
    vec3 right = normalize(cross(forward, vec3(0.0, 1.0, 0.0)));
    vec3 up = cross(right, forward);
    vec3 rd = normalize(
        forward +
        uv.x * right * 1.2 +
        uv.y * up * 1.2
    );
    float distance = rayMarch(ro, rd);
    vec3 color = vec3(0.02, 0.04, 0.08);
    if (distance > 0.0)
    {
        vec3 p = ro + rd * distance;
        vec3 normal = getNormal(p);
        vec3 lightPos = vec3(4.0, 5.0, 3.0);
        vec3 lightDir = normalize(lightPos - p);
        float diffuse = max(dot(normal, lightDir), 0.0);
        float ambient = 0.15;
        vec3 reflected = reflect(-lightDir, normal);
        float specular = pow(max(dot(reflected, -rd), 0.0), 32.0);
        color = vec3(0.15, 0.35, 0.8) * diffuse;
        color += vec3(0.03, 0.05, 0.1) * ambient;
        color += vec3(0.5, 0.7, 1.0) * specular;
        color *= 1.0 + 0.15 * sin(p.y * 8.0);
    }
    finalColor = vec4(color, 1.0);
}
)";
    Texture2D texture = LoadTextureFromImage(GenImageColor(1, 1, WHITE));

    Shader shader = LoadShaderFromMemory(nullptr, fragmentShader);
    if (!IsShaderValid(shader)) TraceLog(LOG_ERROR, "Could LoadShader!");
    i32 resolutionLoc = GetShaderLocation(shader, "resolution");
    i32 timeLoc = GetShaderLocation(shader, "time");
    Vector2 resolution = {(f32)WIDTH, (f32)HEIGHT};
    while (!WindowShouldClose()) {
        f32 time = GetTime();
        SetShaderValue(shader, resolutionLoc, &resolution, SHADER_UNIFORM_VEC2);
        SetShaderValue(shader, timeLoc, &time, SHADER_UNIFORM_FLOAT);
        BeginDrawing();
        ClearBackground(BLACK);
        BeginShaderMode(shader);
        DrawTexturePro(texture,(Rectangle){ 0, 0, 1, 1 },(Rectangle){ 0,0, (f32)WIDTH, (f32)HEIGHT},(Vector2){ 0, 0 }, 0.0f, WHITE);
        TraceLog(LOG_INFO, "resolutionLoc: %d, timeLoc: %d", resolutionLoc, timeLoc);
        EndShaderMode();
        DrawText("Mandelblub", 20, 20, 20, WHITE);
        DrawText("ESC for exit", 20, 45, 16, GRAY);
        EndDrawing();
    }
    UnloadShader(shader);
    CloseWindow();
    return 0;
}
