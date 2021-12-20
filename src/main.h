// This file includes: defines, includes, usings, functions and structs

#pragma once
#define _USE_MATH_DEFINES
#define GLEW_STATIC

#include "glew.h"
#include "glfw3.h"
#include "math.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cerrno>
#include <map>
#include <array>
#include <vector>
#include <Windows.h>

using namespace std;

std::string get_file_contents(const char* fileName) {
    std::ifstream in(fileName, std::ios::binary);
    if (in) {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return contents;
    }
    throw(errno);
};
struct EBO {
    GLuint id;
    GLuint* indices;
    GLsizeiptr size;
    
    EBO(GLuint* indices, GLsizeiptr size) {

        glGenBuffers(1, &id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_DYNAMIC_DRAW);
    }
    
    EBO(vector<vector<GLuint>> indices, GLsizeiptr size) {
        GLfloat indiceArray[625 * 6];
        for (int i = 0; i < indices.size(); i++) {
            indiceArray[6 * i] = indices[i][0];
            indiceArray[6 * i + 1] = indices[i][1];
            indiceArray[6 * i + 2] = indices[i][2];
            indiceArray[6 * i + 3] = indices[i][3];
            indiceArray[6 * i + 4] = indices[i][4];
            indiceArray[6 * i + 5] = indices[i][5];
        }
        
        glGenBuffers(1, &id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, &indices[0][0], GL_DYNAMIC_DRAW);
    }

    void Bind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    }

    void Unbind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void Delete() {
        glDeleteBuffers(1, &id);
    }
};

struct VBO {
    GLuint id;
    GLfloat* vertices;
    GLsizeiptr size;
    
    VBO(GLfloat* vertices, GLsizeiptr size) {
        this->vertices = vertices;
        this->size = size;
        glGenBuffers(1, &id);
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
    }
    
    VBO(array<array<GLfloat,12>,512> vertices, GLsizeiptr size) {
        glGenBuffers(1, &id);
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, size, &vertices[0][0], GL_DYNAMIC_DRAW);
    }
    void Bind() {
        glBindBuffer(GL_ARRAY_BUFFER, id);
    }

    void Unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void Delete() {
        glDeleteBuffers(1, &id);
    }
};

struct VAO {
    GLuint id;

    VAO() {
        glGenVertexArrays(1, &id);
    }

    void linkVBO(VBO vbo, GLuint layout) {
        vbo.Bind();
        glVertexAttribPointer(layout, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(layout);
        vbo.Unbind();
    }

    void Bind() {
        glBindVertexArray(id);
    }

    void Unbind() {
        glBindVertexArray(0);
    }

    void Delete() {
        glDeleteVertexArrays(1, &id);
    }
};

struct Shader {

    GLuint id;
    std::string vertexFile;
    std::string fragmentFile;

    Shader(const char* vertexFile,const char* fragmentFile) {
        this->vertexFile = get_file_contents(vertexFile);
        this->fragmentFile = get_file_contents(fragmentFile);

        const char* vertexSource = this->vertexFile.c_str();
        const char* fragmentSource = this->fragmentFile.c_str();

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexSource, NULL);
        glCompileShader(vertexShader);

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
        glCompileShader(fragmentShader);

        id = glCreateProgram();
        glAttachShader(id, vertexShader);
        glAttachShader(id, fragmentShader);
        glLinkProgram(id);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    void Activate() {
        glUseProgram(id);
    }
    void Delete(int id) {
        glDeleteProgram(id);
    }
};

struct Vector2 {
    float x, y;
    Vector2() {
        x = 0;
        y = 0;
    }
    Vector2(float x, float y) {
        this->x = x;
        this->y = y;
    }
};

struct Vector3 {
    float x, y, z;

    Vector3() {
        x = 0;
        y = 0;
        z = 0;
    }
    Vector3(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }


