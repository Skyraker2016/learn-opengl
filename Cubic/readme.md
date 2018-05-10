# 彩色旋转立方体

## 环境说明
* 开发环境：Linux
* 运行环境：Linux
* 运行方式：
    * 首先在`/Cubic`文件夹（本文件夹）下打开终端，用`export LD_LIBRARY_PATH=./../dll`，增加动态库搜索路径
    * 运行可执行文件`./cubic`
    * 若失败，则按照GLFW、GLEW的配置方法配置好环境后在本机重新编译
* 使用库：GLFW、GLEW、GLM

## 知识点
1. 矩阵变换
    * 矩阵的变换分为三种：放缩、旋转、平移，三者顺序不应混乱，用矩阵乘法表示：
    ```cpp
        glm::mat4 mvp = Projection * View * model;
    ```
    * 在本次实验中，使用的是透视视角，即近大远小，使用projection函数可以生成透视变换视角，属于放缩变换：
    ```
    glm::mat4 Projection = glm::perspective(glm::radians(90.0f), (float) width / (float)height, 0.1f, 100.0f);
    ```
    * 为了让矩阵旋转，我们首先预设摄像机位置：
    ```cpp
    glm::mat4 View = glm::lookAt(
        glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
        glm::vec3(0,0,0), // and looks at the origin
        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    ); 
    ```
    再于每次渲染改变角度（旋转矩阵）：
    ```cpp
        View = glm::rotate(View, glm::radians(1.0f), glm::vec3(-1.0f, -1.0f, 1.0f)); 
    ```
3. 着色器：
    * 着色器分类：本次实验中用到像素着色器与顶点着色器，顶点着色器用于将外部的顶点信息打包给其他着色器绘制，像素着色器用于染色。
    * 着色器使用`GLSL`语言进行编写，并在C++中用字符串形式进行输入：
    ```cpp
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
    ```
    * 着色器的编译与测试：
    ```cpp
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
    ```
    * 着色器链接：
    ```cpp
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);  
    if (!success) {  
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);  
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;  
    }  
    ```
    * 着色器参数属性：
    ```cpp
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    ```
    * 将变换矩阵传入着色器的uniform变量中：
    ```cpp
        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(mvp));

    ```
4. 深度测试：若不开启深度测试，会导致着色器无法判断覆盖信息，深度测试的用途是用于舍弃被遮盖住的图像，使其更加真实：
```cpp
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
```

## [源码](https://github.com/Skyraker2016/learn-opengl/blob/master/Cubic/cubic.cpp)


## 结果
![cubic.gif](https://raw.githubusercontent.com/Skyraker2016/markdownpic/master/cubic.gif)


## 注意事项
* 本环境中未配置glad，而使用glew，在代码开始时注意此初始化顺序：
    ```cpp
    glfwInit();//初始化  
    // Set all the required options for GLFW   
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//设置主版本号  
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//设置次版本号  
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);  

    // Create a GLFWwindow object that we can use for GLFW's functions  创建一个窗口  
    GLFWwindow* window = glfwCreateWindow(800, 600, "The perspective projection", NULL, NULL);  
    glfwMakeContextCurrent(window);//设置为当前窗口  

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions  
    glewExperimental = GL_TRUE;  
    // Initialize GLEW to setup the OpenGL Function pointers  
    glewInit();  
    ```
    否则会导致此错误：
    ```
    Segmentation fault (core dumped)
    ```
2. 若提示`./cubic command not found`则先`chmod u+x cubic`
3. 在未配置opengl环境的电脑上运行时可能缺少动态链接库，因此将动态链接库打包于项目文件中的`dll`文件夹，使用时需要用
    ```
    export LD_LIBRARY_PATH=xxx
    ```
    增加动态链接库搜索位置，其中`xxx`为`dll`文件夹。

## 参考来源
[绘制旋转立方体](https://blog.csdn.net/wodownload2/article/details/78139273)

[规范代码](https://blog.csdn.net/qq_28637193/article/details/52395945)

[透视教程](http://www.opengl-tutorial.org/cn/beginners-tutorials/tutorial-4-a-colored-cube/)

[录屏转gif](https://www.linuxidc.com/Linux/2017-12/149800.htm)

[动态库链接](https://blog.csdn.net/hktkfly6/article/details/61922685)