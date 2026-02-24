#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../common/debugging.h"
#include "../common/renderable.h"
#include "../common/simple_shapes.h"
#include "../common/matrix_stack.h"
#include "../common/shaders.h"


float alpha_S, alpha_E,alpha_W,alpha_k1, alpha_k2, alpha_k3;

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
        case GLFW_KEY_J:
            alpha_k1 += 0.04;
            alpha_k2 += 0.04;
            alpha_k3 += 0.04;
            break;
        case GLFW_KEY_K:
            alpha_k1 -= 0.04;
            alpha_k2 -= 0.04;
            alpha_k3 -= 0.04;
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
    window = glfwCreateWindow(1024, 1024, "code_04_robotic_arm_transformations_esercizio", NULL, NULL);


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
    quad = shape_maker::quad();
    

    shader s;
    s.bind_attribute("aPosition", 0);
    s.create_program("shaders/basic.vert", "shaders/basic.frag");
    glUseProgram(s.program);

    /* cal glGetError and print out the result in a more verbose style
    * __LINE__ and __FILE__ are precompiler directive that replace the value with the
    * line and file of this call, so you know where the error happened
    */
    check_gl_errors(__LINE__, __FILE__);

    glm::mat4 glob = glm::scale(glm::vec3(0.01, 0.01, 1));

    // Shoulder 
    glm::mat4 s_s = glm::scale(glm::vec3(10, 10, 1));

    // Arm
    glm::mat4 s_a = glm::scale(glm::vec3(15, 5, 1));
    glm::mat4 t_a = glm::translate(glm::vec3(25, 0, 0));

    // Elbow
    glm::mat4 s_e = glm::scale(glm::vec3(8, 8, 1));
    glm::mat4 t_e = glm::translate(glm::vec3(40, 0, 0));

    // Forearm
    glm::mat4 s_f = glm::scale(glm::vec3(12, 4, 1));
    glm::mat4 t_f = glm::translate(glm::vec3(20, 0, 0));

    // Wrist
    glm::mat4 s_w = glm::scale(glm::vec3(6, 6, 1));
    glm::mat4 t_w = glm::translate(glm::vec3(30, 0, 0));

    // Knuckles
    glm::mat4 s_k1 = glm::scale(glm::vec3(1,1,1));
    glm::mat4 t_k1 = glm::translate(glm::vec3(6,0,0));

    glm::mat4 s_k2 = glm::scale(glm::vec3(1,1,1));
    glm::mat4 t_k2 = glm::translate(glm::vec3(6,3,0));

    glm::mat4 s_k3 = glm::scale(glm::vec3(1,1,1));
    glm::mat4 t_k3 = glm::translate(glm::vec3(6,-3,0));

    // Fingers
    glm::mat4 s_f1 = glm::scale(glm::vec3(3,1,1));
    glm::mat4 t_f1 = glm::translate(glm::vec3(4,0,0));

    glm::mat4 s_f2 = glm::scale(glm::vec3(3,1,1));
    glm::mat4 t_f2 = glm::translate(glm::vec3(4,0,0));

    glm::mat4 s_f3 = glm::scale(glm::vec3(3,1,1));
    glm::mat4 t_f3 = glm::translate(glm::vec3(4,0,0));

    alpha_S,alpha_E ,alpha_W, alpha_k1, alpha_k2, alpha_k3 = 0.0;

    matrix_stack stack;

    stack.mult(glob);

    glDisable(GL_DEPTH_TEST);
    
    while (!glfwWindowShouldClose(window))
    {
        glm::mat4 r_S = glm::rotate(alpha_S, glm::vec3(0, 0, 1));
        glm::mat4 r_E = glm::rotate(alpha_E, glm::vec3(0, 0, 1));
        glm::mat4 r_W = glm::rotate(alpha_W, glm::vec3(0, 0, 1));
        glm::mat4 r_k1 = glm::rotate(alpha_k1, glm::vec3(0, 0, 1));
        glm::mat4 r_k2 = glm::rotate(alpha_k2, glm::vec3(0, 0, 1));
        glm::mat4 r_k3 = glm::rotate(alpha_k3, glm::vec3(0, 0, 1));

        /* Render here */
        glClearColor(0.2, 0.2, 0.2, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
        
        stack.push(); 

        stack.mult(glm::translate(glm::vec3(0, -30, 0))); 

        // Shoulder frame
        stack.mult(r_S); 
        
        // Shoulder
        stack.push(); 
            stack.mult(s_s);
            glUniformMatrix4fv(s["uM"], 1, GL_FALSE, glm::value_ptr(stack.m()));
            glUniform3f(s["uCol"], 0.3, 0.3, 0.6);
            quad.bind();
            glDrawElements(quad().mode, quad().count, quad().itype, NULL);
        stack.pop();

        // Arm
        stack.push(); 
            stack.mult(t_a * s_a);  
            glUniformMatrix4fv(s["uM"], 1, GL_FALSE, glm::value_ptr(stack.m()));
            glUniform3f(s["uCol"], 0.2, 0.3, 0.6);
            glDrawElements(quad().mode, quad().count, quad().itype, NULL);
        stack.pop();

        // Elbow frame
        stack.mult(t_e * r_E); 
        
        // Elbow
        stack.push(); 
            stack.mult(s_e);
            glUniformMatrix4fv(s["uM"], 1, GL_FALSE, glm::value_ptr(stack.m()));
            glUniform3f(s["uCol"], 0.4, 0.3, 0.5);
            glDrawElements(quad().mode, quad().count, quad().itype, NULL);
        stack.pop();

        // Forearm
        stack.push();
            stack.mult(t_f * s_f);
            glUniformMatrix4fv(s["uM"], 1, GL_FALSE, glm::value_ptr(stack.m()));
            glUniform3f(s["uCol"], 0.4, 0.2, 0.5);
            glDrawElements(quad().mode, quad().count, quad().itype, NULL);
        stack.pop();

        // Wrist frame
        stack.mult(t_w * r_W); 
        
        // Wrist
        stack.push(); 
            stack.mult(s_w);
            glUniformMatrix4fv(s["uM"], 1, GL_FALSE, glm::value_ptr(stack.m()));
            glUniform3f(s["uCol"], 0.4, 0.5, 0.0);
            glDrawElements(quad().mode, quad().count, quad().itype, NULL);
        stack.pop();


        // ----------------------------------------------------------------------------------------------------------------------------------------------


        // Knuckle frame 1
        stack.push(); 
            stack.mult(t_k1 * r_k1); 
            
            // Knuckle
            stack.push(); 
                stack.mult(s_k1);
                glUniformMatrix4fv(s["uM"], 1, GL_FALSE, glm::value_ptr(stack.m()));
                glUniform3f(s["uCol"], 0.8, 0.3, 0.2);
                glDrawElements(quad().mode, quad().count, quad().itype, NULL);
            stack.pop();
            
            // Finger
            stack.push(); 
                stack.mult(t_f1 * s_f1);
                glUniformMatrix4fv(s["uM"], 1, GL_FALSE, glm::value_ptr(stack.m()));
                glUniform3f(s["uCol"], 0.8, 0.2, 0.2); 
                glDrawElements(quad().mode, quad().count, quad().itype, NULL);
            stack.pop();
        stack.pop(); 

        // Knuckle frame 2
        stack.push(); 
            stack.mult(t_k2 * r_k2); 

            // Knuckle 2
            stack.push(); 
                stack.mult(s_k2);
                glUniformMatrix4fv(s["uM"], 1, GL_FALSE, glm::value_ptr(stack.m()));
                glUniform3f(s["uCol"], 0.8, 0.3, 0.2);
                glDrawElements(quad().mode, quad().count, quad().itype, NULL);
            stack.pop();
            
            // Finger 2
            stack.push(); 
                stack.mult(t_f2 * s_f2);
                glUniformMatrix4fv(s["uM"], 1, GL_FALSE, glm::value_ptr(stack.m()));
                glUniform3f(s["uCol"], 0.8, 0.2, 0.2); 
                glDrawElements(quad().mode, quad().count, quad().itype, NULL);
            stack.pop();
        stack.pop();

        // Knuckle frame 3
        stack.push(); 
            stack.mult(t_k3 * r_k3); 
            
            // Knuckle 3
            stack.push(); 
                stack.mult(s_k3);
                glUniformMatrix4fv(s["uM"], 1, GL_FALSE, glm::value_ptr(stack.m()));
                glUniform3f(s["uCol"], 0.8, 0.3, 0.2);
                glDrawElements(quad().mode, quad().count, quad().itype, NULL);
            stack.pop();
            
            // Finger 3
            stack.push();
                stack.mult(t_f3 * s_f3);
                glUniformMatrix4fv(s["uM"], 1, GL_FALSE, glm::value_ptr(stack.m()));
                glUniform3f(s["uCol"], 0.8, 0.2, 0.2); 
                glDrawElements(quad().mode, quad().count, quad().itype, NULL);
            stack.pop();
        stack.pop(); 

        
        stack.pop(); 

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
