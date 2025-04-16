#include <raylib.h>
#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <filesystem>

const std::string base_directory = "money.txt";

void getFileContents(const std::string& filename, std::string& fileContents) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Не удалось открыть файл: " << filename << std::endl;
		std::ofstream create_file(filename);
		create_file << "";
		create_file.close();
		std::cout << "Новый файл был успешно создан!";
		return;
	}
	std::ostringstream oss;
	oss << file.rdbuf();
	fileContents = oss.str();
	file.close();
}

void saveToFile(const std::string& filename, const std::string& newContent) {
	std::ofstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Не удалось открыть файл: " << filename << std::endl;
		return;
	}
	file << newContent;
	file.close();
}
struct Bar {
private:
	const int margin = 5;
	Rectangle bar_body;
	Rectangle body;
	float max_value;
	float current_value;
	Color background_color;
	Color bar_color;
	bool show_text;
public:
	void Draw() {
		DrawRectangleRec(body, background_color);
		DrawRectangleRec(bar_body, bar_color);
		if (show_text) {
			std::string value = (std::to_string(current_value) + "/" + std::to_string(max_value));
			int fontSize = 14;
			DrawText(value.c_str(), body.x + body.width / 2 - MeasureText(value.c_str(), fontSize) / 2, body.y + body.height / 4, fontSize, BLACK);
		}
	}
	bool SetValue(float value) {
		if (value < 0 || value > max_value) {
			return false;
		}
		float percentage = value / max_value;
		bar_body.width = body.width * percentage - 2 * margin;
	}
	Bar() {};

	Bar(Rectangle body, float max_value, Color background_color, Color bar_color, bool show_text) :
		body(body),
		max_value(max_value),
		background_color(background_color),
		bar_color(bar_color),
		show_text(show_text)
	{
		bar_body = { body.x + margin,body.y + margin,body.width - 2 * margin,body.height - 2 * margin };
	};
};

struct Player {
	Vector2 position;
	int health;
	int attack;
	int stamina;
	Texture2D Body;
	Vector2 speed;
	Bar healthBar;

	Player() {

	}
};
struct Enemy {
	Vector2 position;
	int health;
	int attack;
	int stamina;
	Texture2D Body;
	bool active;
	Vector2 speed;
};

struct NPCButton {
	Rectangle body;
	Color color;
};
struct ShopButton {
	Rectangle body;
	Color color;
};
struct LocationInfo {
	Rectangle bounds;
	std::string name;
	Color color;
	int id;
	Texture2D Texture{ 0 }; // Переменная для текстуры
};
struct Button {
	Rectangle body;
	Color color;
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

Player player_location = { 0 };
Player player = { 0 };
Enemy enemy = { 0 };
std::vector<LocationInfo> locations;
int currentLocation = 0;
void DrawPlayer(Player& player) {
	//DrawRectangle(player.position.x, player.position.y, 50, 50, BLACK);
	DrawTexture(player.Body, player.position.x, player.position.y, RAYWHITE);
	DrawText(TextFormat("HP: %d", player.health), player.position.x, player.position.y - 20, 20, RED);
	DrawText(TextFormat("STAMINA: %d", player.stamina), player.position.x, player.position.y - 50, 20, RED);

}

void DrawEnemy(Enemy& enemy) {
	DrawRectangle((int)enemy.position.x, (int)enemy.position.y, 100, 100, RED);
	DrawText(TextFormat("HP: %d", enemy.health), (int)enemy.position.x, (int)enemy.position.y - 20, 20, RED);
	DrawText(TextFormat("STAMINA: %d", enemy.stamina), (int)enemy.position.x, (int)enemy.position.y - 50, 20, RED);
}
void DrawLocations() {

	for (int i = 0; i < locations.size(); i++)
	{
		DrawRectangleRec(locations[i].bounds, locations[i].color);
		std::string locationName = "Location" + (i + 1) + locations[i].name;
		DrawText(locationName.c_str(), locations[i].bounds.x, locations[i].bounds.y, 10, BLACK);
	}
}
std::string money{};
//денюжки
void DrawMoney() {
	DrawText(/*TextFormat("")*/money.c_str(), 10, 20, 20, BLACK);
};

/// <summary>
/// Обработка ввода для движения игрока
/// </summary>
void HandleInput() {
	// Обработка ввода        
	if (IsKeyDown(KEY_RIGHT)) player_location.position.x += 5.0f;
	if (IsKeyDown(KEY_LEFT)) player_location.position.x -= 5.0f;
	if (IsKeyDown(KEY_UP)) player_location.position.y -= 5.0f;
	if (IsKeyDown(KEY_DOWN)) player_location.position.y += 5.0f;
	if (IsKeyDown(KEY_D)) player_location.position.x += 5.0f;
	if (IsKeyDown(KEY_A)) player_location.position.x -= 5.0f;
	if (IsKeyDown(KEY_W)) player_location.position.y -= 5.0f;
	if (IsKeyDown(KEY_S)) player_location.position.y += 5.0f;
	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
		Vector2 mousePosition = GetMousePosition();
		player_location.position = mousePosition;
		// Перемещение игрока к позиции курсора
	}
}

