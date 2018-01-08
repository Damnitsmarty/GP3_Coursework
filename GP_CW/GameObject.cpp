#include "GameObject.h"

GameObject::GameObject(const string& name, const char * modelname, const char * texname, Shader * shader)
{
	this->name = name;
	this->shader = shader;

	transform = Transform();
	//reset the transform
	this->transform.SetPos(glm::vec3(0, 0, 0));
	this->transform.SetRot(glm::vec3(0, 0, 0));
	this->transform.SetScale(glm::vec3(1, 1, 1));


	mesh = new Mesh(modelname);
	tex = new Texture(texname);
}

GameObject::GameObject(const string& name, Mesh* mesh, Texture* texture, Shader* shader)
{
	this->name = name;
	this->shader = shader;

	transform = Transform();
	//reset the transform
	this->transform.SetPos(glm::vec3(0, 0, 0));
	this->transform.SetRot(glm::vec3(0, 0, 0));
	this->transform.SetScale(glm::vec3(1, 1, 1));


	this->mesh = mesh;
	this->tex = texture;
}
GameObject::~GameObject()
{
	delete mesh;
	delete tex;
}

void GameObject::Draw()
{
	tex->Bind(0);
	mesh->draw();
}
