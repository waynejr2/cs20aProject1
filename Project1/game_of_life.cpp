#include <iostream>
#include <string>
#include <cstdlib>
using std::string;
using std::cout;
using std::cin;
using std::getline;
using std::endl;

///////////////////////////////////////////////////////////////////////////
// Global constants
///////////////////////////////////////////////////////////////////////////

const int MAX_ROWS = 11;
const int MAX_COLS = 11;

const int TOAD_HEIGHT = 2;
const int TOAD_WIDTH = 4;

const int GLIDER_SIZE = 3;

const int MAX_STEPS = 50;

const char ALIVE = 'X';
const char DEAD = '.';

///////////////////////////////////////////////////////////////////////////
//  Utility function declarations
///////////////////////////////////////////////////////////////////////////

void clearScreen();
void delay(int ms);
void report();



///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class World;

class Life {
public:

	int getCol() const; // const member functions cannot modify member variables.
	int getRow() const;
	int getHeight() const;
	int getWidth() const;
	char getFromFigure(int r, int c) const;

protected:
	int m_col;
	int m_row;
	int m_height;
	int m_width;
	char **m_figure;
	World *m_world;
};

class Toad : public Life {
public:

	// Constructor/destructor
	Toad(int r, int c);
	~Toad();
};
class Glider : public Life {
public:
	// Constructor/destructor
	Glider(int r, int c);
	~Glider();
};

class World {
public:
	// Constructor/destructor
	World();
	~World();

	void print() const;
	bool hasWorldChanged() const;

	bool initLife(Life *life);
	void updateWorld();
private:

	char getNextState(char state, char row, char col, bool toggle) const;

	// The rules of Conway's Game of Life requires each cell to
	// examine it's neighbors.  So, we have to check the entire world
	// first before we change it.  We can use two copies of the world, 
	// one to check the current state (current day) then another to generate 
	// the next state(for the next day). We can toggle between them each 
	// step, to avoid having to copy between worlds each step (day) of the game.
	char **m_world;
	char **m_otherWorld;
	bool m_toggle;


};

class Game {
public:
	// Constructor/destructor
	Game(Life **life, int numLife);
	~Game();

	void play();
private:
	World * m_world;
	int m_steps;
	bool m_automate;
};

///////////////////////////////////////////////////////////////////////////
// Life Implemention
///////////////////////////////////////////////////////////////////////////

int Life::getCol() const {
	return m_col;
}
int Life::getRow() const {
	return m_row;
}
int Life::getHeight() const {
	return m_height;
}
int Life::getWidth() const {
	return m_width;
}
char Life::getFromFigure(int r, int c) const {
	return m_figure[r][c];
}

///////////////////////////////////////////////////////////////////////////
// Toad Implemention
///////////////////////////////////////////////////////////////////////////

Toad::Toad(int r, int c) {

	m_col = c;
	m_row = r;
	m_height = TOAD_HEIGHT;
	m_width = TOAD_WIDTH;

	//Allocate space for figure
	m_figure = new char*[TOAD_HEIGHT];
	for (int i = 0; i < TOAD_HEIGHT; i++) {
		m_figure[i] = new char[TOAD_WIDTH];
	}

	//Initialize figure
	for (int i = 0; i < TOAD_HEIGHT; i++) {
		for (int j = 0; j < TOAD_WIDTH; j++) {
			m_figure[i][j] = ALIVE;
		}
	}

	m_figure[0][0] = DEAD;
	m_figure[1][3] = DEAD;
}

Toad::~Toad() {
	for (int i = 0; i < TOAD_HEIGHT; i++) {
		delete[] m_figure[i];
	}
	delete[] m_figure;
}
///////////////////////////////////////////////////////////////////////////
// Glider Implemention
///////////////////////////////////////////////////////////////////////////

