
//基本代码模式：
int main(int argc, const char * argv[]) {
//U,these 4 lines are crucial on macos in order to get the latest version of opengl.

//U,initialise  glfw:
if(!glfwInit()){ std::cout<<"Failed to initialize GLFW\n"<<std::endl; return -1;}
//U,在直接创建窗口前可以先设置一下：glfwWindowHint(GLFW_SAMPLES, 4);
GLFWwindow* win;

glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//一定要紧跟在windows创建前，否则会默认使用2.1版本的opengl
win = glfwCreateWindow(640, 480, "OpenGL Demo Project", NULL, NULL);

if(!win){ glfwTerminate(); exit(EXIT_FAILURE); }

glfwMakeContextCurrent(win);
glewExperimental = GL_TRUE;
glewInit();//要进行glew的初始化，否则会导致EXC_BAD_ACCESS

always_prep();

float points[] = {
0.0f,  0.5f,  0.0f,
0.5f, -0.5f,  0.0f,
-0.5f, -0.5f,  0.0f,
0.0f, -0.5f, 0.0f,
-0.5f, 0.5f,  0.0f,
0.5f, 0.5f,  0.0f
};//修改点数之后，在glBufferData中修改sizeof，在glDrawArrays中修改范围
GLuint vbo = 0;
glGenBuffers(1, &vbo);
glBindBuffer(GL_ARRAY_BUFFER, vbo);
glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), points, GL_STATIC_DRAW);

GLuint vao = 0;
glGenVertexArrays(1, &vao);
glBindVertexArray(vao);
glEnableVertexAttribArray(0);
glBindBuffer(GL_ARRAY_BUFFER, vbo);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

const char* vertex_shader =
"#version 400\n"
"in vec3 vp;"
"void main() {"
"  gl_Position = vec4(vp, 1.0);"
"}";

const char* fragment_shader =
"#version 400\n"
"out vec4 frag_colour;"
"void main() {"
"  frag_colour = vec4(0.5, 0.0, 0.5, 1.0);"
"}";
GLuint vs = glCreateShader(GL_VERTEX_SHADER);
glShaderSource(vs, 1, &vertex_shader, NULL);
glCompileShader(vs);
GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
glShaderSource(fs, 1, &fragment_shader, NULL);
glCompileShader(fs);

GLuint shader_programme = glCreateProgram();
glAttachShader(shader_programme, fs);
glAttachShader(shader_programme, vs);
glLinkProgram(shader_programme);
glUseProgram(shader_programme);

glClearColor(0.6f, 0.6f, 0.8f, 1.0f);//bd color

while(!glfwWindowShouldClose(win)){
//v_display(shader_programme,vao);
// wipe the drawing surface clear
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

glBindVertexArray(vao);
// draw points 0-3 from the currently bound VAO with current in-use shader
glDrawArrays(GL_TRIANGLES, 0, 6);//GL_TRIANGLES, 0, 3

glfwPollEvents();//与waitEvents相对
glfwSwapBuffers(win);//交换缓存，重复显示
}

glfwDestroyWindow(win);
glfwTerminate();
//exit(EXIT_SUCCESS);
return 0;
}



//以下代码是能成功跑的，如果配置对了Framework那些东西的话。
//http://blog.csdn.net/longzh_cn/article/details/55001345

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "LoadShaders.hpp"

void ez_display(void)
{
glBegin(GL_TRIANGLES);
{
glColor3f(1.0,0.0,0.0);
glVertex2f(0, .5);
glColor3f(0.0,1.0,0.0);
glVertex2f(-.5,-.5);
glColor3f(0.0, 0.0, 1.0);
glVertex2f(.5, -.5);
}
glEnd();
}

void ez_init(){
glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
glClear(GL_COLOR_BUFFER_BIT);

}

int main(int argc, const char * argv[]) {
GLFWwindow* win;
//U,initialise  glfw:
if(!glfwInit()){ std::cout<<"Failed to initialize GLFW\n"<<std::endl; return -1;}
//U,在直接创建窗口前可以先设置一下：glfwWindowHint(GLFW_SAMPLES, 4);

win = glfwCreateWindow(640, 480, "OpenGL Demo Project", NULL, NULL);

if(!win){ glfwTerminate(); exit(EXIT_FAILURE); }
if(!glewInit()){ return -1; }

glfwMakeContextCurrent(win);
ez_init();

while(!glfwWindowShouldClose(win)){
ez_display();
glfwSwapBuffers(win);//交换缓存，重复显示
glfwPollEvents();//与waitEvents相对
}
glfwDestroyWindow(win);
glfwTerminate();
//exit(EXIT_SUCCESS);
return 0;
}



//old code: don;t use file for shaders:
/*
const char* vertex_shader =
"#version 400\n"
"in vec3 vp;"
"void main() {"
"  gl_Position = vec4(vp, 1.0);"
"}";

const char* fragment_shader =
"#version 400\n"
"out vec4 frag_colour;"
"void main() {"
"  frag_colour = vec4(0.5, 0.0, 0.5, 1.0);"
"}";
GLuint vs = glCreateShader(GL_VERTEX_SHADER);
glShaderSource(vs, 1, &vertex_shader, NULL);
glCompileShader(vs);
GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
glShaderSource(fs, 1, &fragment_shader, NULL);
glCompileShader(fs);

GLuint shader_programme = glCreateProgram();
glAttachShader(shader_programme, fs);
glAttachShader(shader_programme, vs);
glLinkProgram(shader_programme);
glUseProgram(shader_programme);
*/

unseccessful:
/*ShaderInfo shaders[] = {
{GL_VERTEX_SHADER,"triangle.vert"},
{GL_FRAGMENT_SHADER,"triangle.frag"},
{GL_NONE,NULL}
};

GLuint program = LoadShaders(shaders);
glUseProgram(program);
*/
