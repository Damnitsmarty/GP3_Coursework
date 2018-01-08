#include "ABCamera.h"
#include <iostream>
//#include "Camera.h"
void ABCamera::initCamera(const glm::vec3 & anchorPos, const glm::vec3 & camOffset, float fov, float aspect, float nearClip, float farClip)
{
	this->anchorPos = anchorPos;
	this->camPos = anchorPos + camOffset;

	this->forward = glm::vec3(0.0f, 0.0f, 1.0f);
	this->up = glm::vec3(0.0f, 1.0f, 0.0f);
	this->projection = glm::perspective(fov, aspect, nearClip, farClip);
	inputs.Reset();
}

glm::mat4 ABCamera::GetViewProjection() const
{
	return projection * view;
}

void ABCamera::Update()
{
	UpdatePosition();
	UpdateRotation();
}

void ABCamera::setAnchorPos(glm::vec3 anchorPos)
{
	glm::vec3 diff = anchorPos - this->anchorPos;
	this->camPos += diff;
	this->anchorPos = anchorPos;

}

void ABCamera::UpdatePosition()
{
	float amt = 0.5;

	glm::vec3 movement(
		(int)inputs.A - (int)inputs.D,
		(int)inputs.Space - (int)inputs.Ctrl,
		(int)inputs.W - (int)inputs.S);
	movement = movement * glm::vec3(amt);

	Move(movement);
}

void ABCamera::UpdateRotation()
{
	float mvmt = 200.0;
	rotateAroundAnchor(inputs.mouseMotionY / -mvmt, inputs.mouseMotionX / mvmt);

	//reset mouse motion after applying rotation!
	inputs.mouseMotionX = inputs.mouseMotionY = 0;
}

void ABCamera::Move(glm::vec3 amt)
{
	camPos += forward * amt.z;
	camPos += glm::cross(up, forward) * amt.x;
	camPos += up * amt.y;

	UpdateView();
}

void ABCamera::Pitch(float angle)
{
	rotateAroundAnchor(angle, 0.0);
}

void ABCamera::rotateAroundAnchor(float anglePitch, float angleYaw)
{
	glm::vec3 right = glm::normalize(glm::cross(up, forward));

	glm::mat4 pitch = glm::rotate(anglePitch, right);
	glm::mat4 yaw = glm::rotate(angleYaw, up);
	//pitch
	camPos = glm::vec3(pitch * yaw * glm::vec4(camPos - anchorPos, 0.0)) + anchorPos;

	//yaw
	camPos = glm::vec3(yaw * glm::vec4(camPos - anchorPos, 0.0)) + anchorPos;

	UpdateView();
	

	inputs.mouseMotionX = inputs.mouseMotionY = 0;

}
void ABCamera::Yaw(float angle)
{
	rotateAroundAnchor(0.0, angle);
}
void ABCamera::UpdateView() 
{
	view = glm::lookAt(camPos, anchorPos, up);
}