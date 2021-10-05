#include <reskinner/StatusManager.h>

/*
StatusManager::StatusManager(GLFWwindow* const window, Animator& const animator, Camera& const camera, float screenWidth, float screenHeight) : window(window), animator(animator), camera(camera), screenWidth(screenWidth), screenHeight(screenHeight)
{
	screenAspectRatio = screenWidth / screenHeight;
}
*/

StatusManager::StatusManager(Animator* animator, const glm::vec3& cameraPos, float screenWidth, float screenHeight, const std::string& windowName) : animator(animator), screenWidth(screenWidth), screenHeight(screenHeight)
{
	Camera cam(cameraPos);
	camera = &cam;
	screenAspectRatio = screenWidth / screenHeight;
	lastFrame = glfwGetTime();
	deltaTime = 0.0f;
	mouseLastPos = glm::vec2(0.0f, 0.0f);
	InitializeWindow(windowName);
	//init status
	status[WIREFRAME_KEY_PRESSED] = false;
	status[WIREFRAME] = false;
	status[HIDDEN_LINE_KEY_PRESSED] = false;
	status[HIDDEN_LINE] = true;
	status[ROTATE] = false;
	status[PAUSE_KEY_PRESSED] = false;
	status[PAUSE] = false;
	status[BAKED_MODEL] = false;
}

bool StatusManager::IsRotationLocked() const
{
	return status[ROTATE];
}

void StatusManager::ResizeWindow(float width, float height)
{
	screenWidth = width;
	screenHeight = height;
	screenAspectRatio = width / height;
	glViewport(0, 0, width, height);
}

void StatusManager::processInput()
{
	// switch wireframe mode
	if (!status[WIREFRAME_KEY_PRESSED] && glfwGetKey(window, WIREFRAME_KEY) == GLFW_PRESS)
		status[WIREFRAME] = !status[WIREFRAME];
	// switch hidden line mode
	if (!status[HIDDEN_LINE_KEY_PRESSED] && glfwGetKey(window, HIDDEN_LINE_KEY) == GLFW_PRESS)
		status[HIDDEN_LINE] = !status[HIDDEN_LINE];
	// pause/unpause the animation
	if (!status[PAUSE_KEY_PRESSED] && !status[BAKED_MODEL] && glfwGetKey(window, PAUSE_KEY) == GLFW_PRESS)
		status[PAUSE] = !status[PAUSE];
	// switch animation
	//if (!status[SWITCH_ANIMATION_KEY_PRESSED] && glfwGetKey(window, SWITCH_ANIMATION_KEY) == GLFW_PRESS)
	//	animator.
	// bake the model
	if (!status[BAKED_MODEL] && glfwGetKey(window, BAKE_MODEL_KEY) == GLFW_PRESS) {
		status[PAUSE] = true;
		status[BAKED_MODEL] = true;
	}
	// reset the camera pos
	if (glfwGetKey(window, ROTATE_KEY) == GLFW_PRESS) {
		camera->Reset();
	}
	// move the camera pos
	if (glfwGetKey(window, CAMERA_UP_KEY) == GLFW_PRESS)
		camera->ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, CAMERA_DOWN_KEY) == GLFW_PRESS)
		camera->ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, CAMERA_LEFT_KEY) == GLFW_PRESS)
		camera->ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, CAMERA_RIGHT_KEY) == GLFW_PRESS)
		camera->ProcessKeyboard(RIGHT, deltaTime);

	// reset input key statuts if released
	status[WIREFRAME_KEY_PRESSED] = glfwGetKey(window, WIREFRAME_KEY) == GLFW_PRESS;
	status[HIDDEN_LINE_KEY_PRESSED] = glfwGetKey(window, HIDDEN_LINE_KEY) == GLFW_PRESS;
	status[PAUSE_KEY_PRESSED] = glfwGetKey(window, PAUSE_KEY) == GLFW_PRESS;
	// rotate the camera
	status[ROTATE] = glfwGetKey(window, ROTATE_KEY) == GLFW_PRESS;
}

bool StatusManager::IsAnimated() const
{
	return status[PAUSE];
}

bool StatusManager::IsBaked() const
{
	return status[BAKED_MODEL];
}

void StatusManager::InitializeWindow(const std::string& windowName) {
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	window = glfwCreateWindow(screenWidth, screenHeight, windowName.c_str(), NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
}