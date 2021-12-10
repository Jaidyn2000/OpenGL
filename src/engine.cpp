#pragma once

#include "main.h"

StartInit Start() {
    StartInit out;

    Camera camera(Transform(Vector3(0.0f, 0.0f, -2.5f), Quaternion(0, 0, 0, 1)));
    out.camera = camera;

    GLFWwindow* window = camera.createScreen();
    out.window = window;

    out.objectList.addObject(Quad(Transform(Vector3(1, 0, 0)), camera));
    out.objectList.addObject(Quad(Transform(Vector3(-1, 0, 0)), camera));

    return out;
}

StartInit Update(float deltaTime, VAO vao, StartInit startInit) {

    startInit.objectList.Rotate(Vector3(deltaTime * 3, 0, deltaTime*3), startInit.camera,0);
    startInit.objectList.Rotate(Vector3(-deltaTime * 3, 0, -deltaTime * 3), startInit.camera, 1);
   
    startInit.objectList.drawObjects(vao, startInit.camera);
    return startInit;
}


int main(void)
{   
    /* Initialize the library*/
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    StartInit startInit = Start();
    float deltaTime = 0;

    /* Make the window's context current */
    glfwMakeContextCurrent(startInit.window);

    glewInit();

    Shader shaderProgram("src/default.vert","src/default.frag");
    
    // Variable creation here: 

    VAO vao;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(startInit.window))
    {   
        auto t_start = std::chrono::high_resolution_clock::now();
        glClear(GL_COLOR_BUFFER_BIT);
        shaderProgram.Activate();

        startInit = Update(deltaTime, vao, startInit);

        glfwSwapBuffers(startInit.window);
        /* Poll for and process events */
        glfwPollEvents();
        auto t_end = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration<float>(t_end - t_start).count();
    }

    vao.Delete();
    shaderProgram.Delete();

    glfwDestroyWindow(startInit.window);
    glfwTerminate();
    return 0;
}