void LocationScreen(LocationInfo info) {
	// Обработка ввода        
	HandleInput();
	// Проверка на столкновение с локациями
	for (int i = 0; i < locations.size(); i++) {
		if (CheckCollisionRecs({ player_location.position.x, player_location.position.y, 100, 100 }, locations[i].bounds)) {
			if (IsKeyPressed(KEY_ENTER)) {
				currentLocation = locations[i].id;
				player.position = { screenWidth / 4,screenHeight / 2 };
				player.stamina = 10;
				enemy.position = { screenWidth * 3 / 4,screenHeight / 2 };
				// Открытие нового окна для информации о локации
				DrawText(locations[i].name.c_str(), screenWidth / 2 - MeasureText(locations[i].name.c_str(), 20) / 2, screenHeight / 2, 20, BLACK);
				DrawText("Press ESC to return", screenWidth / 2 - MeasureText("Press ESC to return", 20) / 2, screenHeight / 2 + 30, 20, BLACK);
			}
		}
	}
	// Логика игры (например, столкновения, бой и т.д.)
	if (player_location.position.y < 0) {
		player_location.position.y = 0;
	}
	if (player_location.position.x < 0) {
		player_location.position.x = 0;
	}
	if (player_location.position.x + 50 > screenWidth) {
		player_location.position.x = screenWidth - 50;
	}
	if (player_location.position.y + 50 > screenHeight) {
		player_location.position.y = screenHeight - 50;
	}

	// Отрисовка        
	BeginDrawing();
	ClearBackground(RAYWHITE);
	DrawTexture(info.Texture, 0, 0, RAYWHITE);
	DrawPlayer(player_location);
	DrawLocations(); // Отрисовка всех локаций
	DrawMoney();
	EndDrawing();
}
Rectangle npc_body = { 250,350,50,50 };
NPCButton npc = {
	npc_body,
	BLUE,
};
Rectangle shop_body = { 100,50,1400,700 };
ShopButton shop = {
	shop_body,
	YELLOW,
};
Rectangle shopExit_body = { 150,75,25,25 };
ShopButton shopExit = {
	shopExit_body,
	RED,
};
bool showDialog = false;

void VillageScreen(LocationInfo info) {
	if (IsKeyPressed(KEY_Q))
	{
		currentLocation = 0;
	}
	if (CheckCollisionPointRec({player_location.position.x,player_location.position.y }, { npc_body })) {
		if (IsKeyPressed(KEY_ENTER)) {
			std::cout << "shop";

			showDialog = true;
		}
	}
	if(!showDialog){
		HandleInput();
	}
	else {
		//Обработка нажатия на кнопки в диалоговом окне
		Vector2 mouse = GetMousePosition();
		if (CheckCollisionPointRec(mouse, shopExit.body) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
			showDialog = false;
		}
			
		
	}
	// Отрисовка        
	BeginDrawing();
	//DrawEnemy(enemy);
	ClearBackground(GRAY);
	DrawTexture(info.Texture, 0, 0, WHITE);
	DrawPlayer(player_location);
	DrawMoney();
	DrawRectangleRec(npc.body, npc.color);
	if (showDialog) {
		//Отрисовка диалогового окна
		DrawRectangleRec(shop.body, shop.color);
		DrawRectangleRec(shopExit.body, shopExit.color);
	}
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
	DrawMoney();
	// Отрисовка текстуры
	DrawTexture(info.Texture, 0, 0, WHITE); // Отрисовка текстуры в верхнем левом углу

	DrawPlayer(player_location); // Отрисовка игрока
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
	DrawPlayer(player_location);
	DrawMoney();
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
	DrawPlayer(player_location);
	DrawMoney();
	EndDrawing();
}

int counter = 0;
Rectangle button_body = { 10,650,100,50 };
Button button = {
	button_body,
	YELLOW,
};

Rectangle button1_body = { 130,650,100,50 };
Button button1 = {
	button1_body,
	GREEN,
};

Rectangle button2_body = { 250,650,100,50 };
Button button2 = {
	button2_body,
	BLUE,
};

Rectangle button3_body = { 1490,650,100,50 };
Button button3 = {
	button3_body,
	RED,
};
Rectangle button4_body = { 1370,650,100,50 };
Button button4 = {
	button4_body,
	BLACK,
};

