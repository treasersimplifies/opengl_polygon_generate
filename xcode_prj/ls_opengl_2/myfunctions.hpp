//
//  myfunctions.hpp
//  ls_opengl_2
//
//  Created by Treaser Lusio on 2017/11/6.
//  Copyright © 2017年 treasersimplifies. All rights reserved.
//

#ifndef myfunctions_hpp
#define myfunctions_hpp

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void show_point(GLfloat point[3]);
void show_polygon2(const GLfloat * polygon, int num);
void show_polygon3(GLfloat polygon[][3], int num);

GLfloat * intersection(GLfloat * point1, GLfloat * point2, GLfloat * point3, GLfloat * point4);
GLfloat angle(GLfloat * point0, GLfloat * point1, GLfloat * point2);
GLfloat totalangle(GLfloat polygon[][3], const GLuint num , GLfloat * detectpoint);
GLfloat bigger(GLfloat p1, GLfloat p2);
GLfloat smaller(GLfloat p1, GLfloat p2);
bool has_intersection(GLfloat * point1, GLfloat * point2, GLfloat * point3, GLfloat * point4);
bool is_in_polygon(GLfloat polygon[][3], const GLuint num , GLfloat * detectpoint);
bool is_in_array(GLuint * t,int num, int index);
GLuint find_index(GLuint * array, int num, int value);
GLuint find_secondindex(GLuint * array, int num, int value);
GLuint count_element(GLuint * array, int num, int value);

bool is_case_1(GLfloat polygon1[][3], const GLuint num1, GLfloat polygon2[][3], const GLuint num2);
bool is_case_2(GLfloat polygon1[][3], const GLuint num1, GLfloat polygon2[][3], const GLuint num2);
bool is_case_3(GLfloat polygon1[][3], const GLuint num1, GLfloat polygon2[][3], const GLuint num2);
bool is_case_4(GLfloat polygon1[][3], const GLuint num1, GLfloat polygon2[][3], const GLuint num2);
int adjust_polygon(GLfloat polygon1[][3], const GLuint num1, GLfloat polygon2[][3], const GLuint num2);

GLfloat * deal_case2_substact(GLfloat polygon1[][3], const GLuint num1, GLfloat polygon2[][3], const GLuint num2, GLuint * length);
GLfloat * deal_case2_combine(GLfloat polygon1[][3], const GLuint num1, GLfloat polygon2[][3], const GLuint num2, GLuint * length);



#endif /* myfunctions_hpp */
