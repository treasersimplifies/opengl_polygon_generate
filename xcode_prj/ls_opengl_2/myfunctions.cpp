//
//  myfunctions.cpp
//  ls_opengl_2
//
//  Created by Treaser Lusio on 2017/11/6.
//  Copyright © 2017年 treasersimplifies. All rights reserved.
//

#include "myfunctions.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLdouble eps = 0.00001;

void show_point(GLfloat point[3]){
    std::cout
    <<"{"<<point[0]<<'\t'<<point[1]<<'\t'<<point[2]<<"}"<<'\n';
}

void show_polygon2(const GLfloat * polygon, int num){
    for(int i =0;i < 3*num ; i+=3 ){
        std::cout
        <<"{"<<polygon[i]<<'\t'<<polygon[i+1]<<'\t'<<polygon[i+2]<<"}"<<'\n';
    }
}

void show_polygon3(GLfloat polygon[][3], int num){
    for(int i = 0; i<num;i++){
        std::cout
        <<"{"<<polygon[i][0]<<'\t'<<polygon[i][1]<<'\t'<<polygon[i][2]<<"}"<<'\n';
    }
}

//1,3确定一条直线，2，4确定一条直线,返回交点{x,y,0.0f}
GLfloat * intersection(GLfloat * point1, GLfloat * point2, GLfloat * point3, GLfloat * point4){
    GLfloat x1 = point1[0];
    GLfloat y1 = point1[1];
    GLfloat x2 = point2[0];
    GLfloat y2 = point2[1];
    GLfloat x3 = point3[0];
    GLfloat y3 = point3[1];
    GLfloat x4 = point4[0];
    GLfloat y4 = point4[1];
    
    
    if(fabs(x1-x3)<eps&&fabs(x2-x4)<eps){
        GLfloat * point = new GLfloat[3];
        point[0] = INFINITY;
        point[1] = INFINITY;
        point[2] = 1.0f;//两条直线都是平行y轴的,则将点的z坐标设为1.0f
        return point;
    }
    else if(fabs(x1-x3)<eps){
        GLfloat * point = new GLfloat[3];
        GLfloat k2 = (y4-y2)/(x4-x2);
        GLfloat b2 = y2 - k2*x2;
        
        point[0] = x1;
        point[1] = k2*x1+b2;
        point[2] = 0.0f;
        return point;
    }
    else if(fabs(x2-x4)<eps){
        GLfloat * point = new GLfloat[3];
        GLfloat k1 = (y3-y1)/(x3-x1);
        GLfloat b1 = y1 - k1*x1;
        
        point[0] = x2;
        point[1] = k1*x2+b1;
        point[2] = 0.0f;
        return point;
    }
    else{
        GLfloat k1 = (y3-y1)/(x3-x1);
        GLfloat b1 = y1 - k1*x1;
        GLfloat k2 = (y4-y2)/(x4-x2);
        GLfloat b2 = y2 - k2*x2;
        
        GLfloat x = (b2-b1)/(k1-k2);
        GLfloat y = k1*x+b1;
        
        GLfloat * point = new GLfloat[3];
        point[0] = x;
        point[1] = y;
        point[2] = 0.0f;
        return point;
    }
    
}



//返回三角形内角的角度,弧度制
GLfloat angle(GLfloat * point0, GLfloat * point1, GLfloat * point2){
    GLfloat x1 = point1[0];
    GLfloat y1 = point1[1];//y1-y2
    
    GLfloat x2 = point2[0];//x1-x2
    GLfloat y2 = point2[1];//y1-y2
    
    GLfloat x0 =point0[0];//x1-x2
    GLfloat y0 =point0[1];//y1-y2
    GLfloat m1 = pow(x1-x2, 2)+pow(y1-y2, 2);
    GLfloat m2 = pow(x0-x2, 2)+pow(y0-y2, 2);
    GLfloat m3 = pow(x1-x0, 2)+pow(y1-y0, 2);
    
    GLfloat a = sqrt(m1);
    GLfloat b = sqrt(m2);
    GLfloat c = sqrt(m3);
    
    GLfloat p = (b*b+c*c-a*a)/(2*b*c);
    
    GLfloat angle = acos(p);
    
    if(b!=0&&c!=0){
        return angle;//弧度制
    }
    else {
        return M_PI;//只有在点重合的时候才会出现，情况很少
    }
}

GLfloat totalangle(GLfloat polygon[][3], const GLuint num , GLfloat * detectpoint){
    //要注意一种情况，就是当一个点在边上的时候，返回值会大于2PI
    //但是点在外面时肯定小于2PI
    uint i;
    GLfloat totalangle = 0.0f;
    for(i = 0; i < num -1 ; i++){//如果i=num-1,那么i+1就越界了，但是编译器竟然不提示！！！！！！！
        GLfloat * point1 = polygon[i];
        GLfloat * point2 = polygon[i+1];
        GLfloat  a = angle(detectpoint, point1, point2);
        totalangle = totalangle + a;
    }//循环结束后还有最后num-1与0组成顶点的三角形没被算入
    
    totalangle = totalangle + angle(detectpoint, polygon[num-1], polygon[0]);
    
    
    return totalangle;
}


