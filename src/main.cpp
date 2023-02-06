/* OpenGL */
#include <glad/glad.h>
#include <GLFW/glfw3.h>

/* C Standard Library */
#include <cstdio>
#include <cstdlib>
#include <cstring>

/* C++ Standard Library */
#include <iostream>
#include <vector>

/* to get vector amount of elements, use `std::vector<T>::size()`. to get the size of each element, use `sizeof(std::vector<T>::value_type) */
template <typename T>
std::vector<T>& insertTriangleToVec(std::vector<T> &vec, T e1, T e2, T e3, T e4, T e5, T e6);

template <typename T>
std::vector<T> &insertQuadToVec(std::vector<T> &vec, T e1, T e2, T e3, T e4, T e5, T e6, T e7, T e8);

namespace my
{
	namespace math
	{
		template <typename T>
		struct coords
		{
			T x, y;
			coords()
			{
				x = (T)0;
				y = (T)0;
			}
			coords(T x, T y)
			{
				this->x = x;
				this->y = y;
			}
		};
	}

	namespace GL
	{
		std::string get_vertex_shader_src(const char*);


		char* get_fragment_shader_src(char*);

		int get_vertex_shader(char*);
		int get_fragment_shader(char*);

		int make_shader_program(int& vertexShader, int& fragmentShader)
		{
			std::cout << "test" << std::endl;
			int shaderProgram = glCreateProgram();
			glAttachShader(shaderProgram, vertexShader);
			glAttachShader(shaderProgram, fragmentShader);
			glLinkProgram(shaderProgram);
			std::cout << "test" << std::endl;
			int success = 0;
			glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
			if (!success)
			{
				printf("shader program linking not successful\n");
			}
			return shaderProgram;
		}
		void clean_up(unsigned int& VAO, unsigned int& VBO, int shaderProgram);

        /* we define triangle based on its amount of sides */
        const unsigned short TRIANGLE = 3;
		template <typename T, const unsigned short VERTEX_COUNT>
		class vertex_properties
		{
		public:
			std::vector<T> vertices;
			int object_count;
			
			unsigned short vertex_count()
			{
				return VERTEX_COUNT;
			}

			vertex_properties()
			{
				object_count = 0;
			}

			const std::type_info& type()
			{
				return typeid(T);
			}

			const size_t element_size()
			{
				return sizeof(T);
			}

            vertex_properties& insert_triangle(T x1, T y1, T x2, T y2, T x3, T y3)
            {
                object_count++;

				vertices.push_back(x1); vertices.push_back(y1); vertices.push_back((T)0);
				vertices.push_back(x2); vertices.push_back(y2); vertices.push_back((T)0);
				vertices.push_back(x3); vertices.push_back(y3); vertices.push_back((T)0);

				return *this;
            }

			vertex_properties& insert_quad(T x1, T y1, T x2, T y2, T x3, T y3, T x4, T y4)
			{
				insert_triangle(x1, y1, x2, y2, x3, y3);
				insert_triangle(x1, y1, x3, y3, x4, y4);
			}
		};
	}
}

template <typename T>
T order_list(T t1, T t2, T t3, T t4)
{
	T unordered[4] = {t1, t2, t3, t4};
	T ordered[4] = {t1, t2, t3, t4};
	unsigned int ordered_relativeIndex[4] = {0, 1, 2, 3};

	for (int i = 0; i < 4; i++)
	{
		if (ordered[0] < unordered[i])
		{
			ordered[0] = unordered[i];
			ordered_relativeIndex[0] = i;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		if (i != ordered_relativeIndex[0])
		{
			if (ordered[1] < unordered[i])
			{
				ordered[1] = unordered[i];
				ordered_relativeIndex[1] = i;
			}
		}
	}
	for (int i = 0; i < 4; i++)
	{
		if ((i != ordered_relativeIndex[0]) && (i != ordered_relativeIndex[1]))
		{
			if (ordered[2] < unordered[i])
			{
				ordered[2] = unordered[i];
				ordered_relativeIndex[2] = i;
			}
		}
	}
	for (int i = 0; i < 4; i++)
	{
		if ((i != ordered_relativeIndex[0]) && (i != ordered_relativeIndex[1]) && (i != ordered_relativeIndex[2]))
		{
			if (ordered[3] < unordered[i])
			{
				ordered[3] = unordered[i];
				ordered_relativeIndex[2] = i;
			}
		}
	}
}

void windowResizeCallback(GLFWwindow*, int, int);
int gladMain(int, char**);
int main(int argc, char **argv)
{
	std::cout << "main:" << std::endl;

	int gladMain_returned = gladMain(argc, argv);
	if (!gladMain_returned)
	{
		return gladMain_returned;
	}

    return 0;
}

int make_shader_program(int& vertexShader, int& fragmentShader)
{
	std::cout << "test" << std::endl;
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	std::cout << "test" << std::endl;
	int success = 0;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		printf("shader program linking not successful\n");
	}
	return shaderProgram;
}

