/*
Student Information
Student ID: 1155100532
Student Name: CHOI, Jun Seok
*/

#include <algorithm>

#include "Skybox.h"
#include "ShadowMap.h"
#include "Scene.h"

// screen setting
const int SCR_WIDTH = 1920;
const int SCR_HEIGHT = 1080;

// time
float lastTime = 0.0f;
float timestep = 10.0f;

// Mouse control
struct MouseController
{
	bool IS_FIRST = true;
	bool LEFT_BUTTON = false;
	bool RIGHT_BUTTON = false;
	double currentX = 0.0, currentY = 0.0;
	double lastX = 0.0, lastY = 0.0;
};
MouseController mouseCtl;

// globals
Scene* g_SceneManager = new Scene;




void get_OpenGL_info()
{
	// OpenGL information
	const GLubyte* name = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* glversion = glGetString(GL_VERSION);
	std::cout << "OpenGL company: " << name << std::endl;
	std::cout << "Renderer name: " << renderer << std::endl;
	std::cout << "OpenGL version: " << glversion << std::endl;
}

void sendDataToOpenGL()
{
	g_SceneManager->Create();
}

void initializedGL(void) //run only once
{
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW not OK." << std::endl;
	}

	get_OpenGL_info();
	sendDataToOpenGL();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void paintGL(void)  //always run
{
	float currentTime = glfwGetTime();
	timestep = currentTime - lastTime;
	lastTime = currentTime;

	g_SceneManager->Render();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
}

void cursor_position_callback(GLFWwindow* window, double x, double y)
{
	if (abs(mouseCtl.lastX - x) > 200.0)
	{
		mouseCtl.lastX = x;
		mouseCtl.lastY = y;
		mouseCtl.IS_FIRST = false;
	}
	float offsetX = x - mouseCtl.lastX;
	mouseCtl.lastX = x;
	
	glm::vec3 rotation = g_SceneManager->GetCamera()->GetRotation();
	rotation.y += offsetX * 0.05f;
	g_SceneManager->GetCamera()->SetRotation(rotation);

	glm::vec3 direction = g_SceneManager->GetCamera()->GetDirection();
	direction.x = cos(glm::radians(rotation.y));
	direction.z = sin(glm::radians(rotation.y));
	g_SceneManager->GetCamera()->SetDirection(glm::normalize(direction));
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	// Sets the scoll callback for the current window.
	float speedFactor = 200.0f;
	float fov = g_SceneManager->GetCamera()->GetFOV();
	fov = glm::clamp(fov - (float)yoffset * timestep * speedFactor, 1.0f, 90.0f);
	g_SceneManager->GetCamera()->SetFOV(fov);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Sets the Keyboard callback for the current window.

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// Object control
	float speed = 20.0f;
	if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		glm::vec3 position = g_SceneManager->GetCamera()->GetPosition();
		glm::vec3 direction = g_SceneManager->GetCamera()->GetDirection();
		position += direction * timestep * speed;
		g_SceneManager->GetCamera()->SetPosition(position);
	}
	if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		glm::vec3 position = g_SceneManager->GetCamera()->GetPosition();
		glm::vec3 direction = g_SceneManager->GetCamera()->GetDirection();
		position -= direction * timestep * speed;
		g_SceneManager->GetCamera()->SetPosition(position);
	}
	if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		glm::vec3 position = g_SceneManager->GetCamera()->GetPosition();
		glm::vec3 direction = g_SceneManager->GetCamera()->GetDirection();
		glm::vec3 left = glm::normalize(glm::cross(glm::vec3(0, 1, 0), direction));
		position += left * timestep * speed;
		g_SceneManager->GetCamera()->SetPosition(position);
	}
	if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		glm::vec3 position = g_SceneManager->GetCamera()->GetPosition();
		glm::vec3 direction = g_SceneManager->GetCamera()->GetDirection();
		glm::vec3 right = glm::normalize(glm::cross(direction, glm::vec3(0, 1, 0)));
		position += right * timestep * speed;
		g_SceneManager->GetCamera()->SetPosition(position);
	}

	// light control
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		g_SceneManager->ToggleDirLightFlag();
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		g_SceneManager->TogglePointLightFlag();
	}
}


int main(int argc, char* argv[])
{
	GLFWwindow* window;

	/* Initialize the glfw */
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}
	/* glfw: configure; necessary for MAC */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Course Project", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/*register callback functions*/
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);                                                                  //    
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	initializedGL();

	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		paintGL();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}






