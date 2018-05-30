#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std; 

#define WIDTH 800
#define HEIGHT 800

void set_callback(GLFWwindow *window);
void key_callback(GLFWwindow *window,int key,int sccncode,int action,int mode);  
void mouse_move_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_click_callback(GLFWwindow* window, int button, int action, int mods);
void display();
void bezier(vector<vector<GLfloat>> &line, int n);

GLfloat mouseX, mouseY;
int click = 0;
bool add = true;
vector<vector<GLfloat> > pos(4, vector<GLfloat>(3));
vector<vector<vector<GLfloat> > > lines;




int main()  
{  
//初始化------------------------------------------------------------------------------------------------------------    
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Bezier", NULL, NULL);  
    glfwMakeContextCurrent(window);

    set_callback(window);


    while (!glfwWindowShouldClose(window)){

        display();

        if (click==4 && add){
            vector<vector<GLfloat>> tmp;
            bezier(tmp, 20);
            lines.push_back(tmp);
            add = false;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    glfwTerminate();  
    return 0;  

}  
// Is called whenever a key is pressed/released via GLFW  
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)  
{  
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)  {
        glfwSetWindowShouldClose(window, GL_TRUE);  
    }
    else if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
        click = 0;
        add = true;
    }
    else if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS ) {
        click = 0;
        add = true;
        if (!lines.empty())
            lines.pop_back();
    }
}  


void mouse_move_callback(GLFWwindow* window, double xpos, double ypos){
    mouseX = xpos;
    mouseY = ypos;
}
void mouse_click_callback(GLFWwindow* window, int button, int action, int mods){
    if (button==GLFW_MOUSE_BUTTON_LEFT && action==GLFW_PRESS){
        if (click < 4){
            pos[click][0] = mouseX-400;
            pos[click][1] = 400-mouseY;
            pos[click][2] = 0;
            // cout<<click<<": "<<pos[click][0]<<" "<<pos[click][1]<<" "<<pos[click][2]<<endl;
            click++;
        }
    }
}
void set_callback(GLFWwindow* window){
    glfwSetKeyCallback(window, key_callback);  
    glfwSetCursorPosCallback(window, mouse_move_callback);//鼠标移动回调函数
    glfwSetMouseButtonCallback(window, mouse_click_callback);//鼠标点击回调函数
    return;
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();
	glLoadIdentity();

	glLineWidth(1.5);
	glColor3f(1.0, 1.0, 1.0);

	glOrtho(-400, 400, -400, 400, -400, 400);
	gluLookAt(0, 0, 0, 
    0, 0, -1, 
    0, 1, 0);

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < click; i++) {
		glVertex3f(pos[i][0], pos[i][1], pos[i][2]);
	}
	glEnd();
    glColor3f(1.0, 1.0, 0.0);
	for (int i = 0; i < lines.size(); i++) {
	    glBegin(GL_LINE_STRIP);
        for (int j=0; j<lines[i].size(); j++){
            glVertex3f(lines[i][j][0], lines[i][j][1], lines[i][j][2]);
        }
        glEnd();
	}

	glPopMatrix();

}



void bezier(vector<vector<GLfloat>> &line, int n){
    float step = 1.0f / n;
    float a, b, c, d;
    for (int i=0; i<=n; i++){
        float these = step * i;
        a = (1-these) * (1-these) * (1-these);
        b = (1-these) * (1-these) * these * 3;
        c = (1-these) * these * these * 3;
        d = these * these * these;
        vector<GLfloat> tmp;
        tmp.push_back(a * pos[0][0] + b * pos[1][0] + c * pos[2][0] + d * pos[3][0]);
        tmp.push_back(a * pos[0][1] + b * pos[1][1] + c * pos[2][1] + d * pos[3][1]);
        tmp.push_back(a * pos[0][2] + b * pos[1][2] + c * pos[2][2] + d * pos[3][2]);
        line.push_back(tmp);
    }
}