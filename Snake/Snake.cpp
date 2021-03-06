
#include <string>
#include<conio.h>
#include<iostream>
#include<vector>
#include <ctime>
#include <windows.h>
#include <random>
#include <chrono>

#define PLANEHEIGHT 20
#define PLANEWIDTH 40
unsigned short food_X, food_Y;

// obiekty potrzebne to generowania przypadkowych liczb 
std::random_device                 randomDevice;							
std::mt19937                       engine(randomDevice());
std::uniform_int_distribution<int> distribution_X(1, PLANEHEIGHT-1);
std::uniform_int_distribution<int> distribution_Y(1, PLANEWIDTH-1);



// struktura stworzona do przechowywania polozenia snake'a
struct Cord
{
	int x, y;

	Cord(int x, int y) {
		this->x = x;
		this->y = y;
	}
	bool operator==(const Cord &cord){
		return ((this->x == cord.x) && (this->y == cord.y));
	}
	
	Cord operator+(const Cord &cord)
	{
		this->x += cord.x;
		this->y += cord.y;
		return *this;
	}
	Cord operator-(const Cord &cord)
	{
		this->x -= cord.x;
		this->y -= cord.y;
	}
	Cord getCord() { return *this; }

	
};
// typ wyliczeniowy stworzony do klarownego rozrozniania kierunkow
enum Direction{RIGHT = 0, LEFT, UP, DOWN};


class GameSnake 
{

private:

	Direction direct;
	bool gameover;
	int score;
	std::vector<Cord> snake;
	char waz;
	std::chrono::high_resolution_clock::time_point time;

public:
	
	GameSnake();
	char field[PLANEHEIGHT][PLANEWIDTH];
	void move();
	void draw();
	void inPut();
	void Game();
	void moveCordSnake(Cord newcord);
	void addLengthSnake();
	void checkHead();
	void win();
	void esc();
	
};



int main()
{
	bool koniec = false;
	char litera;
	GameSnake *snake = new GameSnake();
	do {
		snake->Game();
		system("cls");
		std::cout << "\n\n\n\n\n";
		std::cout << "\t\t\t#######################" << std::endl;
		std::cout << "\t\t\t#####Chcesz zagrac#####" << std::endl;
		std::cout << "\t\t\t#######ponownie?#######" << std::endl;
		std::cout << "\t\t\t####T-Tak####w-Wyjdz###" << std::endl;
		std::cout << "\t\t\t#######################" << std::endl;

		if (_kbhit())
		{
			switch (_getch())
			{
			case 't':
				system("cls");
				delete snake;
				snake = new GameSnake();
				break;
			case 'w':
				koniec = true;
				system("cls");
				break;
			defualt:
				break;
			}
		}
	} while (koniec == false);
}

void GameSnake::move()
{
		
	switch (direct)
		{
		case UP:
			if (snake[0].x == 0) moveCordSnake(Cord(PLANEHEIGHT,snake[0].y));
			moveCordSnake(snake[0].getCord() + Cord(-1, 0));
			break;

		case DOWN:
			if (snake[0].x == PLANEHEIGHT) moveCordSnake(Cord(0, snake[0].y));
			moveCordSnake(snake[0].getCord() + Cord(1, 0));
			break;

		case RIGHT:
			if (snake[0].y == PLANEWIDTH) moveCordSnake(Cord(snake[0].x,0));
			moveCordSnake(snake[0].getCord() + Cord(0, 1));
			break;

		case LEFT:
			if (snake[0].y == 0) moveCordSnake(Cord(snake[0].x, PLANEWIDTH));
			moveCordSnake(snake[0].getCord() + Cord(0, -1));
			break;

		default:
			break;
		}
	
}

