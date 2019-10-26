#include <SFML/Graphics.hpp>

auto viewport_width = 1024;
auto viewport_height = 768;

float min_real_x = -10;
float max_real_x = 10;
float real_width = abs(min_real_x) + abs(max_real_x);

float min_real_y = -2;
float max_real_y = 2;
float real_height = abs(min_real_y) + abs(max_real_y);

int accuracy = 1000;

float get_real_x_coordinate(const float x)
{
	return real_width / viewport_width * x + min_real_x;
}

float get_real_y_coordinate(const float y)
{
	return real_height / viewport_height * (viewport_height - y) + min_real_y;
}

float get_viewport_x_coordinate(const float real_x)
{
	return viewport_width / real_width * (real_x - min_real_x);
}

float get_viewport_y_coordinate(const float real_y)
{
	return viewport_height - viewport_height / real_height * (real_y - min_real_y);
}

float graph_function(const float x)
{
	return pow(-1,x>=0) * sin(exp(sin(x)));
}


std::vector<sf::Vertex> create_plot_points()
{
	std::vector<sf::Vertex> vertices(accuracy + 1);
	auto i = 0;
	while (i <= accuracy)
	{
		const auto real_x = get_real_x_coordinate(float(viewport_width) * i / accuracy);
		const auto real_y = graph_function(real_x);

		const auto x = get_viewport_x_coordinate(real_x);
		const auto y = get_viewport_y_coordinate(real_y);

		sf::Vertex vertex{sf::Vector2f{x, y}};
		vertices.push_back(vertex);

		i++;
	}
	return vertices;
}


int main()
{
	sf::RenderWindow window(sf::VideoMode(viewport_width, viewport_height), "Graph Plot");
	auto vertices = create_plot_points();

	while (window.isOpen())
	{
		sf::Event event{};
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		auto pixel_pos = sf::Mouse::getPosition(window);
		auto viewport_pos = window.mapPixelToCoords(pixel_pos);


		auto mouse_x = viewport_pos.x;
		auto mouse_y = viewport_pos.y;
		auto real_x = get_real_x_coordinate(mouse_x);
		auto real_y = graph_function(real_x);


		window.clear();

		if (viewport_width > mouse_x && mouse_x > 0 && (viewport_height > mouse_y && mouse_y > 0))
		{
			auto radius = 10;
			auto x = get_viewport_x_coordinate(real_x) - radius;
			auto y = get_viewport_y_coordinate(real_y) - radius;

			sf::CircleShape shape(radius);
			shape.setFillColor(sf::Color::Green);
			shape.move(x, y);

			auto coordinates = "x=" + std::to_string(real_x) + "\ny=" + std::to_string(real_y);
			
			sf::Text text;
			sf::Font font;
			font.loadFromFile("arial.ttf");
			text.setFont(font);
			text.setCharacterSize(24);
			text.setString(coordinates);
			

			window.draw(text);
			window.draw(shape);
		}

		auto color = sf::Color::Red;

		sf::Vertex x_axis[] =
		{
			sf::Vertex(sf::Vector2f(get_viewport_x_coordinate(min_real_x), get_viewport_y_coordinate(0)), color),
			sf::Vertex(sf::Vector2f(get_viewport_x_coordinate(max_real_x), get_viewport_y_coordinate(0)), color)
		};

		sf::Vertex y_axis[] =
		{
			sf::Vertex(sf::Vector2f(get_viewport_x_coordinate(0), get_viewport_y_coordinate(min_real_y)), color),
			sf::Vertex(sf::Vector2f(get_viewport_x_coordinate(0), get_viewport_y_coordinate(max_real_y)), color)
		};

		window.draw(x_axis, 2, sf::Lines);
		window.draw(y_axis, 2, sf::Lines);

		for (unsigned int i = 0; i < vertices.size() - 1; i++)
		{
			sf::Vertex line[] = {vertices[i], vertices[i + 1]};
			window.draw(line, 2, sf::Lines);
		}

		window.display();
	}

	return 0;
}
