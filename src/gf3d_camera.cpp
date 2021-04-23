#include "gf3d_camera.h"
void Camera::OnUpdate()
{
	viewMatrix = glm::translate(glm::mat4(1.0f), position);
	//projectionMartix = glm::perspective(glm::radians(60.f), 800.f / 600.f, 0.1f, 200.f);
	projectionMartix = glm::ortho(-1.f, 1.f, -1.f, 1.f, -50.f, 50.f);
	//projectionMartix[1][1] *= -1;
}