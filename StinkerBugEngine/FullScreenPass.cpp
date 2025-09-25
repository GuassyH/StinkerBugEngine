#include "FullScreenPass.h"

#include "SceneManager.h"
#include "Scene.h"

Display& display = Display::getInstance();
FullScreenPass::FullScreenPass(Material& material) : material(&material) {
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

	FullScreenPass::material->RemoveFlag(MaterialFlags_Lit);
	FullScreenPass::material->RemoveFlag(MaterialFlags_Depth);
	FullScreenPass::material->AddFlag(MaterialFlags_FullscreenPass);
	
	for (auto& vert : vertices) {
		vert.pos.z = -0.1f;
	}
}

void FullScreenPass::Draw(Camera& camera, Light* light, Transform* l_transform) {
	material->shader.Use();



	glUniform1i(glGetUniformLocation(material->shader.ID, "screenWidth"), camera.width);
	glUniform1i(glGetUniformLocation(material->shader.ID, "screenHeight"), camera.height);

	glUniform3f(glGetUniformLocation(material->shader.ID, "camUp"), camera.localUp.x, camera.localUp.y, camera.localUp.z);
	glUniform3f(glGetUniformLocation(material->shader.ID, "camForward"), camera.forward.x, camera.forward.y, camera.forward.z);
	glUniform3f(glGetUniformLocation(material->shader.ID, "camRight"), camera.right.x, camera.right.y, camera.right.z);
	glUniform1f(glGetUniformLocation(material->shader.ID, "FOVdeg"), camera.FOVdeg);
	glUniform3f(glGetUniformLocation(material->shader.ID, "camPos"), camera.transform->position.x, camera.transform->position.y, camera.transform->position.z);

	glUniform1f(glGetUniformLocation(material->shader.ID, "camNearPlane"), camera.nearPlane);
	glUniform1f(glGetUniformLocation(material->shader.ID, "camFarPlane"), camera.farPlane);

	glm::vec3 l_dir;
	glm::vec3 l_col;

	if (light) {
		glm::vec3 direction;
		float pitch = glm::radians(l_transform->rotation.x);
		float yaw = glm::radians(l_transform->rotation.y);

		direction.x = cos(pitch) * cos(yaw);
		direction.y = sin(pitch);
		direction.z = cos(pitch) * sin(yaw);
	
		l_dir = direction;
		l_col = light->color;
	}
	else {
		l_dir = glm::vec3(0.0f, -1.0f, 0.0f);
		l_col = glm::vec3(1.0f, 1.0f, 1.0f);
	}
	
	glUniform3f(glGetUniformLocation(material->shader.ID, "sunDir"), l_dir.x, l_dir.y, l_dir.z);
	glUniform4f(glGetUniformLocation(material->shader.ID, "sunColor"), l_col.r, l_col.g, l_col.b, 1.0f);

	glDisable(GL_DEPTH_TEST);

	VAO1.Bind();
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);

	glEnable(GL_DEPTH_TEST);
}