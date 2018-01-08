#include "transform.h"

glm::mat4 Transform::GetModel() const
{
	glm::mat4 posMat = glm::translate(pos);
	glm::mat4 scaleMat = glm::scale(scale);
	glm::mat4 rotX = glm::rotate(rot.x, glm::vec3(1.0, 0.0, 0.0));
	glm::mat4 rotY = glm::rotate(rot.y, glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 rotZ = glm::rotate(rot.z, glm::vec3(0.0, 0.0, 1.0));
	glm::mat4 rotMat = rotX * rotY * rotZ;

	return posMat * rotMat * scaleMat;
}
void Transform::Move(glm::vec3 amt)
{
	//x,y,z
	pos += amt;
	//z
	//camPos += forward * amt.z;
	//x
	//camPos += glm::cross(up, forward) * amt.x;
	//y
	//camPos += up * amt.y;
}