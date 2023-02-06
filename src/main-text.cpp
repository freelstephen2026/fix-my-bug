#include <string>

#include <iostream>
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
    GLuint Advance;     // Horizontal offset to advance to next glyph
};

bool file_exists(const char* buf)
{
    FILE* fp = fopen(buf, "r");
    if (fp == nullptr)
    {
        fclose(fp);
        return false;
    }
    fclose(fp);
    return true;
}

int main() { 

    // initialize GLFW and OpenGL 
    glfwInit(); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

    // create a window object 
    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello", NULL, NULL); 

    if (window == NULL) { 
        std::cout << "Failed to create GLFW window" << std::endl; 
        glfwTerminate(); 

        return -1; 
    }

    // make the created window the current context  
    glfwMakeContextCurrent(window);  

    // initialize GLAD before using any OpenGL function  
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {  
        std::cout << "Failed to initialize GLAD" << std::endl;  

        return -1;  
    }    

    // Set up FreeType library and font face object.  
    FT_Library ft;                                                                                         
    if (FT_Init_FreeType(&ft)) {                                                                          
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;                     
        return -1;                                                                                       
    }
    FT_Face face;
    if (FT_New_Face(ft, "/usr/share/fonts/gnu-free/FreeSerif.otf", 0, &face)) //fonts/arial.ttf
    {
        if (!file_exists("/usr/share/fonts/gnu-free/FreeSerif.otf"))
        {
            std::cerr << "error: /usr/share/fonts/gnu-free/FreeSerif.otf  does not exist" << std::endl;
        }
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
     }
     glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Render text
    std::string text = "Hello";
    // .... (rendering text logic)
    // ...

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // ...
        // Clear the color buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // Set the font size.
    FT_Set_Pixel_Sizes(face, 0, 48);

    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

    // Load the first 128 characters of the ASCII set.
    for (GLubyte c = 0; c < 128; c++)
    {
        // Load the character glyph into face->glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
            continue;
        }

        // Generate a texture for the character.
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Store the character for later use.
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        character.insert(std::pair<GLchar, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    // Destroy the FreeType library and face object.
    FT_Done_Face(face);
    FT_Done_FreeType(ft);


    // Set up the vertex and fragment shaders.
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the shaders from the files.
    const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec4 vertex;\n"
    "out vec2 TexCoord;\n"
    "uniform mat4 projection;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
    "    TexCoord = vertex.zw;\n"
    "}";

    
        // set font size to 72 points
    FT_Set_Pixel_Sizes(face, 0, 72);

    // loop until the window is closed
    while (!glfwWindowShouldClose(window)) {
        // check for events (keyboard, mouse, etc.)
        glfwPollEvents();

        // clear the screen with a solid color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render text
        glColor3f(1.0f, 1.0f, 1.0f);
        FT_GlyphSlot slot = face->glyph;
        int x = 10, y = 540;
        const char* text = "Hello";
        for (const char* p = text; *p; p++) {
            if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
                continue;
            glBitmap(slot->bitmap.width, slot->bitmap.rows, x + slot->bitmap_left, y - slot->bitmap_top, slot->advance.x/64, slot->advance.y/64, slot->bitmap.buffer);
            x += slot->advance.x >> 6;
        }

        // swap the front and back buffers
        glfwSwapBuffers(window);
    }

    // deallocate resources
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
}