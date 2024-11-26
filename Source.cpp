#include <iostream>
#include <gl/glew/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include<gl\glm\glm.hpp>

const float PI = 22/7;
using namespace std;
using namespace glm;

struct Vertex
{
	vec3 Position;
	vec3 Color;
};

GLuint InitShader(const char* vertex_shader_file_name, const char* fragment_shader_file_name);

const GLint WIDTH = 600, HEIGHT = 600;
GLuint VBO, BasiceprogramId;

std::vector<std::vector<vec3>*>lines;

void CompileShader(const char* vertex_shader_file_name, const char* fragment_shader_file_namering, GLuint& programId)
{
	programId = InitShader(vertex_shader_file_name, fragment_shader_file_namering);
	glUseProgram(programId);
}
void drawLine(std::vector<vec3>*points)
{
	// create buffer object
	//glGenBuffers(1, &VBO);

	// binding buffer object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, points->size() * sizeof(vec3), points->data(), GL_DYNAMIC_DRAW);

	// shader
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(vec3), 0);
	glEnableVertexAttribArray(0);

	/*glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (char*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);*/

	glDrawArrays(GL_LINE_STRIP, 0, points->size());

	//glDisableVertexAttribArray(0);
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

	CompileShader("VS.glsl", "FS.glsl", BasiceprogramId);
	glClearColor(1, 1, 1, 1);
	glGenBuffers(1, &VBO);
	return 0;
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	// Use the shader program before drawing
	glUseProgram(BasiceprogramId);

	for (std::vector<vec3>* line : lines)
	{
		glLineWidth(4);
		drawLine(line);
	}
}

int main()
{
	sf::ContextSettings context;
	context.depthBits = 24;
	sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!", sf::Style::Close, context);

	if (Init()) return 1;
		
	vec3 mousePos;

	bool pressed = false;

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
				case sf::Event::MouseButtonPressed:
				{
					if (!pressed)
					{
						lines.push_back(new (std::vector<vec3>));
						mousePos = vec3(sf::Mouse::getPosition(window).x/ float(WIDTH/2) - 1.0f, -sf::Mouse::getPosition(window).y/ float(HEIGHT/2) + 1.0f,0);
						lines[lines.size()-1]->push_back(mousePos);
						pressed = true;
					}
					break;
				}
				case sf::Event::MouseButtonReleased:
				{
					if (pressed)
					{
						pressed = false;
					}
					break;
				}
				case sf::Event::MouseMoved:
				{
					if (pressed)
					{
						mousePos = vec3(sf::Mouse::getPosition(window).x / float(WIDTH / 2) - 1.0f, -sf::Mouse::getPosition(window).y / float(HEIGHT / 2) + 1.0f, 0);
						lines[lines.size()-1]->push_back(mousePos);
					}
					break;
				}
			}
			
		}

		Render();
		window.display();
	}
	return 0;
}