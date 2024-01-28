#pragma once

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "window.h"

class Camera
{

  public:
	Camera(int w, int h, float fov);

	void Input(int, int);

	glm::vec2 position = glm::vec2(0.0f);

	glm::mat4 GetMatrix();
	glm::mat4 GetView();
	glm::mat4 GetProjection();

	void ChangeFov(float newFov);
	void UpdateFov(float dt);

	int width, height;
	float aspect;

	void CalculateBounds();
	float leftBound;
	float rightBound;

	float fov;
	float newFov;
	float startFov;
	float fovChangeDuration = 0.7f;
	float fovChangeCrntTime = fovChangeDuration;
	float scrollSpeed = 0.200f;
	float fovTransitionSpeed = 4.0f;

	float lastcamX = 0;
	float lastcamY = 0;
	bool firstClick = true;

	int fovChangeCounter = 0;
};
