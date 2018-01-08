#include "Mesh.h"
#include <vector>


//void Mesh::init(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
//{
//	IndexedModel model;
//
//	for (unsigned int i = 0; i < numVertices; i++)
//	{
//		model.positions.push_back(*vertices[i].GetPos());
//		model.texCoords.push_back(*vertices[i].GetTexCoord());
//		//model.normals.push_back(*vertices[i].GetNormal());
//	}
//
//	for (unsigned int i = 0; i < numIndices; i++)
//		model.indices.push_back(indices[i]);
//
//	init(model);
//
//	//glGenBuffers(NUM_BUFFERS, vertexArrayBuffers); //generate our buffers based of our array of data/buffers - GLuint vertexArrayBuffers[NUM_BUFFERS];
//	//glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[VERTEX]); //tell opengl what type of data the buffer is (GL_ARRAY_BUFFER), and pass the data
//	//glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(positions[0]), &positions[0], GL_STATIC_DRAW); //move the data to the GPU - type of data, size of data, starting address (pointer) of data, where do we store the data on the GPU (determined by type)
//
//	//glEnableVertexAttribArray(0);
//	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
//
//	//glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[TEXEL]); //tell opengl what type of data the buffer is (GL_ARRAY_BUFFER), and pass the data
//	//glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(textCoords[0]), &textCoords[0], GL_STATIC_DRAW); //move the data to the GPU - type of data, size of data, starting address (pointer) of data, where do we store the data on the GPU
//	//
//	//glEnableVertexAttribArray(1);
//	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
//
//	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArrayBuffers[INDEX]); //tell opengl what type of data the buffer is (GL_ARRAY_BUFFER), and pass the data
//	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW); //move the data to the GPU - type of data, size of data, starting address (pointer) of data, where do we store the data on the GPU
//
//	//glBindVertexArray(0); // unbind our VAO
//}

void Mesh::init(const IndexedModel& model)
{
	
	drawCount = model.indices.size();

	//generate and bind mesh VAO
	glGenVertexArrays(1, &vao); 
	glBindVertexArray(vao); 

	//generate VBOs
	glGenBuffers(NUM_BUFFERS, vbos);
	
	//bind VBO 0 and pass vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vbos[VERTEX]);
	glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.positions[0]), &model.positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//bind VBO 1 and pass texel data
	glBindBuffer(GL_ARRAY_BUFFER, vbos[TEXEL]); 
	glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.texCoords[0]), &model.texCoords[0], GL_STATIC_DRAW); 
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//bind VBO 2 and pass normals data
	glBindBuffer(GL_ARRAY_BUFFER, vbos[NORMAL]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.normals[0]) * model.normals.size(), &model.normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//bind VBO 3 and pass index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[INDEX]); //tell opengl what type of data the buffer is (GL_ARRAY_BUFFER), and pass the data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(model.indices[0]), &model.indices[0], GL_STATIC_DRAW); //move the data to the GPU - type of data, size of data, starting address (pointer) of data, where do we store the data on the GPU

	//unbind VAO
	glBindVertexArray(0);
}

Mesh::Mesh()
{
	drawCount = NULL;
}

Mesh::Mesh(const std::string & filename)
{
	IndexedModel model = OBJModel(filename).ToIndexedModel();
	init(model);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vao); // delete arrays
	for (GLuint i : vbos)
		glDeleteBuffers(1, &i);
}

void Mesh::draw()
{
	glBindVertexArray(vao);
	
	glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, drawCount);
	
	glBindVertexArray(0);
}