    void add(float x, float y, float z) {
        this->x += x;
        this->y += y;
        this->z += z;
    }
    void add(Vector3 v) {
        this->x += v.x;
        this->y += v.y;
        this->z += v.z;
    }
    Vector3 add(Vector3 v1, Vector3 v2) {
        Vector3 out = Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
        return out;
    }
    // Subtract function input from where the function is being called
    Vector3 difference(Vector3 v1) {
        Vector3 out = Vector3(this->x - v1.x, this->y - v1.y, this->z - v1.z);
        return out;
    }

    Vector3 difference(Vector3 v1, Vector3 v2) {
        Vector3 out = Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
        return out;
    }
    void scalarMult(float n) {
        this->x = x * n;
        this->y = y * n;
        this->z = z * n;
    }

    float norm() {
        float out = (float)sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
        return out;
    }
};

struct Quaternion {
    float w, x, y, z;

    Quaternion() {
        this->w = 0;
        this->x = 0;
        this->y = 0;
        this->z = 1;
    }
    Quaternion(float w, float x, float y, float z) {
        this->w = w;
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Vector3* GetUnitVectors() {
        Vector3* unitVectors = new Vector3[3];

        Vector3 zUnit = Vector3(this->x, this->y, this->z);


        float cosProject = (float)cos(M_PI * this->w);
        float sinProject = (float)sin(M_PI * this->w);
        Vector3 xUnit, yUnit;

        if (this->z != 0) {
            xUnit = Vector3(1, 0, (this->x) / this->z);
            yUnit = Vector3(0, 1, (this->y) / this->z);
        }
        else {
            xUnit = Vector3(1, 0, 0);
            yUnit = Vector3(0, 1, 0);
        }

        Vector3 xx = Vector3(xUnit);
        Vector3 xy = Vector3(yUnit);
        Vector3 yx = Vector3(xUnit);
        Vector3 yy = Vector3(yUnit);

        xx.scalarMult(cosProject);
        xy.scalarMult(sinProject);
        yx.scalarMult(sinProject);
        yy.scalarMult(cosProject);

        xUnit = xUnit.add(xx, xy);
        yUnit = yUnit.add(yx, yy);

        xUnit.scalarMult(1 / xUnit.norm());
        yUnit.scalarMult(1 / yUnit.norm());

        unitVectors[0] = xUnit;
        unitVectors[1] = yUnit;
        unitVectors[2] = zUnit;
        return unitVectors;
    }

    // Give angle in radians for a desired rotation
    // This function converts an axis and an angle into a quaternion
    Quaternion getFromVector(Vector3 v, float angle) {
        Quaternion out;
        out.w = cos(angle / 2);
        out.x = v.x * sin(angle / 2);
        out.y = v.y * sin(angle / 2);
        out.z = v.z * sin(angle / 2);

        return out;
    }

    Quaternion multiply(Quaternion q1, Quaternion q2) {
        Quaternion out;
        out.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
        out.x = q1.w * q2.x + q2.w * q1.x + q1.y * q2.z - q1.z * q2.y;
        out.y = q1.w * q2.y + q2.w * q1.y - q1.x * q2.z + q1.z * q2.x;
        out.z = q1.w * q2.z + q2.w * q1.z + q1.x * q2.y - q1.y * q2.x;
        return out;
    }

    Quaternion conjugate() {
        Quaternion out;
        out.w = this->w;
        out.x = -this->x;
        out.y = -this->y;
        out.z = -this->z;
        return out;
    }
    Quaternion getVectorQuaternion() {
        Quaternion out;
        out.w = 0;
        out.x = this->x;
        out.y = this->y;
        out.z = this->z;
        return out;
    }
};
struct Transform {
    Vector3 position, scale;
    Quaternion rotation;
    Vector3 xUnit, yUnit, zUnit;


    Transform() {
        this->position = Vector3();
        this->rotation = Quaternion();
        this->scale = Vector3(1, 1, 1);

        Vector3* unitVectors = new Vector3[3];
        unitVectors = this->rotation.GetUnitVectors();
        this->xUnit = unitVectors[0];
        this->yUnit = unitVectors[1];
        this->zUnit = unitVectors[2];

    }

