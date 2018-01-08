#pragma once
#include "transform.h"
#include "Mesh.h"
#include "Texture.h"
#include "FPSCamera.h"
#include "Shader.h"

class Scene;

using namespace std;
class GameObject
{
public:
	GameObject(const string& name, const char* modelname, const char* texname, Shader* shader);
	GameObject(const string& name, Mesh* mesh, Texture* texture, Shader* shader);
	~GameObject();
	void Draw();

	string name;

	Transform transform;
	Texture* tex;
	Mesh* mesh;
	Shader* shader;
};

