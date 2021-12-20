#include "main.h"

// Is called at engine launch
App Start() {

    App out;
    out.onCreate();
    for (int i = 0; i < 3; i++) {
        out.objectList.addObject(Quad(Transform(Vector3(-1 + 2 * i, 0, 0), Quaternion(0, 0, 0, 1), Vector3(1,1,1)), out.camera), 1);
    }
    out.speed = 3;

    return out;
}

// Is called every frame
App Update(float deltaTime, VAO vao, App app) {
    /*
    for (int i = 0; i < 576; i++) {
        app.objectList.quads[i].Move(Vector3(0, 0, deltaTime), app.camera);
        app.objectList.quads[i].Rotate(Vector3(0, 0, deltaTime *( i % 2 * 2 - 1)), app.camera);
    }
    */
    //app.camera.move(Vector3(0,0,-deltaTime));

    //app.objectList.drawObjects(vao,app.camera,app.shaders);

    app.input(deltaTime);

    app.objectList.drawObjects(vao,app.camera,app.shaders);
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
        //app.update(deltaTime, vao);

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