    Transform(Vector3 position) {
        this->position = position;
        this->rotation = Quaternion();
        this->scale = Vector3(1, 1, 1);

        Vector3* unitVectors = new Vector3[3];
        unitVectors = this->rotation.GetUnitVectors();
        this->xUnit = unitVectors[0];
        this->yUnit = unitVectors[1];
        this->zUnit = unitVectors[2];
    }

    Transform(Vector3 position, Quaternion rotation) {
        this->position = position;
        this->rotation = rotation;
        this->scale = Vector3(1, 1, 1);

        Vector3* unitVectors = new Vector3[3];
        unitVectors = this->rotation.GetUnitVectors();
        this->xUnit = unitVectors[0];
        this->yUnit = unitVectors[1];
        this->zUnit = unitVectors[2];
    }

    Transform(Vector3 position, Quaternion rotation, Vector3 scale) {
        this->position = position;
        this->rotation = rotation;
        this->scale = scale;

        Vector3* unitVectors = new Vector3[3];
        unitVectors = this->rotation.GetUnitVectors();
        this->xUnit = unitVectors[0];
        this->yUnit = unitVectors[1];
        this->zUnit = unitVectors[2];
    }

    void Move(Vector3 delta) {
        this->position.add(delta);
    }
    void Scale(Vector3 delta) {
        this->scale.add(delta);
    }
    // Give angles in radians
    // zUnit is the unit vector of the sphere, xUnit and yUnit are the perpendicular vectors based on zUnit and w.
    // rotation is always in the form of a vector quaternion (we are applying p' = qp(q^-1)) (which for unit quaternions always equals the conjugate)
    // We store w but do not include it in the rotation calculation (it is only used to determine xUnit and yUnit)
    void Rotate(Vector3 angles) {
        Quaternion q;
        Quaternion qOut;
        qOut = this->rotation.getVectorQuaternion();

        Vector3 angleX = this->xUnit;
        Vector3 angleY = this->yUnit;

        angleX.scalarMult(angles.x);
        angleY.scalarMult(angles.y);
        Vector3 rotUnitVector = rotUnitVector.add(angleX,angleY);

        float rotVectorNorm = rotUnitVector.norm();
        if (rotVectorNorm != 0) {
            rotUnitVector.scalarMult(1 / rotVectorNorm);
        }

        float angleNorm = (float)sqrt(pow(angles.x, 2) + pow(angles.y, 2));

        q = q.getFromVector(rotUnitVector, angleNorm);

        qOut = qOut.multiply(q, qOut);
        qOut = qOut.multiply(qOut, q.conjugate());

        qOut.w = this->rotation.w + angles.z / (float)M_PI;

        this->rotation = qOut;

        if (abs(this->rotation.w) > 1) {
            this->rotation.w -= 2 * this->rotation.w/abs(this->rotation.w);
        }

        Vector3* unitVectors = new Vector3[3];
        unitVectors = this->rotation.GetUnitVectors();
        this->xUnit = unitVectors[0];
        this->yUnit = unitVectors[1];
        this->zUnit = unitVectors[2];
    }

