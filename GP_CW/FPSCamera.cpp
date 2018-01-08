#include "FPSCamera.h"
#include <iostream>
void FPSCamera::initCamera(const glm::vec3 & pos, float fov, float aspect, float nearClip, float farClip)
{
	this->pos = pos;
	this->forward = glm::vec3(0.0f, 0.0f, 1.0f);
	this->up = glm::vec3(0.0f, 1.0f, 0.0f);
	this->projection = glm::perspective(fov, aspect, nearClip, farClip);
	inputs.Reset();
}

glm::mat4 FPSCamera::GetViewProjection() const
{
	return projection * glm::lookAt(pos, pos + forward, up);
}

void FPSCamera::Update()
{
	UpdatePosition();
	UpdateRotation();
	//rotateAboutPoint(glm::vec3(0,-3,9), inputs.mouseMotionY / 500.0, inputs.mouseMotionX / 500.0);
}

void FPSCamera::UpdatePosition()
{
	float amt = 0.05;

	glm::vec3 movement(
		(int)inputs.A - (int)inputs.D,
		(int)inputs.Space - (int)inputs.Ctrl,
		(int)inputs.W - (int)inputs.S);
	movement = movement * glm::vec3(amt);

	Move(movement);
}

void FPSCamera::UpdateRotation()
{

	float mvmt = 500.0;
	Pitch((float)inputs.mouseMotionY / mvmt);
	//inverting the yaw to simulate accurate head movement
	Yaw((float)inputs.mouseMotionX / -mvmt);


	//reset mouse motion after applying rotation!
	inputs.mouseMotionX = inputs.mouseMotionY = 0;
}
void FPSCamera::SetPos(glm::vec3 pos)
{
	this->pos = pos;
}
void FPSCamera::Move(glm::vec3 amt)
{
	//x,y,z

	//z
	pos += forward * amt.z;
	//x
	pos += glm::cross(up, forward) * amt.x;
	//y
	pos += up * amt.y;
}

void FPSCamera::Pitch(float angle)
{
	glm::vec3 right = glm::normalize(glm::cross(up, forward));

	glm::mat4 rotation = glm::rotate(angle, right);
	forward = glm::vec3(glm::normalize(rotation * glm::vec4(forward, 0.0)));
	up = glm::normalize(glm::cross(forward, right));
}

void FPSCamera::rotateAboutPoint(glm::vec3 target, float anglePitch, float angleYaw) 
{
	glm::vec3 right = glm::normalize(glm::cross(up, forward));
	
	glm::mat4 pitch = glm::rotate(anglePitch, right);
	glm::mat4 yaw = glm::rotate(angleYaw, up);
	//pitch
	pos = glm::vec3(pitch * yaw * glm::vec4(pos - target, 0.0)) + target;

	//yaw
	//camPos = glm::vec3(yaw * glm::vec4(camPos - target, 0.0)) + target;

	forward = glm::normalize(target-pos);
	up = glm::normalize(glm::cross(forward, right));
	//view = glm::lookAt(camPos, target, up);

	inputs.mouseMotionX = inputs.mouseMotionY = 0;
	std::cout << "pos: " << pos.x << " " << pos.y << " " << pos.z << std::endl;

}
void FPSCamera::Yaw(float angle)
{
	static const glm::vec3 UP(0.0f, 1.0f, 0.0f);

	glm::mat4 rotation = glm::rotate(angle, UP);

	forward = glm::vec3(glm::normalize(rotation * glm::vec4(forward, 0.0)));
	up = glm::vec3(glm::normalize(rotation * glm::vec4(up, 0.0)));
}