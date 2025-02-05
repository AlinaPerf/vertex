#include "raylib.h"

struct Player {
    Vector2 position;
    int health;
    int attack;
};

void DrawPlayer(Player player) {
    DrawRectangle(player.position.x, player.position.y, 50, 50, BLUE);
    DrawText(TextFormat("Health: %d", player.health), player.position.x, player.position.y - 20, 20, WHITE);
}

int main() {
    // Инициализация окна
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Simple RPG Game");

    // Создание игрока
    Player player = { {screenWidth / 2, screenHeight / 2}, 100, 10 };

    SetTargetFPS(60); // Установка FPS

    while (!WindowShouldClose()) {
        // Обработка ввода
        if (IsKeyDown(KEY_RIGHT)) player.position.x += 5.0f;
        if (IsKeyDown(KEY_LEFT)) player.position.x -= 5.0f;
        if (IsKeyDown(KEY_UP)) player.position.y -= 5.0f;
        if (IsKeyDown(KEY_DOWN)) player.position.y += 5.0f;

        // Логика игры (например, столкновения, бой и т.д.) может быть добавлена здесь
        
        // Отрисовка
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawPlayer(player);
        EndDrawing();
    }

    // Закрытие окна
    CloseWindow();
    return 0;
}