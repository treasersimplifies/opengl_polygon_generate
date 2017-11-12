//
//  corefunctions.hpp
//  ls_opengl_2
//
//  Created by Treaser Lusio on 2017/11/7.
//  Copyright © 2017年 treasersimplifies. All rights reserved.
//

#ifndef corefunctions_hpp
#define corefunctions_hpp

#include <stdio.h>
#include "myfunctions.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLfloat * polygon_combine(GLfloat polygon1[][3], const GLuint num1, GLfloat polygon2[][3], const GLuint num2, GLuint * length);
GLfloat * polygon_substract(GLfloat polygon1[][3], const GLuint num1, GLfloat polygon2[][3], const GLuint num2, GLuint * length);

#endif /* corefunctions_hpp */
