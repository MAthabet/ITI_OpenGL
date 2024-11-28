#define  _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <gl/glew/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include<gl\glm\glm.hpp>
#include<gl\glm\gtc\type_ptr.hpp>
#include<gl\glm\gtx\transform.hpp>
#include "objLoader.h"

const float PI = 22/7;
using namespace std;
using namespace glm;

vector<vec3>vertices;
GLuint modelMatLoc, viewMatLoc, projMatLoc;

GLuint InitShader(const char* vertex_shader_file_name, const char* fragment_shader_file_name);

const GLint WIDTH = 600, HEIGHT = 600;
GLuint VBO, BasiceprogramId;

void CompileShader(const char* vertex_shader_file_name, const char* fragment_shader_file_namering, GLuint& programId)
{
	programId = InitShader(vertex_shader_file_name, fragment_shader_file_namering);
	glUseProgram(programId);
}

void CreateCow()
{
	// create buffer object
	glGenBuffers(1, &VBO);

	// binding buffer object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), vertices.data(), GL_STATIC_DRAW);

	// shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
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

	CreateCow();
	modelMatLoc = glGetUniformLocation(BasiceprogramId, "modelMat");
	viewMatLoc = glGetUniformLocation(BasiceprogramId, "viewMat");
	projMatLoc = glGetUniformLocation(BasiceprogramId, "projMat");

	glm::mat4 viewMat = glm::lookAt(glm::vec3(0, 0, 2.5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, glm::value_ptr(viewMat));

	glm::mat4 projMat = glm::perspectiveFov(60.0f, (float)WIDTH, (float)HEIGHT, 0.1f, 100.0f);
	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, glm::value_ptr(projMat));

	glClearColor(0, .2, 0, 1);

	return 0;
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

void Update(float time)
{
	// add all tick code
	mat4 ModelMat = glm::translate(glm::vec3(0, 0, 0.0f)) *
		glm::rotate(46.7f*5, glm::vec3(0, 1, 0)) * glm::rotate(sin(2.0f * time) * 20, glm::vec3(1, 0, 0)) *
		glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
	
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(ModelMat));
}

int main()
{
	sf::ContextSettings context;
	context.depthBits = 24;
	sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!", sf::Style::Close, context);
	
	bool res = loadOBJ("./test.txt", vertices);
	if (Init()) return 1;
	sf::Clock clk;
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

		Update(clk.getElapsedTime().asSeconds());
		if (clk.getElapsedTime().asSeconds()>20) clk.restart();
		Render();

		window.display();
	}
	return 0;
}
