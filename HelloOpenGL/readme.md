# VSCode + GLFW + GLEW 在ubuntu下的安装
## 环境说明
* 开发环境：Linux
* 运行环境：Linux
* 运行方式：
    * 首先在`/HelloOpenGL`文件夹（本文件夹）下打开终端，用`export LD_LIBRARY_PATH=./../dll`，增加动态库搜索路径
    * 运行可执行文件`./helloOpenGL`
    * 若失败，则按照GLFW、GLEW的配置方法配置好环境后在本机重新编译
* 使用库：GLFW、GLEW、GLM
## 安装
### OpenGL（包括GLUT）：
```
sudo apt-get install build-essential libgl1-mesa-dev
sudo apt-get install freeglut3-dev
sudo apt-get install libglew-dev libsdl2-dev ibsdl2-image-dev libglm-dev libfreetype6-dev
```
### GLFW：
1. 去[官网](http://www.glfw.org/download.html)下载源码
2. 编译安装：

    * 安装cmake: `sudo apt-get install cmake`
    * 解压刚刚下载的源码: `sudo unzip xxx.zip -d glfw`，其中`xxx`为你刚刚下载的压缩包名
    * 进入解压后的GLFW根目录（含有include、CMake的那个），进行安装: 
    ```
    sudo cmake .
    sudo make
    sudo make install
    ```
### GLEW：
* 查看glew：`sudo apt-cache search glew`
* 将显示的内容用`sudo apt-get install xxx`下载下来，其中`xxx`为要下载的内容

## 编译

```
g++ -o helloOpenGL helloOpenGL.cpp -lGL -lGLU -lglut -lglfw3 -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lpthread -lGLEW -ldl
```
## 实例源码
```cpp
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
```
## 效果
![helloOpenGL-result](https://raw.githubusercontent.com/Skyraker2016/markdownpic/master/Screenshot%20from%202018-05-07%2016-17-27.png)
## 注意事项
1. GLEW应该在GLFW、GLUT等之前include。
2. 要加上-ldl，否则会出现错误：
```
/usr/bin/x86_64-linux-gnu-ld: //usr/local/lib/libglfw3.a(vulkan.c.o): undefined reference to symbol 'dlclose@@GLIBC_2.2.5'
//lib/x86_64-linux-gnu/libdl.so.2: error adding symbols: DSO missing from command line
```
3. 由于很多教程基于GLUT，又有很多教程基于GLFW、GLEW，因此编译连接了大部分可能用到的库

4. 若不调用GLEW，很有可能造成shader的许多问题

## 参考来源：
[关于GLEW的错误](https://stackoverflow.com/questions/26372359/error-loading-shared-library-glew)

[GLFW的安装](http://www.kennyspace.com/wordpress/computer-graphics/glfw-config.html)

[GLEW的安装](https://blog.csdn.net/seamanj/article/details/50178607)

[OpenGL下载安装+GLEW的include顺序问题](https://blog.csdn.net/karizhang/article/details/51434398)

[OpenGL环境配置](https://www.jianshu.com/p/e4a90503d4a6)

[渲染器](https://blog.csdn.net/qq_28637193/article/details/52395945)

[绘制demo](https://www.cnblogs.com/graphics/p/3661500.html)