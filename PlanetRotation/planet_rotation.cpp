#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std; 
#include "draw_planet.cpp"

#define WIDTH 800
#define HEIGHT 800

void key_callback(GLFWwindow *window,int key,int sccncode,int action,int mode);  
// 旋转相关
float public_rotate = -0.5f; //公转，+为逆时针，-为顺时针（从北半球观察）
float self_rotate = -5.0f;   //自转，+为逆时针，-为顺时针（从北半球观察）
bool view_flag = false;     //是否旋转视角
glm::vec3 view_rotate_dir;  //旋转绕轴，0,0,1为左右，0,1,0为上下
float view_rotate; //视角旋转方向

//顶点着色器  
const GLchar* vertexShaderSource="#version 330 core\n"  
    "layout (location=0) in vec3 aPos;\n"  
    "uniform mat4 transform;\n"
    "void main()\n"  
    "{\n"  
        "gl_Position = transform * vec4(aPos, 1.0);\n"  
    "}\0";  
//像素着色器  
const GLchar* fragmentShaderSource = "#version 330 core\n"
    "out vec3 color;\n"  
    "void main()\n"  
    "{\n"  
    "color = vec3(1.0f, 0.5f, 0.2f);\n"  
    "}\n\0";  



int main()  
{  
//初始化------------------------------------------------------------------------------------------------------------    
    glfwInit();
    // Set all the required options for GLFW   
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//设置主版本号  
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//设置次版本号  
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);  

    // Create a GLFWwindow object that we can use for GLFW's functions 
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "The routing planet", NULL, NULL);  
    glfwMakeContextCurrent(window);


    // Set the required callback functions 
    glfwSetKeyCallback(window, key_callback);  
  
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions  
    glewExperimental = GL_TRUE;  
    // Initialize GLEW to setup the OpenGL Function pointers  
    glewInit();  


    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    
    int width, height;  
    glfwGetFramebufferSize(window, &width, &height);    
    glViewport(0, 0, width, height);

// DATA SUN ××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××
    vector<float> vexs;
    vector<unsigned int> inds;
    generatePlanetTriangle(vexs, inds, 0, 0, 0, 1.8, 36, 18);
    // SUN
    GLuint SUN;
    glGenVertexArrays(1, &SUN);
    glBindVertexArray(SUN);
        // Vertices
        GLuint vert_buffer;
        glGenBuffers(1, &vert_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, vert_buffer);
        glBufferData(GL_ARRAY_BUFFER, vexs.size()*sizeof(GLfloat), vexs.data(), GL_STATIC_DRAW);

        // Indices
        GLuint indices_buffer;
        glGenBuffers(1, &indices_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, inds.size()*sizeof(GLuint), inds.data(), GL_STATIC_DRAW);

    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);      
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

// DATA PLANET ××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××
    vector<float> vexsP;
    vector<unsigned int> indsP;
    generatePlanetTriangle(vexsP, indsP, 0, 0, 0, 0.8, 18, 9);

    GLuint PLANET;
    glGenVertexArrays(2, &PLANET);
    glBindVertexArray(PLANET);
        // Vertices
        GLuint vert_buffer_planet;
        glGenBuffers(2, &vert_buffer_planet);
        glBindBuffer(GL_ARRAY_BUFFER, vert_buffer_planet);
        glBufferData(GL_ARRAY_BUFFER, vexsP.size()*sizeof(GLfloat), vexsP.data(), GL_STATIC_DRAW);

        // Indices
        GLuint indices_buffer_planet;
        glGenBuffers(2, &indices_buffer_planet);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer_planet);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indsP.size()*sizeof(GLuint), indsP.data(), GL_STATIC_DRAW);

    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);      
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
// Shader *****************************************************************************************
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    GLint success;  
    GLchar infoLog[512];  
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);  
    if (!success)  
    {  
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);  
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;  
    }  
  
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glCompileShader(fragmentShader);  
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);  
    if (!success)  
    {  
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);  
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;  
    }  

    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);  
    if (!success) {  
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);  
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;  
    }  
    
    //删除原本的着色器
    glDeleteShader(vertexShader);  
    glDeleteShader(fragmentShader);  

