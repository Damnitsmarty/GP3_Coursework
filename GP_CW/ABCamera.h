#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "InputInfo.h"


struct ABCamera
{
public:
	ABCamera() {}

	//void initCamera(const glm::vec3& pos, const glm::vec3 & anchorPos, float fov, float aspect, float nearClip, float farClip);

	/// <summary>
	///	Constructor for the ArcBall camera
	/// </summary>
	///<param name="anchorPos">The Camera Anchor's position</param>
	///<param name="camOffset">The Camera's offset to the Anchor position</param>
	///<param name="fov">The Field of View</param>
	///<param name="aspect">The Display's aspect ratio</param>
	///<param name="nearClip">The Camera's near clip plane distance</param>
	///<param name="farClip">The Camera's far clip plane distance</param>
	void initCamera(const glm::vec3& anchorPos, const glm::vec3& camOffset, float fov, float aspect, float nearClip, float farClip);


	/// <summary>
	///	Returns the VP matrix for this camera
	/// </summary>
	glm::mat4 GetViewProjection() const;

	/// <summary>
	///	Updates the position and rotation of the camera.
	/// </summary>
	void Update();

	/// <summary>
	///	Updates the anchor position and then translates the camera so it's at the same relative point to the anchor.
	/// </summary>
	///<param name="anchorPos"> The new Anchor Position.</param>
	void setAnchorPos(glm::vec3 anchorPos);

	//
	// transform
	//

	///<summary>
	/// Moves the camera by the specified amount.
	///</summary>
	///<param name="amt"> The distance to move.</param>
	void Move(glm::vec3 amt);


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

	void UpdateView();

	void rotateAroundAnchor(float anglePitch, float angleYaw);
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
	glm::mat4 view;

	glm::vec3 camPos;
	glm::vec3 anchorPos;
	glm::vec3 forward;
	glm::vec3 up;
};


