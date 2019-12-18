#include <SFML/Graphics.hpp>

auto brown = sf::Color{50, 25, 1};
auto green = sf::Color{1, 191, 1};
auto darkGreen = sf::Color{1, 127, 1};
auto yellow = sf::Color{255, 255, 1};
auto white = sf::Color{255, 255, 255};
auto red = sf::Color{255, 1, 1};
auto black = sf::Color{1, 1, 1};
auto blue = sf::Color{1, 1, 255};
auto darkGrey = sf::Color{63, 63, 63};

auto viewportWidth = 640;
auto viewportHeight = 640;

float scale = 10;

auto constant1 = sqrt(2);
auto constant2 = sqrt(6);
auto ratio = 2.0f;
auto lightCoefficient = 1.1;

bool gridOn = false;


float GetRealXCoordinate(const float x)
{
	return (scale * 2) / viewportWidth * x - scale;
}

float GetRealYCoordinate(const float y)
{
	return (scale * 2) / viewportHeight * (viewportHeight - y) - scale;
}

float GetViewportXCoordinate(const float realX)
{
	return viewportWidth / (scale * 2) * (realX + scale);
}

float GetViewportYCoordinate(const float realY)
{
	return viewportHeight - viewportHeight / (scale * 2) * (realY + scale);
}

float GetProjectedX(const float pointX, const float pointY, const float pointZ)
{
	return (pointX - pointZ) / constant1;
}

float GetProjectedY(const float pointX, const float pointY, const float pointZ)
{
	return (pointX + ratio * pointY + pointZ) / constant2;
}


sf::Color DarkenColor(const sf::Color color, const float intensity)
{
	return sf::Color{
		uint8_t(std::min(color.r / pow(lightCoefficient, intensity), 255.0)),
		uint8_t(std::min(color.g / pow(lightCoefficient, intensity), 255.0)),
		uint8_t(std::min(color.b / pow(lightCoefficient, intensity), 255.0))
	};
}


void DrawSurface(sf::RenderWindow& window,
                 const float point1X,
                 const float point1Y,
                 const float point1Z,
                 const float point2X,
                 const float point2Y,
                 const float point2Z,
                 const float point3X,
                 const float point3Y,
                 const float point3Z,
                 const float point4X,
                 const float point4Y,
                 const float point4Z,
                 const sf::Color color)
{
	const auto position1 = sf::Vector2f(GetViewportXCoordinate(GetProjectedX(point1X, point1Y, point1Z)),
	                                    GetViewportYCoordinate(GetProjectedY(point1X, point1Y, point1Z)));

	const auto position2 = sf::Vector2f(GetViewportXCoordinate(GetProjectedX(point2X, point2Y, point2Z)),
	                                    GetViewportYCoordinate(GetProjectedY(point2X, point2Y, point2Z)));

	const auto position3 = sf::Vector2f(GetViewportXCoordinate(GetProjectedX(point3X, point3Y, point3Z)),
	                                    GetViewportYCoordinate(GetProjectedY(point3X, point3Y, point3Z)));

	const auto position4 = sf::Vector2f(GetViewportXCoordinate(GetProjectedX(point4X, point4Y, point4Z)),
	                                    GetViewportYCoordinate(GetProjectedY(point4X, point4Y, point4Z)));

	sf::ConvexShape convex;

	convex.setPointCount(4);
	convex.setPoint(0, position1);
	convex.setPoint(1, position2);
	convex.setPoint(2, position3);
	convex.setPoint(3, position4);

	convex.setFillColor(color);

	if (gridOn)
	{
		auto outlineColor = sf::Color{uint8_t(color.r / 2), uint8_t(color.g / 2), uint8_t(color.b / 2)};
		convex.setOutlineColor(outlineColor);
		convex.setOutlineThickness(1);
	}
	window.draw(convex);
}


