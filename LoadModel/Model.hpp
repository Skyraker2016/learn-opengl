#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
using namespace std; 
#include "Mesh.hpp"

class Model 
{
public:
    Model(GLchar* path)
    {
        this->loadModel(path);
    }

    void Draw(GLenum style, int size)
    {
        //遍历mesh绘制
        for(GLuint i = 0; i < this->meshes.size(); i++){
            this->meshes[i].Draw(style, size);
        }
    }
    
private:
    vector<Mesh> meshes;

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

  
};


