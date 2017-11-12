//
//  corefunctions.cpp
//  ls_opengl_2
//
//  Created by Treaser Lusio on 2017/11/7.
//  Copyright © 2017年 treasersimplifies. All rights reserved.
//

#include "corefunctions.hpp"
#include "myfunctions.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLfloat * polygon_combine(GLfloat polygon1[][3], const GLuint num1, GLfloat polygon2[][3], const GLuint num2, GLuint * length){
    
    GLfloat newpolygon[2*num1+2*num2][3];
    
    GLint p = -1,q = -1;//polygon1在polygon2中的点的索引，头和尾。具体见图
    GLint m = -1,n = -1;//polygon2在polygon1中的点的索引，头和尾。
    
    //用于(2)(3)情况的r、t数组：
    GLuint * r = new GLuint[num1+num2];
    GLuint * t = new GLuint[num1+num2];
    
    GLfloat * newpolygon_vertices = new GLfloat[(num1+num2)*3];//最后需要返回的数据格式
    
    //一定要加new,否则返回没用，因为返回的是指针，而当函数返回时，局部变量都没了，所以这个局部变量指正必须是new出来的。
    
    //先求出polygon2在1内部的点
    int i,j;
    int k,s;//专门给newpolygon_vertices的索引
    
    
    //2.1修改多边形，使得其顶点0必位于另一个多边形的外部
    adjust_polygon(polygon1, num1, polygon2, num2);
    
    //以上处理完了所有的polygon,完成了2.1步骤：把0顶点都放到外面
    
    //保险起见，再判断一次
    if ((is_in_polygon(polygon1, num1, polygon2[0]))||(is_in_polygon(polygon2, num2, polygon1[0]))) {
        std::cout<<"error:Vertex 0 of two ploygons is still in the other polygon!";
    }
    
    
    //2.2判断这2个多边形符合（1）（2）（3）（4）中哪种情况，方法是分别遍历2个polygon中的所有顶点，看其是否在另一个polygon内。
    
    //如果是情况(1):参考报告中2.3部分
    if(is_case_1(polygon1, num1, polygon2, num2)){
        
        //2.3.1先求出m,n(位于ploygon2),p,q(位于ploygon1).
        for(i = 0; i<num2 ; i++){
            if(is_in_polygon(polygon1, num1, polygon2[i])){
                //如果i点在polygon1内部
                if((m == -1)||(i<m)){
                    m = i;//得到polygon2的最小索引m
                }
            }
            else{//先找n+1，找到以后 n = n?(num1-1):(n-1);
                if((i<n)||(n==-1)){
                    n = i;//得到的其实是n+1（要小心0这种特殊情况）
                }
            }
        }
        n = n?(n-1):(num2-1);
        
        std::cout
        <<"m:"<<m<<'\t'<<"n:"<<n<<'\t'
        <<std::endl;
        
        //再先寻找polygon1在polygon2的点
        for(i = 0; i<num1 ; i++){
            if(is_in_polygon(polygon2, num2, polygon1[i])){
                if((p==-1)||(i<p))
                    p = i;//得到polygon1的最小索引p
            }
            else{//先找q+1，找到以后 q = q?(num1-1):(q-1);
                if((i<q)||(q==-1))
                    q = i;
            }
        }
        q = q?(q-1):(num1-1);
        std::cout
        <<"p:"<<p<<'\t'<<"q:"<<q<<'\t'
        <<std::endl;
        //以上求出了m,n(位于ploygon2),p,q(位于ploygon1).
        
        //2.3.3接下来的任务是计算新的交点：（2.3.2直接在[]内求出）
        GLfloat * newpoint_m_q = intersection(polygon2[m], polygon1[q], polygon2[m-1], polygon1[(q+1<num1)?q+1:0]);
        //(q+1<num1)?q+1:0,对应于2.3.2中的判断
        GLfloat * newpoint_n_p = intersection(polygon2[n], polygon1[p], polygon2[(n+1<num2)?n+1:0], polygon1[p-1]);
        
        std::cout
        <<"intersections(newpoints) are:\n"
        <<newpoint_m_q[0]<<","<<newpoint_m_q[1]<<","<<newpoint_m_q[2]<<'\n'
        <<newpoint_n_p[0]<<","<<newpoint_n_p[1]<<","<<newpoint_n_p[2]<<'\n'
        <<std::endl;
        
        //2.3.5一切就绪，可以直接来生产combine了：
        //n p 的交点右边是polygon1在2内部的点；
        //m q 的交点左边是polygon2在1内部的点：
        
        //随便选一个点开始，不妨选n、p相交的那个newpoint开始：
        k = 0;
        newpolygon[0][0] = newpoint_n_p[0];//先把新的点赋给它，作为第一个点
        newpolygon[0][1] = newpoint_n_p[1];
        newpolygon[0][2] = newpoint_n_p[2];
        k++;
        
        i = (n+1<num2)?n+1:0;
        int max = (n+1<num2)?num2:m;
        for( ; i<max ; i++){//把polygon2的一部分点(n+1到num2-1)赋值给 newpolygon
            newpolygon[k][0] = polygon2[i][0];
            newpolygon[k][1] = polygon2[i][1];
            newpolygon[k][2] = polygon2[i][2];
            k++;
        }
        for(i = (i==num2?0:m); i < m ; i++){//把polygon2的一部分点(0到m-1)赋值给 newpolygon
            newpolygon[k][0] = polygon2[i][0];
            newpolygon[k][1] = polygon2[i][1];
            newpolygon[k][2] = polygon2[i][2];
            k++;
        }
        
        newpolygon[k][0] = newpoint_m_q[0];//把第二个新的点赋给它
        newpolygon[k][1] = newpoint_m_q[1];
        newpolygon[k][2] = newpoint_m_q[2];
        k++;
        
        i = (q+1<num1)?q+1:0;
        max = (q+1<num1)?num1:p;//这种式子不能放到判断里，会动态变化的。
        
        for(; i < max; i++){//把polygon1的一部分点(q+1到num1-1)赋值给 newpolygon
            newpolygon[k][0] = polygon1[i][0];
            newpolygon[k][1] = polygon1[i][1];
            newpolygon[k][2] = polygon2[i][2];
            k++;
        }
        
        for(i = (i==num1?0:p); i < p ; i++){//把polygon2的一部分点(0到p-1)赋值给 newpolygon
            newpolygon[k][0] = polygon1[i][0];
            newpolygon[k][1] = polygon1[i][1];
            newpolygon[k][2] = polygon1[i][2];
            k++;
        }
        
        *length = k;//记录了这个新的polygon里到底有多少个点，需要改变传入的length
        
        std::cout
        <<"Finish to COMBINE newpolygon in case1:";
        
        for(int p = 0; p < *length ; p++){
            newpolygon_vertices[p*3+0] = newpolygon[p][0];
            newpolygon_vertices[p*3+1] = newpolygon[p][1];
            newpolygon_vertices[p*3+2] = newpolygon[p][2];
        }
        
        show_polygon2(newpolygon_vertices, *length);
        return newpolygon_vertices;
    }
    else if(is_case_2(polygon1, num1, polygon2, num2)){
        //如果是报告中的情况(2)：polygon2有顶点位于polygon1内部，但 polygon1无顶点位于polygon2内部；
        newpolygon_vertices = deal_case2_combine(polygon1, num1, polygon2, num2, length);
        return newpolygon_vertices;
        
    }
    else if(is_case_3(polygon1, num1, polygon2, num2)){
        //如果是情况(3)：将ploygon1和polygon2交换后就是case2
        newpolygon_vertices = deal_case2_combine(polygon2, num2, polygon1, num1, length);
        return newpolygon_vertices;
    }
    else{
        std::cout<<"case 4: true\n";
        //2.6处理最后一种情况：情况(4):
        //polygon2无顶点位于polygon1内部，且polygon1也无顶点位于polygon2内部；
        //特殊情况：两者相离；
        //一般情况：两者有边相交，但是没有顶点包含在另一个多边形中，其实就是case2.
        j = 0;
        i = 0;
        
        bool polygon1_separate_polygon2 = true;
        for(k = 0; k < num2 ; k++){
            //2.6.1,对每个接下去顶点i进行如下处理
            //对polygon2中的每条边：
            
            for(s = 0; s < num1 ; s++){
                //如果polygon1有边和它相交，则记录
                
                bool has_int =has_intersection(polygon2[k], polygon1[s], polygon2[(k+1<num2)?k+1:0], polygon1[(s+1<num1)?s+1:0]);
                if(has_int){
                    r[i] = k;//r数组记录polygon2的顶点
                    t[j] = s;//t数组记录polygon1的顶点
                    i++;
                    j++;
                    polygon1_separate_polygon2 = false;
                }
            }
        }
        
        if(polygon1_separate_polygon2){
            //两个polygon没有交集，返回，用一根直线将两者相连的顶点数组：
            k = 0;
            for(i = 0;i<num1;i++){//先将polygon1原封不动地赋给newpolygon_vertices
                newpolygon_vertices[3*k+0] = polygon1[i][0];
                newpolygon_vertices[3*k+1] = polygon1[i][1];
                newpolygon_vertices[3*k+2] = polygon1[i][2];
                k++;
            }
            newpolygon_vertices[3*k+0] = polygon1[0][0];
            newpolygon_vertices[3*k+1] = polygon1[0][1];
            newpolygon_vertices[3*k+2] = polygon1[0][2];
            k++;

            for(i = 0;i<num2;i++){//再将polygon2原封不动地赋给newpolygon_vertices
                newpolygon_vertices[3*k+0] = polygon2[i][0];
                newpolygon_vertices[3*k+1] = polygon2[i][1];
                newpolygon_vertices[3*k+2] = polygon2[i][2];
                k++;
            }
            
            newpolygon_vertices[3*k+0] = polygon2[0][0];
            newpolygon_vertices[3*k+1] = polygon2[0][1];
            newpolygon_vertices[3*k+2] = polygon2[0][2];
            k++;
            *length = k;
            
            std::cout
            <<"Actually，polygon1 and polygon2 have no coincident area.\n";
            return newpolygon_vertices;
        }
        else{
            //这是由于两者相互没有交点
            //虽然deal_case2中有返回语言
            newpolygon_vertices = deal_case2_combine(polygon1, num1, polygon2 , num2, length);
            return newpolygon_vertices;
        }
    }
    return 0;
}




