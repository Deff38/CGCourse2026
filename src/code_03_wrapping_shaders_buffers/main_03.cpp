#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../common/debugging.h"
#include "../common/renderable.h"
#include "../common/shaders.h"


int main(int argc, char** argv) {

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // Request OpenGL 4.1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    // Ask specifically for the core profile (recommended)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // macOS requires this for 3.2+ contexts
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(512, 512, "code_03_wrapping_shaders_buffers", NULL, NULL);


    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Load GL symbols *after* the context is current
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::fprintf(stderr, "Failed to initialize GLAD\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    /* query for the hardware and software specs and print the result on the console*/
    printout_opengl_glsl_info();

    ///* vertex position */
    GLuint positionAttribIndex = 0;
    float positions[] = { 0.0, 0.0,  // 1st vertex
                          0.5, 0.0,  // 2nd vertex
                          0.5, 0.5,
                          0.0, 0.5
    };

    ///* Color attribute */
    GLuint colorAttribIndex = 1;
    float colors[] = { 1.0, 0.0, 0.0,    // 1st vertex's color
                        0.0, 1.0, 0.0,   // 2nd vertex's color
                        0.0, 0.0, 1.0,
                        1.0, 1.0, 1.0
    };

    //* indices */
    GLuint indices[] = { 0,1,2,0,2,3 };
    check_gl_errors(__LINE__, __FILE__);
    renderable r;
    r.create();
    r.add_vertex_attribute(positions, 8, positionAttribIndex, 2);
    r.add_vertex_attribute(colors, 12, colorAttribIndex, 3);
    r.add_indices<int>(indices, 6, GL_TRIANGLES);


    shader s;
    s.bind_attribute("aPosition", positionAttribIndex);
    s.bind_attribute("aColor", colorAttribIndex);
    s.create_program("shaders/basic.vert", "shaders/basic.frag");
    glUseProgram(s.program);

    /* cal glGetError and print out the result in a more verbose style
    * __LINE__ and __FILE__ are precompiler directive that replace the value with the
    * line and file of this call, so you know where the error happened
    */
    check_gl_errors(__LINE__, __FILE__);


    float d = 0.01;
    float delta = 0;
    while (!glfwWindowShouldClose(window))
    {
        if (delta < 0 || delta > 0.5)
            d = -d;
        delta += d;

        glUniform1f(s["uDelta"], delta);
        /* Render here */
        glClearColor(0.2, 0.2, 0.2, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // glDrawArrays(GL_TRIANGLES, 0, 6);
        r.bind();
        glDrawElements(r().mode, r().count, r().itype, NULL);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}