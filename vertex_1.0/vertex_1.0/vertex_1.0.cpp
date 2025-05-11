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
	int max_value;
	int current_value;
	Color background_color;
	Color bar_color;
	Color text_color;
	bool show_text;
public:
	void Draw() {
		DrawRectangleRec(body, background_color);
		DrawRectangleRec(bar_body, bar_color);
		if (show_text) {
			std::string value = (std::to_string((int)current_value) + "/" + std::to_string((int)max_value));
			int fontSize = 14;
			DrawText(value.c_str(), body.x + body.width / 2 - MeasureText(value.c_str(), fontSize) / 2, body.y + body.height / 4, fontSize, text_color);
		}
	}
	bool SetValue(int value) {
		if (value < 0 || value > max_value) {
			return false;
		}
		float percentage = (float)value / max_value;
		this->current_value = value;
		this->bar_body.width = body.width * percentage - 2 * margin;
	}
	void SetMaxValue(int value) {
		this->max_value = value;
	}
	int GetMaxValue() {
		return this->max_value;
	}
	void setPosition(Vector2 next) {
		this->body.x = next.x;
		this->body.y = next.y;

		this->bar_body.x = body.x + margin;
		this->bar_body.y = body.y + margin;
	}
	Vector2 getPosition() {
		return { this->body.x,this->body.y };
	}

	Bar(Rectangle body, int max_value, Color background_color, Color bar_color, Color text_color, bool show_text) :
		body(body),
		max_value(max_value),
		background_color(background_color),
		bar_color(bar_color),
		text_color(text_color),
		show_text(show_text),
		current_value(max_value)
	{
		bar_body = { body.x + margin,body.y + margin,body.width - 2 * margin,body.height - 2 * margin };
	};
};

struct Player {
	Vector2 position;
	int body_size;
	int current_health;
	int health_bonus = 0;
	int current_attack;
	int attack_bonus = 0;
	int max_stamina;
	int current_stamina;
	int stamina_bonus = 0;
	int regen_bonus = 0;
	Texture2D Body;
	Vector2 speed;
	Bar healthBar;
	Bar staminaBar;
	bool showBars = false;

	Player(Vector2 position, float health, float attack, float stamina, int body_size, Texture2D body, Vector2 speed) :
		position(position),
		current_health(health),
		current_attack(attack),
		max_stamina(stamina),
		Body(body),
		speed(speed),
		healthBar({ position.x,position.y - 80,health,30 }, health, BLACK, GREEN, WHITE, true),
		staminaBar({ position.x,position.y - 40,100,30 }, health, BLACK, BLUE, WHITE, true),
		body_size(body_size)
	{}

	void SetPosition(Vector2 next) {
		position = next;
		this->healthBar.setPosition({ next.x,next.y - 80 });
		this->staminaBar.setPosition({ next.x,next.y - 40 });
	}
	void SetHealth(int next) {
		this->current_health = next;
		this->healthBar.SetValue(next);
	}
	void SetMaxHealth(int next) {
		this->current_health = next;
		this->healthBar.SetMaxValue(next);
		this->healthBar.SetValue(next);
	}
	void SetStamina(int next) {
		this->current_stamina = next;
		this->staminaBar.SetValue(next);
	}
};

struct Enemy {

	Vector2 position;
	int body_size;
	int health;
	int attack;
	int stamina;
	Texture2D Body;
	bool active;
	Vector2 speed;
	Bar healthBar;
	Bar staminaBar;

	Enemy(Vector2 position, int health, int attack, int stamina, int body_size, Texture2D body, Vector2 speed) :
		position(position),
		health(health),
		attack(attack),
		stamina(stamina),
		Body(body),
		speed(speed),
		healthBar({ position.x,position.y - 80,100,30 }, health, BLACK, RED, WHITE, true),
		staminaBar({ position.x,position.y - 40,100,30 }, health, BLACK, BLUE, WHITE, true),
		body_size(body_size),
		active(false)
	{}

	void SetPosition(Vector2 next) {
		position = next;
		this->healthBar.setPosition({ next.x,next.y - 80 });
		this->staminaBar.setPosition({ next.x,next.y - 40 });
	}
	void SetHealth(int next) {
		this->health = next;
		this->healthBar.SetValue(next);
	}
	void SetStamina(int next) {
		this->stamina = next;
		this->staminaBar.SetValue(next);
	}
};