//进行遍历生成最后的多边形,注意不能用**来代替[][3]
GLfloat * polygon_substract(GLfloat polygon1[][3], const GLuint num1, GLfloat polygon2[][3], const GLuint num2, GLuint * length){
    
    GLfloat newpolygon[2*num1+2*num2][3];
    
    GLint p = -1,q = -1;//polygon1在polygon2中的点的索引，头和尾。具体见图
    GLint m = -1,n = -1;//polygon2在polygon1中的点的索引，头和尾。
    
    //用于(2)(3)情况的r、t数组：
    GLuint * r = new GLuint[num1+num2];
    GLuint * t = new GLuint[num1+num2];
    
    GLfloat * newpolygon_vertices = new GLfloat[(num1+num2)*3];//最后需要返回的数据格式
    
    //一定要加new,否则返回没用，因为返回的是指针，而当函数返回时，局部变量都没了，所以这个局部变量指正必须是new出来的。
    
    //先求出polygon2在1内部的点
    int i,j;
    int k,s;//专门给newpolygon_vertices的索引
    
    
    //2.1修改多边形，使得其顶点0必位于另一个多边形的外部
    int index = adjust_polygon(polygon1, num1, polygon2, num2);
    
    
    
    //以上处理完了所有的polygon,完成了2.1步骤：把0顶点都放到外面，并且2.2 返回了polygon1与相互2包含的特殊情况
    
    //2.3下面开始处理报告中情况(1)的顶点合并：
    if ((is_in_polygon(polygon1, num1, polygon2[0]))||(is_in_polygon(polygon2, num2, polygon1[0]))) {
        //保险起见，再判断一次
        std::cout<<"Error:Vertex 0 of two ploygons is still in the other polygon!\n"
        <<"Maybe because polygon1 contains polygon2...\n";
        
    }
    
    
    /******************需要修改：*********************
     //顶点0不在另一个多边形内部，则两者相交的点是其序号必定是连续的，所直接赋值给新的多边形即可
     //写此代码时考虑的是polygon1位于左边，polygon2位于右边，但是考虑的顺逆时针，所以左右交换也是对的。
     
     //GLint p = -1,q = -1;polygon1在polygon2中的点的索引，头和尾。具体见图
     //GLint m = -1,n = -1;polygon2在polygon1中的点的索引，头和尾。
     
     //先寻找polygon2在polygon1的点
     //有2种特殊情况，1是两者相互包含,在前面已经返回，2是相互分离，需要在这里写明~
     //还有一种特殊情况是n+1或者q+1越界（m-1和p-1肯定不会越界）
     ***********************************************/
    
    //2.2判断这2个多边形符合（1）（2）（3）（4）中哪种情况，方法是分别遍历2个polygon中的所有顶点，看其是否在另一个polygon内。
    
    if(is_case_1(polygon1, num1, polygon2, num2)){
        
        //如果是情况(1):参考报告中2.3的部分
        //2.3.1先求出m,n,p,q.
        for(i = 0; i<num2 ; i++){
            if(is_in_polygon(polygon1, num1, polygon2[i])){
                //如果i点在polygon1内部
                if((m == -1)||(i<m)){
                    m = i;//得到polygon2的最小索引m
                }
            }
            else{//先找n+1，找到以后 n = n?(num1-1):(n-1);
                if((i<n)||(n==-1)){
                    n = i;//得到的其实是n+1（要小心0这种特殊情况）
                }
            }
        }
        n = n?(n-1):(num2-1);
        
        std::cout
        <<"m:"<<m<<'\t'<<"n:"<<n<<'\t'
        <<std::endl;
        
        //再先寻找polygon1在polygon2的点
        for(i = 0; i<num1 ; i++){
            if(is_in_polygon(polygon2, num2, polygon1[i])){
                if((p==-1)||(i<p))
                    p = i;//得到polygon1的最小索引p
            }
            else{//先找q+1，找到以后 q = q?(num1-1):(q-1);
                if((i<q)||(q==-1))
                    q = i;
            }
        }
        q = q?(q-1):(num1-1);
        std::cout
        <<"p:"<<p<<'\t'<<"q:"<<q<<'\t'
        <<std::endl;
        //以上求出了m,n,p,q.
        
        //2.3.3接下来的任务是计算新的交点：（2.3.2直接在[]内求出）
        GLfloat * newpoint_m_q = intersection(polygon2[m], polygon1[q], polygon2[m-1], polygon1[(q+1<num1)?q+1:0]);
        //(q+1<num1)?q+1:0,对应于2.3.2中的判断
        GLfloat * newpoint_n_p = intersection(polygon2[n], polygon1[p], polygon2[(n+1<num2)?n+1:0], polygon1[p-1]);
        
        std::cout
        <<"intersections are:\n"
        <<newpoint_m_q[0]<<","<<newpoint_m_q[1]<<","<<newpoint_m_q[2]<<'\n'
        <<newpoint_n_p[0]<<","<<newpoint_n_p[1]<<","<<newpoint_n_p[2]<<'\n'
        <<std::endl;
        
        //2.3.4一切就绪，求交运算：
        //n p 交点右边是polygon1在2内部的点；
        //m q 交点左边是polygon2在1内部的点：
        k = 0;
        newpolygon[0][0] = newpoint_n_p[0];//先把新的点赋给它，作为第一个点
        newpolygon[0][1] = newpoint_n_p[1];
        newpolygon[0][2] = newpoint_n_p[2];
        k = 1;
        for(i = p;i<q+1;i++){//把polygon1的点赋值给了newpolygon
            newpolygon[k][0] = polygon1[i][0];
            newpolygon[k][1] = polygon1[i][1];
            newpolygon[k][2] = polygon1[i][2];
            k++;
        }
        newpolygon[k][0] = newpoint_m_q[0];//把第二个新的点赋给它(0.06,-0.4,0)
        newpolygon[k][1] = newpoint_m_q[1];
        newpolygon[k][2] = newpoint_m_q[2];
        
        k++;
        for(i = m; i<n+1 ; i++){//把polygon2的点赋值给了newpolygon
            newpolygon[k][0] = polygon2[i][0];
            newpolygon[k][1] = polygon2[i][1];
            newpolygon[k][2] = polygon2[i][2];
            k++;
        }
        
        *length = k;//记录了这个新的polygon里到底有多少个点，需要改变传入的length
        
        std::cout
        <<"Finish to SUBSTRACT newpolygon in case1:";
        
        for(int p = 0; p < *length ; p++){
            newpolygon_vertices[p*3+0] = newpolygon[p][0];
            newpolygon_vertices[p*3+1] = newpolygon[p][1];
            newpolygon_vertices[p*3+2] = newpolygon[p][2];
        }
        show_polygon2(newpolygon_vertices, *length);
        return newpolygon_vertices;
    }
    else if(is_case_2(polygon1, num1, polygon2, num2)){
        //2.4情况（2）的处理：
        newpolygon_vertices = deal_case2_substact(polygon1, num1, polygon2, num2, length);
        return newpolygon_vertices;
    }
    else if(is_case_3(polygon1, num1, polygon2, num2)){
        //2.5情况（3）：将ploygon1和polygon2交换后就是case2
        newpolygon_vertices = deal_case2_substact(polygon2, num2, polygon1, num1, length);
        return newpolygon_vertices;
    }
    else{
        std::cout<<"case 4: true\n";
        //2.6处理最后一种情况：情况(4):
        //polygon2无顶点位于polygon1内部，且polygon1也无顶点位于polygon2内部；
        //特殊情况：两者相离；
        //一般情况：两者有边相交，但是没有顶点包含在另一个多边形中，其实就是case2.
        j = 0;
        i = 0;
        
        bool polygon1_separate_polygon2 = true;
        for(k = 0; k < num2 ; k++){
            //2.6.1,对每个接下去顶点i进行如下处理
            //对polygon2中的每条边：
            
            for(s = 0; s < num1 ; s++){
                //如果polygon1有边和它相交，则记录
                
                bool has_int =has_intersection(polygon2[k], polygon1[s], polygon2[(k+1<num2)?k+1:0], polygon1[(s+1<num1)?s+1:0]);
                if(has_int){
                    r[i] = k;//r数组记录polygon2的顶点
                    t[j] = s;//t数组记录polygon1的顶点
                    i++;
                    j++;
                    polygon1_separate_polygon2 = false;
                }
            }
        }
        
        if(polygon1_separate_polygon2){
            //两个polygon没有交集，返回全是0元素的顶点数组：
            for(i = 0;i<(num1+num2)*3;i++){
                newpolygon_vertices[i] = 0.0f;
            }
            k = 0 ;
            *length = k;
            std::cout
            <<"Actually，polygon1 and polygon2 have no coincident area.\n";
            return newpolygon_vertices;
        }
        else{
            //这是由于两者相互没有交点
            //虽然deal_case2中有返回语言
            newpolygon_vertices = deal_case2_substact(polygon1, num1, polygon2 , num2, length);
            return newpolygon_vertices;
        }
    }
    return 0;
}



