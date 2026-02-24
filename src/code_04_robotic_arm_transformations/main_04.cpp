#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../common/debugging.h"
#include "../common/renderable.h"
#include "../common/simple_shapes.h"
#include "../common/matrix_stack.h"
#include "../common/shaders.h"


float alpha_S, alpha_E,alpha_W;

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {
        case GLFW_KEY_A:
            alpha_S += 0.04;
            break;
        case GLFW_KEY_S:
            alpha_S -= 0.04;
            break;
        case GLFW_KEY_D:
            alpha_E += 0.04;
            break;
        case GLFW_KEY_F:
            alpha_E -= 0.04;
            break;
        case GLFW_KEY_G:
            alpha_W += 0.04;
            break;
        case GLFW_KEY_H:
            alpha_W -= 0.04;
            break;
        default:
            break;
        }
    }
}

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
    window = glfwCreateWindow(1024, 1024, "code_04_robotic_arm_transformations", NULL, NULL);


    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

  
    glfwSetKeyCallback(window, keyboard_callback);

   // Load GL symbols *after* the context is current
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::fprintf(stderr, "Failed to initialize GLAD\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    /* query for the hardware and software specs and print the result on the console*/
    printout_opengl_glsl_info();

    check_gl_errors(__LINE__, __FILE__);

    renderable quad, frame;
    frame = shape_maker::frame();
    quad = shape_maker::cube();
    

    shader s;
    s.bind_attribute("aPosition", 0);
    s.create_program("shaders/basic.vert", "shaders/basic.frag");
    glUseProgram(s.program);

    /* cal glGetError and print out the result in a more verbose style
    * __LINE__ and __FILE__ are precompiler directive that replace the value with the
    * line and file of this call, so you know where the error happened
    */
    check_gl_errors(__LINE__, __FILE__);

	glm::mat4 view = glm::lookAt(glm::vec3(200, 200, 200), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1.0f, 1.f, 500.0f);



    glm::mat4 s_s = glm::scale(glm::vec3(10, 10, 10));
    glm::mat4 s_a = glm::scale(glm::vec3(15, 5, 5));
    glm::mat4 t_a = glm::translate(glm::vec3(25, 0, 0));

    glm::mat4 s_e = glm::scale(glm::vec3(8, 8, 8));
    glm::mat4 s_f = glm::scale(glm::vec3(12, 4, 4));
    glm::mat4 t_f = glm::translate(glm::vec3(20, 0, 0));

    glm::mat4 s_w = glm::scale(glm::vec3(6, 6, 6));

    glm::mat4 W = glm::translate(glm::vec3(30, 0, 0));

    glm::mat4 E = glm::translate(glm::vec3(40, 0, 0));



    alpha_S,alpha_E,alpha_W = 0.0;

    matrix_stack stack;

    stack.mult(proj*view );

    float alpha = 0.0;
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window))
    {
        glm::mat4 r_S = glm::rotate(alpha_S, glm::vec3(0, 0, 1));
        glm::mat4 r_E = glm::rotate(alpha_E, glm::vec3(0, 0, 1));
        glm::mat4 r_W = glm::rotate(alpha_W, glm::vec3(0, 0, 1));


		glm::mat4 r = glm::rotate(alpha+=0.01, glm::vec3(0, 1, 0));

        /* Render here */
        glClearColor(0.2, 0.2, 0.2, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
        stack.push();
        stack.mult(r);
        stack.mult(glm::translate(glm::vec3(0, -30, 0)));

        stack.mult(r_S);

        stack.push();
        stack.mult(s_s);


        // Shoulder
        glUniformMatrix4fv(s["uM"], 1, GL_FALSE, glm::value_ptr(stack.m()));
        stack.pop();
        glUniform3f(s["uCol"], 0.3, 0.3, 0.6);
        quad.bind();
        glDrawElements(quad().mode, quad().count, quad().itype, NULL);
        
        // Arm
        stack.push();
        stack.mult(t_a * s_a);       
        glUniformMatrix4fv(s["uM"], 1, GL_FALSE, glm::value_ptr(stack.m()));
        stack.pop();
        glUniform3f(s["uCol"], 0.2, 0.3, 0.6);
        quad.bind();
        glDrawElements(quad().mode, quad().count, quad().itype, NULL);


        // Elbow frame
        stack.mult(E * r_E);
        stack.push();

        // Elbow
        stack.mult(s_e);

        glUniformMatrix4fv(s["uM"], 1, GL_FALSE, glm::value_ptr(stack.m()));
        stack.pop();

        glUniform3f(s["uCol"], 0.4, 0.3, 0.5);
        quad.bind();
        glDrawElements(quad().mode, quad().count, quad().itype, NULL);

        
        stack.push();

        // forearm
        stack.mult(t_f * s_f);

        glUniformMatrix4fv(s["uM"], 1, GL_FALSE, glm::value_ptr(stack.m()));
        stack.pop();
        glUniform3f(s["uCol"], 0.4, 0.2, 0.5);
        quad.bind();
        glDrawElements(quad().mode, quad().count, quad().itype, NULL);

        // Wrist Frame
        stack.mult(W * r_W);
        stack.push();

        // wrist
        stack.mult(s_w);
        glUniformMatrix4fv(s["uM"], 1, GL_FALSE, glm::value_ptr(stack.m()));
        stack.pop();
        
        glUniform3f(s["uCol"], 0.4, 0.5, 0.0);
        quad.bind();
        glDrawElements(quad().mode, quad().count, quad().itype, NULL);

        stack.pop();
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}