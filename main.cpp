#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <iostream>
#include <fstream>

using namespace std;

// GLOBAL VARS

GLFWwindow* window;
int width, height;
string tex_num = "1";

// GLOBAL VARS

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);


GLFWwindow* create_window() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Hello Window!", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &width, &height);
    if (window == nullptr) {
        glfwTerminate();
        cout << "ERROR: Could not create the window.\n";
        exit(-1);
    }
    glfwSetKeyCallback(window, key_callback);
    return window;
}

void init_glew() {
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        cout << "ERROR: Coult not initialize GLEW.\n";
        exit(-1);
    }
    glViewport(0, 0, width, height);
}

string read_shader(string filename) {
    string shader = "";
    ifstream fin(filename);
    char c;
    while ((c = fin.get()) && c != EOF) {
        shader += c;
    }
    return shader;
}

GLuint init_vshader(string filename) {
    GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
    string s = read_shader("../src/vertexshader.vert");
    const char *vshaderSource = s.c_str();
    glShaderSource(vshader, 1, &vshaderSource, NULL);
    glCompileShader(vshader);

    int success;
    char infolog[512];

    glGetShaderiv(vshader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vshader, 512, NULL, infolog);
        cout << "ERROR: Could not compile the vertex shader\n";
        cout << infolog << '\n';
        exit(-1);
    }
    return vshader;
}

GLuint init_fshader(string filename) {
    GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
    string s = read_shader(filename);
    const char *fshaderSource = s.c_str();
    glShaderSource(fshader, 1, &fshaderSource, NULL);
    glCompileShader(fshader);

    int success;
    char infolog[512];

    glGetShaderiv(fshader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fshader, 512, NULL, infolog);
        cout << "ERROR: Could not compile the fragment shader\n";
        cout << infolog << '\n';
        exit(-1);
    }
    return fshader;
}

GLuint init_program() {
    GLuint vshader = init_vshader("../src/vertexshader.vert");
    GLuint fshader = init_fshader("../src/fragmentshader.frag");
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vshader);
    glAttachShader(shader_program, fshader);
    glLinkProgram(shader_program);
    int success;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infolog[512];
        glGetProgramInfoLog(shader_program, 512, NULL, infolog);
        cout << "ERROR: Could not compile shader program\n" << infolog << '\n';
    }
    glDeleteShader(vshader);
    glDeleteShader(fshader);
    return shader_program;
}

int num_of_vertices;
GLfloat vertices[65000000];
GLuint texture;

void load_texture() {
    string path = "../res/pal"+tex_num+".png";
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_1D, texture);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    int tex_width, tex_height;
    unsigned char* image = SOIL_load_image(path.c_str(), &tex_width, &tex_height, 0, SOIL_LOAD_RGB);
    if (!image) {
        cout << "ERROR: Could not load texture.\n";
        exit(-1);
    }
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, tex_width, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_1D, 0);
    
}

int main() {
    window = create_window();
    init_glew();

    int cnt = 0;
    for (GLfloat x = 0; x <= width; x += 1) {
        for (GLfloat y = 0; y <= height; y += 1) {
            vertices[cnt++] = (x-width*0.5f) / (width*0.5f);
            vertices[cnt++] = (y-height*0.5f) / (height*0.5f);
            vertices[cnt++] = 0.0f;
        }
    }

    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid*)(0));
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    glPointSize(1);
    
    GLuint program = init_program();

    load_texture();

    GLfloat kek = 0.5f;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glBindTexture(GL_TEXTURE_1D, texture);
        GLint centerLocation = glGetUniformLocation(program, "center");
        glUseProgram(program);
        glUniform2f(centerLocation, 0.0f, kek);
        kek += 0.001f;
        glBindVertexArray(vao);
        glDrawArrays(GL_POINTS, 0, cnt);
        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glfwTerminate();
    return 0;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        tex_num = "1";
        load_texture();
    }
    if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        tex_num = "2";
        load_texture();
    }
    if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
        tex_num = "3";
        load_texture();
    }
    if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
        tex_num = "4";
        load_texture();
    }
    if (key == GLFW_KEY_5 && action == GLFW_PRESS) {
        tex_num = "5";
        load_texture();
    }
    
    if (key == GLFW_KEY_6 && action == GLFW_PRESS) {
        tex_num = "6";
        load_texture();
    }
}
