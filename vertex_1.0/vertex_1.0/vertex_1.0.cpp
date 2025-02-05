#include "raylib.h"
struct Player {
    Vector2 position;
    int health;    
    int attack;
};
enum Location {
    LOCATION_1,
    LOCATION_2, 
    LOCATION_3,
    LOCATION_4, 
    LOCATION_5,
    LOCATION_COUNT
};
void DrawPlayer(Player player) {
    DrawRectangle(player.position.x, player.position.y, 50, 50, BLUE);    
    DrawText(TextFormat("HP: %d", player.health), player.position.x, player.position.y - 20, 20, RED);
}void DrawLocation(Location location) {
    switch (location) {
    case LOCATION_1:
        ClearBackground(RAYWHITE);        
        DrawText("Location 1: The Starting Point", 10, 10, 20, BLACK);
    break;    case LOCATION_2:
        ClearBackground(GREEN);        
        DrawText("Location 2: The Forest", 10, 10, 20, BLACK);
    break;    case LOCATION_3:
        ClearBackground(BROWN);        
        DrawText("Location 3: The Cave", 10, 10, 20, BLACK);
    break;    case LOCATION_4:
        ClearBackground(BLUE);        
        DrawText("Location 4: The Castle", 10, 10, 20, BLACK);
    break;    case LOCATION_5:
        ClearBackground(GRAY);        
        DrawText("Location 5: The Village", 10, 10, 20, BLACK);
    break;    default:
        break;
    }
}
int main() {
    // Инициализация окна    
    const int screenWidth = 1600;
    const int screenHeight = 800;    
    InitWindow(screenWidth, screenHeight, "Simple RPG Game with Locations");
    // Создание игрока
    Player player = { {screenWidth / 2, screenHeight / 2}, 100, 10 };    Location currentLocation = LOCATION_1;
    SetTargetFPS(60); // Установка FPS
    while (!WindowShouldClose()) {
        // Обработка ввода        
        if (IsKeyDown(KEY_RIGHT)) player.position.x += 5.0f;
        if (IsKeyDown(KEY_LEFT)) player.position.x -= 5.0f;        
        if (IsKeyDown(KEY_UP)) player.position.y -= 5.0f;
        if (IsKeyDown(KEY_DOWN)) player.position.y += 5.0f;        
        if (IsKeyDown(KEY_D)) player.position.x += 5.0f;
        if (IsKeyDown(KEY_A)) player.position.x -= 5.0f;        
        if (IsKeyDown(KEY_W)) player.position.y -= 5.0f;
        if (IsKeyDown(KEY_S)) player.position.y += 5.0f;        
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mousePosition = GetMousePosition();            
            player.position = mousePosition; 
            // Перемещение игрока к позиции курсора
        }


        // Логика игры (например, столкновения, бой и т.д.) может быть добавлена здесь
        if (player.position.y < 0) {
            player.position.y = 0;
        }       
        if (player.position.x < 0) {
            player.position.x = 0;
        }
        if (player.position.x + 50 > screenWidth) {
            player.position.x = screenWidth - 50;
        }        
        if (player.position.y + 50 > screenHeight) {
            player.position.y = screenHeight - 50;
        }

        // Отрисовка        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawPlayer(player);        
        EndDrawing();
    }
    // Проверка границ экрана для игрока
    if (player.position.x < 0) player.position.x = 0;    
    if (player.position.x > screenWidth - 50) 
        player.position.x = screenWidth - 50;
    if (player.position.y < 0) player.position.y = 0;    
    if (player.position.y > screenHeight - 50) 
        player.position.y = screenHeight - 50;

    while (!WindowShouldClose()) {
        // Переключение локаций
        if (IsKeyPressed(KEY_ONE)) currentLocation = LOCATION_1;        
        if (IsKeyPressed(KEY_TWO)) currentLocation = LOCATION_2;
        if (IsKeyPressed(KEY_THREE)) currentLocation = LOCATION_3;        
        if (IsKeyPressed(KEY_FOUR)) currentLocation = LOCATION_4;
        if (IsKeyPressed(KEY_FIVE)) currentLocation = LOCATION_5;
        // Отрисовка
        BeginDrawing();        
        DrawLocation(currentLocation);
        DrawPlayer(player);        
        EndDrawing();
    }
    // Закрытие окна
    CloseWindow();    
    return 0;
}