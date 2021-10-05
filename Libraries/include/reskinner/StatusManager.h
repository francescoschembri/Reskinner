#pragma once

#include <bitset>

#include <glm/glm.hpp>
#include <reskinner/Camera.h>
#include <reskinner/Animator.h>

#include <GLFW/glfw3.h>

constexpr int WIREFRAME_KEY = GLFW_KEY_W;
constexpr int WIREFRAME_KEY_PRESSED = 0;
constexpr int WIREFRAME = 1;
constexpr int HIDDEN_LINE_KEY = GLFW_KEY_H;
constexpr int HIDDEN_LINE_KEY_PRESSED = 2;
constexpr int HIDDEN_LINE = 3;
constexpr int ROTATE_KEY = GLFW_MOUSE_BUTTON_RIGHT;
constexpr int RESET_CAMERA_KEY = GLFW_KEY_R;
constexpr int CAMERA_UP_KEY = GLFW_KEY_UP;
constexpr int CAMERA_DOWN_KEY = GLFW_KEY_DOWN;
constexpr int CAMERA_LEFT_KEY = GLFW_KEY_LEFT;
constexpr int CAMERA_RIGHT_KEY = GLFW_KEY_RIGHT;
constexpr int ROTATE = 5;
constexpr int PAUSE_KEY = GLFW_KEY_P;
constexpr int PAUSE_KEY_PRESSED = 6;
constexpr int PAUSE = 7;
constexpr int BAKE_MODEL_KEY = GLFW_KEY_B;
constexpr int BAKED_MODEL = 10;

class StatusManager
{
public:
	float screenWidth, screenHeight;
	float screenAspectRatio;
	glm::vec2 mouseLastPos;
	GLFWwindow* window = NULL;
	Camera* camera;
	Animator* animator;
	float lastFrame;
	float deltaTime;

	StatusManager(Animator* animator, const glm::vec3& cameraPos = glm::vec3(0.0f, 0.0f, 3.0f), float screenWidth = 800.0f, float screenHeight = 800.0f, const std::string& windowName = "Reskinner");

	bool IsRotationLocked() const;
	void ResizeWindow(float screenWidth, float screenHeight);
	void processInput();
	bool IsAnimated() const;
	bool IsBaked() const;
private:
	std::bitset<11> status;

	void InitializeWindow(const std::string& windowName);
	//void SwitchAnimation();
};