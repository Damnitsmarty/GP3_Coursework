#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>



struct FPSCamera
{
public:
	FPSCamera(){}

	void initCamera(const glm::vec3& pos, float fov, float aspect, float nearClip, float farClip);
	
	/// <summary>
	///	Returns the VP matrix for this camera
	/// </summary>
	glm::mat4 GetViewProjection() const;

	/// <summary>
	///	Updates the position and rotation of the camera.
	/// </summary>
	void Update();


	//
	// transform
	//

	///<summary>
	/// Moves the camera by the specified amount.
	///</summary>
	///<param name="amt"> The distance to move.</param>
	void Move(glm::vec3 amt);

	void SetPos(glm::vec3 pos);
	
	///<summary>
	/// Pitches the camera by the specified amount.
	///</summary>
	///<param name="angle"> The rotation in radians.</param>
	void Pitch(float angle);

	///<summary>
	/// Yaws the camera by the specified amount.
	///</summary>
	///<param name="angle"> The rotation in radians.</param>
	void Yaw(float angle);
	
	void rotateAboutPoint(glm::vec3 point, float anglePitch, float angleYaw);
	InputInfo inputs;
protected:
private:
	/// <summary>
	///	Updates the position of the camera.
	/// </summary>
	void UpdatePosition();

	/// <summary>
	/// Updates the camera rotation.
	/// Called by Update
	/// </summary>
	void UpdateRotation();

	glm::mat4 projection;
	glm::vec3 pos;
	glm::vec3 forward;
	glm::vec3 up;
};


