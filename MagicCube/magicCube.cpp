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
void generateCubic(vector<float> &verx, vector<float> &colorGroup, vector<vector<float>> &color);
void key_callback(GLFWwindow *window,int key,int sccncode,int action,int mode);  
// 旋转相关
bool view_flag = false;     //是否旋转视角
glm::vec3 view_rotate_dir;  //旋转绕轴，0,0,1为左右，0,1,0为上下
float view_rotate; //视角旋转方向
bool lock = true;
float v = 10.0f;

float cub_right = 0.0f;
float cub_left = 0.0f;
float cub_col = 0.0f;
float cub_up = 0.0f;
float cub_down = 0.0f;
float cub_row = 0.0f;
float cub_face = 0.0f;


vector<float> RED;
vector<float> ORANGE;
vector<float> YELLOW;
vector<float> WHITE;
vector<float> BLUE;
vector<float> GREEN;

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
    RED.push_back(1.0000);
    RED.push_back(0.4156);
    RED.push_back(0.4156);
    GREEN.push_back(0.7568);
    GREEN.push_back(1.0000);
    GREEN.push_back(0.7568);
    BLUE.push_back(0.3921);
    BLUE.push_back(0.5843);
    BLUE.push_back(0.9294);
    YELLOW.push_back(0.9333);
    YELLOW.push_back(0.9333);
    YELLOW.push_back(0.000);
    ORANGE.push_back(0.9568);
    ORANGE.push_back(0.5843);
    ORANGE.push_back(0.0000);
    WHITE.push_back(0.9500);
    WHITE.push_back(0.9607);
    WHITE.push_back(0.9333);
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

