#include "raylib.h"
#include <math.h>
#include <stdlib.h>

// --------------------------------------------------
// بدائل لدوال غير موجودة في نسختك من raylib
// --------------------------------------------------

// حساب المسافة بين متجهين
static inline float Vector2Distance(Vector2 v1, Vector2 v2) {
    float dx = v1.x - v2.x;
    float dy = v1.y - v2.y;
    return sqrtf(dx*dx + dy*dy);
}

// Clamp للـ float
static inline float Clamp(float value, float min, float max) {
    if (value < min) return min;
    else if (value > max) return max;
    else return value;
}

// Clamp للـ int
static inline int ClampInt(int value, int min, int max) {
    if (value < min) return min;
    else if (value > max) return max;
    else return value;
}

// --------------------------------------------------
// الهياكل
// --------------------------------------------------
typedef struct {
    Vector2 position;
    float mass;
} BlackHole;

typedef struct {
    Vector2 position;
    Vector2 velocity;
    Color color;
} Particle;

// --------------------------------------------------
#define NUM_PARTICLES 500
Particle particles[NUM_PARTICLES];
BlackHole blackHole;

// --------------------------------------------------
void InitAccretionParticles(void) {
    for (int i = 0; i < NUM_PARTICLES; i++) {
        float angle = (float)GetRandomValue(0, 360) * DEG2RAD;
        float radius = GetRandomValue(80, 200);

        particles[i].position = (Vector2){
            blackHole.position.x + cosf(angle) * radius,
            blackHole.position.y + sinf(angle) * radius
        };

        particles[i].velocity = (Vector2){ -sinf(angle), cosf(angle) };
        float speed = GetRandomValue(20, 50) / 10.0f;
        particles[i].velocity.x *= speed;
        particles[i].velocity.y *= speed;

        // Doppler shift بسيط للألوان
        float dopplerFactor = particles[i].velocity.x * 0.05f;
        particles[i].color = (Color){
            (unsigned char)ClampInt(200 + (int)(55 * dopplerFactor), 0, 255),
            (unsigned char)ClampInt(150 + (int)(105 * dopplerFactor), 0, 255),
            (unsigned char)ClampInt(255 - (int)(155 * dopplerFactor), 0, 255),
            255
        };
    }
}

// --------------------------------------------------
int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Black Hole Simulation (raylib)");

    blackHole.position = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
    blackHole.mass = 5000.0f;

    InitAccretionParticles();

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();

        // تحريك الثقب الأسود بالفأرة
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            blackHole.position = mousePos;
        }

        // تحديث حركة الجزيئات
        for (int i = 0; i < NUM_PARTICLES; i++) {
            Vector2 dir = {
                blackHole.position.x - particles[i].position.x,
                blackHole.position.y - particles[i].position.y
            };
            float dist = Vector2Distance(particles[i].position, blackHole.position);
            float force = blackHole.mass / (dist*dist + 100.0f);

            dir.x /= dist;
            dir.y /= dist;

            particles[i].velocity.x += dir.x * force * 0.1f;
            particles[i].velocity.y += dir.y * force * 0.1f;

            particles[i].position.x += particles[i].velocity.x;
            particles[i].position.y += particles[i].velocity.y;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        // رسم الثقب الأسود
        DrawCircleV(blackHole.position, 30, DARKPURPLE);

        // رسم الجزيئات
        for (int i = 0; i < NUM_PARTICLES; i++) {
            DrawPixelV(particles[i].position, particles[i].color);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

