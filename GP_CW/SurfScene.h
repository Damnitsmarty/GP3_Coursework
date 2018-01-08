#pragma once
#include <iostream>
#include <cmath>
#include <utility> //for std::pair
#include <array>	//for std::array
#include "OsuFileParser.h"
#include "Mp3.h"
#include "Camera.h"
#include "Mesh.h"
#include "Texture.h"
#include "transform.h"
#include "Shader.h"
#include "HitsoundManager.h"

///<summary>
/// Bounding Box structure
///</summary>
struct AABB 
{
	glm::vec3 pos;
	glm::vec3 size;


	///<summary>Constructs a bounding box using the object centre's position and the object's size.</summary>
	///<param name="pos">The position of the object's centre point.</param>
	///<param name="size">The size of the model.</param>
	AABB(glm::vec3 pos, glm::vec3 size) : 
		size(size) {
		this->pos = pos - (size / 2.0f);
	}
	
	///<summary>Checks collision between this bounding box and another.</summary>
	///<param name="that">The other bounding box.</param>
	bool checkCollision(AABB* that)
	{
		glm::vec3 posMax  = this->pos + this->size;
		glm::vec3 posMax2 = that->pos + that->size;

		if (posMax.z < that->pos.z || this->pos.z > posMax2.z) return false;
		if (posMax.x < that->pos.x || this->pos.x > posMax2.x) return false;
		if (posMax.y < that->pos.y || this->pos.y > posMax2.y) return false;
		return true;
	}
};

///<summary>
/// Structure containing information about the player object
///</summary>
struct PlayerData
{
	const glm::vec3 PLAYER_SIZE = glm::vec3(2.0f, 2.0f, 4.0f);

	Mesh* mesh;
	Texture* texture;
	Transform* transform;
	AABB* bounds;
	Camera::InputInfo inputs;

	///<summary>Constructor</summary>
	///<param name="meshPath">Relative path to the OBJ model file.</param>
	///<param name="texPath">Relative path to the Texture file.</param>
	///<param name="pos">The player's start position.</param>
	PlayerData(string meshPath, string texPath, glm::vec3 pos)
	{
		this->mesh = new Mesh(meshPath);
		this->texture = new Texture(texPath);
		this->transform = new Transform(pos);
		this->bounds = new AABB(pos, PLAYER_SIZE);
		inputs.Reset();
	}
	~PlayerData()
	{
		delete mesh;
		delete texture;
		delete transform;
		delete bounds;
	}

	//Bounds must be manually set due to the player mesh's custom pivot position
	///<summary> Updates and returns the bounding box of the player</summary>
	AABB* getBounds()
	{
		glm::vec3 pPos = this->transform->GetPos();
		this->bounds->pos = glm::vec3(pPos.x - PLAYER_SIZE.x / 2, -1, pPos.z - PLAYER_SIZE.z);
		return bounds;
	}
};


///<summary>
/// Structure containing transform and object data for all beats.
///</summary>
struct BeatData
{
	///<summary>Size of beat models</summary>
	const glm::vec3 BEAT_MODEL_SIZE = glm::vec3(2.0f, 1.0f, 2.0f);

	Mesh* mesh;
	Texture* texture;
	vector<Transform*> transforms;
	vector<AABB*> bounds;
	vector<bool> isHit;
	//index of next beat to hit
	size_t next_beat = 0;
	
	///<summary>Constructor</summary>
	///<param name="meshPath">Relative path to the OBJ model file.</param>
	///<param name="texPath">Relative path to the Texture file.</param>
	BeatData(string meshPath, string texPath)
	{
		this->mesh = new Mesh(meshPath);
		this->texture = new Texture(texPath);
	}
	~BeatData()
	{
		delete mesh;
		delete texture;
		for (Transform* t : transforms)
			delete t;
		for (AABB* bb : bounds)
			delete bb;
		transforms.clear();
		bounds.clear();
	}


