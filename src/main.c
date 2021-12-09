/*
* Dependencies:
*   1) ../glew-2.1.0-win32
*   2) ../glfw-3.3.5.bin.WIN64
*/





#include <stdio.h>

#include <gl/glew.h>
#include <gl/glfw3.h>






/*
* Use GLEW to load OpenGL functions from
* GPU drivers.
* Use GLFW to create a Window and drawing context.
*/
int createWindowWithGLEWAndGLFW(void);






int main(int argc, char* argv[])
{
     if (createWindowWithGLEWAndGLFW())
        return -1;

    return 0;
}






/*
* Use GLEW to load OpenGL functions from
* GPU drivers.
* Use GLFW to create a Window and drawing context.
*/
int createWindowWithGLEWAndGLFW(void)
{
    GLFWwindow* window;
    GLenum err;

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

    /* Initialize GLEW (Load OpenGL symbols from GPU drivers) */
    glewExperimental = GL_TRUE;
    err = glewInit();
    if (err != GLEW_OK) {
        /* GLEW init failed */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        glfwTerminate();
        return -1;
    }
    fprintf(stdout, "Using GLEW %s\n", glewGetString(GLEW_VERSION));
    fprintf(stdout, "Using OpenGL %s\n", glGetString(GL_VERSION));

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /*
        *   Rendering Begins Here
        */

        /* Draw Triangle old way */
        //{
        //    const float points[] = {
        //         0.0,  0.5, 0.0,
        //         0.5, -0.5, 0.0,
        //        -0.5, -0.5, 0.0
        //    };
        //    glBegin(GL_TRIANGLES);
        //    glVertex2d(points[0], points[1]);
        //    glVertex2d(points[3], points[4]);
        //    glVertex2d(points[6], points[7]);
        //    glEnd();
        //}

        /* Draw Triangle using modern OpenGL */
        {
                const float points[] = {
                     0.0,  0.5, 0.0,
                     0.5, -0.5, 0.0,
                    -0.5, -0.5, 0.0
                };
                GLuint vbo, vao;

                glGenBuffers(1, &vbo);
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

                glGenVertexArrays(1, &vao);
                glBindVertexArray(vao);
                glEnableVertexAttribArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);


        }

        /*
        *   Rendering Ends Here
        */

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
}