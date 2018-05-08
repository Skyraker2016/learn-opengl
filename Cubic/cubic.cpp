#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;  
  
void key_callback(GLFWwindow *window,int key,int sccncode,int action,int mode);  
// Window dimensions  
const GLuint WIDTH = 800, HEIGHT = 800;  
  
//顶点着色器  
const GLchar* vertexShaderSource="#version 330 core\n"  
    "layout (location=0) in vec3 aPos;\n"  
    "layout (location=1) in vec3 vertexColor;\n"
    "out vec3 fragmentColor;\n"
    "uniform mat4 transform;\n"
    "void main()\n"  
    "{\n"  
        "gl_Position = transform * vec4(aPos, 1.0);\n"  
        "fragmentColor = vertexColor;\n"
    "}\0";  
//像素着色器  
const GLchar* fragmentShaderSource = "#version 330 core\n"
    "in vec3 fragmentColor;\n"
    "out vec3 color;\n"  
    "void main()\n"  
    "{\n"  
    "color = fragmentColor;\n"  
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
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "The perspective projection", NULL, NULL);  
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
  
//图形数据---------------------------------------------------------------------------------------------
    //顶点
    float vertices[] = {
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
    };
    srand((int)time(NULL));
    float color[108];
    for (int i=0; i<108; i++){
        color[i] = (float)rand()/RAND_MAX;
        // cout<<color[i]<<endl;
    }

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);
//着色器--------------------------------------------------------------------------------------------------------------------------------
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

//安排透视视角----------------------------------------------------------------------------------------------------------------------------
    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(90.0f), (float) width / (float)height, 0.1f, 100.0f);
    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 View = glm::lookAt(
        glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
        glm::vec3(0,0,0), // and looks at the origin
        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    ); 
//主循环--------------------------------------------------------------------------------------------------------------------------------
    while (!glfwWindowShouldClose(window))
    {

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);


        View = glm::rotate(View, glm::radians(1.0f), glm::vec3(-1.0f, -1.0f, 1.0f)); 


        glm::mat4 mvp = Projection * View * model;

        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(mvp));


        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }
//收尾
    glDeleteVertexArrays(1, &VAO);  
    glDeleteBuffers(1, &VBO);  
    glfwTerminate();  
    return 0;  
}  
// Is called whenever a key is pressed/released via GLFW  
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)  
{  
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)  
        glfwSetWindowShouldClose(window, GL_TRUE);  
}  
