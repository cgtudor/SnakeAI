#include <SFML/Graphics.hpp>
//#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <queue>
#include <unordered_map>;


float distance(sf::Vector2i snakePos, sf::Vector2f cherryPos, sf::Vector2f snake)
{
	float dist = sqrt(pow(((snakePos.x + snake.x) - cherryPos.x), 2) + pow(((snakePos.y + snake.y) - cherryPos.y), 2));
	return dist;
}

std::vector<sf::Vector2f> neighbors(sf::Vector2f current)
{
	std::vector<sf::Vector2f> total;
	sf::Vector2f neighbor;
	neighbor.x = current.x + 1;
	neighbor.x = current.y;
	//if(neighbor.x * 16 = snake)
}

int map[56][100], cmap[56][100];

/*
std::unordered_map<sf::Vector2f, sf::Vector2f> findPath(sf::Vector2f start, sf::Vector2f end)
{
	std::queue<sf::Vector2f> frontier;
	frontier.push(start);
	std::unordered_map<sf::Vector2f, sf::Vector2f> came_from;
	while (!frontier.empty()) {
		sf::Vector2f current = frontier.front();
		frontier.pop();
		for (sf::Vector2f next : graph.neighbors(current)) {
			if (came_from.find(next) == came_from.end()) {
				frontier.push(next);
				came_from[next] = current;
			}
		}
		return came_from;
	}
}
*/

