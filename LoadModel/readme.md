
# 使用assimp库进行模型文件的加载与操作
## 环境说明
* 开发环境: Linux
* 库: assimp， glew， glfw， glut
* 运行环境: Linux
* 编译指令
    ```
    g++ -o loadModel loadModel.cpp -lGL -lGLU -lglut -lglfw3 -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lpthread -lGLEW -ldl -lassimp
    ```
* 文件说明
    ```
    .
    ├── bunny.ply           #兔子模型 
    ├── cow.obj             #奶牛模型
    ├── loadModel           #可执行文件
    ├── loadModel.cpp       #主程序
    ├── Mesh.hpp            #Mesh库
    ├── Model.hpp           #Model库
    └── readme.md           
    ```

## 功能说明
可加载常用的模型文件(obj, ply ...)，并能进行平移、缩放与旋转，支持三种查看模式——wire、flat、flat lines

### 运行方法
将可执行文件与待加载的模型文件（obj、ply等）放在同一目录下，linux命令行下运行：
```
./loadModel name
```
其中`name`为模型文件完整文件名

### 操作说明
* 空格：切换查看模式`wire`, `flat`, `flat lines`
* 鼠标滚轮：缩放模型
* 鼠标拖拽：移动模型
* 方向键：旋转模型

## 运行效果

![旋转](https://raw.githubusercontent.com/Skyraker2016/markdownpic/master/loadmodel-rotating.gif)

**旋转**

![切换显示模式](https://raw.githubusercontent.com/Skyraker2016/markdownpic/master/loadmodel-style.gif)

**切换显示模式**

![平移](https://raw.githubusercontent.com/Skyraker2016/markdownpic/master/loadmodel-move.gif)

**平移**

![缩放](https://raw.githubusercontent.com/Skyraker2016/markdownpic/master/loadmodel-size.gif)

**缩放**

![加载ply](https://raw.githubusercontent.com/Skyraker2016/markdownpic/master/loadmodel-bunny.gif)

**加载ply格式的bunny**


*具体实现思路见博客[基于assimp库的模型加载](https://skyraker2016.github.io/2018/06/04/%E5%9F%BA%E4%BA%8Eassimp%E5%BA%93%E7%9A%84%E6%A8%A1%E5%9E%8B%E5%8A%A0%E8%BD%BD/)*