void DrawHorTile(sf::RenderWindow& window, const float x, const float y, const float z, const sf::Color color)
{
	DrawSurface(window,
	            x, y, z,
	            x + 1, y, z,
	            x + 1, y, z + 1,
	            x, y, z + 1,
	            DarkenColor(color, 1));
}

void DrawVertRightTile(sf::RenderWindow& window, const float x, const float y, const float z, const sf::Color color)
{
	DrawSurface(window,
	            x, y, z,
	            x, y + 1, z,
	            x + 1, y + 1, z,
	            x + 1, y, z,
	            DarkenColor(color, 2));
}

void DrawVertLeftTile(sf::RenderWindow& window, const float x, const float y, const float z, const sf::Color color)
{
	DrawSurface(window,
	            x, y, z,
	            x, y + 1, z,
	            x, y + 1, z + 1,
	            x, y, z + 1,
	            DarkenColor(color, 3));
}

void DrawDiagonalUpLeftTile(sf::RenderWindow& window,
                            const float x,
                            const float y,
                            const float z,
                            const sf::Color color)
{
	DrawSurface(window,
	            x, y, z,
	            x, y + 1, z + 1,
	            x + 1, y + 1, z + 1,
	            x + 1, y, z,
	            DarkenColor(color, 1.5));
}

void DrawDiagonalDownLeftTile(sf::RenderWindow& window,
                              const float x,
                              const float y,
                              const float z,
                              const sf::Color color)
{
	DrawSurface(window,
	            x, y + 1, z,
	            x + 1, y + 1, z,
	            x + 1, y, z + 1,
	            x, y, z + 1,
	            DarkenColor(color, 3));
}

void DrawDiagonalUpRightTile(sf::RenderWindow& window,
                             const float x,
                             const float y,
                             const float z,
                             const sf::Color color)
{
	DrawSurface(window,
	            x, y, z,
	            x + 1, y + 1, z,
	            x + 1, y + 1, z + 1,
	            x, y, z + 1,
	            DarkenColor(color, 3));
}

void DrawDiagonalUpRightBottomLeftHalfTile(sf::RenderWindow& window,
                                           const float x,
                                           const float y,
                                           const float z,
                                           const sf::Color color)
{
	DrawSurface(window,
	            x, y, z,
	            x + 1, y + 1, z + 1,
	            x, y, z + 1,
	            x, y, z,
	            DarkenColor(color, 3));
}

void DrawDiagonalUpRightBottomRightHalfTile(sf::RenderWindow& window,
                                            const float x,
                                            const float y,
                                            const float z,
                                            const sf::Color color)
{
	DrawSurface(window,
	            x, y, z,
	            x + 1, y + 1, z,
	            x, y, z + 1,
	            x, y, z,
	            DarkenColor(color, 3));
}

void DrawDiagonalDownRightTile(sf::RenderWindow& window,
                               const float x,
                               const float y,
                               const float z,
                               const sf::Color color)
{
	DrawSurface(window,
	            x, y + 1, z,
	            x + 1, y, z,
	            x + 1, y, z + 1,
	            x, y + 1, z + 1,
	            DarkenColor(color, 1));
}

void DrawDiagonalDownRightBottomLeftHalfTile(sf::RenderWindow& window,
                                             const float x,
                                             const float y,
                                             const float z,
                                             const sf::Color color)
{
	DrawSurface(window,
	            x + 1, y, z,
	            x + 1, y, z + 1,
	            x, y + 1, z + 1,
	            x + 1, y, z,
	            DarkenColor(color, 1));
}

void DrawDiagonalDownRightBottomRightHalfTile(sf::RenderWindow& window,
                                              const float x,
                                              const float y,
                                              const float z,
                                              const sf::Color color)
{
	DrawSurface(window,
	            x, y + 1, z,
	            x + 1, y, z,
	            x + 1, y, z + 1,
	            x, y + 1, z,
	            DarkenColor(color, 1));
}

