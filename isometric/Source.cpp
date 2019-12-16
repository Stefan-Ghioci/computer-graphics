#include <SFML/Graphics.hpp>

auto darkBrown = sf::Color{50, 25, 0};
auto green = sf::Color{0, 255, 0};
auto yellow = sf::Color{255, 255, 0};
auto white = sf::Color{255, 255, 255};
auto red = sf::Color{255, 0, 0};
auto black = sf::Color{0, 0, 0};

auto viewportWidth = 640;
auto viewportHeight = 640;

float scale = 10;

auto constant1 = sqrt(2);
auto constant2 = sqrt(6);
auto ratio = 2.0f;


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

	// auto outlineColor = sf::Color{uint8_t(color.r / 2), uint8_t(color.g / 2), uint8_t(color.b / 2)};
	// convex.setOutlineColor(outlineColor);
	// convex.setOutlineThickness(1);

	window.draw(convex);
}


void DrawHorTile(sf::RenderWindow& window, const float x, const float y, const float z, const sf::Color color)
{
	DrawSurface(window,
	            x, y, z,
	            x + 1, y, z,
	            x + 1, y, z + 1,
	            x, y, z + 1,
	            color);
}

void DrawVertRightTile(sf::RenderWindow& window, const float x, const float y, const float z, const sf::Color color)
{
	DrawSurface(window,
	            x, y, z,
	            x, y + 1, z,
	            x + 1, y + 1, z,
	            x + 1, y, z,
	            color);
}

void DrawVertLeftTile(sf::RenderWindow& window, const float x, const float y, const float z, const sf::Color color)
{
	DrawSurface(window,
	            x, y, z,
	            x, y + 1, z,
	            x, y + 1, z + 1,
	            x, y, z + 1,
	            color);
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
	            color);
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
	            color);
}

void DrawGrass(sf::RenderWindow& window)
{
	for (auto x = -6; x <= 5; x++) // NOLINT(cert-flp30-c)
		for (auto z = -6; z <= 5; z++) // NOLINT(cert-flp30-c)
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

	// todo: cover attic front wall 
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
		}

		window.clear();

		DrawGrass(window);
		DrawHouse(window);

		window.display();
	}

	return 0;
}
