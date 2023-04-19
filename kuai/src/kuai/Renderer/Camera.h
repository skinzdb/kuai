#pragma once

#include <memory>

#include "Framebuffer.h"

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

namespace kuai {
    class Camera
    {
    public:
        enum class ProjectionType
		{
			Perspective,
			Ortho
		};

		Camera(float fov, float width, float height, float zNear, float zFar) : aspect(width / height) 
		{ 
			setPerspective(fov, zNear, zFar); 
			updateViewMatrix(glm::vec3(0.0f), glm::vec3(0.0f));
		}

        Camera() : Camera(1.0f, 1.0f, 1.0f, 1.0f, 10.0f) {}

		void setPerspective(float fov, float zNear, float zFar)
		{
			projectionType = ProjectionType::Perspective;
			this->fov = glm::radians(fov);
			this->zNear = zNear;
			this->zFar = zFar;
			updateProjectionMatrix();
		}
		void setOrtho(float size, float zNear, float zFar)
		{
			projectionType = ProjectionType::Ortho;
			this->orthoSize = size;
			this->orthoNear = zNear;
			this->orthoFar = zFar;
			updateProjectionMatrix();
		}
		void setAspect(float width, float height)
		{
			aspect = width / height;
		}

		inline glm::mat4& getViewMatrix()  { return viewMatrix; }
		inline glm::mat4& getProjectionMatrix() { return projectionMatrix; }

		void updateViewMatrix(glm::vec3 pos, glm::vec3 rot)
        {
	        viewMatrix = glm::translate(glm::mat4(1.0f), pos) * glm::toMat4(glm::quat(rot)); // Rotate then translate, aka TR
		    viewMatrix = glm::inverse(viewMatrix); // Calculate inverse to get correct operation, aka (TR)^-1 = R^-1T^-1
        }

		void setTarget(std::shared_ptr<Framebuffer> target) { this->target = target; }
		std::shared_ptr<Framebuffer> getTarget() { return target; }

	private:
		void updateProjectionMatrix()
        {
            if (projectionType == ProjectionType::Perspective)
            {
                projectionMatrix = glm::perspective(fov, aspect, zNear, zFar);
            }
            else
            {
                projectionMatrix = glm::ortho(
					-orthoSize * aspect * 0.5f, orthoSize * aspect * 0.5f, 
					-orthoSize * 0.5f, orthoSize * 0.5f,
					orthoNear, orthoFar
				);
            }
        }

	private:
		ProjectionType projectionType;

		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;

		float aspect;

		// Perspective params
		float fov;
		float zNear, zFar;

		// Ortho params
		float orthoSize;
		float orthoNear, orthoFar;

		// Framebuffer this camera will render to; none is default framebuffer
		std::shared_ptr<Framebuffer> target = nullptr;

		friend class CameraSystem;
    };
}