#pragma once

#include "main.h"

// Is called at engine launch
App Start() {

    App out;
    out.onCreate();

    out.objectList.addObject(Quad(Transform(Vector3(0, 0,0), Quaternion(0,0,0,1), Vector3(1,1,1)), out.camera),1);
    out.objectList.addObject(Quad(Transform(Vector3(-2, 0, 0), Quaternion(0, 0, 0, 1), Vector3(1,1, 1)), out.camera), 0);
    out.speed = 3;

    return out;
}

// Is called every frame
App Update(float deltaTime, VAO vao, App app) {
    app.objectList.quads[0].Move(Vector3(deltaTime, 0, 0),app.camera);
    return app;
}


int main(void)
{   
    /* Initialize the library*/
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    App app = Start();
    float deltaTime = 0;

    /* Make the window's context current */
    glfwMakeContextCurrent(app.window);

    glewInit();

    // Variable creation here: 
    app.createShaders();

    VAO vao;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(app.window))
    {   
        auto t_start = std::chrono::high_resolution_clock::now();
        glClear(GL_COLOR_BUFFER_BIT);


        app = Update(deltaTime, vao, app);
        app.update(deltaTime, vao);

        glfwSwapBuffers(app.window);
        /* Poll for and process events */
        glfwPollEvents();
        auto t_end = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration<float>(t_end - t_start).count();
    }

    vao.Delete();
    app.deleteShaders();

    glfwDestroyWindow(app.window);
    glfwTerminate();
    return 0;
}