//安排视角----------------------------------------------------------------------------------------------------------------------------
    glm::mat4 Projection = glm::perspective(glm::radians(90.0f), (float) width / (float)height, 0.1f, 100.0f);
    glm::mat4 View = glm::lookAt(
        glm::vec3(0,0,5), // Camera is at (4,3,3), in World Space
        glm::vec3(0,0,0), // and looks at the origin
        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    ); 

    glm::mat4 model = glm::mat4(1.0f);
    float planet_self = 0;
    float planet_public = 0;
    glm::vec3 position = glm::vec3(3, 0, 0);
    glm::mat4 mvp;

//××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        if(view_flag){
            View = glm::rotate(View, glm::radians(view_rotate), view_rotate_dir);   //旋转视角以观察
            view_flag = false;
        }

        mvp = Projection * View * model;

        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(mvp));

        glBindVertexArray(SUN);
        glDrawElements(GL_LINE_STRIP, inds.size(), GL_UNSIGNED_INT, (void*)0); // we use index buffer, so set it to null.  


        glBindVertexArray(PLANET);
        
        planet_self = planet_self + self_rotate;
        if (planet_self > 360)
            planet_self -= 360;
        else if (planet_self < 0)
            planet_self += 360;
        planet_public = (planet_public + public_rotate);
        if (planet_public > 360)
            planet_public -= 360;
        else if (planet_public < 0)
            planet_public += 360;
        glm::mat4 public_trans = glm::rotate(model, glm::radians(planet_public), glm::vec3(0, 0, 1));
        glm::mat4 position_trans = glm::translate(model, position);
        glm::mat4 self_trans = glm::rotate(model, glm::radians(planet_self), glm::vec3(0, 0, 1));
        mvp = Projection * View * public_trans * position_trans * self_trans;
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(mvp));

        glDrawElements(GL_LINE_STRIP, indsP.size(), GL_UNSIGNED_INT, (void*)0); // we use index buffer, so set it to null.  

        glfwSwapBuffers(window);
    }
//收尾
    glDeleteVertexArrays(1, &SUN);  
    glDeleteVertexArrays(2, &PLANET);  
    glDeleteBuffers(1, &vert_buffer);  
    glDeleteBuffers(2, &vert_buffer_planet);  
    glfwTerminate();  
    return 0;  

}  
// Is called whenever a key is pressed/released via GLFW  
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)  
{  
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)  {
        glfwSetWindowShouldClose(window, GL_TRUE);  
    }
    else if (key == GLFW_KEY_D && action == GLFW_PRESS){
        if (mode == GLFW_MOD_SHIFT)
            self_rotate = -5.0f;
        else
            self_rotate = 5.0f;
    }
    else if (key == GLFW_KEY_Y && action == GLFW_PRESS){
        if (mode == GLFW_MOD_SHIFT)
            public_rotate = -0.5f;
        else
            public_rotate = 0.5f;
    }
    else if (key == GLFW_KEY_UP && action == GLFW_REPEAT){
        view_flag = true;
        view_rotate = 3.0f;
        view_rotate_dir = glm::vec3(0.0f, 1.0f, 0.0f);
    }
    else if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT){
        view_flag = true;
        view_rotate = -3.0f;
        view_rotate_dir = glm::vec3(0.0f, 1.0f, 0.0f);
    }
    else if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT){
        view_flag = true;
        view_rotate = -3.0f;
        view_rotate_dir = glm::vec3(0.0f, 0.0f, 1.0f);
    }
    else if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT){
        view_flag = true;
        view_rotate = 3.0f;
        view_rotate_dir = glm::vec3(0.0f, 0.0f, 1.0f);
    }
}  
