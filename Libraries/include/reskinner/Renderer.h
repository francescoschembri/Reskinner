#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include <reskinner/Shader.h>
#include <reskinner/Model.h>
#include <reskinner/Camera.h>
#include <reskinner/StatusManager.h>
#include <reskinner/Animator.h>

class Renderer {
public:
	// initialize the Rendered
	Renderer(const Shader& shader, const StatusManager& const s);
	// draws the frame
	void Render(const Model& model);
	// add a shaders that can be used by the renderer
	void AddShader(const Shader& shader);
	// add multiple shaders that can be used by the renderer
	void AddShaders(const std::vector<Shader>& shaders);
	// switch the current Shader with the next one in shaders
	void SwitchShader();
	// change the current Shader to the Shader in shaders[index]
	void ChangeShader(int index);

private:
	int currentShaderIndex = 0;
	std::vector<Shader> shaders;
	Shader currentShader;
	const StatusManager& const status;

	// draws the given model, and thus all its meshes
	void DrawModel(const Model& model, const bool faces = true, const bool lines = false) const;
	// draws the given mesh
	void DrawMesh(const Mesh& mesh, const bool faces = true, const bool lines = false) const;
	// sets model view projection matrices;
	void SetModelViewProjection() const;
	// pass bones matrices to the shader
	void SetBoneMatrices() const;
};