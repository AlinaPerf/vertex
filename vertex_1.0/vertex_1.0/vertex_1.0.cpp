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

struct LocationInfo {
	Rectangle bounds;
	std::string name;
	Color color;
	int id;
};
// Инициализация окна    
const int screenWidth = 1600;
const int screenHeight = 800;
Player player = { {screenWidth / 2, screenHeight / 2}, 100, 10 };
std::vector<LocationInfo> locations;
int currentLocation = 0;

void DrawPlayer(Player player) {
	DrawRectangle(player.position.x, player.position.y, 50, 50, BLACK);
	DrawText(TextFormat("HP: %d", player.health), player.position.x, player.position.y - 20, 20, RED);
}

void DrawEnemy(Enemy enemy) {
	DrawRectangle((int)enemy.position.x, (int)enemy.position.y, 50, 50, GREEN);
	DrawText(TextFormat("HP: %d", enemy.health), (int)enemy.position.x, (int)enemy.position.y - 20, 20, RED);
}
void DrawLocations() {

	for (int i = 0; i < locations.size(); i++)
	{
		DrawRectangleRec(locations[i].bounds, locations[i].color);
		std::string locationName = "Location" + (i + 1) + locations[i].name;
		DrawText(locationName.c_str(), locations[i].bounds.x, locations[i].bounds.y, 10, BLACK);
	}

}

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
	for (int i = 0; i < locations.size(); i++) {
		if (CheckCollisionRecs({ player.position.x, player.position.y, 50, 50 }, locations[i].bounds)) {
			if (IsKeyPressed(KEY_ENTER)) {
				currentLocation = locations[i].id;
				// Открытие нового окна для информации о локации
				DrawText(locations[i].name.c_str(), screenWidth / 2 - MeasureText(locations[i].name.c_str(), 20) / 2, screenHeight / 2, 20, BLACK);
				DrawText("Press ESC to return", screenWidth / 2 - MeasureText("Press ESC to return", 20) / 2, screenHeight / 2 + 30, 20, BLACK);
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

void VillageScreen() {
	if (IsKeyPressed(KEY_Q))
	{
		currentLocation = 0;
	}
	// Отрисовка        
	BeginDrawing();
	//DrawEnemy(enemy);
	ClearBackground(GRAY);
	DrawPlayer(player);
	EndDrawing();
}

void ForestScreen() {
	if (IsKeyPressed(KEY_Q))
	{
		currentLocation = 0;
	}
	// Отрисовка        
	BeginDrawing();
	//DrawEnemy(enemy);

	ClearBackground(GREEN);
	DrawPlayer(player);
	EndDrawing();
}

void CaveScreen() {
	if (IsKeyPressed(KEY_Q))
	{
		currentLocation = 0;
	}
	// Отрисовка        
	BeginDrawing();
	//DrawEnemy(enemy);

	ClearBackground(BROWN);
	DrawPlayer(player);
	EndDrawing();
}

void CastleScreen() {
	if (IsKeyPressed(KEY_Q))
	{
		currentLocation = 0;
	}
	// Отрисовка        
	BeginDrawing();
	//DrawEnemy(enemy);

	ClearBackground(BLUE);
	DrawPlayer(player);
	EndDrawing();
}
void BattleScreen() {
	if (IsKeyPressed(KEY_Q))
	{
		currentLocation = 0;
	}
	// Отрисовка        
	BeginDrawing();
	//DrawEnemy(enemy);

	ClearBackground(RAYWHITE);
	DrawPlayer(player);
	EndDrawing();
}

void InitializeGame() {
	LocationInfo location1 = {
		{ 50, 50, 75, 75},
			"Start",
			RED,
			1
	};
	locations.push_back(location1);

	LocationInfo location2 = {
		{ 1000, 150, 75, 75 },
			"Forest",
			GREEN,
			2
	};
	locations.push_back(location2);

	LocationInfo location3 = {
		{ 150, 600, 75, 75 },
		"Cave",
			BROWN,
			3
	};
	locations.push_back(location3);

	LocationInfo location4 = {
		{ 150, 150, 75, 75 },
		"Castle",
			BLUE,
			4
	};
	locations.push_back(location4);

	LocationInfo location5 = {
		{ 1000, 600, 75, 75 },
		"Village",
			GRAY,
			5
	};
	locations.push_back(location5);

	//	// Отрисовка локаций
	//	DrawRectangle(50, 50, 75, 75, RED); // Location 1
	//	DrawText("Location 1: Start", 50, 50, 10, BLACK);
	//
	//	DrawRectangle(1000, 150, 75, 75, GREEN); // Location 2
	//	DrawText("Location 2: Forest", 1000, 150, 10, BLACK);
	//
	//	DrawRectangle(150, 600, 75, 75, BROWN); // Location 3
	//	DrawText("Location 3: Cave", 150, 600, 10, BLACK);
	//
	//	DrawRectangle(150, 150, 75, 75, BLUE); // Location 4
	//	DrawText("Location 4: Castle", 150, 150, 10, BLACK);
	//
	//	DrawRectangle(1000, 600, 75, 75, GRAY); // Location 5
	//	DrawText("Location 5: Village", 1000, 600, 10, BLACK);

}

int main() {
	InitWindow(screenWidth, screenHeight, "Game with Locations");
	// Создание игрока
	SetTargetFPS(60); // Установка FPS
	InitializeGame();

	while (!WindowShouldClose()) {
		switch (currentLocation)
		{
		case 0:
			LocationScreen();
			break;
		case 1:
			BattleScreen();
			break;
		case 2:
			ForestScreen();
			break;

		case 3:
			CaveScreen();
			break;
		case 4:
			CastleScreen();
			break;
		case 5:
			VillageScreen();
			break;
		default:
			break;
		}

		//LocationScreen();
		//BattleScreen();
	}


	// Закрытие окна
	CloseWindow();
	return 0;
}