// DATA CUB ××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××
    unsigned int VAO;
        vector<float> vexs;
        vector<float> color;
        vector<vector<float>> colorTmp;
        colorTmp.push_back(BLUE);
        colorTmp.push_back(RED);
        colorTmp.push_back(ORANGE);
        colorTmp.push_back(YELLOW);
        colorTmp.push_back(WHITE);
        colorTmp.push_back(GREEN);
        generateCubic(vexs, color, colorTmp);
        // CUB
        unsigned int VBO;
        glGenVertexArrays(1, &(VAO));
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);

        GLuint colorbuffer;
        glGenBuffers(1, &colorbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*color.size(), color.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vexs.size(), vexs.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(1);
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
    // glm::mat4 Projection = glm::perspective(glm::radians(90.0f), (float) width / (float)height, 0.1f, 100.0f);
    glm::mat4 Projection = glm::ortho(-8.0f, 8.0f, -8.0f, 8.0f, -0.1f, 100.0f);

    glm::mat4 View = glm::lookAt(
        glm::vec3(15,0,0), // Camera is at (4,3,3), in World Space
        glm::vec3(0,0,0), // and looks at the origin
        glm::vec3(0,0,1)  // Head is up (set to 0,-1,0 to look upside-down)
    ); 

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 mvp;
//魔方信息**************************************************************************************
    vector<vector<float>> cub_pos(27);
    int k = 0;
    for (float x = -3; x <=3; x+=3){
        for (float y = -3; y <=3; y+=3){
            for (float z = -3; z <=3; z+=3){
                cub_pos[k].push_back(x);
                cub_pos[k].push_back(y);
                cub_pos[k].push_back(z);
                // cout<<cub_pos[k][0]<<" "<<cub_pos[k][1]<<" "<<cub_pos[k][2]<<" "<<endl;
                k++;
            }
        }
    }
    vector<glm::mat4> cub_color(27);
    vector<glm::mat4> cub_self(27);
    
    for (int i=0; i<27; i++){
        cub_color[i] = glm::mat4(1.0f);
        cub_self[i] = glm::mat4(1.0f);
    }

//××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(1.0f, 1.0f, 0.9803f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        if(view_flag){
            View = glm::rotate(View, glm::radians(view_rotate), view_rotate_dir);   //旋转视角以观察
            view_flag = false;
        }

        int change_finish = 0;

        for (int i=0; i<27; i++) {        
            unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
            glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(cub_pos[i][0]*0.8, cub_pos[i][1]*0.8, cub_pos[i][2]*0.8));

            if (cub_right > 0 && cub_pos[i][1] == 3.0f ){
                cub_right -= v;
                cub_color[i] = glm::rotate(cub_color[i], glm::radians(v), glm::vec3(0.0f, 1.0f, 0.0f));
                // cout<<"D"<<endl;
                if (cub_right < v*9.0f){
                    change_finish ++;
                    float ftmp = cub_pos[i][0];
                    cub_pos[i][0] = cub_pos[i][2];
                    cub_pos[i][2] = -ftmp;
                    cub_self[i] =  cub_color[i] * cub_self[i];
                    cub_color[i] = glm::mat4(1.0f);
                    // cout<<i<<endl;
                }
            }
            else if (cub_col > 0 && cub_pos[i][1] == 0.0f){
                cub_col -= v;
                cub_color[i] = glm::rotate(cub_color[i], glm::radians(v), glm::vec3(0.0f, 1.0f, 0.0f));
                // cout<<"S"<<endl;
                if (cub_col < v*9.0f){
                    change_finish ++;
                    float ftmp = cub_pos[i][0];
                    cub_pos[i][0] = cub_pos[i][2];
                    cub_pos[i][2] = -ftmp;
                    cub_self[i] = cub_color[i] * cub_self[i];
                    cub_color[i] = glm::mat4(1.0f);
                }
            }
            else if (cub_left > 0 && cub_pos[i][1] == -3.0f){
                cub_left -= v;
                cub_color[i] = glm::rotate(cub_color[i], glm::radians(v), glm::vec3(0.0f, 1.0f, 0.0f));
                // cout<<"A"<<endl;
                if (cub_left < v*9.0f){
                    change_finish ++;
                    float ftmp = cub_pos[i][0];
                    cub_pos[i][0] = cub_pos[i][2];
                    cub_pos[i][2] = -ftmp;
                    cub_self[i] = cub_color[i] * cub_self[i];
                    cub_color[i] = glm::mat4(1.0f);
                }
            }

            else if (cub_up > 0 && cub_pos[i][2] == 3.0f){
                cub_up -= v;
                cub_color[i] = glm::rotate(cub_color[i], glm::radians(v), glm::vec3(0.0f, 0.0f, 1.0f));
                // cout<<"Y"<<endl;
                if (cub_up < v*9.0f){
                    change_finish ++;
                    float ftmp = cub_pos[i][0];
                    cub_pos[i][0] = -cub_pos[i][1];
                    cub_pos[i][1] = ftmp;
                    cub_self[i] = cub_color[i] * cub_self[i];
                    cub_color[i] = glm::mat4(1.0f);
                }
            }
            else if (cub_row > 0 && cub_pos[i][2] == 0.0f){
                cub_row -= v;
                cub_color[i] = glm::rotate(cub_color[i], glm::radians(v), glm::vec3(0.0f, 0.0f, 1.0f));
                // cout<<"H"<<endl;
                if (cub_row < v*9.0f){
                    change_finish ++;
                    float ftmp = cub_pos[i][0];
                    cub_pos[i][0] = -cub_pos[i][1];
                    cub_pos[i][1] = ftmp;
                    cub_self[i] = cub_color[i] * cub_self[i];
                    cub_color[i] = glm::mat4(1.0f);
                }
            }
            else if (cub_down > 0 && cub_pos[i][2] == -3.0f){
                cub_down -= v;
                cub_color[i] = glm::rotate(cub_color[i], glm::radians(v), glm::vec3(0.0f, 0.0f, 1.0f));
                // cout<<"N"<<endl;
                if (cub_down < v*9.0f){
                    change_finish ++;
                    float ftmp = cub_pos[i][0];
                    cub_pos[i][0] = -cub_pos[i][1];
                    cub_pos[i][1] = ftmp;
                    cub_self[i] = cub_color[i] * cub_self[i];
                    cub_color[i] = glm::mat4(1.0f);
                }
            }

            else if (cub_face > 0 && cub_pos[i][0] == 3.0f){
                cub_face -= v;
                cub_color[i] = glm::rotate(cub_color[i], glm::radians(v), glm::vec3(1.0f, 0.0f, 0.0f));
                // cout<<"SPACE"<<endl;
                if (cub_face < v*9.0f){
                    change_finish ++;
                    float ftmp = cub_pos[i][1];
                    cub_pos[i][1] = -cub_pos[i][2];
                    cub_pos[i][2] = ftmp;
                    cub_self[i] = cub_color[i] * cub_self[i];
                    cub_color[i] = glm::mat4(1.0f);
                }
            }

            mvp = Projection * View * cub_color[i] * trans * cub_self[i];
            // mvp *= glm::translate(model, glm::vec3(i*3, 0, 0));
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(mvp));

            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36); // we use index buffer, so set it to null.  
        }
        if (change_finish == 9){
            lock = true;
        }

        glfwSwapBuffers(window);
    }
