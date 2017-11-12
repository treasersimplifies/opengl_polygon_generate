//
//  LSshaders.c
//  ls_opengl_2
//
//  Created by Treaser Lusio on 2017/10/29.
//  Copyright © 2017年 treasersimplifies. All rights reserved.
//

#include "LSshaders.h"

void LSshaders(const char* vertex_shader,const char* fragment_shader){
    
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
}
