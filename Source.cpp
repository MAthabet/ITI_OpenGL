#include <iostream>
#include <gl/glew/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include<gl\glm\glm.hpp>

using namespace std;
using namespace glm;

GLuint InitShader(const char* vertex_shader_file_name, const char* fragment_shader_file_name);

const GLint WIDTH = 600, HEIGHT = 600;
GLuint VBO, BasiceprogramId;


int pointsVerticies(int n)
{
	vector<vec3>points;
	for (float i = 0; i < n; i ++)
	{
		points.push_back(vec3(2*i/float(n)-1, 0,0));
	}

	// create buffer object
	glGenBuffers(1, &VBO);

	// binding buffer object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(vec3), points.data(), GL_STATIC_DRAW);

	// shader
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(vec3), 0);
	glEnableVertexAttribArray(0);
	return n;
}

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

	CompileShader("VS.glsl", "FS.glsl", BasiceprogramId);

	glClearColor(0, .2, 0, 1);

	return 0;
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	//it is just my birth date 
	glDrawArrays(GL_LINE_STRIP, 0, pointsVerticies(2001));
}

void Update(float time)
{
	// add all tick code
	GLuint Time_Location = glGetUniformLocation(BasiceprogramId, "time");
	glUniform1f(Time_Location, time);
}

int main()
{
	sf::ContextSettings context;
	context.depthBits = 24;
	sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "SINE_WAVE!", sf::Style::Close, context);
	window.setFramerateLimit(60);

	if (Init()) return 1;

	sf::Clock clock;

	float time = 0;
	float sign = 1;
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
			}
		}
		if (time > 1 || time < 0)
		{
			sign = -sign;

		}
			time += sign * clock.restart().asSeconds();
		Update(time);

		Render();

		window.display();

	}
	return 0;
}