void DrawDiagonalUpLeftBottomRightHalfTile(sf::RenderWindow& window,
                                           const float x,
                                           const float y,
                                           const float z,
                                           const sf::Color color)
{
	DrawSurface(window,
	            x, y, z,
	            x + 1, y, z,
	            x + 1, y + 1, z + 1,
	            x, y, z,
	            DarkenColor(color, 1.5));
}

void DrawDiagonalUpLeftBottomLeftHalfTile(sf::RenderWindow& window,
                                          const float x,
                                          const float y,
                                          const float z,
                                          const sf::Color color)
{
	DrawSurface(window,
	            x, y, z,
	            x + 1, y, z,
	            x, y + 1, z + 1,
	            x, y, z,
	            DarkenColor(color, 1.5));
}


void DrawDiagonalDownLeftBottomLeftHalfTile(sf::RenderWindow& window,
                                            const float x,
                                            const float y,
                                            const float z,
                                            const sf::Color color)
{
	DrawSurface(window,
	            x, y + 1, z,
	            x + 1, y, z + 1,
	            x, y, z + 1,
	            x, y + 1, z,
	            DarkenColor(color, 3));
}

void DrawDiagonalDownLeftBottomRightHalfTile(sf::RenderWindow& window,
                                             const float x,
                                             const float y,
                                             const float z,
                                             const sf::Color color)
{
	DrawSurface(window,
	            x + 1, y + 1, z,
	            x + 1, y, z + 1,
	            x, y, z + 1,
	            x + 1, y + 1, z,
	            DarkenColor(color, 3));
}

void DrawGrass(sf::RenderWindow& window)
{
	for (auto x = -10; x <= 10; x++)
		for (auto z = -10; z <= 10; z++)
			if (abs(x) * abs(x) + abs(z) * abs(z) < 9 * 9)
				DrawHorTile(window, x, 0, z, green);
}

void DrawHouse(sf::RenderWindow& window)
{
	// 4 tiles wall height 
	for (auto y = 0; y <= 3; y++)
	{
		// right wall
		for (auto x = -3; x <= 2; x++)
			DrawVertRightTile(window, x, y, -3, yellow);

		// left wall
		for (auto z = -3; z <= 2; z++)
			DrawVertLeftTile(window, -3, y, z, yellow);
	}

	// ceiling behind
	for (auto x = -3; x <= 2; x++)
	{
		DrawDiagonalDownLeftTile(window, x, 6, 0, red);
		DrawDiagonalDownLeftTile(window, x, 5, 1, red);
		DrawDiagonalDownLeftTile(window, x, 4, 2, red);
	}

	// ceiling front
	for (auto x = -3; x <= 2; x++)
	{
		DrawDiagonalUpLeftTile(window, x, 4, -3, red);
		DrawDiagonalUpLeftTile(window, x, 5, -2, red);
		DrawDiagonalUpLeftTile(window, x, 6, -1, red);
	}

	// attic front wall
	DrawSurface(window,
	            -3, 4, 3,
	            -3, 4, -3,
	            -3, 7, 0,
	            -3, 4, 3,
	            DarkenColor(yellow, 3));

	// windows
	DrawVertLeftTile(window, -3, 2, -2, blue);
	DrawVertLeftTile(window, -3, 1, -2, blue);

	DrawVertLeftTile(window, -3, 2, 0, blue);
	DrawVertLeftTile(window, -3, 1, 0, blue);

	DrawVertLeftTile(window, -3, 2, 1, blue);
	DrawVertLeftTile(window, -3, 1, 1, blue);

	DrawVertRightTile(window, 0, 2, -3, blue);
	DrawVertRightTile(window, 0, 1, -3, blue);
	DrawVertRightTile(window, 1, 2, -3, blue);
	DrawVertRightTile(window, 1, 1, -3, blue);

	// door
	DrawVertRightTile(window, -2, 2, -3, brown);
	DrawVertRightTile(window, -2, 1, -3, brown);
	DrawVertRightTile(window, -2, 0, -3, brown);

	// lane

	DrawHorTile(window, -2, 0, -4, darkGrey);
	DrawHorTile(window, -2, 0, -5, darkGrey);
	DrawHorTile(window, -2, 0, -6, darkGrey);
	DrawHorTile(window, -1, 0, -6, darkGrey);
	DrawHorTile(window, -1, 0, -7, darkGrey);
	DrawHorTile(window, -1, 0, -8, darkGrey);
}

