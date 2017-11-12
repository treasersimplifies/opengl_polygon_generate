//
//  LS_loadshaders.hpp
//  ls_opengl_2
//
//  Created by Treaser Lusio on 2017/11/3.
//  Copyright © 2017年 treasersimplifies. All rights reserved.
//

#ifndef LS_loadshaders_hpp
#define LS_loadshaders_hpp

#include <stdio.h>
#include <iostream>
//#include <openGL/gl3.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "LoadShaders.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

#endif /* LS_loadshaders_hpp */