    Vector3 solve(Quaternion rotation, Vector3 displacement) {
        float xAmount, yAmount, zAmount;
        // For explanation, see Kramer's Rule
        // For default: D = 1, Dx = 0, Dy = 0, Dz = 5
        float xz = (float)sqrt(pow(rotation.x, 2) + pow(rotation.z, 2));
        float yz = (float)sqrt(pow(rotation.y, 2) + pow(rotation.z, 2));

        float x = displacement.x;
        float y = displacement.y;
        float z = displacement.z;

        float cosx = rotation.z / xz;
        float cosy = rotation.z / yz;
        float sinx = -rotation.x / xz;
        float siny = rotation.y / yz;

        xAmount = z * sinx + x * cosx + y * siny * sinx;
        yAmount = y * cosy + x * siny * sinx + z * siny * cosx;
        zAmount = z * cosx + x * sinx + y * siny * sinx;
        return Vector3(xAmount, yAmount, zAmount);
        //
    }
};
struct Camera {
    Transform transform;
    float fieldOfView, vertFOV;
    int xRes, yRes;
    Camera() {
        this->transform = Transform(Vector3(0,0,0));
        this->fieldOfView = 12 * (float)M_PI / 18;
        this->xRes = 1920;
        this->yRes = 1080;
        this->vertFOV = atan(tan(fieldOfView / 2) * yRes / xRes) * 2;
    }
    Camera(Transform transform) {
        this->transform = transform;
        this->fieldOfView = 12 * (float)M_PI/18;
        this->xRes = 1920;
        this->yRes = 1080;
        this->vertFOV = atan(tan(fieldOfView / 2) * yRes / xRes) * 2;
    }

    Vector2 getScreenSpace(Vector3 position) {
        Vector3 displacement = displacement.difference(position, this->transform.position);
        Vector3 relativeSpace = this->transform.solve(this->transform.rotation, displacement);
        Vector2 screenSpace;
        
        if (relativeSpace.z < 0) {
            relativeSpace.z = (float)0.001;
        }
        
        // Perspective Simple Projection
        screenSpace.x = relativeSpace.x / relativeSpace.z / tan(fieldOfView / 2);
        screenSpace.y = relativeSpace.y / relativeSpace.z / tan(vertFOV / 2);

        return screenSpace;
    }

    void move(Vector3 delta) {
        this->transform.position = Vector3(transform.position.x + delta.x, transform.position.y + delta.y, transform.position.z + delta.z);
    }

    GLFWwindow* createScreen() {
        GLFWwindow* window = glfwCreateWindow(xRes, yRes, "Hello World", NULL, NULL);
        return window;
    }

    void updateUnitVectors() {
        Vector3* unitVectors = new Vector3[3];
        unitVectors = this->transform.rotation.GetUnitVectors();
        this->transform.xUnit = unitVectors[0];
        this->transform.yUnit = unitVectors[1];
        this->transform.zUnit = unitVectors[2];
    }
};

struct Triangle {

    Vector3 pos1, pos2, pos3;
    Vector2 drawPos1, drawPos2, drawPos3;

    Triangle() {}
    Triangle(Vector3 pos1, Vector3 pos2, Vector3 pos3, Camera camera) {
        this->pos1 = pos1;
        this->pos2 = pos2;
        this->pos3 = pos3;

        this->drawPos1 = camera.getScreenSpace(this->pos1);
        this->drawPos2 = camera.getScreenSpace(this->pos2);
        this->drawPos3 = camera.getScreenSpace(this->pos3);

    }
    void move(Vector3 delta) {
        this->pos1.add(delta);
        this->pos2.add(delta);
        this->pos3.add(delta);
    }

    void drawTriangle(VAO vao, Camera camera) {

        updatePosition(camera);
        GLfloat vertices[] = {
            this->drawPos1.x,this->drawPos1.y,
            this->drawPos2.x,this->drawPos2.y,
            this->drawPos3.x,this->drawPos3.y, };

        GLuint indices[] = { 0,1,2, };

        vao.Bind();
        VBO vbo(vertices, sizeof(vertices));
        EBO ebo(indices, sizeof(indices));
        vao.linkVBO(vbo, 0);
        vbo.Bind();
        ebo.Bind();
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        vao.Unbind();
        vbo.Unbind();
        ebo.Unbind();
        vbo.Delete();
        ebo.Delete();
    }

    void updatePosition(Camera camera) {
        this->drawPos1 = camera.getScreenSpace(this->pos1);
        this->drawPos2 = camera.getScreenSpace(this->pos2);
        this->drawPos3 = camera.getScreenSpace(this->pos3);
    }

};

struct Quad {
    Triangle tri1;
    Triangle tri2;
    Transform transform;

