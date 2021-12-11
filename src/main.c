/*
* Dependencies:
*   1) ../glew-2.1.0-win32
*   2) ../glfw-3.3.5.bin.WIN64
*/





#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include <gl/glew.h>
#include <gl/glfw3.h>






#define DEBUG






typedef struct {
    GLfloat x, y;
} Point2d;

typedef struct {
    Point2d p1, p2, p3;
} Triangle;






static const char* vertex_shader =
    "#version 330 core\n"
    "layout(location = 0) in vec4 position;\n"
    "void main()\n"
    "{    gl_Position = position;    }\n";
static const char* fragment_shader =
    "#version 330 core\n"
    "out vec4 color;\n"
    "void main()\n"
    "{    color = vec4(0.0, 1.0, 0.0, 1.0);    }\n";






/*
* Use GLEW to load OpenGL functions from
* GPU drivers.
* Use GLFW to create a Window and drawing context.
*/
int createWindowWithGLEWAndGLFW(void);

/*
* Create and compile shader of type shader_type from shader_str
*
* Return id of the shader.
*/
GLuint createShader(const char* shader_str, GLenum shader_type);

/*
* Create a shader program.
*
* vshader_str - vertex shader source string
* fshader_str - fragment shader source string
*
* Return id of the shader.
*/
GLuint createProgram(const char* vshader_str, const char* fshader_str);

/* Draw Triangle old way */
void drawSimpleTriangle(void);

/* Draw Triangle using modern OpenGL */
void drawModernTriangle(void);






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

        glClear(GL_COLOR_BUFFER_BIT);

        //drawSimpleTriangle();
        drawModernTriangle();

        /*
        *   Rendering Ends Here
        */

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}




/*
* Create and compile shader of type shader_type from shader_str
*
* Return id of the shader.
*/
GLuint createShader(const char* shader_str, GLenum shader_type)
{
    GLuint id;

#   ifdef DEBUG
        GLint param;
        GLchar* msg;
#   endif /* ifdef DEBUG */

    id = glCreateShader(shader_type);
    glShaderSource(id, 1, &shader_str, NULL);
    glCompileShader(id);

#   ifdef DEBUG
        glGetShaderiv(id, GL_COMPILE_STATUS, &param);
        if (param == GL_FALSE) {
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &param);
            msg = malloc(param);
            glGetShaderInfoLog(id, param, &param, msg);
            fprintf(stderr, "Error: shader compilation failed. Message: %s\n", msg);
            free(msg);
            glDeleteShader(id);
            id = 0;
        }
#   endif /* ifdef DEBUG */

    return id;
}

/*
* Create a shader program.
*
* vshader_str - vertex shader source string
* fshader_str - fragment shader source string
*
* Return id of the shader.
*/
GLuint createProgram(const char* vshader_str, const char* fshader_str)
{
    GLuint program;
    GLuint vs;
    GLuint fs;

    program = glCreateProgram();
    vs = createShader(vshader_str, GL_VERTEX_SHADER);
    fs = createShader(fshader_str, GL_FRAGMENT_SHADER);
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

#   ifdef DEBUG
    glValidateProgram(program);
    /* TODO: check validation status */
#   endif

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}


/* Draw Triangle old way */
void drawSimpleTriangle(void)
{
    static const float points[] = {
         0.0,  0.5,
         0.5, -0.5,
        -0.5, -0.5
    };
    glBegin(GL_TRIANGLES);
    glVertex2d(points[0], points[1]);
    glVertex2d(points[2], points[3]);
    glVertex2d(points[4], points[5]);
    glEnd();
}


/* Draw Triangle using modern OpenGL */
void drawModernTriangle(void)
{
    static const Triangle a[2] = {
        {
            -0.25f, 0.50f,
             0.00f, 0.00f,
            -0.50f, 0.00f
        },
        {
            0.25f, -0.50f,
            0.50f,  0.00f,
            0.00f,  0.00f
        }
    };
    static const GLuint pointattr = 0U;     /* Point Attribute Index */
    GLuint vbo;                             /* Vertex Buffer Object index */
    GLuint program;


    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(a), a, GL_STATIC_DRAW);

    /* Array of Points for each Triangle */
    glVertexAttribPointer(
        pointattr,
        sizeof(Point2d) / sizeof(((Point2d*)0)->x),   /* How much members are in the attribute */
        GL_FLOAT,
        GL_FALSE,
        sizeof(Point2d),            /* Where to find next vertex attribute*/
        offsetof(Triangle, p1)      /* Starting Point for vertex attribute */
    );
    glEnableVertexAttribArray(pointattr);

    program = createProgram(vertex_shader, fragment_shader);
    glUseProgram(program);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDeleteProgram(program);
}