void GameSnake::draw()
{
	
	system("cls");
	std::cout << "Score: [" << score << "]";
	for (int i = 0; i < PLANEHEIGHT; i++)
	{
		std::cout << "\n";

		for (int j = 0; j < PLANEWIDTH;j++)
		{
			if (std::find(snake.begin(), snake.end(), Cord(i, j)) != snake.end()) // jezeli znaleziono takie i j 
			{																		// ktore ma snake to pole przyjmuje 1
				field[i][j] = waz;
			}
			else if (food_X == i && food_Y == j) field[i][j] = '@';		// jezeli jedzenie na takie kordy to wpisuje 
			else
			{
				field[i][j] = ' '; // a jezeli nie to znacyz ze jest to pusta przestrzen
				
			}
			//setCursorPosition(i, j);
			std::cout << field[i][j]; // wypisywanie pola de facto
			}

	}
}
// ustawianie kierunku w zaleznosci od klikniecia 
void GameSnake::inPut() {

	if (_kbhit())
	{
		switch (_getch())
		{
		case 'w':
			if (direct != UP && direct != DOWN) // waz nie moze sie poruszac w sobie
				direct = UP;
			break;

		case 's':
			if (direct != UP && direct != DOWN)
				direct = DOWN;
			break;

		case 'd':
			if (direct != RIGHT && direct != LEFT)
				direct = RIGHT;
			break;

		case 'a':
				if (direct != RIGHT && direct != LEFT)
			direct = LEFT;
			break;
		case char(27) : esc();
			break;

		default:
			
			break;
		}
	}
}
// funkcja z glowna petla gry
void GameSnake::Game() {

	while (!gameover)
	{
		draw();
		move();
		inPut();
		checkHead();
		addLengthSnake();
		if (snake.size() == (PLANEWIDTH * PLANEHEIGHT)) win();
		
		Sleep(100-(snake.size()));
	}
}
// inicjalizacja snakea o dlugosci dwoch jednostek
// oraz przypisanie jedzeniu przypadkowych kordynantow 
// domyslny kierunek w prawo
GameSnake::GameSnake() 
{
	for(int i = 0; i < 2; i++)
		snake.push_back(Cord(10, i+10));

	food_X = distribution_X(engine);
	food_Y = distribution_Y(engine);
	 time = std::chrono::high_resolution_clock::now(); // ustawiam czas startowy w ktorym zostalo stworzone jedzenie

	direct = RIGHT;
	score = 0;
	gameover = false;
	waz = 219; // ustawiam wyglad naszego weza
	

}
//dodajemy do vectorowego snakea nowe kordy tzn tam gdzie sie ruszyl
// i jezeli snake jest wiekszy niz 1 to usuwamy jego ogon  
void GameSnake::moveCordSnake(Cord newcord)
{
	
	auto helpvar = snake[0];
	snake.insert(snake.begin(), newcord);

	if(snake.size() > 1)
		snake.pop_back();
	
}
// jezeli glowa snakea byla na polu jedzenia to dodajemy ten kord staje sie kordem snakea
// przy okazji jedzenie dostaje nowe kordy oraz dodajemy punkty
void GameSnake::addLengthSnake()
{
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now(); // czas aktualny
	std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end - time); // roznica miedzy stworzeniem jedzenia a czasem aktualnym
	
	 if (food_X == snake.at(0).x && food_Y == snake.at(0).y)
	{
		snake.insert(snake.begin(), Cord(food_X, food_Y));
		food_X = distribution_X(engine);
		food_Y = distribution_Y(engine);
		time = std::chrono::high_resolution_clock::now(); // nowy czas stworzenia jablka
		score += snake.size()*7;
	}
	 else if (time_span.count() > 6.0) // jezeli minelo 6 sekund od stworzenia ostatniego jablka stworz nowy
	 {
		 food_X = distribution_X(engine);
		 food_Y = distribution_Y(engine);
		 time = std::chrono::high_resolution_clock::now(); // nowy czas stworzenia jablka
	 }
}

// sprawdzamy czy glowa weza dotyka siebie
void GameSnake::checkHead()
{
	int x_head = 0, y_head = 0;
	x_head = snake[0].x;
	y_head = snake[0].y;
	for (int i = 1; i < snake.size(); i++)
	{
		if (snake[i].x == x_head && snake[i].y == y_head)
		{
			gameover = true;
			break; // skoro uderzyl to po co sprawdzac dalej
		}
	}
	
}
void GameSnake::win()
{
	system("cls");
	std::cout << "\n\n\n\n\n";
	std::cout << "\t\t\t#######################" << std::endl;
	std::cout << "\t\t\t#######WYGRALES########" << std::endl;
	std::cout << "\t\t\t#######################" << std::endl;
	std::cout << "\t\t\t#######################" << std::endl;
	std::cout << "\t\t\t#######################" << std::endl;
	gameover = true;
}

void GameSnake::esc()
{
	system("cls");
	std::cout << "\n\n\n\n\n";
	std::cout << "\t\t#######################" << std::endl;
	std::cout << "\t\t#   Chcesz Wrocic?    #" << std::endl;
	std::cout << "\t\t#Tak - T  #  Wyjdz - W#" << std::endl;
	std::cout << "\t\t#                     #" << std::endl;
	std::cout << "\t\t#######################" << std::endl;
	
		switch (_getch())
		{
		case 't':
			system("cls");
			break;
		case 'w':
			gameover = true;
			
			system("cls");
			break;
		defualt:
			break;
		}
	
}