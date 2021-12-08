/*
* Dependencies:
*   1) ../glew-2.1.0-win32
*   2) ../glfw-3.3.5.bin.WIN64
*/






#include <gl/glew.h>
#include <gl/glfw3.h>






/*
* 1 - Setting up
* 
* Uses Windows outdated API to OpenGL
* to draw a triangle.
*/
int first(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* My Code */
        {
            glBegin(GL_TRIANGLES);
            glVertex2d(-0.5, -0.5);
            glVertex2d(0, 0.5);
            glVertex2d(0.5, -0.5);
            glEnd();
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
}


/*
* 2 - ...
*/






int main(int argc, char* argv[])
{


    if (first())
        return -1;

    return 0;
}