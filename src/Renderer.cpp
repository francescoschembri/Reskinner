#include <reskinner/Renderer.h>

Renderer::Renderer(const Shader& shader, const StatusManager& const s) : currentShader(shader), status(s)
{
    AddShader(shader);
}

void Renderer::Render(const Model& model)
{
    glClearColor(1.0f, 0.5f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // don't forget to enable shader before setting uniforms
    currentShader.use();
    // sets model view projection matrices;
    SetModelViewProjection();
    // if the model shoul be animated, animate it to update bone matrices
    if (status.IsAnimated())
        status.animator->UpdateAnimation(status.deltaTime);
    if(!status.IsBaked())
        SetBoneMatrices();
    DrawModel(model);
    glfwSwapBuffers(status.window);
}

void Renderer::AddShader(const Shader& shader)
{
    shaders.push_back(shader);
}

void Renderer::AddShaders(const std::vector<Shader>& shaders)
{
    for (const Shader& s : shaders) {
        AddShader(s);
    }
}

void Renderer::DrawModel(const Model& model, const bool faces, const bool lines) const
{
	for (const Mesh& m : model.meshes) {
		DrawMesh(m);
	}
}

void Renderer::SwitchShader()
{
    currentShaderIndex++;
    currentShaderIndex %= shaders.size();
    currentShader = shaders[currentShaderIndex];
}

void Renderer::ChangeShader(int index)
{
    //Meglio con ASSERT?????
    currentShaderIndex = index % shaders.size();
    currentShader = shaders[currentShaderIndex];
}

void Renderer::DrawMesh(const Mesh& mesh, const bool faces, const bool lines) const
{
    // bind appropriate textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;

    int i = 0;
    for (const Texture& t : mesh.textures)
    {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = t.type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++); // transfer unsigned int to stream
        else if (name == "texture_normal")
            number = std::to_string(normalNr++); // transfer unsigned int to stream
        else if (name == "texture_height")
            number = std::to_string(heightNr++); // transfer unsigned int to stream

        // now set the sampler to the correct texture unit
        glUniform1i(glGetUniformLocation(currentShader.ID, (name + number).c_str()), i);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, t.id);
        i++;
    }

    unsigned int VAO = mesh.GetVAO();
    //draw lines
    if (lines) {
        currentShader.use();
        currentShader.wireframeMode(true);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, mesh.faces.size(), GL_UNSIGNED_INT, (void*)0);
    }

    // draw mesh aka faces
    if (faces || !lines) { //we want to draw at least something
        currentShader.use();
        currentShader.wireframeMode(false);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0, 1.0);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, mesh.faces.size(), GL_UNSIGNED_INT, (void*)0);
    }

    // always good practice to set everything back to defaults once configured.
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

void Renderer::SetModelViewProjection() const
{
    currentShader.setMat4("model", glm::mat4(1.0f));
    glm::mat4 projection = glm::perspective(glm::radians(ZOOM), 3.0f, 0.1f, 100.0f); //TODO STATUS ASPECT RATIO
    glm::mat4 view = status.camera->GetViewMatrix();
    currentShader.setMat4("projection", projection);
    currentShader.setMat4("view", view);
}

void Renderer::SetBoneMatrices() const
{
    auto transforms = status.animator->GetFinalBoneMatrices();
    for (int i = 0; i < transforms.size(); i++)
        currentShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
}
