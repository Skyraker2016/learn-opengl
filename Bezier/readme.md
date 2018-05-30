## 环境说明
* 开发环境：Linux
* 运行环境：Linux
* 运行方式：
    * 首先在`/Bezier`文件夹（本文件夹）下打开终端，用`export LD_LIBRARY_PATH=./../dll`，增加动态库搜索路径
    * 运行可执行文件`./Bezier`
    * 若失败，则按照GLFW、GLEW的配置方法配置好环境后在本机重新编译
* 使用库：GLFW、GLEW、GLM
* 编译参数：
```
g++ -o Bezier Bezier.cpp -lGL -lGLU -lglut -lglfw3 -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lpthread -lGLEW -ldl
```

## 功能说明：
* 鼠标左键点击，选择四个点绘制成相应的bezier三次曲线
* 回车键保留该曲线，并去除草稿线
* 退格键删除上一条曲线

## 效果演示：

![绘制一条曲线](https://raw.githubusercontent.com/Skyraker2016/markdownpic/master/bezier_add.gif)
*绘制一条曲线*

![删除曲线](https://raw.githubusercontent.com/Skyraker2016/markdownpic/master/bezier_del.gif)
*删除曲线*

![rabbit](https://raw.githubusercontent.com/Skyraker2016/markdownpic/master/bezier_rabbit.gif)
*兔子*


## 知识点
1. 获取鼠标位置：鼠标移动回调函数：
    ```cpp
    void mouse_move_callback(GLFWwindow* window, double xpos, double ypos){
        mouseX = xpos;
        mouseY = ypos;
    }
    ```
1. Bezier公式：
    $$ B(t) = P_0(1-t)^3 + 3P_1t(1-t)^2+3P_2t^2(1-t)+P_3t^3, t \in [0,1] $$
    具体实现：
    ```cpp
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
    ```

    ## [源代码]()

    ## 参考
    [绘制bezier曲线](https://blog.csdn.net/qa1041335855/article/details/72083760)