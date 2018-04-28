#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <shader.hpp>

#include <stdlib.h>
#include <stdio.h>

#include <iostream> // for cout and endl which we can use for debugging

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <time.h> // for get_nanos

using namespace glm;

// The get_nanos function was taken from:  
//     https://stackoverflow.com/questions/361363/how-to-measure-time-in-milliseconds-using-ansi-c/36095407#36095407
static long get_nanos(void) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (long)ts.tv_sec * 1000000000L + ts.tv_nsec;
}

// State machine for pressed buttons
static bool incPressed[6] = { 0 }; // init button state to false
static bool decPressed[6] = { 0 }; // init button state to false
static long old_t = get_nanos();
glm::vec3 camera_loc(0.0f, 5.0f, 20.0f);

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    int keys = 6;
    int upKeys[6] = {
              GLFW_KEY_1,
              GLFW_KEY_2,
              GLFW_KEY_3,
              GLFW_KEY_4,
              GLFW_KEY_5,
              GLFW_KEY_6};
    int downKeys[6] = {
              GLFW_KEY_Q,
              GLFW_KEY_W,
              GLFW_KEY_E,
              GLFW_KEY_R,
              GLFW_KEY_T,
              GLFW_KEY_Y};

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    for (int i=0; i < keys; i++){
        if (key == upKeys[i] && action == GLFW_PRESS){
            incPressed[i] = true;
        }
        if (key == upKeys[i] && action == GLFW_RELEASE){
            incPressed[i] = false;
        }
        if (key == downKeys[i] && action == GLFW_PRESS){
            decPressed[i] = true;
        }
        if (key == downKeys[i] && action == GLFW_RELEASE){
            decPressed[i] = false;
        }
    }
}
static void recalcCameraLoc(long dt){
    // first and only movement mode - fps style(hold key for constant velocity
    float unitsPerSec = 8;
    float distance = dt / 1000000000.0f * unitsPerSec;
    if (incPressed[0] == true){
        camera_loc.x += distance;
    }
    if (incPressed[1] == true){
        camera_loc.y += distance;
    }
    if (incPressed[2] == true){
        camera_loc.z += distance;
    }
    if (decPressed[0] == true){
        camera_loc.x -= distance;
    }
    if (decPressed[1] == true){
        camera_loc.y -= distance;
    }
    if (decPressed[2] == true){
        camera_loc.z -= distance;
    }
    // Todo: add other movement modes like velocity control in an air plane
}

static const GLfloat g_vertex_buffer_data[] = {
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    -10.0f, -1.0f, -10.0f, // begin floor square
     10.0f, -1.0f,  10.0f,
     10.0f, -1.0f, -10.0f,
    -10.0f, -1.0f, -10.0f,
    -10.0f, -1.0f,  10.0f,
     10.0f, -1.0f,  10.0f,
};
static const GLfloat g_color_buffer_data[] = {
    0.583f,  0.771f,  0.014f,
    0.609f,  0.115f,  0.436f,
    0.327f,  0.483f,  0.844f,
    0.822f,  0.569f,  0.201f,
    0.435f,  0.602f,  0.223f,
    0.310f,  0.747f,  0.185f,
    0.597f,  0.770f,  0.761f,
    0.559f,  0.436f,  0.730f,
    0.359f,  0.583f,  0.152f,
    0.483f,  0.596f,  0.789f,
    0.559f,  0.861f,  0.639f,
    0.195f,  0.548f,  0.859f,
    0.014f,  0.184f,  0.576f,
    0.771f,  0.328f,  0.970f,
    0.406f,  0.615f,  0.116f,
    0.676f,  0.977f,  0.133f,
    0.971f,  0.572f,  0.833f,
    0.140f,  0.616f,  0.489f,
    0.997f,  0.513f,  0.064f,
    0.945f,  0.719f,  0.592f,
    0.543f,  0.021f,  0.978f,
    0.279f,  0.317f,  0.505f,
    0.167f,  0.620f,  0.077f,
    0.347f,  0.857f,  0.137f,
    0.055f,  0.953f,  0.042f,
    0.714f,  0.505f,  0.345f,
    0.783f,  0.290f,  0.734f,
    0.722f,  0.645f,  0.174f,
    0.302f,  0.455f,  0.848f,
    0.225f,  0.587f,  0.040f,
    0.517f,  0.713f,  0.338f,
    0.053f,  0.959f,  0.120f,
    0.393f,  0.621f,  0.362f,
    0.673f,  0.211f,  0.457f,
    0.820f,  0.883f,  0.371f,
    0.982f,  0.099f,  0.879f,
    0.5f, 0.5f, 0.5f, // begin grey floor square
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
};


int main(void)
{
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
	getchar();
	glfwTerminate();
	return -1;
    }
    glEnable(GL_DEPTH_TEST); // use z buffer to only show close fragments
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE); // enable backface culling

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f); // bg = dark blue

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLuint programID = LoadShaders( "Vertex.glsl",
                                    "Fragment.glsl" );

    GLuint MatrixID = glGetUniformLocation(programID, "MVP");


    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    glm::mat4 View = glm::lookAt(
        glm::vec3(camera_loc.x, camera_loc.y, camera_loc.z),
        glm::vec3(camera_loc.x,camera_loc.y-5.0f,camera_loc.z-20.0f),
        glm::vec3(0,1,0)  // up vector
    );
    glm::mat4 Model = glm::mat4(1.0f);
    glm::mat4 mvp = Model * Projection * View * Model;

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),
                 g_vertex_buffer_data, GL_STATIC_DRAW);

    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    // bind, buffer, and draw in the main loop if you want
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data),
                 g_color_buffer_data, GL_STATIC_DRAW);


    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);

        long t = get_nanos();
        long dt = t - old_t;
        old_t = t;

        recalcCameraLoc(dt);

        glm::mat4 View = glm::lookAt(
            glm::vec3(camera_loc.x, camera_loc.y, camera_loc.z),
            glm::vec3(camera_loc.x,camera_loc.y-5.0f,camera_loc.z-20.0f),
            glm::vec3(0,1,0)  // up vector
        );
        mvp = Model * Projection * View * Model;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

        glEnableVertexAttribArray(0); // 0th attrib means verticies
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
        );
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
            1,                                // match layout in shader
            3,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );
        glDrawArrays(GL_TRIANGLES, 0, 3*14);
        glDisableVertexAttribArray(0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

