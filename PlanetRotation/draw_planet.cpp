#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp> // glm::pi  
using namespace std; 

void generatePlanetTriangle(vector<float>& vects, vector<unsigned int>& inds, float x=0, float y=0, float z=0, float radius=1, int VerSlide=40, int HorSlide=20){
    float ver_step = 2 * glm::pi<float>() / (float)VerSlide;    //经度（横）方向步长
    float hor_step = glm::pi<float>() / (float)HorSlide;        //纬度（纵）方向步长

    unsigned int index = 0;

    //遍历所有顶点，生成顶点坐标、顶点索引
    for (unsigned int i=0; i <= HorSlide; i++){
        float hor_angle = hor_step * i;
        float z_cur = radius * cos(hor_angle);   //z原点坐标
        float r_cur = radius * sin(hor_angle);   //纬度平面半径

        for (unsigned int j=0; j <= VerSlide; j++){
            float ver_angle = j*ver_step;
            float x_cur = r_cur * cos(ver_angle);   //x原点坐标
            float y_cur = r_cur * sin(ver_angle);   //y原点坐标

            //根据原点坐标，进行平移
            // x_cur += x;
            // y_cur += y;
            // z_cur += z;
            // cout<<x_cur<<" "<<y_cur<<" "<<z_cur<<endl;
            //顶点入vector 
            vects.push_back(x_cur+x);
            vects.push_back(y_cur+y);
            vects.push_back(z_cur+z);

            //插入球面三角形顶点索引
            if (i > 0 && j > 0){
                //若以自我为一个四边形的右下角，其绘制曲面三角的点：
                unsigned int ru = index - VerSlide;
                unsigned int lu = index - VerSlide - 1;
                unsigned int ld = index -1;
                unsigned int rd = index;
                //上或下一圈为一个点，添加的为三角形
                if (i == 1){               
                    inds.push_back(ru);
                    inds.push_back(ld);
                    inds.push_back(rd);
                }
                else if (i == HorSlide){
                    inds.push_back(ru);
                    inds.push_back(lu);
                    inds.push_back(rd);
                }
                //四边行情况——添加两个三角形
                else{
                    inds.push_back(ru);
                    inds.push_back(ld);
                    inds.push_back(rd);
                    inds.push_back(ru);
                    inds.push_back(lu);
                    inds.push_back(ld);
                }
            }
            index += 1;

        }
    }

}
