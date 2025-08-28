#include "FullScreenPass.h"

#include "SceneManager.h"
#include "Scene.h"

Display& display = Display::getInstance();
FullScreenPass::FullScreenPass(Camera& camera, Material& material) : material(&material) {
	VAO1 = VAO();
	VBO1 = VBO();
	EBO1 = EBO();

	VBO1.BindBufferData(sizeof(Vertex) * vertices.size(), vertices.data());

	VAO1.LinkAttrib(0, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, pos));
	VAO1.LinkAttrib(1, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, normal));
	VAO1.LinkAttrib(2, 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, texCoords));

	EBO1.BindBufferData(indices.size() * sizeof(GLuint), indices.data());

	VBO1.Unbind();
	VAO1.Unbind();
	EBO1.Unbind();

	FullScreenPass::material->Lit = false;
	FullScreenPass::material->Depth = false;
	FullScreenPass::material->fullscreen_pass = true;
	
	for (auto& vert : vertices) {
		vert.pos.z = -camera.nearPlane;
	}
}

void FullScreenPass::Draw(Camera& camera) {
	material->shader.Use();



	glUniform1i(glGetUniformLocation(material->shader.ID, "screenWidth"), display.windowWidth);
	glUniform1i(glGetUniformLocation(material->shader.ID, "screenHeight"), display.windowHeight);

	glUniform3f(glGetUniformLocation(material->shader.ID, "camUp"), camera.localUp.x, camera.localUp.y, camera.localUp.z);
	glUniform3f(glGetUniformLocation(material->shader.ID, "camForward"), camera.forward.x, camera.forward.y, camera.forward.z);
	glUniform3f(glGetUniformLocation(material->shader.ID, "camRight"), camera.right.x, camera.right.y, camera.right.z);
	glUniform1f(glGetUniformLocation(material->shader.ID, "FOVdeg"), camera.FOVdeg);
	glUniform3f(glGetUniformLocation(material->shader.ID, "camPos"), camera.transform->position.x, camera.transform->position.y, camera.transform->position.z);

	glUniform1f(glGetUniformLocation(material->shader.ID, "camNearPlane"), camera.nearPlane);
	glUniform1f(glGetUniformLocation(material->shader.ID, "camFarPlane"), camera.farPlane);

	glm::vec3 l_dir = SceneManager::getInstance().GetActiveScene().light_direction;
	glm::vec3 l_col = SceneManager::getInstance().GetActiveScene().light_color;
	glUniform3f(glGetUniformLocation(material->shader.ID, "sunDir"), l_dir.x, l_dir.y, l_dir.z);
	glUniform4f(glGetUniformLocation(material->shader.ID, "sunColor"), l_col.r, l_col.g, l_col.b, 1.0f);



	glDisable(GL_DEPTH_TEST);

	VAO1.Bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	glEnable(GL_DEPTH_TEST);
}