static bool compare_strings(const char* str1, const char* str2)
{
	printf("\n\n");
	for (int i = 0; i < std::strlen(str1); i++)
	{
		std::printf("%d: %c\t%c\n", i, str1[i], str2[i]);
	}
	std::cout << std::endl;
	return true;
}

int gladMain(int argc, char **argv)
{
    my::GL::vertex_properties<float, my::GL::TRIANGLE> vertexProperties;

	if (!glfwInit()) {
        std::fprintf(stderr, "Failed to initialize GLFW\n");
        exit(-1);
    }

	printf("reading from vertex.shader:\n");

	const char* std_str = my::GL::get_vertex_shader_src("src/shaders/vertex.shader").c_str();

	std::cout << "\n\n\nstd_str:" << std_str << std::endl;
	std::cout << "\n\n\nstd_str:" << my::GL::get_vertex_shader_src("src/shaders/vertex.shader") << std::endl;

	/* shaders */
	char* vertexShaderSource = (char*)	my::GL::get_vertex_shader_src("src/shaders/vertex.shader").c_str();
	vertexShaderSource = (char*) (my::GL::get_vertex_shader_src("src/shaders/vertex.shader").c_str());
	std::cout << "vertex shader:\n" << vertexShaderSource << std::endl;

	const char *fragmentShaderSource = (char*)	"#version 330 core\n"
                                   				"out vec4 FragColor;\n"
                                   				"void main()\n"
                                   				"{\n"
                                   				"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   				"}\0";
	
	GLFWwindow *window = glfwCreateWindow(800, 600, *argv, NULL, NULL);
    if (!window)
	{
        std::fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

	glfwMakeContextCurrent(window);

    // initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        return -1;
    }

	/* compile shaders */
	int vertexShader = my::GL::get_vertex_shader(vertexShaderSource);
    int fragmentShader = my::GL::get_fragment_shader((char*)fragmentShaderSource);

    // create shader program
	int shaderProgram = my::GL::make_shader_program(vertexShader, fragmentShader);

    // delete shaders
    glDeleteShader(vertexShader); glDeleteShader(fragmentShader);

	std::cout << "test" << std::endl;

	vertexProperties.insert_triangle(-1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f);
	vertexProperties.insert_triangle(-1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f);
	printf("size = %d\n", vertexProperties.vertices.size());
    
    // create vertex buffer object (VBO)
	unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexProperties.vertices.size() * sizeof(float), vertexProperties.vertices.data(), GL_STATIC_DRAW);

    // create vertex array object (VAO)
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexProperties.vertex_count() * vertexProperties.element_size(), (void*)0);
    glEnableVertexAttribArray(0);

    /* make sure all the graphics scale when resizing a window */
    glfwSetWindowSizeCallback(window, windowResizeCallback);

    // unbind VAO
    glBindVertexArray(0);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        // input
        glfwPollEvents();

        // clear screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // use shader program
        glUseProgram(shaderProgram);

        // bind VAO
        glBindVertexArray(VAO);

        // draw two triangles
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawArrays(GL_TRIANGLES, 3, 3);
		glDrawArrays(GL_TRIANGLES, 6, 9);

        // unbind VAO
        glBindVertexArray(0);

        // swap buffers
        glfwSwapBuffers(window);
    }

    my::GL::clean_up(VAO, VBO, shaderProgram);

	return 0;
}

void my::GL::clean_up(unsigned int& VAO, unsigned int& VBO, int shaderProgram)
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	glfwTerminate();
}

void windowResizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

    // int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // glCompileShader(vertexShader);

    // int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    // glCompileShader(fragmentShader);

int my::GL::get_vertex_shader(char* vertexShaderSource)
{
	int ret = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(ret, 1, &vertexShaderSource, NULL);
	glCompileShader(ret);
	return ret;
}
int my::GL::get_fragment_shader(char* fragmentShaderSource)
{
	int ret = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(ret, 1, &fragmentShaderSource, NULL);
	glCompileShader(ret);
	return ret;
}
std::string my::GL::get_vertex_shader_src(const char* path)
{
	FILE* fp = fopen(path, "r");
	if (fp == nullptr)
	{
		std::cerr << "could not open `" << path << "`. exiting with exit code 1.\n";
		std::exit(1);
	}
	fseek(fp, 0, SEEK_SET);
	std::string contents;
	while (!std::feof(fp))
	{
		contents.push_back(fgetc(fp));
	}
	char* c_str = (char*)contents.c_str();
	c_str[contents.size() - 1] = '\0';
	printf("------\nc_str:\n%s\n------\n", c_str);
	std::fclose(fp);
	std::cout << "std::string(c_str) :   \n" << std::string(c_str) << std::endl;
	return std::string(c_str);
}

char* my::GL::get_fragment_shader_src(char* path)
{
	;
}