void DrawTree(sf::RenderWindow& window, const float x, const float z)
{
	// trunk
	DrawVertLeftTile(window, x, 0, z, brown);
	DrawVertLeftTile(window, x, 1, z, brown);
	DrawVertLeftTile(window, x, 2, z, brown);

	DrawVertRightTile(window, x, 0, z, brown);
	DrawVertRightTile(window, x, 1, z, brown);
	DrawVertRightTile(window, x, 2, z, brown);


	// leaves
	for (auto i = -1; i <= 1; i++)
	{
		DrawVertLeftTile(window, x - 1, 3, z + i, darkGreen);
		DrawVertLeftTile(window, x - 1, 4, z + i, darkGreen);

		DrawVertRightTile(window, x + i, 3, z - 1, darkGreen);
		DrawVertRightTile(window, x + i, 4, z - 1, darkGreen);
	}

	DrawDiagonalDownLeftBottomLeftHalfTile(window, x + 1, 5, z + 1, darkGreen);
	DrawDiagonalDownLeftTile(window, x, 5, z + 1, darkGreen);
	DrawDiagonalDownLeftBottomRightHalfTile(window, x - 1, 5, z + 1, darkGreen);

	DrawDiagonalDownRightBottomRightHalfTile(window, x + 1, 5, z + 1, darkGreen);
	DrawDiagonalDownRightTile(window, x + 1, 5, z, darkGreen);
	DrawDiagonalDownRightBottomLeftHalfTile(window, x + 1, 5, z - 1, darkGreen);

	DrawDiagonalUpRightBottomRightHalfTile(window, x - 1, 5, z + 1, darkGreen);
	DrawDiagonalUpRightTile(window, x - 1, 5, z, darkGreen);
	DrawDiagonalUpRightBottomLeftHalfTile(window, x - 1, 5, z - 1, darkGreen);

	DrawDiagonalUpLeftBottomRightHalfTile(window, x - 1, 5, z - 1, darkGreen);
	DrawDiagonalUpLeftTile(window, x, 5, z - 1, darkGreen);
	DrawDiagonalUpLeftBottomLeftHalfTile(window, x + 1, 5, z - 1, darkGreen);

	DrawHorTile(window, x, 6, z, darkGreen);
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(viewportWidth, viewportHeight), "Isometric");

	while (window.isOpen())
	{
		sf::Event event{};
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::MouseWheelScrolled)
			{
				if (event.mouseWheelScroll.delta > 0)
				{
					ratio *= 1.1;
					constant2 *= 1.1;
				}
				else
				{
					ratio /= 1.1;
					constant2 /= 1.1;
				}
			}
			else if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					scale /= 1.1;
				}
				else if (event.mouseButton.button == sf::Mouse::Right)
				{
					scale *= 1.1;
				}
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::G)
				{
					gridOn = !gridOn;
				}
				else if (event.key.code == sf::Keyboard::Add)
				{
					lightCoefficient = (lightCoefficient / 1.1) >= 1.1 ? lightCoefficient / 1.1 : 1.1;
				}
				else if (event.key.code == sf::Keyboard::Subtract)
				{
					lightCoefficient *= 1.1;
				}
			}
		}

		window.clear();

		DrawGrass(window);
		DrawTree(window, 5, -3);
		DrawTree(window, -2, 8);
		DrawHouse(window);
		DrawTree(window, -7, 0);

		window.display();
	}

	return 0;
}
