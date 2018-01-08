#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
//#include "camera.h"

struct Transform
{
public:
	///<summary>
	/// Initializes the Transform
	///</summary>
	///<param name="camPos">Position of the transform</param>
	///<param name="rot">Rotation of the transform</param>
	///<param name="scale"> Scale of the transform</param>
	Transform(
		const glm::vec3& pos = glm::vec3(), 
		const glm::vec3& rot = glm::vec3(), 
		const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f))
	{
		this->pos = pos;
		this->rot = rot;
		this->scale = scale;
	}

	void Move(glm::vec3 amt);

	///<summary>
	/// Returns the Model Matrix for this transform
	///</summary>
	glm::mat4 GetModel() const;
	
	///<summary>Gets the Transform's position</summary>
	inline glm::vec3 GetPos() { return pos; } //getters
	///<summary>Gets the Transform's rotation</summary>
	inline glm::vec3 GetRot() { return rot; }
	///<summary>Gets the Transform's scale</summary>
	inline glm::vec3 GetScale() { return scale; }

	///<summary>Sets the Transform's position</summary>
	inline void SetPos(const glm::vec3& pos) { this->pos = pos; }
	///<summary>Sets the Transform's position</summary>
	inline void SetPos(float x, float y, float z) { this->pos = glm::vec3(x, y, z); }

	///<summary>Sets the Transform's rotation</summary>
	inline void SetRot(const glm::vec3& rot) { this->rot = rot; }
	///<summary>Sets the Transform's rotation</summary>
	inline void SetRot(float x, float y, float z) { this->rot = glm::vec3(x, y, z); }

	///<summary>Sets the Transform's scale</summary>
	inline void SetScale(const glm::vec3& scale) { this->scale = scale; }
	///<summary>Sets the Transform's scale</summary>
	inline void SetScale(float x, float y, float z) { this->scale = glm::vec3(x, y, z); }

protected:
private:
	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 scale;
};


