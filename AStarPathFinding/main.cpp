#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
using namespace sf;

const int WIDTH = 1400, HEIGHT = 1400;
const int COLUMNS = 20, ROWS = 20;
const int GRID_SQUARE_COUNT = COLUMNS * ROWS;
const int RECT_SIZE = WIDTH / COLUMNS;

class Grid {
	RectangleShape grid[ROWS][COLUMNS];
	bool startSet, endSet;
	RectangleShape startRect, endRect;
public:
	Grid() {
		startSet = false;
		endSet = false;
	}

	void CreateGrid() {
		for (int i = 0; i < ROWS; i++) {
			for (int j = 0; j < COLUMNS; j++) {
				grid[i][j] = RectangleShape(Vector2f(RECT_SIZE, RECT_SIZE));
				grid[i][j].setPosition(i * RECT_SIZE, j * RECT_SIZE);
				grid[i][j].setFillColor(Color::White);
				grid[i][j].setOutlineColor(Color::Black);
				grid[i][j].setOutlineThickness(5.0);
			}
		}
	}

	void DrawGrid(RenderWindow &window) {
		for (int i =0 ; i < ROWS; i++)
			for (int j = 0; j < COLUMNS; j++)
				window.draw(grid[i][j]);
	}

	void SetStart(RenderWindow &window) {
		if (!startSet && Keyboard::isKeyPressed(Keyboard::S)) {
			for (int i = 0; i < ROWS; i++) {
				for (int j = 0; j < COLUMNS; j++) {
					IntRect square(grid[i][j].getPosition().x, grid[i][j].getPosition().y, grid[i][j].getGlobalBounds().width - 10, grid[i][j].getGlobalBounds().height - 10);
					if (square.contains(Mouse::getPosition(window))) {
						grid[i][j].setFillColor(Color::Blue);
						startSet = true;
						startRect = grid[i][j];
						return;
					}
				}
			}
		}
	}

	void SetEnd(RenderWindow &window) {
		if (!endSet && Keyboard::isKeyPressed(Keyboard::E)) {
			for (int i = 0; i < ROWS; i++) {
				for (int j = 0; j < COLUMNS; j++) {
					IntRect square(grid[i][j].getPosition().x, grid[i][j].getPosition().y, grid[i][j].getGlobalBounds().width - 10, grid[i][j].getGlobalBounds().height - 10);
					if (square.contains(Mouse::getPosition(window))) {
						grid[i][j].setFillColor(Color::Red);
						endSet = true;
						endRect = grid[i][j];
						return;
					}
				}
			}
		}
	}

	void SetObstacle(RenderWindow &window) {
		if (endSet && startSet && Mouse::isButtonPressed(Mouse::Left)) {
			for (int i = 0; i < ROWS; i++) {
				for (int j = 0; j < COLUMNS; j++) {
					IntRect square(grid[i][j].getPosition().x, grid[i][j].getPosition().y, grid[i][j].getGlobalBounds().width - 10, grid[i][j].getGlobalBounds().height - 10);
					bool s = (grid[i][j].getFillColor() == startRect.getFillColor()) ? true : false;
					bool e = (grid[i][j].getFillColor() == endRect.getFillColor()) ? true : false;
					if (square.contains(Mouse::getPosition(window))) {
						if (!s && !e) {
							grid[i][j].setFillColor(Color::Black);
						}
					}
				}
			}
		}
	}
};

int main()
{
	// Create the main window
	RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "A* Path Finding");

	Grid *grid = new Grid();
	grid->CreateGrid();
	// Start the game loop
	while (window.isOpen())
	{
		// Process events
		Event event;
		while (window.pollEvent(event))
		{
			// Close window: exit
			if (event.type == Event::Closed) {
				window.close();
			}
			// Escape pressed: exit
			if (event.type == Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
				window.close();
			}

			if (event.type == Event::MouseMoved) {
				grid->SetObstacle(window);
			}

			if (event.type == Event::MouseButtonPressed) {
				if (event.mouseButton.button == Mouse::Left) {
					grid->SetStart(window);
					grid->SetEnd(window);
				}
			}
		}
		// Clear screen
		window.clear(Color::White);

		grid->DrawGrid(window);

		// Update the window
		window.display();
	}
	return EXIT_SUCCESS;
}
