#include <SFML/Graphics.hpp>
#include <fstream>

auto viewport_width = 1024;
auto viewport_height = 768;

const auto file_input = "cube.txt";

float min_real_x = -5;
float max_real_x = 5;
float real_width = abs(min_real_x) + abs(max_real_x);

float min_real_y = -3.75;
float max_real_y = 3.75;
float real_height = abs(min_real_y) + abs(max_real_y);


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

sf::Vector2f get_vertex_projection(const sf::Vector3f& vertex, const float radius, const float alpha)
{
	return
	{
		vertex.x + radius * vertex.z * cos(alpha),
		vertex.y + radius * vertex.z * sin(alpha)
	};
}


struct edge
{
	int vertex_index_1;
	int vertex_index_2;
	int z_index;
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(viewport_width, viewport_height), "3D Render");

	std::ifstream input{file_input};

	int vertices_size;
	input >> vertices_size;

	std::vector<sf::Vector3f> vertices;

	for (auto i = 0; i < vertices_size; i++)
	{
		float x, y, z;
		input >> x >> y >> z;
		vertices.emplace_back(x, y, z);
	}

	int edges_size;
	input >> edges_size;

	std::vector<edge> edges;

	for (auto i = 0; i < edges_size; i++)
	{
		int vertex_index_1, vertex_index_2, z_index;
		input >> vertex_index_1 >> vertex_index_2 >> z_index;
		edges.push_back(edge{vertex_index_1 - 1, vertex_index_2 - 1, z_index - 1});
	}

	std::sort(edges.begin(), edges.end(), [](const edge& edge1, const edge& edge2) { return edge1.z_index > edge2.z_index; });

	int tp;
	float radius, alpha;
	input >> tp >> radius >> alpha;

	std::vector<sf::Vector2f> projected_vertices;

	projected_vertices.reserve(vertices_size);
	for (auto i = 0; i < vertices_size; i++)
		projected_vertices.push_back(get_vertex_projection(vertices[i], radius, alpha));

	input.close();

	while (window.isOpen())
	{
		sf::Event event{};
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();


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


		sf::Color colors[] = 
		{
			sf::Color(0,255,0,255),
			sf::Color(0,204,0,255),
			sf::Color(0,153,0,255)
		};

		for (auto edge : edges)
		{
			auto vertex_1 = projected_vertices.at(edge.vertex_index_1);
			auto vertex_2 = projected_vertices.at(edge.vertex_index_2);
			auto z_index = edge.z_index;

			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(get_viewport_x_coordinate(vertex_1.x), get_viewport_y_coordinate(vertex_1.y)), colors[z_index]),
				sf::Vertex(sf::Vector2f(get_viewport_x_coordinate(vertex_2.x), get_viewport_y_coordinate(vertex_2.y)), colors[z_index])
			};

			window.draw(line, 2, sf::Lines);
		}


		window.display();
	}
}
