#include "camera.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/matrix.hpp"

Camera::Camera(int w, int h, float _fov) : width(w), height(h), fov(_fov)
{
	aspect = width / (float)height;
	leftBound = position.x - fov * aspect;
	rightBound = position.x + fov * aspect;
	newFov = fov;
}

glm::mat4 Camera::GetMatrix()
{
	glm::mat4 projection = GetProjection();
	glm::mat4 view = GetView();
	return projection * view;
}

glm::mat4 Camera::GetProjection()
{
	aspect = width / (float)height;
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::ortho(-fov * aspect, fov * aspect, -fov, fov, 0.1f, 100.0f);
	return projection;
}

glm::mat4 Camera::GetView()
{
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(glm::vec3(position, 1.0f), glm::vec3(position, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	return view;
}
glm::mat4 GetViewForInput()
{
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	return view;
}

void Camera::CalculateBounds()
{
	// for graph boundaries
	leftBound = position.x - newFov * aspect;
	rightBound = position.x + newFov * aspect;
}
void Camera::Input(int mouseX, int mouseY)
{

	double x = 2.0 * mouseX / width - 1;
	double y = 2.0 * mouseY / height - 1;

	glm::vec4 screenPos = glm::vec4(x, -y, -1.0f, 1.0f);

	glm::mat4 viewProjectionInverse = glm::inverse(GetProjection() * GetViewForInput());
	// glm::mat4 viewProjectionInverse = glm::inverse(GetMatrix());
	glm::vec4 worldPos = viewProjectionInverse * screenPos;
	// std::cout << worldPos.x << " " << worldPos.y << std::endl;

	// std::cout << std::endl << std::endl;
	// std::cout << "WIDTH HEIGHT: " << width<< " " <<  height << std::endl;
	// std::cout << "MOUSEXY: " << mouseX << " " <<  mouseY << std::endl;
	// std::cout << "WORLD POS: " << worldPos.x << " " <<  worldPos.y <<
	// std::endl; std::cout << "last - current: " << lastcamX - worldPos.x << " ;
	// " <<  lastcamY - worldPos.y << std::endl;

	// std::cout << "Mouse world coordinates: " << pos.x << " " << pos.y <<
	// std::endl;
	if(!firstClick)
	{
		position.x += (lastcamX - worldPos.x);
		position.y += (lastcamY - worldPos.y);
	}
	firstClick = false;

	lastcamX = worldPos.x;
	lastcamY = worldPos.y;
}

float ZoomLerp(float from, float to, float t)
{
	if(!t) return to;
	from = glm::log(from);
	to = glm::log(to);
	return exp(from + (to - from) * t);
}

void Camera::ChangeFov(float _newFov)
{
	newFov = _newFov;
	startFov = fov;
	fovChangeCrntTime = 0.0f;
	// std::cout << "CURRENT ZOOM: " << fov << " TARGET ZOOM: " << newFov << std::endl;
}

void Camera::UpdateFov(float dt)
{
	fovChangeCrntTime += fovTransitionSpeed * dt;
	float t = 0;
	if(fovChangeDuration) t = fovChangeCrntTime / fovChangeDuration;
	// std::cout << t << std::endl;
	if(t >= 1.0f) return;
	fov = ZoomLerp(startFov, newFov, t);
}
