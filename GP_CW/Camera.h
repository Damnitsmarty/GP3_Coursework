#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>


namespace Camera
{
	
	struct InputInfo 
	{
		bool W, S, A, D, Space, Ctrl = false;
		int mouseMotionX, mouseMotionY = 0;
		InputInfo() {
			Reset();
		}
		void Reset() {
			W = S = A = D = Space = Ctrl = false;
			mouseMotionX = mouseMotionY = 0;
		}
	};
	struct ProjMatrix
	{
		ProjMatrix(float fov, float aspect, float nearClip, float farClip) :
			fov(fov),
			aspect(aspect),
			nearClip(nearClip),
			farClip(farClip) {};
		float fov, aspect, nearClip, farClip;
	};

	class Camera
	{
	public:
		Camera(glm::vec3 pos, ProjMatrix pm) :
			pos(pos),
			vForward(0.0f, 0.0f, 1.0f),
			vUp(0.0f, 1.0f, 0.0f),
			projection(glm::perspective(pm.fov, pm.aspect, pm.nearClip, pm.farClip)) {};
		~Camera()
		{

		};

		//virtual methods derived classes MUST implement
		virtual glm::mat4 getViewProjection() const = 0;
		virtual glm::mat4 getView() const = 0;
		virtual void update() = 0;

		virtual void translate(glm::vec3 amt) = 0;
		virtual void rotate(float pitch, float yaw) = 0;
	
		InputInfo inputs;
		glm::mat4 projection;
		glm::vec3 pos;

		glm::vec3 vForward;
		glm::vec3 vUp;
	};

	class ABCamera : public Camera
	{
	public:
		ABCamera(glm::vec3 anchorPos, glm::vec3 camOffset, ProjMatrix pm) :
			Camera(anchorPos + camOffset, pm), 
			anchorPos(anchorPos) {
			inputs.Reset();
		};


		~ABCamera() {};

		glm::mat4 getViewProjection() const {
			return projection * view;
		};
		glm::mat4 getView() const {
			return view;
		};
		//###########################################################
		//## Transforms 
		//###########################################################

		void setAnchorPosition(glm::vec3 anchorPos)
		{
			glm::vec3 diff = anchorPos - this->anchorPos;
			//no need to multiply by fwd and up, because they do not change
			this->anchorPos = anchorPos;
			translate(diff);

		};

		void translate(glm::vec3 amt)
		{
			pos += amt;			//translate cam position
			updateView();		//apply changes to view matrix
		};
		void rotate(float pitch, float yaw)
		{
			glm::vec3 vRight = glm::normalize(glm::cross(vUp, vForward));

			glm::mat4 matPitch = glm::rotate(pitch, vRight);
			glm::mat4 matYaw = glm::rotate(yaw, vUp);
			//matPitch
			pos = glm::vec3(matPitch * matYaw * glm::vec4(pos - anchorPos, 0.0)) + anchorPos;

			//matYaw
			pos = glm::vec3(matYaw * glm::vec4(pos - anchorPos, 0.0)) + anchorPos;

			//apply changes to view matrix
			updateView();
			//reset mouse inputs
			inputs.mouseMotionX = inputs.mouseMotionY = 0;
		};

		//###########################################################
		//## Updates
		//###########################################################

		void update()
		{

			//update camera position from inputs
			float speed = 0.5;

			glm::vec3 movement(
				(int)inputs.A - (int)inputs.D,			//x
				(int)inputs.Space - (int)inputs.Ctrl,	//y
				(int)inputs.W - (int)inputs.S);			//z
			movement = movement * glm::vec3(speed);

			translate(movement);


			//update camera rotPitch from inputs
			float rotSpeed = 200.0;
			rotate(inputs.mouseMotionY / -rotSpeed, inputs.mouseMotionX / rotSpeed);

			//reset mouse motion after applying rotPitch!
			inputs.mouseMotionX = inputs.mouseMotionY = 0;
		};

		void updateView()
		{
			view = glm::lookAt(pos, anchorPos, vUp);
		}

	private:

		glm::mat4 view;
		glm::vec3 anchorPos;
	};

	class FPSCamera : public Camera
	{
	public:
		FPSCamera(glm::vec3 pos, ProjMatrix pm) :
			Camera(pos,pm)
		{};
		~FPSCamera() {};

		glm::mat4 getViewProjection() const
		{
			return projection * getView();
		};
		glm::mat4 getView() const {
			return glm::lookAt(pos, pos + vForward, vUp);
		}

		//
		// Transforms
		//

		void setPos(glm::vec3 pos)
		{
			this->pos = pos;
		};
		void translate(glm::vec3 amt)
		{
			glm::vec3 vRight = glm::cross(vUp, vForward);
			pos += vForward * amt.z;
			pos += vRight * amt.x;
			pos += vUp * amt.y;
		};
		void rotate(float pitch, float yaw)
		{
			glm::vec3 vRight = glm::normalize(glm::cross(vUp, vForward));
			//pitch

			glm::mat4 rotPitch = glm::rotate(pitch, vRight);
			vForward = glm::vec3(glm::normalize(rotPitch * glm::vec4(vForward, 0.0)));
			vUp = glm::normalize(glm::cross(vForward, vRight));

			//yaw
			glm::mat4 rotYaw = glm::rotate(yaw, glm::vec3(0,1,0));

			vForward = glm::vec3(glm::normalize(rotYaw * glm::vec4(vForward, 0.0)));
			vUp = glm::vec3(glm::normalize(rotYaw * glm::vec4(vUp, 0.0)));
		};

		//
		// Updates
		//
		void update() 
		{
			//update position
			float speed = 0.05;

			glm::vec3 movement(
				(int)inputs.A - (int)inputs.D,
				(int)inputs.Space - (int)inputs.Ctrl,
				(int)inputs.W - (int)inputs.S);
			movement *= speed;

			translate(movement);

			//update rotation
			float rotSpeed = 500.0;
			//inverting the yaw to simulate accurate head movement
			rotate((float)inputs.mouseMotionY / rotSpeed, (float)inputs.mouseMotionX / -rotSpeed);


			//reset mouse motion after applying rotPitch!
			inputs.mouseMotionX = inputs.mouseMotionY = 0;

		};
	private:

	};
}