//收尾
    glfwTerminate();  
    return 0;  

}  
// Is called whenever a key is pressed/released via GLFW  
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)  
{  
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)  {
        glfwSetWindowShouldClose(window, GL_TRUE);  
    }

    else if (key == GLFW_KEY_A && action == GLFW_PRESS && lock)  {
        lock = false;
        cub_left = 810.0f;
    }
    else if (key == GLFW_KEY_S && action == GLFW_PRESS && lock)  {
        lock = false;
        cub_col = 810.0f;
    }
    else if (key == GLFW_KEY_D && action == GLFW_PRESS && lock)  {
        lock = false;
        cub_right = 810.0f;
    }

    else if (key == GLFW_KEY_Y && action == GLFW_PRESS && lock)  {
        lock = false;
        cub_up = 810.0f;
    }
    else if (key == GLFW_KEY_H && action == GLFW_PRESS && lock)  {
        lock = false;
        cub_row = 810.0f;
    }
    else if (key == GLFW_KEY_N && action == GLFW_PRESS && lock)  {
        lock = false;
        cub_down = 810.0f;
    }

    else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS && lock){
        lock = false;
        cub_face = 810.0f;
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




void generateCubic(vector<float> &verx, vector<float> &colorGroup, vector<vector<float>> &color){
        vector<unsigned int> inds;
        float temp[] = {
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
    };
    vector<float> tmpv;
    for (int i=0; i<24 ; i++){
        tmpv.push_back(temp[i]);
    }
    //前
    inds.push_back(6);
    inds.push_back(7);
    inds.push_back(2);
    inds.push_back(2);
    inds.push_back(7);
    inds.push_back(3);
    //上
    inds.push_back(2);
    inds.push_back(3);
    inds.push_back(0);
    inds.push_back(0);
    inds.push_back(3);
    inds.push_back(1);
    //下
    inds.push_back(4);
    inds.push_back(5);
    inds.push_back(6);
    inds.push_back(6);
    inds.push_back(5);
    inds.push_back(7);
    //左
    inds.push_back(4);
    inds.push_back(6);
    inds.push_back(0);
    inds.push_back(0);
    inds.push_back(6);
    inds.push_back(2);
    //右
    inds.push_back(7);
    inds.push_back(5);
    inds.push_back(3);
    inds.push_back(3);
    inds.push_back(5);
    inds.push_back(1);
    //后
    inds.push_back(5);
    inds.push_back(4);
    inds.push_back(1);
    inds.push_back(1);
    inds.push_back(4);
    inds.push_back(0);
    for (int i=0; i<36; i++){
        verx.push_back(tmpv[inds[i]*3]);
        verx.push_back(tmpv[inds[i]*3+1]);
        verx.push_back(tmpv[inds[i]*3+2]);
        colorGroup.push_back(color[i/6][0]);
        colorGroup.push_back(color[i/6][1]);
        colorGroup.push_back(color[i/6][2]);
    }
    // for (int i=0; i<36; i++){
    //     cout << verx[i*3] << verx[i*3+1] << verx[i*3+2]<<"Color: "<<colorGroup[i*3]<<" "<<colorGroup[i*3+1]<<" "<<colorGroup[i*3+2]<<endl;
    // }

    // cout<<verx.size()<<" "<<colorGroup.size();
}