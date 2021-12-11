#pragma once

#include "main.h"

App Start() {
    App out;

    Camera camera(Transform(Vector3(0.0f, 0.0f, -2.5f), Quaternion(0, 0, 0, 1)));
    out.camera = camera;

    GLFWwindow* window = camera.createScreen();
    out.window = window;

    out.objectList.addObject(Quad(Transform(Vector3(1, 0, 0)), camera));
    out.objectList.addObject(Quad(Transform(Vector3(-1, 0, 0)), camera));

    return out;
}

App Update(float deltaTime, VAO vao, App app) {
    app.objectList.quads[0].Rotate(Vector3(0, 0, deltaTime * 3), app.camera);
    //app.objectList.Rotate(Vector3(0, 0, deltaTime*3), app.camera,0);
    //app.objectList.Rotate(Vector3(0, 0, -deltaTime * 3), app.camera, 1);
   
    app.objectList.drawObjects(vao, app.camera);
    return app;
}


int main(void)
{   
    /* Initialize the library*/
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    App app = Start();
    float deltaTime = 0;

    /* Make the window's context current */
    glfwMakeContextCurrent(app.window);

    glewInit();

    Shader shaderProgram("src/default.vert","src/default.frag");
    
    // Variable creation here: 

    VAO vao;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(app.window))
    {   
        auto t_start = std::chrono::high_resolution_clock::now();
        glClear(GL_COLOR_BUFFER_BIT);
        shaderProgram.Activate();

        app = Update(deltaTime, vao, app);

        glfwSwapBuffers(app.window);
        /* Poll for and process events */
        glfwPollEvents();
        auto t_end = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration<float>(t_end - t_start).count();
    }

    vao.Delete();
    shaderProgram.Delete();

    glfwDestroyWindow(app.window);
    glfwTerminate();
    return 0;
}