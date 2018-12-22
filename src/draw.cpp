// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/common.hpp>
#include <complex.hpp>
#include <draw.hpp>

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

BOOL mouse_clicked = FALSE;
BOOL key_pressed = FALSE;
BOOL redraw = FALSE;

extern double clicked_x;
extern double clicked_y;

GLuint VertexArrayID;
GLuint programID;
GLuint vertexbuffer;
GLuint colorbuffer;

GLdouble* g_vertex_buffer_data;
GLdouble* g_color_buffer_data;

int init(long size)
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// Open a window and create its OpenGL context
	window = glfwCreateWindow( X_RES, Y_RES, "Mandelbrot Set", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetKeyCallback(window, key_callback);
   	
	glEnable(GL_PROGRAM_POINT_SIZE);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	programID = LoadShaders( "shaders/SimpleVertexShader.vertexshader", "shaders/SimpleFragmentShader.fragmentshader" );

	g_vertex_buffer_data = (GLdouble*)malloc(3 * size * sizeof(GLdouble));
	g_color_buffer_data = (GLdouble*)malloc(3 * size * sizeof(GLdouble));
}

int draw(complex_number* points, rgb_color* colors, long size)
{
	convert_points_to_ogl_array(points, g_vertex_buffer_data, size);
	convert_colors_to_ogl_array(colors, g_color_buffer_data, size);

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, 3 * size * sizeof(GLdouble), g_vertex_buffer_data, GL_STATIC_DRAW);
	
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, 3 * size * sizeof(GLdouble), g_color_buffer_data, GL_STATIC_DRAW);

	do
	{	
		// Clear the screen
		glClear( GL_COLOR_BUFFER_BIT );

		// Use our shader
		glUseProgram(programID);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_DOUBLE,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
	
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
		    	1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		    	3,                                // size
		    	GL_DOUBLE,                         // type
		    	GL_FALSE,                         // normalized?
		    	0,                                // stride
		    	(void*)0                          // array buffer offset
		);

		glDrawArrays(GL_POINTS, 0, size); // 3 indices starting at 0 -> 1 triangle
	
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
	
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} while(mouse_clicked != TRUE && key_pressed != TRUE);
	
	mouse_clicked = FALSE;
	key_pressed = FALSE;

	return 0;
}

int finalize()
{
	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteBuffers(1, &colorbuffer);

	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	
	free(g_vertex_buffer_data);
	free(g_color_buffer_data);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    	{
		mouse_clicked = TRUE;
		redraw = TRUE;

		glfwGetCursorPos(window, &clicked_x, &clicked_y);
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    	{
		key_pressed = TRUE;
		redraw = FALSE;
	}
}

int convert_points_to_ogl_array(complex_number* points, GLdouble* ogl_array, long size)
{
	if(points == NULL || ogl_array == NULL)
	{
		abort("Null arrays.\n");
	}

	long i;
	for(i = 0; i < size; i++)
	{
		ogl_array[3*i] = points[i].real;
		ogl_array[3*i + 1] = points[i].imag;
		ogl_array[3*i + 2] = 0.0;
		//printf("%lf %lf %lf\n", ogl_array[3*i], ogl_array[3*i + 1], ogl_array[3*i + 2]);
	}

	return RET_SUCCESS;
}

int convert_colors_to_ogl_array(rgb_color* colors, GLdouble* ogl_array, long size)
{
	if(colors == NULL || ogl_array == NULL)
	{
		abort("Null arrays.\n");
	}

	long i;
	for(i = 0; i < size; i++)
	{
		ogl_array[3*i] = colors[i].r;
		ogl_array[3*i + 1] = colors[i].g;
		ogl_array[3*i + 2] = colors[i].b;
		//printf("%lf\n", ogl_array[3*i]);
	}

	return RET_SUCCESS;
}

