#include <iostream>
#include <gl/glew/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include<gl\glm\glm.hpp>


const float PI = 3.14159f;

const GLint WIDTH = 600, HEIGHT = WIDTH * 0.866;

const int SierpinskiDepth = 5;

using namespace std;
using namespace glm;
enum DrawingMode
{
	Points,
	Lines,
	FilledTriangle
};

struct Vertex
{
	vec3 Position;
	vec3 Color;
};

GLuint InitShader(const char* vertex_shader_file_name, const char* fragment_shader_file_name);
void GenerateSierpinski(std::vector<Vertex>* vertices, vec3 a, vec3 b, vec3 c, int depth);
void CreateColoredSierpinski(float depth);

GLuint VBO, BasicProgramId;
DrawingMode Current_DrawingMode = DrawingMode::Lines;

void CompileShader(const char* vertex_shader_file_name, const char* fragment_shader_file_namering, GLuint& programId)
{
	programId = InitShader(vertex_shader_file_name, fragment_shader_file_namering);
	glUseProgram(programId);
}

int Init()
{
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		cout << "Error";
		getchar();
		return 1;
	}
	else
	{
		if (GLEW_VERSION_3_0)
			cout << "Driver support OpenGL 3.0\nDetails:\n";
	}
	cout << "\tUsing glew " << glewGetString(GLEW_VERSION) << endl;
	cout << "\tVendor: " << glGetString(GL_VENDOR) << endl;
	cout << "\tRenderer: " << glGetString(GL_RENDERER) << endl;
	cout << "\tVersion: " << glGetString(GL_VERSION) << endl;
	cout << "\tGLSL:" << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	CompileShader("VS.glsl", "FS.glsl", BasicProgramId);

	glClearColor(0, .2, 0, 1);

	return 0;
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	switch (Current_DrawingMode)
	{
	case Points:
		glPointSize(10);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
	case Lines:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case FilledTriangle:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	default:
		break;
	}

	glDrawArrays(GL_TRIANGLES, 0, std::pow(3, SierpinskiDepth+1));
}

float theta = 0;
void Update() {
	theta += 0.0001f;
	GLuint Theta_Location = glGetUniformLocation(BasicProgramId, "theta");
	glUniform1f(Theta_Location, theta);
}

int main()
{
	sf::ContextSettings context;
	context.depthBits = 24;
	sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!", sf::Style::Close, context);

	if (Init()) return 1;

	CreateColoredSierpinski(SierpinskiDepth);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
			{
				window.close();
				break;
			}
			case sf::Event::KeyPressed:
			{
				if (event.key.code == sf::Keyboard::Num1)
				{
					Current_DrawingMode = DrawingMode::Points;
				}
				if (event.key.code == sf::Keyboard::Num2)
				{
					Current_DrawingMode = DrawingMode::Lines;
				}
				if (event.key.code == sf::Keyboard::Num3)
				{
					Current_DrawingMode = DrawingMode::FilledTriangle;
				}
				break;
			}
			}
		}

		Update();
		Render();

		window.display();
	}
	return 0;
}

void GenerateSierpinski(std::vector<Vertex>* vertices, vec3 a, vec3 b, vec3 c, int depth) {
	if (depth == 0) {
		vec3 color = vec3((a.x + b.x + c.x) / 3.0f, (a.y + b.y + c.y) / 3.0f, 1.0f);
		vertices->push_back({ a, color });
		vertices->push_back({ b, color });
		vertices->push_back({ c, color });
		return;
	}
	else {
		vec3 ab = (a + b) * 0.5f;
		vec3 bc = (b + c) * 0.5f;
		vec3 ca = (c + a) * 0.5f;

		GenerateSierpinski(vertices, a, ab, ca, depth - 1);
		GenerateSierpinski(vertices, ab, b, bc, depth - 1);
		GenerateSierpinski(vertices, ca, bc, c, depth - 1);
	}
}
void CreateColoredSierpinski(float depth)
{
	vector<Vertex> Sierpinski;

	vec3 v1(-1, -1, 0);
	vec3 v2(1, -1, 0);
	vec3 v3(0, 1, 0);

	vector<Vertex> vertices;

	GenerateSierpinski(&vertices, v1, v2, v3, depth);

	// create buffer object
	glGenBuffers(1, &VBO);

	// binding buffer object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);


	// shader
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (char*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
}