bool game_over = false;
void BattleScreen(LocationInfo info) {
	if (IsKeyPressed(KEY_Q))
	{
		currentLocation = 0;
		player.health = 100;
		enemy.health = 100;
		player.stamina = 0;
		game_over = false;
	}
	if (!game_over)
	{
		if (player.health <= 0 or enemy.health <= 0) {
			game_over = true;
		}
		Vector2 mouse = GetMousePosition();


		if (CheckCollisionPointRec(mouse, button.body) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && counter % 2 == 0 && player.speed.x == 0 && player.stamina >= 5) {
			player.speed = { 5,0 };
			player.attack = 10;
			player.stamina -= 5;
		}

		if (CheckCollisionPointRec(mouse, button1.body) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && counter % 2 == 0 && player.speed.x == 0 && player.stamina >= 10) {
			player.speed = { 5,0 };
			player.attack = 15;
			player.stamina -= 10;
		}

		if (CheckCollisionPointRec(mouse, button2.body) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && counter % 2 == 0 && player.speed.x == 0 && player.stamina >= 30) {
			player.speed = { 5,0 };
			player.attack = 35;
			player.stamina -= 30;
		}
		if (CheckCollisionPointRec(mouse, button3.body) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && counter % 2 == 0 && player.speed.x == 0 && player.stamina >= 15) {
			player.speed = { 5,0 };
			enemy.attack = 0;
			player.attack = 0;
			enemy.stamina -= 25;
			player.stamina -= 15;
			player.stamina += 4;
		}
		if (CheckCollisionPointRec(mouse, button4.body) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && counter % 2 == 0 && player.speed.x == 0 && player.stamina >= 20) {
			player.speed = { 5,0 };
			player.health += 20;
			player.attack = 0;
			player.stamina -= 20;
		}

		if (CheckCollisionRecs({ player.position.x,player.position.y,100,100 }, { enemy.position.x,enemy.position.y,100,100 }) && counter % 2 == 0) {
			player.speed = { -5,0 };
			enemy.health -= player.attack;
		}
		if (player.speed.x < 0) {
			if (player.position.x < screenWidth / 4) {
				player.speed = { 0,0 };
				counter++;
			}
		}
		player.position = Vector2Add(player.position, player.speed);

		if (counter % 2 == 1 && enemy.speed.x == 0)
			enemy.speed = { -5,0 };
		if (CheckCollisionRecs({ enemy.position.x,enemy.position.y,100,100 }, { player.position.x,player.position.y,100,100 }) && counter % 2 == 1) {
			enemy.speed = { 5,0 };
			player.health -= GetRandomValue(enemy.attack-10,enemy.attack+10);
		}
		if (enemy.speed.x > 0) {
			if (enemy.position.x > screenWidth * 3 / 4) {
				enemy.speed = { 0,0 };
				counter++;
				player.stamina += 10;
				enemy.attack = 10;
				enemy.stamina = 100;
			}
		}
		enemy.position = Vector2Add(enemy.position, enemy.speed);
	}
	// Отрисовка        
	BeginDrawing();

	ClearBackground(RAYWHITE);
	DrawTexture(info.Texture, 0, 0, WHITE);
	DrawPlayer(player);
	DrawEnemy(enemy);
	if (!game_over) {
		DrawRectangleRec(button.body, button.color);
		DrawRectangleRec(button1.body, button1.color);
		DrawRectangleRec(button2.body, button2.color);
		DrawRectangleRec(button3.body, button3.color);
		DrawRectangleRec(button4.body, button4.color);
	}
	else {
		DrawText("Game over", 100, 100, 50, BLACK);
	}
	EndDrawing();

}

void InitializeGame() {
	Image image = LoadImage(Wind_filename);
	Texture2D texture = { 0 };
	if (image.data != NULL)
	{
		ImageResize(&image, screenWidth, screenHeight);
		texture = LoadTextureFromImage(image);

	}
	LocationInfo location1 = {
		{ 50, 50, 75, 75},
			"Start",
			RED,
			0,
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
			1,
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
			2,
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
			3,
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
			4,
			texture
	};
	locations.push_back(location5);

	image = LoadImage(Village_filename);
	texture = { 0 };
	if (image.data != NULL)
	{
		ImageResize(&image, screenWidth, screenHeight);
		texture = LoadTextureFromImage(image);

	}
	LocationInfo location6 = {
		{ 500, 400, 75, 75 },
		"Battle",
			GRAY,
			5,
			texture
	};
	locations.push_back(location6);

	image = LoadImage(Priest_filename);
	if (image.data != NULL)
	{
		ImageCrop(&image, { 51,133,41,58 });
		ImageResize(&image, 50, 50);
		texture = LoadTextureFromImage(image);

	}

	player_location = {
		{ screenWidth / 2, screenHeight / 2 }, 
		100, 
		10,
		0,
		texture,
		{0,0},
				
		 };
	if (image.data != NULL)
	{
		ImageCrop(&image, { 51,133,41,58 });
		ImageResize(&image, 100, 100);
		texture = LoadTextureFromImage(image);

	}
	player = {
	    { screenWidth / 2, screenHeight / 2 }, 
		100, 
		10,
		0,
		texture ,
		{0,0} ,
		{}
	};

	enemy = {
		{ screenWidth / 2, screenHeight / 2 }, 
		100, 
		10,
		100,
		texture, 
		false,
		{0,0} 
	};

}

int main() {

	InitWindow(screenWidth, screenHeight, "Game with Locations");
	std::string content = "";
	getFileContents(base_directory, content);
	money = content;
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
			ForestScreen(locations[1]);
			break;
		case 2:
			CaveScreen(locations[2]);
			break;
		case 3:
			CastleScreen(locations[3]);
			break;
		case 4:
			VillageScreen(locations[4]);
			break;
		case 5:
			BattleScreen(locations[5]);
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