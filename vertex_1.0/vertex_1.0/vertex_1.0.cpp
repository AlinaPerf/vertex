#include <raylib.h>
#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include <vector>


struct Player {
	Vector2 position;
	int health;
	int attack;
};
struct Enemy {
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

struct LocationInfo {// Информационное окно
	Rectangle bounds;
	const char* name;
};

void DrawPlayer(Player player) {
	DrawRectangle(player.position.x, player.position.y, 50, 50, BLUE);
	DrawText(TextFormat("HP: %d", player.health), player.position.x, player.position.y - 20, 20, RED);
}

void DrawEnemy(Enemy enemy) {
	DrawRectangle(enemy.position.x, enemy.position.y, 50, 50, GREEN);
	DrawText(TextFormat("HP: %d", enemy.health), enemy.position.x, enemy.position.y - 20, 20, RED);
}
void DrawLocations() {
	// Отрисовка локаций
	DrawRectangle(50, 50, 75, 75, RED); // Location 1
	DrawText("Location 1: Start", 50, 50, 10, BLACK);

	DrawRectangle(1000, 150, 75, 75, GREEN); // Location 2
	DrawText("Location 2: Forest", 1000, 150, 10, BLACK);

	DrawRectangle(150, 600, 75, 75, BROWN); // Location 3
	DrawText("Location 3: Cave", 150, 600, 10, BLACK);

	DrawRectangle(150, 150, 75, 75, BLUE); // Location 4
	DrawText("Location 4: Castle", 150, 150, 10, BLACK);

	DrawRectangle(1000, 600, 75, 75, GRAY); // Location 5
	DrawText("Location 5: Village", 1000, 600, 10, BLACK);
}

// Инициализация окна    
const int screenWidth = 1600;
const int screenHeight = 800;
Player player = { {screenWidth / 2, screenHeight / 2}, 100, 10 };
Location currentLocation = LOCATION_1;

void LocationScreen() {
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

	// Проверка на столкновение с локациями
	for (int i = 0; i < LOCATION_COUNT; i++) {
		if (CheckCollisionRecs((Rectangle)(player.position.x, player.position.y, 50, 50), locations[i].bounds)) {
			if (IsKeyPressed(KEY_ENTER)) {
				// Открытие нового окна для информации о локации
				DrawText(locations[i].name, screenWidth / 2 - MeasureText(locations[i].name, 20) / 2, screenHeight / 2, 20, BLACK);
				DrawText("Press ESC to return", screenWidth / 2 - MeasureText("Press ESC to return", 20) / 2, screenHeight / 2 + 30, 20, BLACK);
				EndDrawing();
				while (!WindowShouldClose()) {
					if (IsKeyPressed(KEY_ESCAPE)) break; // Закрытие окна при нажатии ESC
				}
			}
		}
	}

	// Логика игры (например, столкновения, бой и т.д.)
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

	// Отрисовка        
	BeginDrawing();

	ClearBackground(RAYWHITE);
	DrawPlayer(player);
	DrawLocations(); // Отрисовка всех локаций
	EndDrawing();
}

void BattleScreen() {

	// Отрисовка        
	BeginDrawing();
	//DrawEnemy(enemy);

	ClearBackground(RAYWHITE);
	DrawPlayer(player);
	EndDrawing();
}

int main() {
	InitWindow(screenWidth, screenHeight, "Game with Locations");
	// Создание игрока
	SetTargetFPS(60); // Установка FPS


	while (!WindowShouldClose()) {
		LocationScreen();
		//BattleScreen();
	}


	// Закрытие окна
	CloseWindow();
	return 0;
}