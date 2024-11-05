#include "..\Headers\SolarFuel.hpp"



const glm::mat4 SolarFuel::Graphics::Camera::GetViewMatrix() const
{
	return glm::rotate(glm::mat4(1.0f), glm::radians(Angle), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(-Position.x, -Position.y, 0.0f));
}

const glm::mat4 SolarFuel::Graphics::Camera::GetProjectionMatrix(const float _AspectRatio) const
{
	return glm::ortho(-FieldOfView / 2.0f * _AspectRatio, FieldOfView / 2.0f * _AspectRatio, -FieldOfView / 2.0f, FieldOfView / 2.0f);
}



void SolarFuel::Graphics::Renderer::Init()
{
	Destroy();

	std::vector<float> VBO =
	{
		-0.5f, -0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 1.0f, 0.0f
	};

	std::vector<unsigned int> IBO =
	{
		0, 1, 2,
		0, 2, 3
	};

	glGenVertexArrays(1, &QuadVAO);
	glBindVertexArray(QuadVAO);

	glGenBuffers(1, &QuadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, QuadVBO);
	glBufferData(GL_ARRAY_BUFFER, VBO.size() * sizeof(float), VBO.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &QuadIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, QuadIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IBO.size() * sizeof(unsigned int), IBO.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void SolarFuel::Graphics::Renderer::Destroy()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	if (QuadVBO)
	{
		glDeleteBuffers(1, &QuadVBO);
		QuadVBO = 0;
	}
	if (QuadIBO)
	{
		glDeleteBuffers(1, &QuadIBO);
		QuadIBO = 0;
	}
	if (QuadVAO)
	{
		glDeleteVertexArrays(1, &QuadVAO);
		QuadVAO = 0;
	}
}

void SolarFuel::Graphics::Renderer::StartScene(const Camera& _ActiveCamera, const float _AspectRatio)
{
	Projection = _ActiveCamera.GetProjectionMatrix(_AspectRatio);
	View = _ActiveCamera.GetViewMatrix();
	RenderObjects.clear();
}

void SolarFuel::Graphics::Renderer::Submit(const RenderObject& _Object)
{
	RenderObjects.push_back(_Object);
}

void SolarFuel::Graphics::Renderer::Flush()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (const RenderObject& _Object : RenderObjects)
	{
		glm::mat4 _Mvp = Projection * View * _Object.Model;

		glBindVertexArray(QuadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, QuadVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, QuadIBO);

		glUseProgram(_Object.Material->Shader->Id);
		glUniformMatrix4fv(_Object.Material->Shader->MvpId, 1, GL_FALSE, &_Mvp[0][0]);
		glUniform4fv(_Object.Material->Shader->ColorId, 1, &_Object.Material->Color[0]);
		glUniform1iv(_Object.Material->Shader->TextureId, 1, &_Object.Material->Texture);
		glUniform2fv(_Object.Material->Shader->TextureSizeId, 1, &_Object.Material->TextureSize[0]);
		glUniform2fv(_Object.Material->Shader->TexturePositionId, 1, &_Object.Material->TexturePosition[0]);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}

	Projection = glm::mat4(1.0f);
	View = glm::mat4(1.0f);
	RenderObjects.clear();
}
