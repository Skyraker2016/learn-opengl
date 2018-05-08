
## 结果
![cubic.gif](https://raw.githubusercontent.com/Skyraker2016/markdownpic/master/cubic.gif)


## 注意事项
1. 本环境中未配置glad，而使用glew，在代码开始时注意此初始化顺序：
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
2. 

## 参考来源
[绘制旋转立方体](https://blog.csdn.net/wodownload2/article/details/78139273)
[规范代码](https://blog.csdn.net/qq_28637193/article/details/52395945)
[透视教程](http://www.opengl-tutorial.org/cn/beginners-tutorials/tutorial-4-a-colored-cube/)
[录屏转gif](https://www.linuxidc.com/Linux/2017-12/149800.htm)