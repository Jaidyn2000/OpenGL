#include "main.h"

// To run program: g++ src/app.cpp -lopengl32 -lglfw3 -lUser32 -lGdi32 -lShell32; .\a  

int main(void)
{   
    /* Initialize the library*/
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    Camera camera(Transform(Vector3(0,0,-10),Quaternion(0,0, 0, 1)));
    GLFWwindow* window = camera.createScreen();

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glewInit();

    Shader shaderProgram("src/default.vert","src/default.frag");
    
    // Variable creation here: 
    float deltaTime = 0;
    char input;
    // Object creation on program start here:
    //Triangle tri = Triangle(Vector3(0.0f, 0.0f, 5.0f), Vector3(1.0f, 0.0f, 5.0f), Vector3(0.0f, 1.0f, 5.0f), camera);
    //Quad quad = Quad(Transform(Vector3(0, 1, 5)), camera);
    Quad floor = Quad(Transform(Vector3(0, 0.0f, 15), Quaternion(0, 0, 0, 1), Vector3(1, 1, 1)), camera);
    VAO vao;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {   
        auto t_start = std::chrono::high_resolution_clock::now();
        glClear(GL_COLOR_BUFFER_BIT);
        shaderProgram.Activate();
        
        //camera.move(Vector3(0.0f, 0.0f, 1 * deltaTime));

        //tri.drawTriangle(vao);
        //quad.drawQuad(vao, camera);
        floor.drawQuad(vao, camera);

        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
        auto t_end = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration<float>(t_end - t_start).count();
    }

    vao.Delete();
    shaderProgram.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}