GLfloat bigger(GLfloat p1, GLfloat p2){
    return p1>p2?p1:p2;
}
GLfloat smaller(GLfloat p1, GLfloat p2){
    return p1<p2?p1:p2;
}


//判断线段有无交点，线段1为point1和point3决定，线段2为point2和point4
bool has_intersection(GLfloat * point1, GLfloat * point2, GLfloat * point3, GLfloat * point4){
    
    GLfloat * inter_point = intersection(point1, point2, point3, point4);
    GLfloat x = inter_point[0];
    GLfloat y = inter_point[1];
    GLfloat delta1 = fabs(point1[0]-point3[0]);
    GLfloat delta2 = fabs(point2[0]-point4[0]);
    
    //如果端点在线上，认为没有交点,但是这个点会被认为是在多边形内。这样可以避免边相交时判断的一些不必要麻烦。
    
    if(   ( fabs(angle(point1,point2,point4)-M_PI) <eps )
       || ( fabs(angle(point3,point2,point4)-M_PI) <eps )
       || ( fabs(angle(point2,point1,point3)-M_PI) <eps )
       || ( fabs(angle(point4,point1,point3)-M_PI) <eps ))
    {
        return false;
    }
    
    if(delta1<eps||delta2<eps){
        //如果k1或者为无穷大:
        //那么只能通过判断交点的纵左边在k1和k2为无穷大的那个线段的上下限之内，必有交点。
        if((y < bigger(point1[1],point3[1])+eps && y > smaller(point1[1],point3[1])-eps)
           &&(y < bigger(point2[1],point4[1])+eps && y > smaller(point2[1],point4[1])-eps)){
            return true;
        }
        else
            return false;
    }
    else{
        if( ( x <= bigger(point1[0],point3[0]) && x >= smaller(point1[0],point3[0]))
           && ( x <= bigger(point2[0],point4[0]) && x >= smaller(point2[0],point4[0]) ) ){
            
            return true;
        }
        else
            return false;
    }
    
    /*GLfloat totalangle = 0.0f;
     totalangle += angle(inter_point, point1, point2);
     totalangle += angle(inter_point, point2, point3);
     totalangle += angle(inter_point, point3, point4);
     totalangle += angle(inter_point, point4, point1);
     //如果这2条 直线 的交点 位于这四个线段端点组成的四边形的内部，则说明这2条 线段 的交点 存在
     if(totalangle > 0.99*2*M_PI){
     return true;
     }
     else
     return false;*/
}

bool is_in_polygon(GLfloat polygon[][3], const GLuint num , GLfloat * detectpoint){
    
    if(totalangle(polygon, num, detectpoint) > 0.99*2*M_PI){
        return true;
    }
    else
        return false;
    
}


bool is_in_array(GLuint * array,int num, int index){
    
    for(int i = 0; i<num;i++){
        if(array[i] == index){
            
            return true;
        }
    }
    return false;
}

GLuint find_index(GLuint * array, int num, int value){
    
    for(int i = 0; i<num; i++){
        if( array[i] == value ){
            return i;
        }
    }
    return -1;
}

GLuint find_secondindex(GLuint * array, int num, int value){
    
    int count = 0;
    for(int i = 0; i<num; i++){
        if( array[i] == value ){
            count++;
            if(count ==2){
                return i;
            }
        }
    }
    return -1;
}

GLuint count_element(GLuint * array, int num, int value){
    int i = 0;
    
    for(int j = 0;j<num;j++){
        if(array[j] == value){
            i++;
        }
    }
    return i;
}

bool is_case_1(GLfloat polygon1[][3], const GLuint num1, GLfloat polygon2[][3], const GLuint num2){
    //这是报告中的情况（1）
    bool _2_has_points_in_1 = false;
    bool _1_has_points_in_2 = false;
    for(int i = 0; i<num2 ; i++){
        if(is_in_polygon(polygon1, num1, polygon2[i])){
            _2_has_points_in_1 = true;
            break;
        }
    }
    for(int j = 0; j<num1 ; j++){
        if(is_in_polygon(polygon2, num2, polygon1[j])){
            _1_has_points_in_2 = true;
            break;
        }
    }
    
    if(_1_has_points_in_2&&_2_has_points_in_1){
        std::cout<<"case 1: true\n";
    }
    else{
        std::cout<<"case 1: false\n";
    }
    
    return _1_has_points_in_2&&_2_has_points_in_1;
}

bool is_case_2(GLfloat polygon1[][3], const GLuint num1, GLfloat polygon2[][3], const GLuint num2){
    //这是报告中的情况（2） polygon2有顶点位于polygon1内部，但 polygon1无顶点位于polygon2内部；
    //但不包括特殊情况：polygon2 ∈ polygon1。因为在处理顶点数组时已经返回。
    bool _2_has_points_in_1 = false;
    bool _1_has_points_in_2 = false;
    bool case2;
    for(int i = 0; i<num2 ; i++){
        if(is_in_polygon(polygon1, num1, polygon2[i])){
            _2_has_points_in_1 = true;
            break;
        }
    }
    for(int j = 0; j<num1 ; j++){
        if(is_in_polygon(polygon2, num2, polygon1[j])){
            _1_has_points_in_2 = true;
        }
    }
    
    if((!_1_has_points_in_2)&&_2_has_points_in_1){
        std::cout<<"case 2: true\n";
        case2 = true;
    }
    else{
        std::cout<<"case 2: false\n";
        case2 = false;
    }
    return case2;
}