Glider::Glider(int r, int c) {
	m_col = c;
	m_row = r;
	m_height = GLIDER_SIZE;
	m_width = GLIDER_SIZE;

	// Allocate space for figure
	m_figure = new char*[GLIDER_SIZE];
	for (int i = 0; i < GLIDER_SIZE; i++) {
		m_figure[i] = new char[GLIDER_SIZE];
	}

	// Initialize figure
	for (int i = 0; i < GLIDER_SIZE; i++) {
		for (int j = 0; j < GLIDER_SIZE; j++) {
			m_figure[i][j] = DEAD;
		}
	}
	m_figure[0][1] = ALIVE;
	m_figure[1][2] = ALIVE;
	m_figure[2][0] = ALIVE;
	m_figure[2][1] = ALIVE;
	m_figure[2][2] = ALIVE;
}

Glider::~Glider() {
	for (int i = 0; i < GLIDER_SIZE; i++) {
		delete[] m_figure[i];
	}
	delete[] m_figure;
}


///////////////////////////////////////////////////////////////////////////
// World Implemention
///////////////////////////////////////////////////////////////////////////

World::World() :
	m_toggle(true)
{
	m_world = new char*[MAX_ROWS];
	m_otherWorld = new char*[MAX_ROWS];
	for (char i = 0; i < MAX_ROWS; i++) {
		m_world[i] = new char[MAX_COLS];
		m_otherWorld[i] = new char[MAX_COLS];
	}

	for (char i = 0; i < MAX_ROWS; i++) {
		for (char j = 0; j < MAX_COLS; j++) {
			m_world[i][j] = DEAD;
		}
	}

}

World::~World() {
	for (char i = 0; i < MAX_ROWS; i++) {
		delete[] m_world[i];
		delete[] m_otherWorld[i];
	}
	delete[] m_world;
	delete[] m_otherWorld;

}

void World::print() const {
	clearScreen();
	if (m_toggle) {
		for (char i = 0; i < MAX_ROWS; i++) {
			for (char j = 0; j < MAX_COLS; j++) {
				std::cout << m_world[i][j];
			}
			std::cout << std::endl;
		}
	}
	else {
		for (char i = 0; i < MAX_ROWS; i++) {
			for (char j = 0; j < MAX_COLS; j++) {
				std::cout << m_otherWorld[i][j];
			}
			std::cout << std::endl;
		}
	}
	for (char i = 0; i < MAX_COLS; i++) {
		std::cout << '=';
	}
	std::cout << std::endl;
}
bool World::hasWorldChanged() const {

	for (char i = 0; i < MAX_ROWS; i++) {
		for (char j = 0; j < MAX_COLS; j++) {

			if (m_otherWorld[i][j] != m_world[i][j]) {
				return true;
			}

		}
	}
	return false;
}
void World::updateWorld() {
	if (m_toggle) {
		for (char i = 0; i < MAX_ROWS; i++) {
			for (char j = 0; j < MAX_COLS; j++) {
				m_otherWorld[i][j] =
					getNextState(m_world[i][j], i, j, m_toggle);
			}
		}
		m_toggle = !m_toggle;
	}
	else {
		for (char i = 0; i < MAX_ROWS; i++) {
			for (char j = 0; j < MAX_COLS; j++) {
				m_world[i][j] =
					getNextState(m_otherWorld[i][j], i, j, m_toggle);
			}
		}
		m_toggle = !m_toggle;
	}
}

char World::getNextState(char state, char row, char col, bool toggle) const {
	int yCoord = row;
	int xCoord = col;
	char neighbors = 0;
	if (toggle) {
		for (char i = yCoord - 1; i <= yCoord + 1; i++) {
			for (char j = xCoord - 1; j <= xCoord + 1; j++) {
				if (i == yCoord && j == xCoord) {
					continue;
				}
				if (i > -1 && i < MAX_ROWS && j > -1 && j < MAX_COLS) {
					if (m_world[i][j] == ALIVE) {
						neighbors++;
					}
				}
			}
		}
	}
	else {
		for (char i = yCoord - 1; i <= yCoord + 1; i++) {
			for (char j = xCoord - 1; j <= xCoord + 1; j++) {
				if (i == yCoord && j == xCoord) {
					continue;
				}
				if (i > -1 && i < MAX_ROWS && j > -1 && j < MAX_COLS) {
					if (m_otherWorld[i][j] == ALIVE) {
						neighbors++;
					}
				}
			}
		}
	}
	if (state == ALIVE) {
		return (neighbors > 1 && neighbors < 4) ? ALIVE : DEAD;
	}
	else {
		return (neighbors == 3) ? ALIVE : DEAD;
	}
}

