#include <cstdlib>

#include <fstream>
#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <my/GL.hpp>

int main1(int, char**);
int main(int argc, char** argv)
{
	// int exit_code = main1(argc, argv);
	my::GL::vertices* Vertices = new my::GL::vertices;
	Vertices->draw_quad(std::pair<float, float>(0, 0), std::pair<float, float>(0, 0), std::pair<float, float>(0, 0), std::pair<float, float>(0, 0));
	std::vector<float> floatVec = Vertices->float_vec();
	for (int i = 0; i < floatVec.size(); i++)
	{
		std::cout << floatVec.at(i) << std::endl;
	}
	// return exit_code;
}

int main1(int argc, char** argv)
{
	my::GL::start_glfw();
	my::GL::window* window = new my::GL::window("foo");
	my::GL::start_glad();
	glViewport(0, 0, 800, 600);

	my::GL::shader* Shader = new my::GL::shader("shaders/vert.glsl", "shaders/frag.glsl");

	float vertices[] = {
        0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };

    unsigned int VBO, VAO;

	glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!window->shouldClose()) //(!glfwWindowShouldClose(window))
	{
	    window->process_input();

		if (window->getKey(GLFW_KEY_ESCAPE))
		{
			std::cout << "escape key pressed" << std::endl;
			break;
		}

	    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	    glClear(GL_COLOR_BUFFER_BIT);

	    Shader->use();
	    glBindVertexArray(VAO);
	    glDrawArrays(GL_TRIANGLES, 0, 3);

		// glfwSwapBuffers(window);
		window->swap_buffers();
		glfwPollEvents();
	}

	delete Shader, window;

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();

	my::GL::vertices* Vertices = new my::GL::vertices;
	return 0;
}