bool is_case_3(GLfloat polygon1[][3], const GLuint num1, GLfloat polygon2[][3], const GLuint num2){
    //这是报告中的情况（3） polygon2无顶点位于polygon1内部，但 polygon1有顶点位于polygon2内部；
    //但不包括特殊情况：polygon1 ∈ polygon2。因为在处理顶点数组时已经返回。
    bool _2_has_points_in_1 = false;
    bool _1_has_points_in_2 = false;
    bool case3;
    for(int i = 0; i<num2 ; i++){
        if(is_in_polygon(polygon1, num1, polygon2[i])){
            _2_has_points_in_1 = true;
            break;
        }
    }
    for(int j = 0; j<num1 ; j++){
        if(is_in_polygon(polygon2, num2, polygon1[j])){
            _1_has_points_in_2 = true;
        }
    }
    
    if(_1_has_points_in_2&&(!_2_has_points_in_1)){
        std::cout<<"case 3: true\n";
        case3 = true;
    }
    else{
        std::cout<<"case 3: false\n";
        case3 = false;
    }
    return case3;
}
bool is_case_4(GLfloat polygon1[][3], const GLuint num1, GLfloat polygon2[][3], const GLuint num2){
    //这是报告中的情况（4） polygon2无顶点位于polygon1内部，且polygon1无顶点位于polygon2内部；
    //特殊情况1：两者相离；特殊情况2：两者有边相交，但是没有顶点包含在另一个多边形中。
    
    bool _2_has_points_in_1 = false;
    bool _1_has_points_in_2 = false;
    bool case4;
    for(int i = 0; i<num2 ; i++){
        if(is_in_polygon(polygon1, num1, polygon2[i])){
            _2_has_points_in_1 = true;
            break;
        }
    }
    for(int j = 0; j<num1 ; j++){
        if(is_in_polygon(polygon2, num2, polygon1[j])){
            _1_has_points_in_2 = true;
        }
    }
    
    if((!_1_has_points_in_2)&&(!_2_has_points_in_1)){
        std::cout<<"case 4: true\n";
        case4 = true;
    }
    else{
        std::cout<<"case 4: false\n";
        case4 = false;
    }
    return case4;
}

int adjust_polygon(GLfloat polygon1[][3], const GLuint num1, GLfloat polygon2[][3], const GLuint num2){
    
    int index = -1;
    int i,j,k;
    GLfloat polygon1_here[num1][3];
    GLfloat polygon2_here[num2][3];
    
    //2.1修改多边形，使得其顶点0必位于另一个多边形的外部
    //2.1.1先修改polygon2：
    
    if (is_in_polygon(polygon1, num1, polygon2[0])) {
        //如果 polygon2 的顶点0在 polygon1 内部，则变换polygon2的顶点顺序
        for(i = 0; i<num2;i++){
            //寻找一个不在 polygon1 内的顶点
            if (!is_in_polygon(polygon1, num1, polygon2[i])){
                //std::cout<<i<<" is out of polygon1"<<std::endl;
                index = i;
                break;
            }
        }
        
        if(index == -1)//=-1 已经表明polygon2的所有顶点都位于在 polygon1 内
            return index;
        
        //如果得到了适当的index,进行下面的转换,得到新的polygon2_here：
        for(i = index,k=0 ;i<num2;i++){//先大于index的部分，到头部
            polygon2_here[k][0] = polygon2[i][0];
            polygon2_here[k][1] = polygon2[i][1];
            polygon2_here[k][2] = polygon2[i][2];
            k++;
        }
        for(i = 0 ; i<index ; i++){//再将大于index的部分，到尾部，k承接原来的k
            polygon2_here[k][0] = polygon2[i][0];
            polygon2_here[k][1] = polygon2[i][1];
            polygon2_here[k][2] = polygon2[i][2];
            k++;//k最后的值肯定到num2为止
        }
    }
    else{//直接将polygon2赋给polygon2_here
        for(i = 0,k = 0 ; i<num2 ; i++){
            polygon2_here[k][0] = polygon2[i][0];
            polygon2_here[k][1] = polygon2[i][1];
            polygon2_here[k][2] = polygon2[i][2];
            k++;
        }
    }
    
    
    //将polygon2变成修改好的polygon2_here
    for(i=0;i<num2;i++){
        for(j=0;j<3;j++){
            polygon2[i][j] = polygon2_here[i][j];
        }
    }
    
    std::cout
    <<"finish to adjust polygon2:";
    show_polygon3(polygon2, num2);
    
    //将修改后的polygon1_here重新赋给polygon1改变了传进来的多边形，至此改好了多边形了。
    /*std::cout<<"polygon2_here:"<<std::endl;
    
    for(i=0;i<num2;i++){
        for(j=0;j<3;j++){
            std::cout
            <<polygon2_here[i][j]
            <<'\t';
            polygon2[i][j] = polygon2_here[i][j];
        }
        std::cout<<std::endl;
    }
    */
    index = -2;
    
    //2.1.2 对于polygon1也做同样的处理（上面的代码用完，index变量就没用了）
    if (is_in_polygon(polygon2, num2, polygon1[0])) {
        //如果 polygon1 的顶点0在 polygon1 内部，则变换 polygon1 的顶点顺序
        for(i = 0; i<num1;i++){
            //寻找一个不在 polygon2 内的顶点
            if (!is_in_polygon(polygon2, num2, polygon1[i])){
                index = i;
                break;
            }
        }
        
        if(index ==-2)//表明polygon1的点都位于polygon2内，且polygon2有点在polygon1外。
            return index;
        
        //如果得到了适当的index,进行下面的转换,得到新的polygon2_here：
        for(i = index,k=0 ;i<num1;i++){//先大于index的部分，到头部
            polygon1_here[k][0] = polygon1[i][0];
            polygon1_here[k][1] = polygon1[i][1];
            polygon1_here[k][2] = polygon1[i][2];
            k++;
        }
        for(i = 0 ; i<index ; i++){//再大于index的部分，到尾部，k承接原来的k
            polygon1_here[k][0] = polygon1[i][0];
            polygon1_here[k][1] = polygon1[i][1];
            polygon1_here[k][2] = polygon1[i][2];
            k++;//k最后的值肯定到num1为止
        }
    }
    else{//原顶点0不位于另一个多边形内，直接将 polygon1 赋给 polygon1_here
        for(i=0,k=0 ; i<num2 ; i++){
            polygon1_here[k][0] = polygon1[i][0];
            polygon1_here[k][1] = polygon1[i][1];
            polygon1_here[k][2] = polygon1[i][2];
            k++;
        }
    }
    
    for(i=0;i<num1;i++){
        for(j=0;j<3;j++){
            polygon1[i][j] = polygon1_here[i][j];
        }
    }
    
    std::cout
    <<"finish to adjust polygon1:";
    show_polygon3(polygon1, num1);
    
    /*std::cout<<"polygon1_here:"<<std::endl;
    for(i=0;i<num1;i++){
        for(j=0;j<3;j++){
            std::cout
            <<polygon1_here[i][j]
            <<'\t';
            polygon1[i][j] = polygon1_here[i][j];
        }
        std::cout<<std::endl;
    }*/
    
    return index;
}



