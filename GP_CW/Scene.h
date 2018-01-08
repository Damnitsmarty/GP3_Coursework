#pragma once
#include <fstream>
#include <iostream>
#include <regex>
#include "FPSCamera.h"
#include "ABCamera.h"
#include "GameObject.h"
#include "Window.h"
#include "Shader.h"

class Scene
{
public:
	Scene(Window * d, const string& filename);
	~Scene();
	
	void load();
	void unload();
	bool isLoaded() { return loaded; };
	
	ABCamera* getActiveCamera() {
		return &abcam;
	}
	Shader* getActiveShader() {
		return shader;
	}

	void Draw();

	string fileName;
	Window* display;
	std::vector<GameObject*> gos;
private:
	FPSCamera cam;
	ABCamera abcam;
	Shader* shader;
	bool loaded = false;
	void parseScene(string text);

	static string loadFile(const string& fileName);
	static float nextFloat(string &text);
	static glm::vec3 nextVec3(string &text);
	static string nextString(string &text);
};