    Quad() {}
    Quad(Transform transform, Camera camera) {
        // Because this is a rectangle, only x and y scales have effect
        setPositions(transform, camera);
        this->transform = transform;
        }


    void setPositions(Transform transform, Camera camera) {
        float x = transform.scale.x / 2;
        float y = transform.scale.y / 2;
        float r = (float)sqrt(pow(x, 2) + pow(y, 2));

        float wRot = transform.rotation.w;
        float xRot = transform.rotation.x;
        float yRot = transform.rotation.y;
        float zRot = transform.rotation.z;

        float angle = (float)atan(y / x)/ (float)M_PI;

        Vector2 pos1 = Vector2(r * (float)cos(M_PI * (wRot - 1 + angle)), r * (float)sin(M_PI * (wRot - 1 + angle)));
        Vector2 pos2 = Vector2(r * (float)cos(M_PI * (wRot - angle)), r * (float)sin(M_PI * (wRot - angle)));
        Vector2 pos3 = Vector2(r * (float)cos(M_PI * (wRot + angle)), r * (float)sin(M_PI * (wRot + angle)));
        Vector2 pos4 = Vector2(r * (float)cos(M_PI * (wRot + 1 - angle)), r * (float)sin(M_PI * (wRot + 1 - angle)));

        float xz, yz;

        if (zRot < 0) {
            xz = (float) - sqrt(pow(xRot, 2) + pow(zRot, 2));
            yz = (float)-sqrt(pow(yRot, 2) + pow(zRot, 2));
        }
        else if (zRot > 0) {
            xz = (float)sqrt(pow(xRot, 2) + pow(zRot, 2));
            yz = (float)sqrt(pow(yRot, 2) + pow(zRot, 2));
        }
        else {
            xz = xRot;
            yz = yRot;
        }
        Vector3 triPos1 = transform.position;
        Vector3 triPos2 = transform.position;
        Vector3 triPos3 = transform.position;
        Vector3 triPos4 = transform.position;

        triPos1.add(Vector3(pos1.x * yz, pos1.y * xz, pos1.y * yRot + pos1.x * xRot));
        triPos2.add(Vector3(pos2.x * yz, pos2.y * xz, pos2.y * yRot + pos2.x * xRot));
        triPos3.add(Vector3(pos3.x * yz, pos3.y * xz, pos3.y * yRot + pos3.x * xRot));
        triPos4.add(Vector3(pos4.x * yz, pos4.y * xz, pos4.y * yRot + pos4.x * xRot));
        
        this->tri1 = Triangle(triPos1,triPos2,triPos3, camera);
        this->tri2 = Triangle(triPos1, triPos3, triPos4, camera);

    }

    void drawQuad(VAO vao, Camera camera, Shader shader) {

        this->tri1.updatePosition(camera);
        this->tri2.updatePosition(camera);

        GLfloat vertices[] = {
            this->tri1.drawPos1.x,this->tri1.drawPos1.y,
            this->tri1.drawPos2.x,this->tri1.drawPos2.y,
            this->tri1.drawPos3.x,this->tri1.drawPos3.y,
            this->tri2.drawPos1.x,this->tri2.drawPos1.y,
            this->tri2.drawPos2.x,this->tri2.drawPos2.y,
            this->tri2.drawPos3.x,this->tri2.drawPos3.y, };


     
        vao.Bind();
        VBO vbo(vertices, sizeof(vertices));
        vao.linkVBO(vbo, 0);
        vbo.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        vao.Unbind();
        vbo.Unbind();
        vbo.Delete();

    }

    void Rotate(Vector3 angles, Camera camera) {
        this->transform.Rotate(angles);
        setPositions(this->transform, camera);
    }

    void Move(Vector3 delta, Camera camera) {
        this->transform.Move(delta);
        setPositions(this->transform, camera);
    }
};

struct Cube {
    Quad quad1, quad2, quad3, quad4, quad5, quad6;
    Transform transform;