bool World::initLife(Life *life) {

	if (life == nullptr) {
		cout << "Cannot add nullptr life" << endl;
		return false;
	}

	// Should check life extents with world bounds.

	for (char i = life->getRow(); i - life->getRow() < life->getHeight(); i++) {
		for (char j = life->getCol(); j - life->getCol() < life->getWidth(); j++) {
			if (i < MAX_ROWS && j < MAX_COLS) {
				m_world[i][j] =
					life->getFromFigure(i - life->getRow(), j - life->getCol());
			}
		}
	}
	return true;
}


///////////////////////////////////////////////////////////////////////////
// Game Implemention
///////////////////////////////////////////////////////////////////////////

Game::Game(Life **life, int numLife) {

	m_steps = 0;
	m_automate = false;
	m_world = new World();

	if (life != nullptr) {
		for (int i = 0; i < numLife; i++) {
			if (life[i] != nullptr) {
				bool success = m_world->initLife(life[i]);
				if (!success) {
					cout << "Failed to add life to the world" << endl;
				}
			}

		}
	}

}
Game::~Game() {

	delete m_world;
}

void Game::play() {
	while (true) {
		m_world->print();

		if (!m_world->hasWorldChanged()) {
			cout << "The world has not changed, ending game." << endl;
			return;
		}
		else {

			if (!m_automate) {
				cout << "command (<space> to step, <a> to automate, <q> to quit): ";

				string action;
				getline(cin, action);

				switch (action[0])
				{

				default:
					cout << '\a' << endl;  // beep
					continue;
				case 'q':
					cout << "Quitting Game." << endl;
					return;
				case 's':

					continue;
				case ' ':

					break;
				case 'a':
					m_automate = true;
					break;

				}
			}
			else {
				if (m_steps >= MAX_STEPS) {
					cout << "Reached max steps, quitting." << endl;
					return;
				}
				delay(300);
			}
			m_steps++;
		}
		m_world->updateWorld();

	}
}


///////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////

int main() {

	Life **population = new Life*[2];

	//population[0] = new Toad(4, 5); //toad alone
	//int numLife = 1;

	//population[0] = new Glider(0,0); //glider alone
	//int numLife = 1;


	population[0] = new Glider(0, 0); // Toad and Glider
	population[1] = new Toad(4, 5);
	int numLife = 2;

	// Create the game
	Game g(population, numLife);

	// Run the game
	g.play();

	// Report
	report();

	// Clean up
	delete population[0];
	delete population[1];
	delete[] population;

}

void report() {
	std::string msg = "Hello World!"; // Replace Hello World with your answer.
	std::cout << msg << std::endl;
}

// DO NOT MODIFY ANY CODE BETWEEN HERE AND THE END OF THE FILE!!!
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++ UNDER LINUX.

#include <chrono>
#include <thread>

void delay(int ms) {
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}


///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////


// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)

#ifdef _MSC_VER  //  Microsoft Visual C++

#include <windows.h>

void clearScreen()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	COORD upperLeft = { 0, 0 };
	DWORD dwCharsWritten;
	FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
		&dwCharsWritten);
	SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not Microsoft Visual C++, so assume UNIX interface

#include <iostream>
#include <cstring>
#include <cstdlib>
using namespace std;
void clearScreen()  // will just write a newline in an Xcode output window
{
	static const char* term = getenv("TERM");
	if (term == nullptr || strcmp(term, "dumb") == 0)
		cout << endl;
	else
	{
		static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
		cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
	}
}

#endif