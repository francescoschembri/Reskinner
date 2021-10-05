#include <reskinner/Shader.h>
#include <reskinner/Camera.h>
#include <reskinner/StatusManager.h>
#include <reskinner/Animator.h>
#include <reskinner/Animation.h>
#include <reskinner/Model.h>
#include <reskinner/Renderer.h>
#include <stb/stb_image.h>

#include <filesystem>
namespace filesystem = std::filesystem;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

StatusManager status(nullptr);

int main() {
    if (status.window == NULL)
        return -1;
    //set callback
    glfwSetFramebufferSizeCallback(status.window, framebuffer_size_callback);
    glfwSetCursorPosCallback(status.window, mouse_callback);
    glfwSetScrollCallback(status.window, scroll_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader ourShader("./Resources/Shaders/animated_model_loading.vs", "./Resources/Shaders/animated_model_loading.fs");

    // generate Texture manager
    // ------------------------
    TextureManager textureManager;

    // load models
    // -----------
    Model ourModel("./Resources/Animations/Capoeira/Capoeira.dae", textureManager);
    // load animations
    // ---------------
    Animation danceAnimation("./Resources/Animations/Capoeira/Capoeira.dae", ourModel);
    Animation flairAnimation("./Resources/Animations/Flair/Flair.dae", ourModel);
    Animation sillyAnimation("./Resources/Animations/Silly Dancing/Silly Dancing.dae", ourModel);
    Animation animations[] = { danceAnimation, flairAnimation, sillyAnimation };
    // make an animator
    Animator animator(animations[0]);
    status.animator = &animator;
    // ma a renderer
    Renderer renderer(ourShader, status);
    // render loop
    // -----------
    while (!glfwWindowShouldClose(status.window))
    {
        status.processInput();
        renderer.Render(ourModel);
        glfwPollEvents();
    }
	return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    status.ResizeWindow((float)width, (float)height);
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (status.IsRotationLocked()) {
        float xoffset = xpos - status.mouseLastPos.x;
        float yoffset = status.mouseLastPos.y - ypos; // reversed since y-coordinates go from bottom to top

        status.camera->ProcessMouseMovement(xoffset, yoffset);
    }
    else {
        status.mouseLastPos.x = xpos;
        status.mouseLastPos.y = ypos;
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    status.camera->ProcessMouseScroll(yoffset);
}