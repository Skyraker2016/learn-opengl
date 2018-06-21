# 光照+茶壶显示
* 运行环境：Linux
* 开发环境：Linux，OpneGL

# 功能
* 使用GLUT的接口，设置了平行光进行光照:
    ```cpp
        //光照设置
    // 激活光照计算
    glEnable(GL_LIGHTING);

    GLfloat light_position[] = { 1, 1, -1, 0.0 };   //光源位置
    GLfloat light_color[] = { 0.7, 0.7, 0.7, 1.0 }; //光源颜色
    glLightfv(GL_LIGHT0,  GL_POSITION,  light_position);    //设置光源位置
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_color);     //设置光源颜色
    // 激活以上设置的0号光源
    glEnable(GL_LIGHT0);     

    GLfloat lightDir[] = {1000, -1000, -1000};
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lightDir);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, -1);
    glLightf(GL_LIGHT1,GL_SPOT_CUTOFF,90);
    glEnable(GL_LIGHT1);


    ```
* 使用内置的`glutSolidTeapot`函数绘制茶壶
* 使用鼠标、方向键，可以进行平移、放缩