    Cube(Transform transform, Camera camera) {
        this->transform = transform;

        float x = transform.position.x;
        float y = transform.position.y;
        float z = transform.position.z;

        float scaleX = transform.scale.x;
        float scaleY = transform.scale.y;
        float scaleZ = transform.scale.z;
        float r = (float)sqrt(pow(scaleX, 2) + pow(scaleY, 2) + pow(scaleZ, 2));

        float wRot = transform.rotation.w;
        float xRot = transform.rotation.x;
        float yRot = transform.rotation.y;
        float zRot = transform.rotation.z;

        Vector3 xUnit = transform.xUnit;
        Vector3 yUnit = transform.yUnit;
        Vector3 zUnit = transform.zUnit;

        //float xz = sqrt(pow(xRot, 2) + pow(zRot, 2)) / zRot * abs(zRot);
        //float yz = sqrt(pow(yRot, 2) + pow(zRot, 2)) / zRot * abs(zRot);
        
        Vector3 xAdd = Vector3(scaleX * xUnit.x * 0.5f, scaleX * xUnit.y * 0.5f, scaleX * xUnit.z * 0.5f);
        Vector3 yAdd = Vector3(scaleY * yUnit.x * 0.5f, scaleY * yUnit.y * 0.5f, scaleY * yUnit.z * 0.5f);
        Vector3 zAdd = Vector3(scaleZ * zUnit.x * 0.5f, scaleZ * zUnit.y * 0.5f, scaleZ * zUnit.z * 0.5f);
        Vector3 pos = Vector3(x, y, z);

        //  Next step is to add rotation to the cube
        Vector3 pos1 = pos.difference(xAdd);
        Vector3 pos2 = pos;
        pos2.add(xAdd);
        Vector3 pos3 = pos.difference(yAdd);
        Vector3 pos4 = pos;
        pos4.add(yAdd);
        Vector3 pos5 = pos.difference(zAdd);
        Vector3 pos6 = pos;
        pos6.add(zAdd);

        Quaternion rot1 = Quaternion(wRot, -xUnit.x, -xUnit.y, -xUnit.z);
        Quaternion rot2 = Quaternion(wRot, xUnit.x, xUnit.y, xUnit.z);
        Quaternion rot3 = Quaternion(wRot, -yUnit.x, -yUnit.y, -yUnit.z);
        Quaternion rot4 = Quaternion(wRot, yUnit.x, yUnit.y, yUnit.z);
        Quaternion rot5 = Quaternion(wRot, -zUnit.x, -zUnit.y, -zUnit.z);
        Quaternion rot6 = Quaternion(wRot, zUnit.x, zUnit.y, zUnit.z);


        this->quad1 = Quad(Transform(pos1, rot1, Vector3(scaleZ, scaleY, 0)), camera);
        this->quad2 = Quad(Transform(pos2, rot2, Vector3(scaleZ, scaleY, 0)), camera);
        this->quad3 = Quad(Transform(pos3, rot3, Vector3(scaleX, scaleZ, 0)), camera);
        this->quad4 = Quad(Transform(pos4, rot4, Vector3(scaleX, scaleZ, 0)), camera);
        this->quad5 = Quad(Transform(pos5, rot5, Vector3(scaleX, scaleY, 0)), camera);
        this->quad6 = Quad(Transform(pos6, rot6, Vector3(scaleX, scaleY, 0)), camera);
    }

    void drawCube(VAO vao, Camera camera, Shader shader) {
        this->quad1.drawQuad(vao, camera, shader);
        this->quad2.drawQuad(vao, camera, shader);
        this->quad3.drawQuad(vao, camera, shader);
        this->quad4.drawQuad(vao, camera, shader);
        this->quad5.drawQuad(vao, camera, shader);
        this->quad6.drawQuad(vao, camera, shader);
    }
};
struct ObjectList {
    vector<Quad> quads;
    vector<Cube> cubes;
    vector<int> shaderID;


