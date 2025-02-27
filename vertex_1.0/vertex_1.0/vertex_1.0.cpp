﻿#include <raylib.h>
#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include <vector>


struct Player {
	Vector2 position;
	int health;
	int attack;
	Texture2D Body;
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
	Texture2D Texture { 0 }; // Переменная для текстуры леса
};

const char* Wind_filename = "./Assets/Locations/wind.png";
const char* Castle_filename = "./Assets/Locations/castle.png";
const char* Cave_filename = "./Assets/Locations/cave.png";
const char* Village_filename = "./Assets/Locations/village.png";
const char* Start_filename = "./Assets/Locations/start.png";
const char* Forest_filename = "./Assets/Locations/forest.png";
const char* Priest_filename = "./Assets/Player/Priest.png";

// Инициализация окна    
const int screenWidth = 1600;
const int screenHeight = 800;

 Image image = LoadImage(Wind_filename);
 

Player player = {0};
std::vector<LocationInfo> locations;
int currentLocation = 0;
void DrawPlayer(Player player) {
	//DrawRectangle(player.position.x, player.position.y, 50, 50, BLACK);
	DrawTexture(player.Body,player.position.x, player.position.y, RAYWHITE);
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

void HandleInput() {
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
}
void LocationScreen(LocationInfo info) {
	// Обработка ввода        

	HandleInput();
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
	DrawTexture(info.Texture,0,0,RAYWHITE);
	DrawPlayer(player);
	DrawLocations(); // Отрисовка всех локаций
	EndDrawing();
}


void VillageScreen(LocationInfo info) {
	if (IsKeyPressed(KEY_Q))
	{
		currentLocation = 0;
	}
	HandleInput();
	// Отрисовка        
	BeginDrawing();
	//DrawEnemy(enemy);
	ClearBackground(GRAY);
	DrawTexture(info.Texture, 0, 0, WHITE);
	DrawPlayer(player);
	EndDrawing();
}

void ForestScreen(LocationInfo& info) {
	if (IsKeyPressed(KEY_Q)) {
		currentLocation = 0; // Возврат в основное меню
	}

	HandleInput();
	// Отрисовка        
	BeginDrawing();
	ClearBackground(GREEN); // Фон для леса

	// Отрисовка текстуры
	DrawTexture(info.Texture, 0, 0, WHITE); // Отрисовка текстуры в верхнем левом углу

	DrawPlayer(player); // Отрисовка игрока
	EndDrawing();
}

void CaveScreen(LocationInfo info) {
	if (IsKeyPressed(KEY_Q))
	{
		currentLocation = 0;
	}

	HandleInput();
	// Отрисовка        
	BeginDrawing();
	//DrawEnemy(enemy);
	ClearBackground(BROWN);
	DrawTexture(info.Texture, 0, 0, WHITE);
	DrawPlayer(player);
	EndDrawing();
}

void CastleScreen(LocationInfo info) {
	if (IsKeyPressed(KEY_Q))
	{
		currentLocation = 0;
	}

	HandleInput();
	// Отрисовка        
	BeginDrawing();
	//DrawEnemy(enemy);
	Texture();
	ClearBackground(BLUE);
	DrawTexture(info.Texture, 0, 0, WHITE);
	DrawPlayer(player);
	EndDrawing();
}
void BattleScreen(LocationInfo info) {
	if (IsKeyPressed(KEY_Q))
	{
		currentLocation = 0;
	}

	HandleInput();
	// Отрисовка        
	BeginDrawing();
	//DrawEnemy(enemy);

	ClearBackground(RAYWHITE);
	DrawTexture(info.Texture, 0, 0, WHITE);
	DrawPlayer(player);
	EndDrawing();
}

void InitializeGame() {
	Image image = LoadImage(Wind_filename);
	Texture2D texture = {0};
	if (image.data != NULL)
	{
		ImageResize(&image, screenWidth, screenHeight);
		texture=LoadTextureFromImage(image);
		
	}
	LocationInfo location1 = {
		{ 50, 50, 75, 75},
			"Start",
			RED,
			1,
			texture
	};
	locations.push_back(location1);

	image = LoadImage(Forest_filename);
	texture = { 0 };
	if (image.data != NULL)
	{
		ImageResize(&image, screenWidth, screenHeight);
		texture = LoadTextureFromImage(image);

	}
	LocationInfo location2 = {
		{ 1000, 150, 75, 75 },
			"Forest",
			GREEN,
			2,
			texture
	};
	locations.push_back(location2);


	image = LoadImage(Cave_filename);
	texture = { 0 };
	if (image.data != NULL)
	{
		ImageResize(&image, screenWidth, screenHeight);
		texture = LoadTextureFromImage(image);

	}
	LocationInfo location3 = {
		{ 150, 600, 75, 75 },
		"Cave",
			BROWN,
			3,
			texture
	};
	locations.push_back(location3);



	image = LoadImage(Castle_filename);
	texture = { 0 };
	if (image.data != NULL)
	{
		ImageResize(&image, screenWidth, screenHeight);
		texture = LoadTextureFromImage(image);

	}
	LocationInfo location4 = {
		{ 150, 150, 75, 75 },
		"Castle",
			BLUE,
			4,
			texture
	};
	locations.push_back(location4);


	image = LoadImage(Village_filename);
	texture = { 0 };
	if (image.data != NULL)
	{
		ImageResize(&image, screenWidth, screenHeight);
		texture = LoadTextureFromImage(image);

	}
	LocationInfo location5 = {
		{ 1000, 600, 75, 75 },
		"Village",
			GRAY,
			5,
			texture
	};
	locations.push_back(location5);

	image = LoadImage(Priest_filename);
	if (image.data != NULL)
	{
		ImageCrop(&image, { 51,133,41,58 });
		ImageResize(&image, 50, 50);
		texture = LoadTextureFromImage(image);

	}
	player = { 
		{ screenWidth / 2, screenHeight / 2 }, 100, 10,
	texture};

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
			LocationScreen(locations[0]);
			break;
		case 1:
			BattleScreen(locations[0]);
			break;
		case 2:
			ForestScreen(locations[1]);
			break;

		case 3:
			CaveScreen(locations[2]);
			break;
		case 4:
			CastleScreen(locations[3]);
			break;
		case 5:
			VillageScreen(locations[4]);
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