GLfloat * deal_case2_substact(GLfloat polygon1[][3], const GLuint num1, GLfloat polygon2[][3], const GLuint num2, GLuint * length){
    
    GLfloat newpolygon[2*num1+2*num2][3];
    
    GLuint * r = new GLuint[num1+num2];
    GLuint * t = new GLuint[num1+num2];
    
    GLfloat * newpolygon_vertices = new GLfloat[(num1+num2)*3];//最后需要返回的数据格式
    
    //一定要加new,否则返回没用，因为返回的是指针，而当函数返回时，局部变量都没了，所以这个局部变量指正必须是new出来的。
    
    //先求出polygon2在1内部的点
    int i,j;
    int k,s;//专门给newpolygon_vertices的索引
    int p,m;
    //如果是情况(2):(2) polygon2有顶点位于polygon1内部，但 polygon1无顶点位于polygon2内部；
    //报告中2.6部分
    //以polygon2为基础，一个一个顶点的找
    //i,j分别给r,t作索引(r数组记录polygon2的顶点，t数组记录polygon1的顶点)。
    //k,s给polygon2/polygon1作索引
    
    int array_length;
    bool polygon1_contains_polygon2 = true;
    i = 0;
    j = 0;
    for(k = 0;k < num2;k++){
        //2.6.1,对每个接下去顶点i进行如下处理
        for(s = 0;s<num1;s++){//如果线段有交点，则记录
            
            bool has_int =has_intersection(polygon2[k], polygon1[s], polygon2[(k+1<num2)?k+1:0], polygon1[(s+1<num1)?s+1:0]);
            
            show_point(intersection(polygon2[k], polygon1[s], polygon2[(k+1<num2)?k+1:0], polygon1[(s+1<num1)?s+1:0]));
            
            if(has_int){
                r[i] = k;//r数组记录polygon2的顶点
                t[j] = s;//t数组记录polygon1的顶点
                i++;
                j++;
                polygon1_contains_polygon2 = false;
            }
        }
    }
    /*
    for(m = 0;m<i;m++){
        std::cout
        <<m<<" r: "<<r[m]<<" t: "<<t[m]<<'\n';
    }*/
    
    if(polygon1_contains_polygon2){
        //2.4.2,这里处理了情况(2)中的特殊情况：
        //  polygon2∈polygon1， 对于求交运算，直接返回polygon2
        std::cout
        <<"Polygon1 contains Polygon2.\n";
        for(i = 0; i < num2 ; i++){
            newpolygon_vertices[i*3+0] = polygon2[i][0];
            newpolygon_vertices[i*3+1] = polygon2[i][1];
            newpolygon_vertices[i*3+2] = polygon2[i][2];
        }
        
        k = num2;
        *length = k;
        
        return newpolygon_vertices;
        
    }
    else{
        //2.4.3如果不是包含关系，情况稍微复杂，需要按一般方法求出新的多边形
        //在这结束之后，得到了包含与相交线段有关的 顶点位置 的2个数组t和r
        //并且必定 i==j
        
        //2.4.3.1一般情况求交运算：
        array_length = i;
        j = 0;
        GLfloat * newpoint;
        GLfloat * newpoint1;
        GLfloat * newpoint2;
        
        for(i = 0; i<num2;i++){
            std::cout
            <<"Substracting with "<<i<<" th"<<" of polygon2:";
            show_point(polygon2[i]);
            
            if( ( !is_in_polygon(polygon1, num1, polygon2[i]) )
               &&( !is_in_array(r, array_length, i) ) ){
                //1)如果其在polygon1外部且不存在于r[]中，则跳过（什么都不做）；
                ;
            }
            else if(( !is_in_polygon(polygon1, num1, polygon2[i]) )
                    && is_in_array(r, array_length, i) ){
                //2）如果其在polygon1外部但存在于r[]中
                //求出新的交点，然后压栈
                
                //下面的p是用于r、t数组的索引，与上文的p没有关系
                p = find_index(r, array_length, i);//在r数组里找到对应索引，然后得到存在t中的polygon1的顶点
                
                if(r[p+1]!= i){
                    //只与一条边相交，直接压栈
                    m = t[p];
                    
                    newpoint = intersection(polygon2[i], polygon1[m], polygon2[(i+1<num2)?i+1:0], polygon1[(m+1<num1)?m+1:0]);
                    //压栈入新的那个newpolygon数组
                    newpolygon[j][0] = newpoint[0];
                    newpolygon[j][1] = newpoint[1];
                    newpolygon[j][2] = newpoint[2];
                    j++;
                    //std::cout<<"show polygon at j: "<<j<<'\n';
                    //show_polygon3(newpolygon, j);
                }
                else{//如果r[p+1] == i
                    //接下去处理特殊情况：有一条边与另外的多边形的两条边相交
                    //如果p+1与p索引对应的r数组元素相同，则说明是特殊情况
                    //有个问题，就是所产生的数组好像有可能不是逆时针的
                    
                    //要先判断接下去两个点的顺序：
                    //新的2个点和j-1点的三角形顺序即可：newpolygon[j]、p、p+1必定是逆时针的，否则就交换p和p+1的顺序
                    
                    m = t[p];
                    newpoint1 = intersection(polygon2[i], polygon1[m], polygon2[(i+1<num2)?i+1:0], polygon1[(m+1<num1)?m+1:0]);
                    
                    show_point(newpoint1);
                    
                    p = p+1<array_length?p+1:0;//p++
                    m = t[p];
                    
                    newpoint2 = intersection(polygon2[i], polygon1[m], polygon2[(i+1<num2)?i+1:0], polygon1[(m+1<num1)?m+1:0]);
                    show_point(newpoint2);
                    
                    //sp = (x1-x0)*(y2-y0)-(x2-x0)*(y2-y0);
                    
                    GLfloat sp = (newpoint1[0] - newpolygon[j-1][0]) * (newpoint2[1] - newpolygon[j][1]) -
                    (newpoint2[0] - newpolygon[j-1][0]) * (newpoint1[1] - newpolygon[j][1]);
                    
                    //j==0呢？？？一个bug，可惜没空修复了。。。要修复的话要等整个数组产生好以后再搞，这是要在newpolygon里找到是交点插入的点，
                    //要处理一大堆索引，可能会产生新的bug。
                    //如果直接先找下一个点，则有一种情况是下一个点还是进入了这个情况，于是产生递归。
                    
                    if(sp > 0 ){
                        newpolygon[j][0] = newpoint1[0];
                        newpolygon[j][1] = newpoint1[1];
                        newpolygon[j][2] = newpoint1[2];
                        j++;
                        newpolygon[j][0] = newpoint2[0];
                        newpolygon[j][1] = newpoint2[1];
                        newpolygon[j][2] = newpoint2[2];
                        j++;
                    }
                    else{
                        newpolygon[j][0] = newpoint2[0];
                        newpolygon[j][1] = newpoint2[1];
                        newpolygon[j][2] = newpoint2[2];
                        j++;
                        newpolygon[j][0] = newpoint1[0];
                        newpolygon[j][1] = newpoint1[1];
                        newpolygon[j][2] = newpoint1[2];
                        j++;
                    }
                    //压栈入新的那个newpolygon数组
                }
                
            }
            else if( is_in_polygon(polygon1, num1, polygon2[i])
                    &&( !is_in_array(r, array_length, i) ) ){
                //2.4.3.1 3)如果其位于polygon1内部且不存在于r[]中,
                //则直接压栈
                newpoint = polygon2[i];
                newpolygon[j][0] = newpoint[0];
                newpolygon[j][1] = newpoint[1];
                newpolygon[j][2] = newpoint[2];
                j++;
                //std::cout<<"show polygon at j: "<<j<<'\n';
                //show_polygon3(newpolygon, j);
            }
            else{
                //2.4.3.1 4)如果其位于polygon1内部且存在于r[]中。
                //将其压栈
                newpoint = polygon2[i];
                newpolygon[j][0] = newpoint[0];
                newpolygon[j][1] = newpoint[1];
                newpolygon[j][2] = newpoint[2];
                j++;
                //std::cout<<"show polygon at j: "<<j<<'\n';
                //show_polygon3(newpolygon, j);
                //并且则计算r[i]r[i]+1与t[i]t[i+1]的交点，再把这个交点压栈
                //这里的i和上述的i其实不是同一个！！！！产生了bug。。。这里的i指的是下面的p
                p = find_index(r, array_length, i);//在r数组里找到对应索引，然后得到存在t中的polygon1的顶点
                m = t[p];
                newpoint = intersection(polygon2[i], polygon1[m], polygon2[(i+1<num2)?i+1:0], polygon1[(m+1<num1)?m+1:0]);
                //压栈入新的那个newpolygon数组
                newpolygon[j][0] = newpoint[0];
                newpolygon[j][1] = newpoint[1];
                newpolygon[j][2] = newpoint[2];
                j++;
                //std::cout<<"show polygon at j: "<<j<<'\n';
                //show_polygon3(newpolygon, j);
            }
        }
        //以上最后得到了该得到的 2维顶点数组，将其转化为OpenGL能绑定的 1维数组 后return。
        
        //show_polygon3(newpolygon, j);
        
        *length = j;
        
        for(int p = 0; p < *length ; p++){
            newpolygon_vertices[p*3+0] = newpolygon[p][0];
            newpolygon_vertices[p*3+1] = newpolygon[p][1];
            newpolygon_vertices[p*3+2] = newpolygon[p][2];
        }
        std::cout<<"\nBefore return:\n";
        show_polygon2(newpolygon_vertices, *length);
        return newpolygon_vertices;
    }
}


