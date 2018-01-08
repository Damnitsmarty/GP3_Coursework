#pragma once
#include <glm\glm.hpp>
#include <GL\glew.h>
#include <string>
#include <vector>
#include "obj_loader.h"

class Mesh
{
public:
	Mesh();
	Mesh(const std::string& filename);
	~Mesh();

	void draw();
private:
	void init(const IndexedModel& model);

	enum
	{
		VERTEX,
		TEXEL,
		NORMAL,
		INDEX,
		NUM_BUFFERS
	};

	GLuint vao;
	GLuint vbos[NUM_BUFFERS];
	unsigned int drawCount;
};