struct NPCButton {
	Rectangle body;
	Color color;
};
struct NPCChest {
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

struct ButtonBattle {
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

Player* player_location;
Player* player;

//Enemy* enemy;
//Enemy* enemy1;
//Enemy* enemy2;
std::vector<Enemy*> enemies;
Enemy* currentEnemy;
std::vector<LocationInfo> locations;
int currentLocation = 0;
void DrawPlayer(Player* player) {
	//DrawRectangle(player->position.x, player->position.y, player->body_size, player->body_size, BLACK);
	DrawTexture(player->Body, player->position.x, player->position.y, RAYWHITE);
	if (player->showBars) {
		player->healthBar.Draw();
		player->staminaBar.Draw();
	}
	/*DrawText(TextFormat("HP: %d", player->health), player->position.x, player->position.y - 20, 20, RED);
	DrawText(TextFormat("STAMINA: %d", player->stamina), player->position.x, player->position.y - 50, 20, RED);*/

}

void DrawEnemy(Enemy* enemy) {
	DrawRectangle((int)enemy->position.x, (int)enemy->position.y, 100, 100, RED);;
	enemy->healthBar.Draw();
	enemy->staminaBar.Draw();
	//DrawText(TextFormat("HP: %d", enemy.health), (int)enemy.position.x, (int)enemy.position.y - 20, 20, RED);
	//DrawText(TextFormat("STAMINA: %d", enemy.stamina), (int)enemy.position.x, (int)enemy.position.y - 50, 20, RED);
}
void DrawLocations() {

	for (int i = 1; i < locations.size(); i++)
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
	if (IsKeyDown(KEY_RIGHT)) player_location->SetPosition({ player_location->position.x + 5.0f, player_location->position.y });
	if (IsKeyDown(KEY_LEFT)) player_location->SetPosition({ player_location->position.x - 5.0f, player_location->position.y });
	if (IsKeyDown(KEY_UP)) player_location->SetPosition({ player_location->position.x, player_location->position.y - 5.0f });
	if (IsKeyDown(KEY_DOWN)) player_location->SetPosition({ player_location->position.x, player_location->position.y + 5.0f });
	if (IsKeyDown(KEY_D)) player_location->SetPosition({ player_location->position.x + 5.0f, player_location->position.y });
	if (IsKeyDown(KEY_A)) player_location->SetPosition({ player_location->position.x - 5.0f, player_location->position.y });
	if (IsKeyDown(KEY_W)) player_location->SetPosition({ player_location->position.x, player_location->position.y - 5.0f });
	if (IsKeyDown(KEY_S)) player_location->SetPosition({ player_location->position.x, player_location->position.y + 5.0f });
	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
		Vector2 mousePosition = GetMousePosition();
		player_location->SetPosition(mousePosition);
		// Перемещение игрока к позиции курсора
	}
}

int battle_turn_counter = 0;
void LocationScreen(LocationInfo info) {
	// Обработка ввода        
	HandleInput();
	// Проверка на столкновение с локациями
	for (int i = 1; i < locations.size(); i++) {
		if (CheckCollisionRecs({ player_location->position.x, player_location->position.y, 50, 50 }, locations[i].bounds)) {
			if (IsKeyPressed(KEY_ENTER)) {
				currentLocation = locations[i].id;
				player->SetPosition({ screenWidth / 4,screenHeight / 2 });
				player->SetStamina(10);
				player->showBars = true;
				currentEnemy = enemies[GetRandomValue(0, 2)];

				currentEnemy->SetPosition({ screenWidth * 3 / 4,screenHeight / 2 });
				currentEnemy->SetStamina(10);
				// Открытие нового окна для информации о локации
				DrawText(locations[i].name.c_str(), screenWidth / 2 - MeasureText(locations[i].name.c_str(), 20) / 2, screenHeight / 2, 20, BLACK);
				DrawText("Press ESC to return", screenWidth / 2 - MeasureText("Press ESC to return", 20) / 2, screenHeight / 2 + 30, 20, BLACK);
				battle_turn_counter = 0;
			}
		}
	}
	// Логика игры (например, столкновения, бой и т.д.)
	if (player_location->position.y < 0) {
		player_location->position.y = 0;
	}
	if (player_location->position.x < 0) {
		player_location->position.x = 0;
	}
	if (player_location->position.x + 50 > screenWidth) {
		player_location->position.x = screenWidth - 50;
	}
	if (player_location->position.y + 50 > screenHeight) {
		player_location->position.y = screenHeight - 50;
	}
	
	// Отрисовка        
	BeginDrawing();
	ClearBackground(RAYWHITE);
	DrawTexture(info.Texture, 0, 0, RAYWHITE);
	DrawPlayer(player_location);
	DrawLocations(); // Отрисовка всех локаций
	DrawMoney();

	DrawText(TextFormat("Welcome"), 3*screenWidth/4, 3*screenHeight/4, 25, WHITE);
	DrawText(TextFormat("Click W, A, S, D to move around"), 3*screenWidth / 4,3*screenHeight / 4 + 30, 25, WHITE);
	DrawText(TextFormat("Click Enter to enter a location"), 3*screenWidth / 4, 3*screenHeight / 4 + 55, 25, WHITE);
	DrawText(TextFormat("Click Q to exit a location"), 3*screenWidth / 4, 3*screenHeight / 4 + 80, 25, WHITE);


	EndDrawing();
}
Rectangle chest_body = { 1050,550,30,30 };
NPCChest chest = {
	chest_body,
	VIOLET,
};
Rectangle chest1_body = { 550,350,30,30 };
NPCChest chest1 = {
	chest1_body,
	VIOLET,
};
Rectangle chest2_body = { 850,400,30,30 };
NPCChest chest2 = {
	chest2_body,
	VIOLET,
};

Rectangle ButtonBattle_body = { 1050,550,100,100 };
NPCChest ButtonBattle = {
	ButtonBattle_body,
	RED,
};


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
Rectangle shopBonus1_body = { 150,175,200,200 };
ShopButton shopBonus1 = {
	shopBonus1_body,
	VIOLET,
};
Rectangle shopBonus2_body = { 450,175,200,200 };
ShopButton shopBonus2 = {
	shopBonus2_body,
	GREEN,
};
Rectangle shopBonus3_body = { 750,175,200,200 };
ShopButton shopBonus3 = {
	shopBonus3_body,
	GRAY,
};
Rectangle shopBonus4_body = { 1050,175,200,200 };
ShopButton shopBonus4 = {
	shopBonus4_body,
	BLUE,
};
bool showDialog = false;
//Деревня
void VillageScreen(LocationInfo info) {
	if (IsKeyPressed(KEY_Q))
	{
		currentLocation = 0;
	}
	if (CheckCollisionPointRec({ player_location->position.x,player_location->position.y }, { npc_body })) {
		if (IsKeyPressed(KEY_ENTER)) {
			std::cout << "shop";

			showDialog = true;
		}
	}
	if (!showDialog) {
		HandleInput();
	}
	else {
		//Обработка нажатия на кнопки в диалоговом окне
		Vector2 mouse = GetMousePosition();
		if (CheckCollisionPointRec(mouse, shopExit.body) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
			showDialog = false;
		}
		else if (CheckCollisionPointRec(mouse, shopBonus1.body) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
			player->attack_bonus += 3;
			money = std::to_string(std::stoi(money) - 10);
		}
		else if (CheckCollisionPointRec(mouse, shopBonus2.body) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
			player->SetMaxHealth(player->current_health + 20);
			money = std::to_string(std::stoi(money) - 10);
		}
		else if (CheckCollisionPointRec(mouse, shopBonus3.body) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
			player->stamina_bonus += 5;
			money = std::to_string(std::stoi(money) - 30);
		}
		else if (CheckCollisionPointRec(mouse, shopBonus4.body) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
			player->regen_bonus += 1;
			money = std::to_string(std::stoi(money) - 20);
		}
	}
	if (CheckCollisionPointRec({ player_location->position.x,player_location->position.y }, { chest_body })) {
		if (IsKeyPressed(KEY_ENTER)) {
			money = std::to_string(std::stoi(money) + 10);
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
	DrawRectangleRec(chest.body, chest.color);
	if (showDialog) {
		//Отрисовка диалогового окна
		DrawRectangleRec(shop.body, shop.color);
		//DrawRectangleRec(shopExit.body, shopExit.color);
		DrawLineEx(
			{ shopExit.body.x, shopExit.body.y}, 
			{ shopExit.body.x + shopExit.body.width, shopExit.body.y + shopExit.body.height}, 
			4, RED);
		DrawLineEx(
			{ shopExit.body.x + shopExit.body.width, shopExit.body.y },
			{ shopExit.body.x, shopExit.body.y + shopExit.body.height },
			4, RED);
		DrawRectangleRec(shopBonus1.body, shopBonus1.color);
		DrawRectangleRec(shopBonus2.body, shopBonus2.color);
		DrawRectangleRec(shopBonus3.body, shopBonus3.color);
		DrawRectangleRec(shopBonus4.body, shopBonus4.color);
	}
	EndDrawing();
}

//лока старт
void ForestScreen(LocationInfo& info) {
	if (IsKeyPressed(KEY_Q)) {
		currentLocation = 0; // Возврат в основное меню
	}
	if (CheckCollisionPointRec({ player_location->position.x,player_location->position.y }, { chest_body })) {
		if (IsKeyPressed(KEY_ENTER)) {
			money = std::to_string(std::stoi(money) + 100);
		}
	}

	if (CheckCollisionPointRec({ player_location->position.x,player_location->position.y }, { ButtonBattle_body })) {
		if (IsKeyPressed(KEY_ENTER)) {
			currentLocation = 5;
			currentEnemy = enemies[GetRandomValue(0, 2)];

			player->SetPosition({ screenWidth / 4,screenHeight / 2 });
			player->SetStamina(10);
			player->showBars = true;
			currentEnemy->SetPosition({ screenWidth * 3 / 4,screenHeight / 2 });
			currentEnemy->SetStamina(10);
			return;
		}
	}
	HandleInput(); 
	// Отрисовка        
	BeginDrawing();
	ClearBackground(GREEN); // Фон для леса
	// Отрисовка текстуры
	DrawTexture(info.Texture, 0, 0, WHITE); // Отрисовка текстуры в верхнем левом углу
	DrawMoney();
	DrawPlayer(player_location); // Отрисовка игрока
	DrawRectangleRec(chest.body, chest.color);
	DrawRectangleRec(ButtonBattle.body, ButtonBattle.color);
	EndDrawing();
}

void CaveScreen(LocationInfo info) {
	if (IsKeyPressed(KEY_Q))
	{
		currentLocation = 0;
	}
	if (CheckCollisionPointRec({ player_location->position.x,player_location->position.y }, { chest1_body })) {
		if (IsKeyPressed(KEY_ENTER)) {
			money = std::to_string(std::stoi(money) + 1000);
		}
	}

	if (CheckCollisionPointRec({ player_location->position.x,player_location->position.y }, { ButtonBattle_body })) {
		if (IsKeyPressed(KEY_ENTER)) {

			currentLocation = 5;
			currentEnemy = enemies[GetRandomValue(0, 2)];
			player->SetPosition({ screenWidth / 4,screenHeight / 2 });
			player->SetStamina(10);
			player->showBars = true;
			currentEnemy->SetPosition({ screenWidth * 3 / 4,screenHeight / 2 });
			currentEnemy->SetStamina(10);
			return;
		}
	}


	HandleInput();
	// Отрисовка        
	BeginDrawing();
	//DrawEnemy(enemy);
	ClearBackground(BROWN);
	DrawTexture(info.Texture, 0, 0, WHITE);
	DrawPlayer(player_location);
	DrawMoney();
	DrawRectangleRec(chest1.body, chest1.color);
	DrawRectangleRec(ButtonBattle.body, ButtonBattle.color);
	EndDrawing();
}

void CastleScreen(LocationInfo info) {
	if (IsKeyPressed(KEY_Q))
	{
		currentLocation = 0;
	}

	if (CheckCollisionPointRec({ player_location->position.x,player_location->position.y }, { chest2_body })) {
		if (IsKeyPressed(KEY_ENTER)) {
			money = std::to_string(std::stoi(money) + 1);
		}
	}
	if (CheckCollisionPointRec({ player_location->position.x,player_location->position.y }, { ButtonBattle_body })) {
		if (IsKeyPressed(KEY_ENTER)) {

			currentLocation = 5;
			currentEnemy = enemies[GetRandomValue(0, 2)];
			player->SetPosition({ screenWidth / 4,screenHeight / 2 });
			player->SetStamina(10);
			player->showBars = true;
			currentEnemy->SetPosition({ screenWidth * 3 / 4,screenHeight / 2 });
			currentEnemy->SetStamina(10);
			return;
		}
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
	DrawRectangleRec(chest2.body, chest2.color);
	DrawRectangleRec(ButtonBattle.body, ButtonBattle.color);
	EndDrawing();
}


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
		player->SetHealth(100);
		player->SetStamina(0);
		player->showBars = false;
		player->speed = { 0,0 };
		player->position.x = screenWidth * 1 / 4;

		currentEnemy->SetHealth(currentEnemy->healthBar.GetMaxValue());
		currentEnemy->SetStamina(0);
		currentEnemy->speed = { 0,0 };
		currentEnemy->position.x = screenWidth * 3 / 4;

		game_over = false;
		battle_turn_counter = 0;
	}
	if (!game_over)
	{
		if (player->current_health <= 0 or currentEnemy->health <= 0) {
			game_over = true;
			money = std::to_string(std::stoi(money) + 15);
		}
		Vector2 mouse = GetMousePosition();


		if (CheckCollisionPointRec(mouse, button.body) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && battle_turn_counter % 2 == 0 && player->speed.x == 0 && player->current_stamina >= 5) {
			player->speed = { 30,0 };
			player->current_attack = 10;
			player->SetStamina(player->current_stamina - 5);
		}

		if (CheckCollisionPointRec(mouse, button1.body) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && battle_turn_counter % 2 == 0 && player->speed.x == 0 && player->current_stamina >= 10) {
			player->speed = { 30,0 };
			player->current_attack = 15;
			player->SetStamina(player->current_stamina - 10);
		}

		if (CheckCollisionPointRec(mouse, button2.body) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && battle_turn_counter % 2 == 0 && player->speed.x == 0 && player->current_stamina >= 30) {
			player->speed = { 30,0 };
			player->current_attack = 35;
			player->SetStamina(player->current_stamina - 30);
		}
		if (CheckCollisionPointRec(mouse, button3.body) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && battle_turn_counter % 2 == 0 && player->speed.x == 0 && player->current_stamina >= 15) {
			player->speed = { 30,0 };
			currentEnemy->attack = 0;
			player->current_attack = 0;
			currentEnemy->SetStamina(currentEnemy->stamina - 25);
			player->SetStamina(player->current_stamina - 11);

		}
		if (CheckCollisionPointRec(mouse, button4.body) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && battle_turn_counter % 2 == 0 && player->speed.x == 0 && player->current_stamina >= 20) {
			player->speed = { 30,0 };
			player->SetHealth(player->current_health + 20 + player->regen_bonus);
			player->current_attack = 0;
			player->SetStamina(player->current_stamina - 20);
		}

		if (CheckCollisionRecs({ player->position.x,player->position.y,100,100 }, { currentEnemy->position.x,currentEnemy->position.y,100,100 }) && battle_turn_counter % 2 == 0) {
			player->speed = { -30,0 };
			currentEnemy->SetHealth(currentEnemy->health - player->current_attack - player->attack_bonus);
		}
		if (player->speed.x < 0) {
			if (player->position.x < screenWidth / 4) {
				player->speed = { 0,0 };
				battle_turn_counter++;
			}
		}
		player->SetPosition(Vector2Add(player->position, player->speed));

		if (battle_turn_counter % 2 == 1 && currentEnemy->speed.x == 0)
			currentEnemy->speed = { -30,0 };
		if (CheckCollisionRecs({ currentEnemy->position.x,currentEnemy->position.y,100,100 }, { player->position.x,player->position.y,100,100 }) && battle_turn_counter % 2 == 1) {
			currentEnemy->speed = { 30,0 };
			player->SetHealth(player->current_health - GetRandomValue(currentEnemy->attack - 10, currentEnemy->attack + 10));
		}
		if (currentEnemy->speed.x > 0) {
			if (currentEnemy->position.x > screenWidth * 3 / 4) {
				currentEnemy->speed = { 0,0 };
				battle_turn_counter++;
				int stamina_regen = player->current_stamina + 10 + player->stamina_bonus;
				player->SetStamina(stamina_regen > player->max_stamina ? player->max_stamina : stamina_regen);
				currentEnemy->attack = 10;
				currentEnemy->SetStamina(currentEnemy->stamina + 10);
				/*	currentEnemy->stamina = 100;*/
			}
		}
		currentEnemy->SetPosition(Vector2Add(currentEnemy->position, currentEnemy->speed));
	}
	// Отрисовка        
	BeginDrawing();

	ClearBackground(RAYWHITE);
	DrawTexture(info.Texture, 0, 0, WHITE);
	DrawPlayer(player);
	DrawEnemy(currentEnemy);
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

	player_location = new Player(
		{ screenWidth / 2, screenHeight / 2 },
		100,
		10,
		0,
		50,
		texture,
		{ 0,0 }
	);
	if (image.data != NULL)
	{
		ImageCrop(&image, { 51,133,41,58 });
		ImageResize(&image, 100, 100);
		texture = LoadTextureFromImage(image);

	}
	player = new Player{
		{ screenWidth / 2, screenHeight / 2 },
		100,
		10,
		100,
		100,
		texture ,
		{0,0}
	};

	Enemy* enemy = new Enemy{
		{ screenWidth / 2, screenHeight / 2 },
		100,
		10,
		0,
		10,
		texture,
		{0,0}
	};
	enemies.push_back(enemy);
	enemy = new Enemy{
		{ screenWidth / 2, screenHeight / 2 },
		200,
		15,
		0,
		100,
		texture,
		{0,0}
	};
	enemies.push_back(enemy);
	enemy = new Enemy{
		{ screenWidth / 2, screenHeight / 2 },
		1000,
		50,
		0,
		500,
		texture,
		{0,0}
	};
	enemies.push_back(enemy);

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