GLfloat * deal_case2_combine(GLfloat polygon1[][3], const GLuint num1, GLfloat polygon2[][3], const GLuint num2, GLuint * length){
    
    //如果是情况(2):(2) polygon2有顶点位于polygon1内部，但 polygon1无顶点位于polygon2内部；
    //报告中2.4部分
    //以polygon2为基础，一个一个顶点的找
    //i,j分别给r,t作索引(r数组记录polygon2的顶点，t数组记录polygon1的顶点)。
    //k,s给polygon2/polygon1作索引
    
    GLfloat newpolygon[2*num1+2*num2][3];
    GLfloat * newpolygon_vertices = new GLfloat[(num1+num2)*6];//最后需要返回的数据格式
    
    GLuint * r = new GLuint[num1+num2];
    GLuint * t = new GLuint[num1+num2];
    
    //一定要加new,否则返回没用，因为返回的是指针，而当函数返回时，局部变量都没了，所以这个局部变量指正必须是new出来的。
    
    //先求出polygon2在1内部的点
    int i,j;
    int k,s;//专门给newpolygon_vertices的索引
    int p,m,q,n;
    
    
    int array_length;
    bool polygon1_contains_polygon2 = true;
    i = 0;
    j = 0;
    for(k = 0;k < num2;k++){
    //2.4.1,对每个接下去顶点i进行如下处理
        for(s = 0;s<num1;s++){//如果线段有交点，则记录
            
            show_point(intersection(polygon2[k], polygon1[s], polygon2[(k+1<num2)?k+1:0], polygon1[(s+1<num1)?s+1:0]));
            
            bool has_int = has_intersection(polygon2[k], polygon1[s], polygon2[(k+1<num2)?k+1:0], polygon1[(s+1<num1)?s+1:0]);
            
            if(has_int){
                r[i] = k;//r数组记录polygon2的顶点
                t[j] = s;//t数组记录polygon1的顶点
                i++;
                j++;
                polygon1_contains_polygon2 = false;
            }
        }
    }
    
     for(m = 0;m<i;m++){
     std::cout
     <<m<<" r: "<<r[m]<<" t: "<<t[m]<<'\n';
     }
    
    if(polygon1_contains_polygon2){
        //2.4.2,这里处理了情况(2)中的特殊情况：
        //  polygon2∈polygon1， 对于求交运算，直接返回polygon2
        
        for(i = 0; i < num1 ; i++){
            newpolygon_vertices[i*3+0] = polygon1[i][0];
            newpolygon_vertices[i*3+1] = polygon1[i][1];
            newpolygon_vertices[i*3+2] = polygon1[i][2];
        }
        
        k = num1;
        *length = k;
        
        return newpolygon_vertices;
    }
    else{//2.4.3如果不是包含关系，情况稍微复杂，需要按一般方法求出新的多边形
        //在上面的操作中，得到了包含与相交线段有关的 顶点位置 的2个数组t和r
        //并且必定 i==j
        
        //2.4.3.2一般情况求并运算：
        array_length = i;//新的交点个数
        
        GLfloat * polygonpoint;
        GLfloat * newpoint;
        GLfloat * newpoint1;
        GLfloat * newpoint2;
        GLfloat (*current_polygon)[3];
        GLfloat (*another_polygon)[3];
        GLfloat (*buffer_polygon)[3];//暂存多边形，到时用来交换时使用
        GLuint * current_array;
        GLuint * another_array;
        GLuint * buffer_array;//暂存数组，到时用来交换时使用
        //先求出到底有多少个点:
        
        bool another_polygon_over = false;//判断交互扫描顶点时对方多边形的顶点是否都被判断过了
        int num_of_total_vertices;
        
        j = 0;
        //j:有多少个点在内部
        for(p = 0; p<num2 ;p++){
            //
            if(is_in_polygon(polygon1, num1, polygon2[p])){
                j++;
            }
        }
        
        num_of_total_vertices = num1 + num2 + array_length - j;
        
        
        j = 0;
        current_polygon = polygon2;//设定当前多边形为polygon2
        another_polygon = polygon1;
        buffer_polygon = polygon1;
        current_array = r ;        //设定当前数组为记录polygon2的 r
        another_array = t ;
        buffer_array = t;
        /*********************************************************/
        
        //
        
        for(i = 0 ; j < num_of_total_vertices  ;){//i<(current_polygon==polygon1?num1:num2) || (!another_polygon_over)
            
            //
            //i：current_polygon的索引
            
            std::cout
            <<"Combine_dealing with "<<i<<" th"<<" vertices of polygon "<<((current_polygon==polygon1)?"1":"2")
            <<" total:"<<num_of_total_vertices<<'\n';
            
            show_point(current_polygon[i]);
            
            if( (!is_in_polygon(another_polygon, another_polygon==polygon1?num1:num2, current_polygon[i]))
               &&( !is_in_array(current_array, array_length, i) ) ){//array_length对于r[]和t[]都是一样的
                //1)如果其在polygon1外部且不存在于r[]中，则压栈即可(无需跳变当前多边形)
                newpoint = current_polygon[i];
                newpolygon[j][0] = newpoint[0];
                newpolygon[j][1] = newpoint[1];
                newpolygon[j][2] = newpoint[2];
                j++;
                i = (i+1)<( (current_polygon==polygon1)?num1:num2) ? i+1 : 0;
                //std::cout<<"show polygon at j: "<<j<<'\n';
                //show_polygon3(newpolygon, j);
            }
            else if(( !is_in_polygon(another_polygon, another_polygon==polygon1?num1:num2, current_polygon[i]) )
                    && is_in_array(current_array, array_length, i) ){
                //2）如果其在another_polygon外部 且存在于current_array [] 中
                //先把该点压栈，
                //然后求出新的交点，亦压栈
                
                newpolygon[j][0] = current_polygon[i][0];//先把该点压栈
                newpolygon[j][1] = current_polygon[i][1];
                newpolygon[j][2] = current_polygon[i][2];
                j++;
                
                std::cout
                <<"Combine_dealing newpoints:\n";
                show_point(newpolygon[j-1]);
                
                p = find_index(current_array, array_length, i);//在current_array里找到对应索引，然后得到存在another_array中的another_polygon的顶点
                
                if(count_element(current_array, array_length, i)==1){  //current_array[p+1<array_length?p+1:0] != i
                    //只与一条边相交
                    m = another_array[p];//对方顶点数组的顶点,与i的地位相等

                    newpoint = intersection(current_polygon[i], another_polygon[m], current_polygon[(i+1 < (current_polygon==polygon1?num1:num2) )?i+1:0], another_polygon[(m+1 < (another_polygon==polygon1?num1:num2) )?m+1:0]);
                    //压栈入新的那个newpolygon数组
                    newpolygon[j][0] = newpoint[0];
                    newpolygon[j][1] = newpoint[1];
                    newpolygon[j][2] = newpoint[2];
                    j++;
                    //std::cout<<"show polygon at j: "<<j<<'\n';
                    //show_polygon3(newpolygon, j);
                    std::cout
                    <<"Combining newpoints:\n";
                    show_point(newpolygon[j-1]);
                    //i如何跳变呢？
                    i = m+1<(another_polygon==polygon1?num1:num2) ?m+1:0;
                }
                else{//如果current_array[p+1] == i
                    //则有一条边与另外的多边形的两条边相交，
                    //
                    //在求并运算中，虽然有2个交点，但是一次只压一个点入栈，因为两者很有可能不相连
                    //接下去决定将哪个交点压栈：
                    
                    int count = 0;
                    
                    std::cout<<"3 concerned points:\n";
                    
                    polygonpoint = current_polygon[i];
                    show_point(polygonpoint);
                    
                    m = -1;
                    n = -1;
                    
                    for(k=0;k<(another_polygon==polygon1?num1:num2);k++){
                        
                        if( has_intersection(current_polygon[i], another_polygon[k], current_polygon[(i+1<(current_polygon==polygon1?num1:num2))?i+1:0] , another_polygon[(k+1<(another_polygon==polygon1?num1:num2))?k+1:0]) ){
                            count++;
                            if(count ==1 ){
                                newpoint1 =intersection(current_polygon[i], another_polygon[k], current_polygon[(i+1<(current_polygon==polygon1?num1:num2))?i+1:0],another_polygon[(k+1<(another_polygon==polygon1?num1:num2))?k+1:0]);
                                m = k;//记录第一个交点，对方多边形上的顶点
                                show_point(newpoint1);
                            }
                            else if (count ==2){
                                newpoint2 = intersection(current_polygon[i], another_polygon[k], current_polygon[(i+1<(current_polygon==polygon1?num1:num2))?i+1:0],another_polygon[(k+1<(another_polygon==polygon1?num1:num2))?k+1:0]);
                                n = k;//记录第二个交点，对方多边形上的顶点
                                show_point(newpoint2);
                            }
                        }
                    }
                    

                    //newpoint1 = intersection(current_polygon[i], another_polygon[m], current_polygon[(i+1<(current_polygon==polygon1?num1:num2))?i+1:0], another_polygon[(m+1<(another_polygon==polygon1?num1:num2))?m+1:0]);
                    
                    //m = another_array[p];
                    //p = p+1<array_length?p+1:0;//p有可能越界吗？有
                    //q = find_secondindex(current_array, array_length, i);
                    
                    //m = another_array[p];
                    
                    //newpoint2 = intersection(current_polygon[i], another_polygon[m], current_polygon[(i+1<(current_polygon==polygon1?num1:num2))?i+1:0], another_polygon[(m+1<(another_polygon==polygon1?num1:num2))?m+1:0]);
                    
                    
                    
                    
                    //判断的依据是这2个点谁离当前polygon的i顶点最近
                    if(fabs(polygonpoint[0]-newpoint1[0])<fabs(polygonpoint[0]-newpoint2[0])){
                        newpolygon[j][0] = newpoint1[0];
                        newpolygon[j][1] = newpoint1[1];
                        newpolygon[j][2] = newpoint1[2];
                        j++;
                        std::cout
                        <<"Combining newpoints:\n";
                        show_point(newpolygon[j-1]);
                        //i的的变化应该加在这里，取newpoint1的话，对应的索引是p-1；
                        i = m+1<(another_polygon==polygon1?num1:num2)?m+1:0;
                        //p = p-1>0?p-1:array_length-1;
//                        m = another_array [p];
//                        i = ( m+1<( (another_polygon == polygon1)?num1:num2 ) )?m+1:0;
                    }
                    else{
                        newpolygon[j][0] = newpoint2[0];
                        newpolygon[j][1] = newpoint2[1];
                        newpolygon[j][2] = newpoint2[2];
                        j++;
                        std::cout
                        <<"Combining newpoints:\n";
                        show_point(newpolygon[j-1]);
                        //i的的变化应该加在这里，取newpoint2的话，对应的索引是p；
                        //顶点跳变的原则是：先跳到和i一样在r、t数组中对等的m，然后再将m+1，为什么？
                        /*因为我们所讨论的是凸多边形合并，而且顶点是逆时针排列的，如果是跳到[i]上,则下一个轮到的点就会是[i+1]，这很明显有问题*/
                        i = n+1<(another_polygon==polygon1?num1:num2)?n+1:0;;
//                        m = another_array[p];
//                        i = ( m+1 <( (another_polygon == polygon1)?num1:num2 ) )?m+1:0;
                    }
                    
                }
                //再接下去跳转polygon之前进行判断，如果当前的polygon已经处理完了最后一个顶点，则：
                
                if( i==((current_polygon==polygon1)?num1:num2) )
                    another_polygon_over = true;
                
                //既然跳到另一个多边形上，i也需要改变.如果跳变前我方数组是r，则i变成对方数组里的t[i+1]
                //下式的p其实相当于报告中的'i'，是记录数组的索引
                //i = another_array[(p+1<array_length)?p+1:0];
                //(another_array[p]+1) < ((another_polygon==polygon1)?num1:num2) ?another_array[p]+1 : 0;
                
                //在这之后，必须跳到另一个polygon上了。
                if(current_polygon == polygon1){
                    current_polygon = polygon2;
                    another_polygon = polygon1;
                    current_array = r;
                    another_array = t;
                }
                else{
                    current_polygon = polygon1;
                    another_polygon = polygon2;
                    current_array = t;
                    another_array = r;
                }
                
                
            }
            else if( is_in_polygon(another_polygon, num1, current_polygon[i])
                    &&( !is_in_array(current_array, array_length, i) ) ){
                std::cout
                <<"error 1: one of is_in_polygon was true in combine !!!";//事实上不可能进入到这个判断条件里面
            }
            else{
                std::cout
                <<"error 2: both of is_in_polygon was true in combine !!!";;//事实上不可能进入到这个判断条件里面
            }
            
        }
        
        //以上最后得到了该得到的 2维顶点数组，将其转化为OpenGL能绑定的 1维数组 后return。
        
        //show_polygon3(newpolygon, j);
        
        *length = num_of_total_vertices;
        
        for(int p = 0; p < *length ; p++){
            newpolygon_vertices[p*3+0] = newpolygon[p][0];
            newpolygon_vertices[p*3+1] = newpolygon[p][1];
            newpolygon_vertices[p*3+2] = newpolygon[p][2];
        }
        
        std::cout<<"\nBefore return:\n";
        show_polygon2(newpolygon_vertices, *length);
        
        return newpolygon_vertices;
    }
}


