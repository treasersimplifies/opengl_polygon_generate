//
//
//Created by Lusio  20171104
//ZJU computer graphic presentation 1.
//
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//以下是我自己写的两个库函数文件：
#include "myfunctions.hpp"
#include "corefunctions.hpp"

GLFWwindow* window;

const GLuint init(){
    
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
    };
    
    GLfloat biggest_polygon[][3]={
        {1.0f,-1.0f,0.0f},
        {1.0f,1.0f,0.0f},
        {-1.0f,1.0f,0.0f},
        {-1.0f,-1.0f,0.0f}
    };
    
    const GLuint num1 = 6;
    const GLuint num2 = 5;
    
    GLfloat polygon_points1[][3] ={ //测试(2)/(3)情况所使用的顶点数组,求并运算中只有一条边相交的情况是时使用。
        { 0.5f , 0.5f,0.0f},
        { -0.5f , 0.5f,0.0f},
        { -0.5f , -0.5f,0.0f},
        { 0.5f,-0.5f,0.0f}
    };
    GLfloat polygon_points2[][3] ={
        {1.0f , 0.0f,0.0f},
        {1.0f , 1.0f,0.0f},
        {0.0f , 1.0f,0.0f},
        {0.0f , 0.0f,0.0f}
    };
    GLfloat polygon_points3[num1][3]={//测试combine的情况(1)
        {0.2f,0.0f,0.0f},
        {0.0f,0.6f,0.0f},
        {-0.5f,0.6f,0.0f},
        {-1.0f,0.0f,0.0f},
        {-0.5f,-0.6f,0.0f},
        {0.0f,-0.6f,0.0f}
    };
    GLfloat polygon_points4[num2][3]={
        {-0.2f,-0.4f,0.0f},
        {0.5f,-0.4f,0.0f},
        {0.8f,0.0f,0.0f},
        {0.5f,0.4f,0.0f},
        //{-0.2f,0.4f,0.0f},
        {-0.5f,0.0f,0.0f}
    };
    
    GLfloat polygon_points5[][3]={//测试情况(4)
        {1.0f , 0.5f,0.0f},
        {-1.0f ,0.5f,0.0f},
        {0.0f ,-1.0f,0.0f}
    };
    GLfloat polygon_points6[][3]={
        {1.0f , -0.5f,0.0f},
        {0.0f , 1.0f,0.0f},
        {-1.0f ,-0.5f,0.0f}
    };
    
    GLfloat polygon_points7[][3]={//测试情况(4)的特殊情况
        {1.0f , 0.0f,0.0f},
        {0.2f , 0.5f,0.0f},
        {0.2f ,-0.5f,0.0f}
    };
    
    GLfloat polygon_points8[][3]={
        {-1.0f , 0.0f,0.0f},
        {-0.2f , 0.5f,0.0f},
        {-0.2f ,-0.5f,0.0f}
    };
    
    GLfloat polygon_points9[][3]={  //测试情况（2）（3）的求并运算，
                                    //一条边与2条边都有交点的情况,成功！
        {1.0f,0.0f,0.0f},
        {0.0f,1.0f,0.0f},
        {0.0f,-1.0f,0.0f}
    };
    
    GLfloat polygon_points10[][3]={
        {0.5f,0.0f,0.0f},
        {-0.5f,1.0f,0.0f},
        {-0.5f,-1.0f,0.0f}
    };
    
    GLfloat polygon_points11[][3]={//和8一起测试情况（4）的特殊情况，求并
        {1.0f , 0.0f,0.0f},
        {0.2f , 0.5f,0.0f},
        {0.2f ,-0.5f,0.0f}
    };
    
    GLuint l_combine = 0;
    GLuint l_substract = 0;
    GLuint * combine_length;
    GLuint * substract_length;
    
    substract_length = &l_substract;
    combine_length = &l_combine;
    
    
    const GLfloat * polygon_vertices_combine = polygon_substract(polygon_points5, 3 , polygon_points6, 3, combine_length);
    
    std::cout
    <<"Get new CombinePoly vertices of "<< l_combine <<" points:\n";
    show_polygon2(polygon_vertices_combine, l_combine);
    std::cout
    <<"sizeof (polygon_vertices_combine): "<<sizeof(polygon_vertices_combine)<<'\n';
    //size只是GLfloat的大小
    
    const GLuint number_of_vertices = (const GLuint)l_combine;//最后输出到drawarray里的长度；
    //static const GLfloat * polygon = (const GLfloat) polygon_vertices_combine ;
    
    static const GLfloat polygon_vertices_in_the_end[3*6]={
        0.0666667 , -0.4 , 0,
        0.2 , 0 , 0,
        0.117647 , 0.247059 , 0,
        -0.5 , 0 , 0,
        -0.2 , -0.4 , 0
    };
    
    glClearColor(0.8f, 0.8f, 1.0f, 0.0f);
    glClear( GL_COLOR_BUFFER_BIT );
    
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, 3 * number_of_vertices * sizeof(GLfloat), polygon_vertices_combine, GL_STATIC_DRAW);
    //sizeof(g_vertex_buffer_data)
    
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    
   
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
    "  frag_colour = vec4(0.0, 0.0, 1.0, 1.0);"
    "}";
    
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);   //vertex_shader
    glCompileShader(vs);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);
    GLuint shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fs);
    glAttachShader(shader_programme, vs);
    glLinkProgram(shader_programme);
    glUseProgram(shader_programme);
    
    // 1rst attribute buffer : vertices
    glVertexAttribPointer(
                          0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                          3,                  // size
                          GL_FLOAT,           // type
                          GL_FALSE,           // normalized?
                          0,                  // stride
                          (void*)0            // array buffer offset
                          );
    
    return number_of_vertices;
}


//由于我们只绘制一次，所以，其实display里的代码只会跑一次
void display(const GLuint number_of_vertices){

    glDrawArrays(GL_LINE_LOOP, 0, number_of_vertices);
    glDisableVertexAttribArray(0);
    
}


int main( void )
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, "strip", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
    
    GLuint vertexbuffer = init();
    
    do{
        
        display(vertexbuffer);
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
          glfwWindowShouldClose(window) == 0 );
    
    // Cleanup VBO
    //glDeleteBuffers(1, &vertexbuffer);
    //glDeleteVertexArrays(1, &VertexArrayID);
    //glDeleteProgram(programID);
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    
    return 0;
}
