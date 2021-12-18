/*
* Dependencies:
*   1) ../glew-2.1.0-win32
*   2) ../glfw-3.3.5.bin.WIN64
*/






#pragma warning(disable : 4996)






#include "HA_utils.h"

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <gl/glew.h>
#include <gl/glfw3.h>






#define DEBUG
#define WORD_SIZE       64
#define LINE_SIZE       256
#define SHADERS_FN      "res/shaders/basic.shader"







typedef struct {
    GLfloat x, y;
} Point2d;

typedef struct {
    Point2d p1, p2, p3;
} Triangle;






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

/*
* Read shader from stream f
* 
* Return shader string allocated on the heap;
* Return NULL on failure;
*/
char *readShader(FILE *f);






int main(int argc, char* argv[])
{
    createWindowWithGLEWAndGLFW();

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
    FILE *shaders_file;
    char *vs; /* vertex shader */
    char *fs; /* fragment shader */
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

    /* Read shader strings from file */
    shaders_file = fopen(SHADERS_FN, "r");
    vs = readShader(shaders_file);
    fs = readShader(shaders_file);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(a), a, GL_STATIC_DRAW);

    /* Array of Points for each Triangle */
    glVertexAttribPointer(
        pointattr,
        sizeof(Point2d) / sizeof(((Point2d*)0)->x),   /* How much members are in the attribute */
        GL_FLOAT,
        GL_FALSE,
        sizeof(Point2d),                    /* Where to find next vertex attribute*/
        (void *) offsetof(Triangle, p1)     /* Starting Point for vertex attribute */
    );
    glEnableVertexAttribArray(pointattr);

    program = createProgram(vs, fs);
    glUseProgram(program);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDeleteProgram(program);
    free(vs);
    free(fs);
    fclose(shaders_file);
}


/*
* Read shader from stream f
* 
* Return shader string allocated on the heap;
* Return NULL on failure;
*/
char *readShader(FILE* f)
{
    fpos_t ip, sp; /* initial position, shader position in the file */
    char word[WORD_SIZE];
    char *shader; /* Shader string on the heap */
    char *tmp; /* For shader reallocation */
    size_t sz_shader; /* Number of bytes allocated for shader */
    size_t len_shader; /* Number of characters stored in shader */

    strcpy(word,  "");
    shader = NULL;
    sz_shader = 0;
    len_shader = 0;
    fgetpos(f, &ip); /* Store Position */

    while (fscanf(f, "%s", word) != EOF) { /* Find where the "#shader" begins */
        HA_skipLine(f); /* Skip to the next line */
        if (strcmp(word, "#shader") == 0) /* compare equal */
            break;
    }

    if (strcmp(word, "#shader") != 0) /* word "#shader" not found */
        goto HANDLE_READ_FAILURE;

    shader = malloc(sz_shader = LINE_SIZE); /* Construct string shader */
    if (!shader)
        goto HANDLE_READ_FAILURE;

    for ( ; ; ) {
        
        if (fgets(shader, sz_shader - len_shader, f) != shader) /* Read The Line into shader */
            break;
        fgetpos(f, &sp); /* Store file position */
        len_shader += strlen(shader);
        shader += strlen(shader); /* Go to trailing '\0' */

        if (len_shader >= sz_shader / 2) { /* Reallocate shader if len_shader >= sz_shader / 2 */
            shader -= len_shader; /* Restore proper memory location */
            tmp = realloc(shader, sz_shader * 2);
            if (!tmp) {
                free(shader);
                goto HANDLE_READ_FAILURE;
            }
            sz_shader *= 2;
            shader = tmp + len_shader;
        }

        if ( /* Restore file position and break if new #shader found */
            fscanf(f, "%s", word) == 1
            && strcmp(word, "#shader") == 0
        ) {
            fsetpos(f, &sp);
            break;
        }

        fsetpos(f, &sp);

    }

    shader -= len_shader; /* Restore proper memory location */
    tmp = realloc(shader, len_shader + 1); /* don't forget about trailing '\0' */
    if (tmp)
        shader = tmp;
    return shader;


    HANDLE_READ_FAILURE:
    fsetpos(f, &ip); /* Restore Position */
    return NULL;
}