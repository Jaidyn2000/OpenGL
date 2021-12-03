// This file includes: defines, includes, usings, functions and structs

#pragma once
#define _USE_MATH_DEFINES

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
        this->indices = indices;
        this->size = size;
        glGenBuffers(1, &id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
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
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
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
    void Delete() {
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
    Vector3 add(Vector3 v) {
        Vector3 out;
        out.x = v.x + this->x;
        out.y = v.y + this->y;
        out.z = v.z + this->z;
        return out;
    }
    Vector3 add(Vector3 v1, Vector3 v2) {
        Vector3 out = Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
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
    Vector3 solve(Vector3 xUnit, Vector3 yUnit, Vector3 zUnit, Vector3 displacement) {
        float xAmount, yAmount, zAmount;
        // For explanation, see Kramer's Rule
        // For default: D = 1, Dx = 0, Dy = 0, Dz = 5
        float D = xUnit.x * (yUnit.y * zUnit.z - zUnit.y * yUnit.z) + yUnit.x * (zUnit.y * xUnit.z + xUnit.y * zUnit.z) + zUnit.x * (xUnit.y * yUnit.z - yUnit.y * xUnit.z);
        float Dx = displacement.x * (yUnit.y * zUnit.z - zUnit.y * yUnit.z) + yUnit.x * (displacement.z * zUnit.y - displacement.y * zUnit.z) + zUnit.x * (displacement.y * yUnit.z - yUnit.y * displacement.z);
        float Dy = xUnit.x * (displacement.y * zUnit.z - zUnit.y * displacement.z) + displacement.x * (zUnit.y * xUnit.z - zUnit.z * xUnit.y) + zUnit.x * (xUnit.y * displacement.z - xUnit.z * displacement.y);
        float Dz = xUnit.x * (yUnit.y * displacement.z - displacement.y * yUnit.z) + yUnit.x * (displacement.y * xUnit.z - displacement.z * xUnit.y) + displacement.x * (xUnit.y * yUnit.z - yUnit.y * xUnit.z);
        xAmount = (float)Dx / (float)D;
        yAmount = (float)Dy / (float)D;
        zAmount = (float)Dz / (float)D;

        return Vector3(xAmount, yAmount, zAmount);
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

        Vector3 xUnit = Vector3(1, 0, (-this->x / this->z));
        Vector3 yUnit = Vector3(0, 1, (this->y / this->z));

        Vector3 xx = Vector3(xUnit);
        Vector3 xy = Vector3(xUnit);
        Vector3 yx = Vector3(yUnit);
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

        q.getFromVector(xUnit, angles.x);
        qOut.multiply(q, qOut);
        qOut.multiply(qOut, q.conjugate());

        q.getFromVector(yUnit, angles.y);
        qOut.multiply(q, qOut);
        qOut.multiply(qOut, q.conjugate());

        this->rotation = qOut;
        this->rotation.w += angles.z / (float)M_PI;

        Vector3* unitVectors = new Vector3[3];
        unitVectors = this->rotation.GetUnitVectors();
        this->xUnit = unitVectors[0];
        this->yUnit = unitVectors[1];
        this->zUnit = unitVectors[2];
    }
};
struct Camera {
    Transform transform;
    float fieldOfView, vertFOV;
    int xRes, yRes;
    Camera() {
        this->transform = Transform(Vector3(0,0,0));
        this->fieldOfView = (float)2 * M_PI / 3;
        this->xRes = 1920;
        this->yRes = 1080;
        this->vertFOV = atan(tan(fieldOfView / 2) * yRes / xRes) * 2;
    }
    Camera(Transform transform) {
        this->transform = transform;
        this->fieldOfView = (float)2 * M_PI / 3;
        this->xRes = 1920;
        this->yRes = 1080;
        this->vertFOV = atan(tan(fieldOfView / 2) * yRes / xRes) * 2;
    }

    Vector2 getScreenSpace(Vector3 position) {
        Vector3 displacement = displacement.difference(position, this->transform.position);
        Vector3 relativeSpace = displacement.solve(transform.xUnit, transform.yUnit, transform.zUnit, displacement);

        Vector2 screenSpace;
        if (relativeSpace.z < 0) {
            relativeSpace.z = 0.001f;
        }
        float xAmount = relativeSpace.x / abs(relativeSpace.z) / tan(fieldOfView / 2);
        float yAmount = relativeSpace.y / abs(relativeSpace.z) / tan(vertFOV / 2);

        screenSpace.x = xAmount;
        screenSpace.y = yAmount;

        if (relativeSpace.z > 0) {
            if (abs(xAmount) > 0.5f) {
                screenSpace.x = xAmount * 2;
            }
            else {
                screenSpace.x = sin(xAmount * M_PI);
            }
            if (abs(yAmount) > 0.5f) {
                screenSpace.y = yAmount * 2;
            }
            else {
                screenSpace.y = sin(yAmount * M_PI);
            }     
        }
        
        return screenSpace;
    }

    void move(Vector3 delta) {
        this->transform.position = Vector3(transform.position.x + delta.x, transform.position.y + delta.y, transform.position.z + delta.z);
    }

    GLFWwindow* createScreen() {
        GLFWwindow* window = glfwCreateWindow(xRes, yRes, "Hello World", NULL, NULL);
        return window;
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
        this->pos1 = this->pos1.add(delta);
        this->pos2 = this->pos2.add(delta);
        this->pos3 = this->pos3.add(delta);
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

    void checkBoundaries() {
        /*
        this->drawPos1

        cout << screenSpace.x << ' ' << screenSpace.y << ' ';
        */
    }
};

struct Quad {
    Triangle tri1;
    Triangle tri2;
    Transform transform;

    Quad() {}
    Quad(Transform transform, Camera camera) {
        // Because this is a rectangle, only x and y scales have effect
        this->transform = transform;
        float x1 = - transform.scale.x / 2;
        float x2 = transform.scale.x / 2;
        float y1 = - transform.scale.y / 2;
        float y2 = transform.scale.y / 2;

        float wRot = transform.rotation.w;
        float xRot = transform.rotation.x;
        float yRot = transform.rotation.y;
        float zRot = transform.rotation.z;

        this->tri1 = Triangle(transform.position.add(Vector3(x1 * (zRot + yRot), y1 * (zRot + xRot), y1 * yRot + x1 * xRot)),
            transform.position.add(Vector3(x2 * (zRot + yRot), y1 * (zRot + xRot), y1 * yRot + x2 * xRot)),
            transform.position.add(Vector3(x1 * (zRot + yRot), y2 * (zRot + xRot), y2 * yRot + x1 * xRot)), camera);
        this->tri2 = Triangle(transform.position.add(Vector3(x2 * (zRot + yRot), y1 * (zRot + xRot), y1 * yRot + x2 * xRot)),
            transform.position.add(Vector3(x1 * (zRot + yRot), y2 * (zRot + xRot), y2 * yRot + x1 * xRot)),
            transform.position.add(Vector3(x2 * (zRot + yRot), y2 * (zRot + xRot), y2 * yRot + x2 * xRot)), camera);
        cout << tri1.pos1.x << ' ' << tri1.pos1.y << ' ' << tri1.pos1.z << ' ' << tri2.pos1.x << ' ' << tri2.pos1.y << ' ' << tri2.pos1.z << ' ' << tri2.pos3.x << ' ' << tri2.pos3.y << ' ' << tri2.pos3.z << ' ';
    }

    void drawQuad(VAO vao, Camera camera) {

        this->tri1.updatePosition(camera);
        this->tri2.updatePosition(camera);

        GLfloat vertices[] = {
            this->tri1.drawPos1.x,this->tri1.drawPos1.y,
            this->tri1.drawPos2.x,this->tri1.drawPos2.y,
            this->tri1.drawPos3.x,this->tri1.drawPos3.y,
            this->tri2.drawPos3.x,this->tri2.drawPos3.y, };

        GLuint indices[] = { 0,1,2,1,2,3, };

        vao.Bind();
        VBO vbo(vertices, sizeof(vertices));
        EBO ebo(indices, sizeof(indices));
        vao.linkVBO(vbo, 0);
        vbo.Bind();
        ebo.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        vao.Unbind();
        vbo.Unbind();
        ebo.Unbind();
        vbo.Delete();
        ebo.Delete();
    }
};

struct Cube {
    Quad quad1, quad2, quad3, quad4, quad5, quad6;

    Cube(Quad q1, Quad q2, Quad q3, Quad q4, Quad q5, Quad q6) {
        this->quad1 = q1;
        this->quad2 = q2;
        this->quad3 = q3;
        this->quad4 = q4;
        this->quad5 = q5;
        this->quad6 = q6;
    }
    Cube(Transform transform) {
    }
};