    void drawObjects(VAO vao, Camera camera, vector<Shader> shader) {
        shader[0].Activate();

        drawQuads(vao, camera, shader[0], this->quads);
        
        //drawCubes(vao,camera, shader[0], this->cubes);
    }

    void addObject(Quad quad, int shader) {
        quads.push_back(quad);
    }

    void addObject(Cube cube, int shader){
        cubes.push_back(cube);
    }

    void Rotate(Vector3 angles, Camera camera, int id) {
        quads[id].Rotate(angles, camera);
    }

    // Working on parallelizing the process of drawing objects
    void drawQuads(VAO vao, Camera camera, Shader shader, vector<Quad> quads) {
        array<array<GLfloat,12>,512> vertices;

        for (int i = 0; i < quads.size(); i++) {   

            quads[i].tri1.updatePosition(camera);
            quads[i].tri2.updatePosition(camera); 

            vertices[i] = {
                quads[i].tri1.drawPos1.x,quads[i].tri1.drawPos1.y,
                quads[i].tri1.drawPos2.x,quads[i].tri1.drawPos2.y,
                quads[i].tri1.drawPos3.x,quads[i].tri1.drawPos3.y,
                quads[i].tri2.drawPos1.x,quads[i].tri2.drawPos1.y,
                quads[i].tri2.drawPos2.x,quads[i].tri2.drawPos2.y,
                quads[i].tri2.drawPos3.x,quads[i].tri2.drawPos3.y };
        }

        vao.Bind();
     
        VBO vbo(vertices, sizeof(vertices[0][0]) * vertices[0].size() * vertices.size());

        vao.linkVBO(vbo, 0);
        vbo.Bind();

        glDrawArrays(GL_TRIANGLES,0, 6 * quads.size());
        vao.Unbind();
        vbo.Unbind();
        vbo.Delete();

    }
};

struct App{
    Camera camera;
    GLFWwindow* window;
    ObjectList objectList;
    vector<Shader> shaders;
    unsigned char mKeyState[256] = {};
    unsigned char mOldKeyState[256] = {};
    float speed;

    virtual void onKeyDown(int key,float deltaTime){
        if (key == 'W') {
            this->camera.move(Vector3(0, 0, deltaTime * speed));
        }
        if (key == 'A') {
            this->camera.move(Vector3(-deltaTime * speed, 0, 0));
        }
        if (key == 'S') {
            this->camera.move(Vector3(0, 0, -deltaTime * speed));
        }
        if (key == 'D') {
            this->camera.move(Vector3(deltaTime * speed, 0, 0));
        }

    }
    virtual void onKeyUp(int key, float deltaTime){

    }

    void onCreate() {
        this->camera = Camera(Transform(Vector3(0.0f, 0.0f, -2.5f), Quaternion(0, 0, 0, 1)));
        this->window = camera.createScreen();
    }

    void update(float deltaTime, VAO vao) {
        this->objectList.drawObjects(vao,this->camera,this->shaders);

    }

    void createShaders() {
        this->addShader("src/default.vert", "src/default.frag");
        this->addShader("src/default.vert", "src/red.frag");
    }

    void addShader(const char* vertexFile, const char* fragmentFile) {
        shaders.push_back(Shader(vertexFile, fragmentFile));
    }

    void deleteShaders() {
        for (int i = 0; i < shaders.size(); i++) {
            shaders[i].Delete(i);
        }
        shaders.erase(shaders.begin(), shaders.end());
    }

    void input(float deltaTime){
        GetKeyboardState(mKeyState);

        for (unsigned int i = 0; i < 256; i++) {
            if (mKeyState[i] & 0x80) {
                onKeyDown(i, deltaTime);
            }

            else if (mKeyState[i] != mOldKeyState[i]) {
                onKeyUp(i, deltaTime);
            }
        }
        ::memcpy(mOldKeyState, mKeyState, sizeof(unsigned char) * 256);
        }
};
