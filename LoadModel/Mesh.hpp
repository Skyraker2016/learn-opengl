#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
using namespace std; 

struct Vertex
{
    vector<GLfloat> pos;
    vector<GLfloat> nor;
};

class Mesh {
public:
    
    vector<Vertex> vertices;
    vector<GLuint> indices;
    Mesh(vector<Vertex> vertices, vector<GLuint> indices)
    {
        this->vertices = vertices;
        this->indices = indices;
    }

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

};
