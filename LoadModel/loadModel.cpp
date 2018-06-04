#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
using namespace std; 
#include "Model.hpp"

#define WIDTH 800
#define HEIGHT 800

void key_callback(GLFWwindow *window,int key,int sccncode,int action,int mode);  
void mouse_move_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_click_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* windowm, double x, double y);
void display(Model&);
void setEvents(GLFWwindow*);


//旋转全局数据
GLfloat angle_rl = 0;
GLfloat angle_ud = 0;
//style全局数据
int sstyle = 0;
//平移向量
GLfloat tx = 0;
GLfloat ty = 0;
//移动速度
GLfloat moveX = 0;
GLfloat moveY = 0;
//鼠标位置
GLfloat mouseX = 0;
GLfloat mouseY = 0;
//是否正在拖拽
bool clicking = false;
//缩放倍数
int ssize = WIDTH/2;


int main(int argc,char **argv)  
{  
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);  

    //获得输入文件名
    if (argc <2){
        cout<<"Please input the file name.";
        return 0;
    }
    GLchar* name = argv[1];
    Model test(name);

    //初始化窗口
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, name, NULL, NULL);  
    glfwMakeContextCurrent(window);

    //注册回调函数
    setEvents(window);

    //初始化glew
    glewExperimental = GL_TRUE;  
    glewInit();  

    //激活深度测试
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);


    //光照设置
    // 激活光照计算
    glEnable(GL_LIGHTING);

    GLfloat light_position[] = { -1.0, 0.3, 0, 0.0 };   //光源位置
    glLightfv(GL_LIGHT0,  GL_POSITION,  light_position);    //设置光源位置
    GLfloat light_color[] = { 0.05, 0.05, 0.05, 1.0 }; //光源颜色
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);     //设置光源颜色
    //激活以上设置的0号光源
    glEnable(GL_LIGHT0);     

    // 指定环境光强度（RGBA）
    GLfloat ambientLight[] = {0.4f, 0.4f, 0.4f, 1.0f};
    // 设置光照模型，将ambientLight所指定的RGBA强度值应用到环境光
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

    // 启用颜色追踪，为了能够在光照反射时能反应颜色
    glEnable(GL_COLOR_MATERIAL);
    // 设置多边形正面的环境光和散射光材料属性，追踪glColor
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);


    //背景色设置
    glClearColor(0.2f, 0.3f, 0.2f, 1.0f);

    while (!glfwWindowShouldClose(window)){
        display(test);
        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    glfwTerminate();  
    return 0;  

}  

//键盘事件
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)  
{  
    //退出
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)  {  
        glfwSetWindowShouldClose(window, GL_TRUE);  
    }
    //旋转
    else if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT){   
        angle_rl -= 1;
    }
    else if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT){
        angle_rl += 1;
    }
    else if (key == GLFW_KEY_UP && action == GLFW_REPEAT){
        angle_ud -= 1;
    }
    else if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT){
        angle_ud += 1;
    }
    //切换style
    else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS){
        sstyle = (sstyle+1)%3;
    }
    
}  

//实时记录鼠标位置及移动趋势
void mouse_move_callback(GLFWwindow* window, double xpos, double ypos){
    //记录鼠标移动趋势——通过当前坐标与上一坐标计算
    moveX = mouseX - xpos;
    moveY = mouseY - ypos;
    //记录鼠标当前坐标
    mouseX = xpos;
    mouseY = ypos;
}
//通知全局鼠标是否按下（即拖拽中）
void mouse_click_callback(GLFWwindow* window, int button, int action, int mods){
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        clicking = true;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
        clicking = false;
    }
}

void scroll_callback (GLFWwindow* window, double xoffset, double yoffset)
{
    ssize += 5*yoffset;
}


void setEvents(GLFWwindow * window){
    glfwSetKeyCallback(window, key_callback);   //键盘事件
    glfwSetMouseButtonCallback(window, mouse_click_callback);   //鼠标按键
    glfwSetCursorPosCallback(window, mouse_move_callback);  //鼠标移动
    glfwSetScrollCallback(window, scroll_callback); //鼠标滚轮
}

void display(Model &test){
    //清缓存
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //保存上一个变换矩阵，本项目中不用
    // glPushMatrix();

    //加载单位矩阵
    glLoadIdentity();
    //透视矩阵
    gluPerspective(60.0, 1, 400, -400);
    //摄像矩阵
    gluLookAt(0, 0, 800, 0, 0, 0, 0, 1, 0);
    //鼠标拖拽时进行相应的移动
    if (clicking){
        tx -= moveX;
        ty += moveY;
    }
    // 将运动趋势归零防止惯性
    moveX = moveY = 0;
    //平移
    glTranslatef(tx, ty, 0.0f);
    //旋转
    glRotatef(angle_ud,1,0,0);
    glRotatef(angle_rl,0,1,0);
    //以上矩阵生效顺序是从下至上

    //设置默认颜色
    glColor3f(1.0, 1.0, 1.0);
    //根据模式来进行绘制
    switch(sstyle){
        case 0: //wire
            glLineWidth(1.0f);
            test.Draw(GL_LINE_STRIP,ssize);
            break;
        case 1: //flat
            test.Draw(GL_TRIANGLES,ssize);
            break;
        case 2: //flat lines
            glColor3f(0.8, 0.8, 0.8);
            test.Draw(GL_TRIANGLES,ssize);
            glLineWidth(1.5f);
            glColor3f(0.2, 0.2, 0.2);
            test.Draw(GL_LINE_STRIP,ssize);
            break;
    }

    // glPopMatrix();
}