int main()
{
	sf::RenderWindow window(sf::VideoMode(1600, 896), "Snake");

	sf::Clock clock;
	sf::Clock deltaClock;

	sf::Time deltaTime = deltaClock.restart();
	sf::Time timeElapsed = deltaClock.restart();
	sf::Time lastTime = deltaClock.restart();

	sf::Text timeText;
	sf::Font arial;

	sf::Texture tileSheet;

	sf::Sprite snakeSprite;
	sf::Sprite cherry;

	sf::IntRect bounds{ 0,0,1600,896 };

	sf::Keyboard key;

	sf::Vector2i direction{ 0,0 };
	sf::Vector2f lastPosition{ 496,496 };
	
	std::vector<sf::Sprite> snake;

	double snakeSpeed = 0.15;

	float distances[4];

	bool placeApple = false;

	if (!tileSheet.loadFromFile("tiles.png"))
	{
		
	}

	snakeSprite.setTexture(tileSheet);
	snakeSprite.setTextureRect(sf::IntRect(80, 0, 16, 16));
	snakeSprite.setPosition(496, 496);
	snake.push_back(snakeSprite);
	map[31][31] = 1;

	cherry.setTexture(tileSheet);
	cherry.setTextureRect(sf::IntRect(112, 80, 16, 16));
	cherry.setPosition((rand() % 100) * 16, (rand() % 50) * 16);
	map[(int)cherry.getPosition().y / 16][(int)cherry.getPosition().x / 16] = 2;

	arial.loadFromFile("arialCE.ttf");
	timeText.setFillColor(sf::Color::Cyan);
	timeText.setFont(arial);
	timeText.setPosition(0, 0);
	timeText.setString(std::to_string(clock.getElapsedTime().asSeconds()));

	while (window.isOpen())
	{
		window.clear(sf::Color::White);
		timeElapsed += deltaTime;
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
		if (key.isKeyPressed(sf::Keyboard::Up))
		{
			direction.x = 0;
			direction.y = -16;
		}
		else if (key.isKeyPressed(sf::Keyboard::Down))
		{
			direction.x = 0;
			direction.y = 16;
		}
		else if (key.isKeyPressed(sf::Keyboard::Left))
		{
			direction.x = -16;
			direction.y = 0;
		}
		else if (key.isKeyPressed(sf::Keyboard::Right))
		{
			direction.x = 16;
			direction.y = 0;
		}
		bool found = false;
		int directioni = 0;
		///AI
		int maxi = 0;
		
		for (int i = 0; i < 4; i++)
			distances[i] = 0;
		direction.x = 0;
		direction.y = -16;
		distances[0] = distance(direction, cherry.getPosition(), (*snake.begin()).getPosition());
		direction.x = 0;
		direction.y = 16;
		distances[1] = distance(direction, cherry.getPosition(), (*snake.begin()).getPosition());
		direction.x = -16;
		direction.y = 0;
		distances[2] = distance(direction, cherry.getPosition(), (*snake.begin()).getPosition());
		direction.x = 16;
		direction.y = 0;
		distances[3] = distance(direction, cherry.getPosition(), (*snake.begin()).getPosition());
		direction.x = 0;
		direction.y = 0;
		while (!found)
		{
			
			for (int i = 0; i < 4; i++)
			{
				if (distances[i] < distances[maxi] && distances[i] != -1)
				{
					maxi = i;
				}
			}
			directioni = maxi;
			switch (directioni)
			{
			case 0:
				direction.x = 0;
				direction.y = -16;
				break;
			case 1:
				direction.x = 0;
				direction.y = 16;
				break;
			case 2:
				direction.x = -16;
				direction.y = 0;
				break;
			case 3:
				direction.x = 16;
				direction.y = 0;
				break;
			default:
				break;
			}
			if (snake.size() == 1)
			{
				if (!bounds.contains((*snake.begin()).getPosition().x + direction.x, (*snake.begin()).getPosition().y + direction.y))
				{
					distances[directioni] = -1;
				}
				else
					found = true;
			}
			else
			{
				for (auto i = snake.begin() + 1; i != snake.end(); ++i)
				{
					if ((*i).getPosition().x == (*snake.begin()).getPosition().x + direction.x && (*i).getPosition().y == (*snake.begin()).getPosition().y + direction.y)
					{
						distances[directioni] = -1;
						break;
					}
					if (!bounds.contains((*snake.begin()).getPosition().x + direction.x, (*snake.begin()).getPosition().y + direction.y))
					{
						distances[directioni] = -1;
						break;
					}
					found = true;
				}
			}
			
			for (int i = 0; i < 4; i++)
			{
				if (distances[i] != -1)
				{
					maxi = i;
					break;
				}
			}
			if (distances[directioni] == -1)
				found = false;
		}
		


		if (timeElapsed.asSeconds() > lastTime.asSeconds() + snakeSpeed)
		{
			if ((*snake.begin()).getPosition() == cherry.getPosition())
			{
				snake.push_back(snakeSprite);
				placeApple = false;
			}
			if (snake.size() == 1) 
			{
				map[(int)(*snake.begin()).getPosition().y / 16][(int)(*snake.begin()).getPosition().x / 16] = 0;
				(*snake.begin()).setPosition(lastPosition.x + direction.x, lastPosition.y + direction.y);
				map[(int)(*snake.begin()).getPosition().y / 16][(int)(*snake.begin()).getPosition().x / 16] = 1;
				lastPosition = (*snake.begin()).getPosition();
			}
			else
			{
				sf::Vector2f lastPos = (*snake.begin()).getPosition();
				sf::Vector2f lastPos2 = (*snake.begin()).getPosition();
				(*snake.begin()).setPosition(lastPosition.x + direction.x, lastPosition.y + direction.y);
				lastPosition = (*snake.begin()).getPosition();
				for (auto i = snake.begin() + 1; i != snake.end(); ++i)
				{
					if ((*i).getPosition() == (*snake.begin()).getPosition())
					{
						return 0;
					}
					lastPos2 = (*i).getPosition();
					map[(int)(*i).getPosition().y / 16][(int)(*i).getPosition().x / 16] = 0;
					(*i).setPosition(lastPos.x, lastPos.y);
					map[(int)(*i).getPosition().y / 16][(int)(*i).getPosition().x / 16] = 1;
					lastPos = lastPos2;
				}
			}
			lastTime = timeElapsed;
		}
		if (!bounds.contains((*snake.begin()).getPosition().x, (*snake.begin()).getPosition().y))
		{
			return 0;
		}
		if (!placeApple)
		{
			map[(int)cherry.getPosition().y / 16][(int)cherry.getPosition().x / 16] = 0;
			cherry.setPosition((rand() % 100) * 16, (rand() % 50) * 16);
			map[(int)cherry.getPosition().y / 16][(int)cherry.getPosition().x / 16] = 2;
			snakeSpeed -= 0.005;
			placeApple = true;
		}

		timeText.setString(std::to_string(timeElapsed.asSeconds()));
		window.draw(cherry);
		for (auto &i : snake)
		{
			window.draw(i);
		}
		window.draw(timeText);
		
		window.display();
		deltaTime = deltaClock.restart();
	}
	return 0;
}