	///<summary>Parses an OsuFileData structure to create scene objects.</summary>
	///<param name="data">The structure containing level data.</param>
	///<param name="sceneCols">Number of columns for the level.</param>
	///<param name="colWidth">Width of a single column</param>
	///<param name="zoomLevel">Zoom level: controls player speed and beat spread</param>
	void createTransforms(OsuFileData data, int sceneCols, float colWidth, float zoomLevel)
	{
		//create transforms for beats using 
		//x=filedata.beats.x, 
		//y=1, 
		//z=filedata.beats.offset * zoom

		//calculate world X positions for all columns
		vector<float> colsXPos;
		for (int i = 0; i < sceneCols; i++)
			colsXPos.push_back((0 - (float(sceneCols - 1) / 2) * colWidth) + i * colWidth);

		for (Beat beat : data.beats)
		{
			//calculate beat column
			//beat.x ranges from 0 to 512. use that to calculate the column of the beat object
			float beatColumn = glm::min((float) floor(beat.x / (512 / sceneCols)), (float) sceneCols - 1.0f); //returns from 0 to SCENE_COLUMNS

			float beatZ = beat.offset * zoomLevel;


			//populate arrays
			glm::vec3 beatPosition(colsXPos[beatColumn], 0.0, beatZ);
			this->transforms.push_back(new Transform(beatPosition));
			this->bounds.push_back(new AABB(beatPosition, BEAT_MODEL_SIZE));
			this->isHit.push_back(false);
		}
	}
	bool checkCollisionsWithPlayer(PlayerData* player)
	{
		bool ret = false;
		//check collisions for beats # next_beat - 3 TO next_beat + 1
		for (
			size_t	i  = glm::max((int)next_beat - 3, 0); 
					i <= glm::min((int)next_beat + 3, (int)transforms.size()-1); 
					i++)
		{
			if (!isHit[i]) //check for collisions only if the target hasn't been hit
			{
				bool collides = player->getBounds()->checkCollision(this->bounds[i]);
				if (collides) {
					ret = true;
					isHit[i] = true;
					//player->score->addHit();
				}
				else {
					//player->score->addMiss();
				}
			}
		}
		return ret;
	}
	void updateNextBeat(PlayerData* player)
	{
		//the player should never be more than 10 beats off course
		for (
			size_t	i  = glm::max((int)next_beat - 1 , 0);
					i <= glm::min((int)next_beat + 10, (int)transforms.size()-1); 
					i++)
		{
			if (transforms[i]->GetPos().z > player->transform->GetPos().z)
			{
				next_beat = i;
				return;
				glm::max(0.0f, 1.0f);
			}
		}
	}
	bool update(PlayerData* player)
	{
		bool ret = checkCollisionsWithPlayer(player);
		updateNextBeat(player);
		return ret;
	}
	void draw(Shader* shader, glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
	{
		texture->Bind(0);
		//draw from next beat onward
		for (size_t	i = glm::max((int)next_beat - 10, 0); i < transforms.size(); i++)
		{
			if (!isHit[i]) {
				shader->Update(projectionMatrix, viewMatrix, transforms[i]->GetModel());
				mesh->draw();
			}
		}
	}
};




///<summary>
/// Structure containing information about all static scene objects.
///</summary>
struct SceneryData
{
	vector<pair<string, Mesh*>> meshes;
	vector<pair<string, Texture*>> textures;
	vector<Transform*> transforms;
	vector<std::array<int, 3>> links;


	SceneryData() {}
	~SceneryData() {
		for (pair<string, Mesh*> p : meshes)
			delete p.second;
		for (pair<string, Texture*> p : textures)
			delete p.second;
		for (Transform* t : transforms)
			delete t;
		meshes.clear();
		textures.clear();
		transforms.clear();

	}
	
	///<summary>Adds an object to the structure.</summary>
	///<param name="meshPath">Relative path to the OBJ model file.</param>
	///<param name="texPath">Relative path to the Texture file.</param>
	///<param name="t">The player's transform.</param>
	size_t addObject(string meshPath, string texpath, Transform* t)
	{
		std::array<int, 3> link = { -1, -1, -1 };

		//check if we've loaded the mesh/texture before
		for (int i = 0; i < meshes.size(); i++)
			if (meshes[i].first == meshPath) 
				link[0] = i;
		for (int i = 0; i < textures.size(); i++)
			if (textures[i].first == texpath)
				link[1] = i;

		//if not, load, then link them
		if (link[0] == -1)
		{
			meshes.push_back(make_pair(meshPath, new Mesh(meshPath)));
			link[0] = meshes.size() - 1;
		}
		if (link[1] == -1)
		{
			textures.push_back(make_pair(texpath, new Texture(texpath)));
			link[1] = textures.size() - 1;
		}
		transforms.push_back(t);
		link[2] = transforms.size() - 1;

		//finally insert the link;
		links.push_back(link);
		return links.size() - 1;
	}
	
	

	///<summary>Returns the Texture of a specified object.</summary>
	///<param name="objId">The index of the object.</param>
	Texture* getTexture(size_t objId) {
		return textures[links[objId][1]].second;
	}

	///<summary>Returns the Mesh of a specified object.</summary>
	///<param name="objId">The index of the object.</param>
	Mesh* getMesh(size_t objId){
		return meshes[links[objId][0]].second;
	}

	///<summary>Returns the Transform of a specified object.</summary>
	///<param name="objId">The index of the object.</param>
	Transform* getTransform(size_t objId) {
		return transforms[links[objId][2]];
	}
};

///<summary>
/// Main scene class
///</summary>
class SurfScene
{
public:
	///<summary>Constructor.</summary>
	///<param name="DisplayRatio">Display Ratio of the render window.</param>
	///<param name="filename">Relative path to the .OSU data file.</param>
	SurfScene(float DisplayRatio, const string& filename);
	~SurfScene();

	///<summary>Loads the scene.</summary>
	void load();
	///<summary>Unloads the scene.</summary>
	void unload();

	///<summary>Plays the song.</summary>
	void play();
	///<summary>Pauses the song.</summary>
	void pause();
	///<summary>Stops the song file.</summary>
	void stop();

	///<summary>Restarts the stage.</summary>
	void restart();

	///<summary>Updates the scene before rendering.</summary>
	void update();
	///<summary>Renders the scene.</summary>
	void draw();
	///<summary>Returns the active camera.</summary>
	Camera::Camera* cam() {return cams[activeCam];}


	string osuMapFileName;

	vector<Camera::Camera*> cams;
	size_t activeCam = 0;
	
	//player data
	PlayerData* player;

	//beat objects data
	BeatData* beats;

	//scenery objects data
	SceneryData* scenery;
private:

	bool loaded = false;
	bool playing = false;
	
	//Osu level data
	Mp3 song;
	OsuFileData osuFileData;

	//GL data
	float displayRatio;
	Shader* shader;
	
	
	
};

