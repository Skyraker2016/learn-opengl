#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

// Is called whenever a key is pressed/released via GLFW  
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)  
{  
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)  
        glfwSetWindowShouldClose(window, GL_TRUE);  
}  

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 600, "The First Opengl Program", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Set the required callback functions 设置事件回调函数  
    glfwSetKeyCallback(window, key_callback);  

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Draw a triangle */
        glBegin(GL_QUAD_STRIP);

        glVertex3f(0.5, 0.5, 0.0);

        glVertex3f(0.5, -0.5, 0.0);

        glVertex3f(-0.5, 0.5, 0.0);

        glVertex3f(-0.5, -0.5, 0.0);


        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}