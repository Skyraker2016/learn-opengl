
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

## 实现思路
### 安装与连接assimp
Ubuntu下安装assimp库：
```
sudo apt-get install libassimp-dev
sudo apt-get install assimp-utils

```
编译链接
```
g++ ... -lassimp
```

### 模型类
模型类的思路参考[learningopengl](https://learnopengl.com/Model-Loading/Assimp)，但去掉了纹理相关（还未开始学习），以及将绘制相关的绑定缓冲区、自定义着色器改为`glut`实现。
1. 数据结构:
    ```cpp
    //模型类——包含meshes
    class Model 
        vector<Mesh> meshes;
    
    //Mesh类——包含顶点及面索引
    class Mesh
        vector<Vertex> vertices;
        vector<GLuint> indices;
    
    //顶点，包含坐标及法向量
    struct Vertex
    {
        vector<GLfloat> pos;
        vector<GLfloat> nor;
    };


    ```
3. 加载:
    1. 读入一个模型文件
        ```cpp
        void loadModel(string path)
        {
            Assimp::Importer importer;
            const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals);
            //模型文件path为路径
            //aiProcess_Triangulate 使文件以三角形作为图元生成
            //aiProcess_GenSmoothNormals 为没有法向量的顶点设置法向量

            //加载错误，反馈信息
            if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
            {
                cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
                return;
            }

            //进行递归遍历节点
            this->processNode(scene->mRootNode, scene);
        }
        ```
    1. 通过递归遍历模型的节点及mesh：
        ```cpp
        void processNode(aiNode* node, const aiScene* scene)
        {
            // 便利节点上的每个mesh
            for(GLuint i = 0; i < node->mNumMeshes; i++)
            {
                //加载mesh
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]]; 
                this->meshes.push_back(this->processMesh(mesh, scene));			
            }

            //遍历子节点
            for(GLuint i = 0; i < node->mNumChildren; i++)
            {
                this->processNode(node->mChildren[i], scene);
            }

        }
        ```
    3. 读取mesh的每个顶点：
        ```cpp

        Mesh processMesh(aiMesh* mesh, const aiScene* scene)
        {
            // Data to fill
            vector<Vertex> vertices;
            vector<GLuint> indices;

            // Walk through each of the mesh's vertices
            for(GLuint i = 0; i < mesh->mNumVertices; i++)
            {
                vector<GLfloat> vertex(3);
                vertex[0] = mesh->mVertices[i].x;
                vertex[1] = mesh->mVertices[i].y;
                vertex[2] = mesh->mVertices[i].z;
                Vertex tmp;
                tmp.pos = vertex;
                vertex[0] = mesh->mNormals[i].x;
                vertex[1] = mesh->mNormals[i].y;
                vertex[2] = mesh->mNormals[i].z;
                tmp.nor = vertex;  
                vertices.push_back(tmp);
            }
            // Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
            for(GLuint i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                // Retrieve all indices of the face and store them in the indices vector
                for(GLuint j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);
            }
            
            // Return a mesh object created from the extracted mesh data
            return Mesh(vertices, indices);
        }
        ```
### 绘制
1. 函数实现
    ```cpp
    //Mesh类中
    void Draw(GLenum style, int size) 
    {
        //遍历三角面
        for (int i=0; i<indices.size(); i+=3){
            //style为绘制模式，GL_LINE_LOOP为线框，GL_TRIANGLES为三角面
            glBegin(style);
                //Normal为法线，用于光照运算
                //vertex为顶点坐标
                //size为放缩的倍数，初始为400，用于滚轮缩放
                glNormal3f(vertices[indices[i]].nor[0]*size, vertices[indices[i]].nor[1]*size, vertices[indices[i]].nor[2]*size);
                glVertex3f(vertices[indices[i]].pos[0]*size, vertices[indices[i]].pos[1]*size, vertices[indices[i]].pos[2]*size);
                glNormal3f(vertices[indices[i+1]].nor[0]*size, vertices[indices[i+1]].nor[1]*size, vertices[indices[i+1]].nor[2]*size);
                glVertex3f(vertices[indices[i+1]].pos[0]*size, vertices[indices[i+1]].pos[1]*size, vertices[indices[i+1]].pos[2]*size);
                glNormal3f(vertices[indices[i+2]].nor[0]*size, vertices[indices[i+2]].nor[1]*size, vertices[indices[i+2]].nor[2]*size);
                glVertex3f(vertices[indices[i+2]].pos[0]*size, vertices[indices[i+2]].pos[1]*size, vertices[indices[i+2]].pos[2]*size);
            glEnd();
        }
    }
    ```
    ```cpp
    //Model类中
    void Draw(GLenum style, int size)
    {
        //遍历mesh绘制
        for(GLuint i = 0; i < this->meshes.size(); i++){
            this->meshes[i].Draw(style, size);
        }
    }
    ```
2. 主循环中绘制
    ```cpp
    void display(Model &test){
        //清缓存
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //保存上一个变换矩阵，本项目中不用
        // glPushMatrix();

        //加载单位矩阵
        glLoadIdentity();
        //透视矩阵
        gluPerspective(60.0, 1, 400, -400);
        //摄像矩阵
        gluLookAt(0, 0, 800, 0, 0, 0, 0, 1, 0);
        //鼠标拖拽时进行相应的移动
        if (clicking){
            tx -= moveX;
            ty += moveY;
        }
        moveX = moveY = 0;
        //平移
        glTranslatef(tx, ty, 0.0f);
        //旋转
        glRotatef(angle_ud,1,0,0);
        glRotatef(angle_rl,0,1,0);
        //以上矩阵生效顺序是从下至上

        //设置默认颜色
        glColor3f(1.0, 1.0, 1.0);
        //根据模式来进行绘制
        switch(sstyle){
            case 0: //wire
                glLineWidth(1.0f);
                test.Draw(GL_LINE_STRIP,ssize);
                break;
            case 1: //flat
                test.Draw(GL_TRIANGLES,ssize);
                break;
            case 2: //flat lines
                glColor3f(0.8, 0.8, 0.8);
                test.Draw(GL_TRIANGLES,ssize);
                glLineWidth(1.5f);
                glColor3f(0.2, 0.2, 0.2);
                test.Draw(GL_LINE_STRIP,ssize);
                break;
        }

        // glPopMatrix();
    }
    ```

### 交互：
1. 注册事件：
    ```cpp
    void setEvents(GLFWwindow * window){
        glfwSetKeyCallback(window, key_callback);   //键盘事件
        glfwSetMouseButtonCallback(window, mouse_click_callback);   //鼠标按键
        glfwSetCursorPosCallback(window, mouse_move_callback);  //鼠标移动
        glfwSetScrollCallback(window, scroll_callback); //鼠标滚轮
    }
    ```
1. 键盘——旋转、切换：
    ```cpp
    //键盘事件回调函数
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)  
    {  
        //退出
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)  {  
            glfwSetWindowShouldClose(window, GL_TRUE);  
        }
        //旋转
        else if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT){   
            angle_rl -= 1;
        }
        else if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT){
            angle_rl += 1;
        }
        else if (key == GLFW_KEY_UP && action == GLFW_REPEAT){
            angle_ud -= 1;
        }
        else if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT){
            angle_ud += 1;
        }
        //切换style
        else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS){
            sstyle = (sstyle+1)%3;
        }
        
    }  
    ```
    ```cpp
    //旋转相关变换

    //旋转全局数据
    GLfloat angle_rl = 0;
    GLfloat angle_ud = 0;

    ...

    //in display()
    glRotatef(angle_ud,1,0,0);
    glRotatef(angle_rl,0,1,0);
    ```
    ```cpp
    //切换style

    //style全局数据
    int sstyle = 0;

    ...

    //in display()
    switch(sstyle){
        case 0: //wire
            glLineWidth(1.0f);
            test.Draw(GL_LINE_STRIP,ssize);
            break;
        case 1: //flat
            test.Draw(GL_TRIANGLES,ssize);
            break;
        case 2: //flat lines
            glColor3f(0.8, 0.8, 0.8);
            test.Draw(GL_TRIANGLES,ssize);
            glLineWidth(1.5f);
            glColor3f(0.2, 0.2, 0.2);
            test.Draw(GL_LINE_STRIP,ssize);
            break;
    }
    ```
1. 鼠标——拖拽、缩放：
    1. 拖拽
        ```cpp
        //全局鼠标坐标数据
        //平移向量
        GLfloat tx = 0;
        GLfloat ty = 0;
        //移动速度
        GLfloat moveX = 0;
        GLfloat moveY = 0;
        //鼠标位置
        GLfloat mouseX = 0;
        GLfloat mouseY = 0;
        //是否正在拖拽
        bool clicking = false;
        ```
        ```cpp
        //回调函数
        //实时记录鼠标位置及移动趋势
        void mouse_move_callback(GLFWwindow* window, double xpos, double ypos){
            //记录鼠标移动趋势——通过当前坐标与上一坐标计算
            moveX = mouseX - xpos;
            moveY = mouseY - ypos;
            //记录鼠标当前坐标
            mouseX = xpos;
            mouseY = ypos;
        }
        //通知全局鼠标是否按下（即拖拽中）
        void mouse_click_callback(GLFWwindow* window, int button, int action, int mods){
            if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
                clicking = true;
            }
            if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
                clicking = false;
            }
        }
        ```
        ```cpp
        // in display()
        //鼠标拖拽时进行相应的移动
        if (clicking){
            tx -= moveX;
            ty += moveY;
        }
        // 将运动趋势归零防止惯性
        moveX = moveY = 0;
        //平移变换
        glTranslatef(tx, ty, 0.0f);
        ```
    2. 滚轮缩放
        ```cpp
        // 全局数据
        // 缩放倍数
        int ssize = WIDTH/2;
        ```
        ```cpp
        // 回调函数
        void scroll_callback (GLFWwindow* window, double xoffset, double yoffset)
        {
            //yoffset 是滚轮转动的程度
            ssize += 5*yoffset;
        }
        ```
        ```cpp
        //in Mesh class
        //先前得到的ssize交给draw方法进行顶点坐标的放缩并绘制
        void Draw(GLenum style, int size) 
        {
            //遍历三角面
            for (int i=0; i<indices.size(); i+=3){
                //style为绘制模式，GL_LINE_LOOP为线框，GL_TRIANGLES为三角面
                glBegin(style);
                    //Normal为法线，用于光照运算
                    //vertex为顶点坐标
                    //size为放缩的背书，初始为400，用于滚轮缩放
                    glNormal3f(vertices[indices[i]].nor[0]*size, vertices[indices[i]].nor[1]*size, vertices[indices[i]].nor[2]*size);
                    glVertex3f(vertices[indices[i]].pos[0]*size, vertices[indices[i]].pos[1]*size, vertices[indices[i]].pos[2]*size);
                    glNormal3f(vertices[indices[i+1]].nor[0]*size, vertices[indices[i+1]].nor[1]*size, vertices[indices[i+1]].nor[2]*size);
                    glVertex3f(vertices[indices[i+1]].pos[0]*size, vertices[indices[i+1]].pos[1]*size, vertices[indices[i+1]].pos[2]*size);
                    glNormal3f(vertices[indices[i+2]].nor[0]*size, vertices[indices[i+2]].nor[1]*size, vertices[indices[i+2]].nor[2]*size);
                    glVertex3f(vertices[indices[i+2]].pos[0]*size, vertices[indices[i+2]].pos[1]*size, vertices[indices[i+2]].pos[2]*size);
                glEnd();
            }
        }
        ```

### 光照：
若如果没有光照，flat模式就会变成一片剪影（如图），因此特意为其添加光照效果

![无光照](https://raw.githubusercontent.com/Skyraker2016/markdownpic/master/loadmodel-nolight.png)

```cpp
//光照设置
// 激活光照计算
glEnable(GL_LIGHTING);

GLfloat light_position[] = { -1.0, 0.3, 0, 0.0 };   //光源位置
glLightfv(GL_LIGHT0,  GL_POSITION,  light_position);    //设置光源位置
GLfloat light_color[] = { 0.05, 0.05, 0.05, 1.0 }; //光源颜色
glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);     //设置光源颜色
//激活以上设置的0号光源
glEnable(GL_LIGHT0);     

// 指定环境光强度（RGBA）
GLfloat ambientLight[] = {0.4f, 0.4f, 0.4f, 1.0f};
// 设置光照模型，将ambientLight所指定的RGBA强度值应用到环境光
glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

// 启用颜色追踪，为了能够在光照反射时能反应颜色
glEnable(GL_COLOR_MATERIAL);
// 设置多边形正面的环境光和散射光材料属性，追踪glColor
glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

```

得到有光照模型：

![有光照](https://raw.githubusercontent.com/Skyraker2016/markdownpic/master/loadmodel-light.png)


## [完整代码]()

## 参考链接
* [assimp基础及模型类参考](https://learnopengl.com/Model-Loading/Assimp)
* [光照设置](https://www.cnblogs.com/kekec/archive/2011/08/16/2140789.html)
* [顶点法线的光照意义](https://blog.csdn.net/u014800094/article/details/53126874)
* [glBegin()](https://www.cnblogs.com/yemeishu/archive/2012/01/03/2310814.html)
* [glut的旋转变换](https://www.cnblogs.com/1024Planet/p/5647224.html)
* [glut的平移变换](https://www.cnblogs.com/1024Planet/p/5647547.html)
* [滚轮事件](https://www.jianshu.com/p/2d9c162d6234)
* [斯坦福bunny兔模型下载](https://www.cc.gatech.edu/projects/large_models/)
