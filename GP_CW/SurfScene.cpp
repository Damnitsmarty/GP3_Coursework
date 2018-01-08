#include "SurfScene.h"

const glm::vec3 PLAYER_DEF_POS = glm::vec3(0.0, 0.0, 0.0);

const glm::vec3 CAM_DEF_OFFSET = glm::vec3(0.0, 15.0, -30.0);
const size_t CAM_DEF_FOV = 70;
const float CAM_DEF_NCLIP = 0.1;
const float CAM_DEF_FCLIP = 500;

const size_t SCENE_COLUMNS = 3;
const float SCENE_COLUMN_WIDTH = 4;
const float SCENE_ZOOM_LEVEL = 0.05;


SurfScene::SurfScene(float DisplayRatio, const string& fileName)
{
	this->osuMapFileName = fileName;
	this->displayRatio = DisplayRatio;
}

SurfScene::~SurfScene()
{
	unload();
}

void SurfScene::load()
{
	if (loaded) return;

	//init player data
	player = new PlayerData("../res/player.obj", "../res/player.png", PLAYER_DEF_POS);
	
	//load and parse Osu file data
	this-> osuFileData = OsuFileParser::parseFile(osuMapFileName);
	
	//init beat data
	beats = new BeatData("../res/beat.obj", "../res/beat.png");
	beats->createTransforms(osuFileData, SCENE_COLUMNS, SCENE_COLUMN_WIDTH, SCENE_ZOOM_LEVEL);

	//init scenery railings
	scenery = new SceneryData();
	float railingSize = 5.0f;
	for (int z = 0; z < 4000 / railingSize; z++) {
		scenery	->addObject(
			"../res/railing.obj", 
			"../res/uvrailing.png", 
			new Transform(glm::vec3(0.0f, -1.5f, 2.0 * railingSize * z),glm::vec3(),glm::vec3(1.0f,1.0f,railingSize))
		);
	}
	//init shader
	this->shader = new Shader("../res/shader");
	
	//init cam
	Camera::ProjMatrix pm(CAM_DEF_FOV, this->displayRatio, CAM_DEF_NCLIP, CAM_DEF_FCLIP);
	this->cams.push_back(new Camera::ABCamera(
		this->player->transform->GetPos(), CAM_DEF_OFFSET, pm
	));
	this->cams.push_back(new Camera::FPSCamera(
		this->player->transform->GetPos(), pm
	));

	//load mp3
	song.Load(wstring(osuFileData.audioPath.begin(), osuFileData.audioPath.end()).c_str());
	play();
	this->loaded = true;
}

void SurfScene::unload()
{
	if (!loaded) return;

	//stop playing song;
	stop();
	
	delete beats;
	delete player;
	delete scenery;
	delete shader;
	loaded = false;
}


void SurfScene::play()
{
	song.Play();
	playing = true;
}
void SurfScene::pause()
{
	if (playing) {
		song.Pause();
		playing = false;
	}
	else {
		song.Play();
		playing = true;
	}
}
void SurfScene::stop()
{
	song.Stop();
	playing = false;
}

void SurfScene::restart()
{
	unload();
	load();
}

void SurfScene::update()
{
	//update only if song is playing
	if (!playing) return;

	//update player position
	float songPos = song.GetCurrentPosition() *0.0001f;
	float playerHSpeed = 4; //horizontal speed
	glm::vec3 newPos(
		(((int)player->inputs.A - (int)player->inputs.D) * SCENE_COLUMN_WIDTH), //X : check inputs
		0.5 + glm::sin(songPos * 0.005) /2,										//Y : bobbing
		songPos * SCENE_ZOOM_LEVEL												//Z : song progress
	);

	player->transform->SetPos(newPos);
	
	//update camera position
		//cast the current camera to find its type and apply the appropriate function
		//abcam
	Camera::ABCamera* abcam = dynamic_cast<Camera::ABCamera*>(cams[activeCam]);
	if (abcam) abcam->setAnchorPosition(
		glm::vec3(0, 0.5f, player->transform->GetPos().z)
	);
		//fpscam
	Camera::FPSCamera* fpscam = dynamic_cast<Camera::FPSCamera*>(cams[activeCam]);
	if (fpscam) fpscam->setPos(
		glm::vec3(0, 0.5f, player->transform->GetPos().z) + CAM_DEF_OFFSET);
	//update camera
	cams[activeCam]->update();


	//check collisions
	bool collided = beats->update(player);
	if (collided) HitsoundManager::getInstance()->playSound();
}

void SurfScene::draw()
{
	//bind shader
	shader->Bind();
	//one camera (and therefore vpmatrix) is used for every object in the level
	//glm::mat4 vp = cam()->getViewProjection();
	glm::mat4 v = cam()->getView();
	glm::mat4 p = cam()->projection;
	for (size_t i = 0; i < scenery->links.size(); i++)
	{
		shader->Update(p,v, scenery->getTransform(i)->GetModel());
		scenery->getTexture(i)->Bind(0);
		scenery->getMesh(i)->draw();
	}
	//draw player
	shader->Update(p,v, player->transform->GetModel());
	player->texture->Bind(0);
	player->mesh->draw();

	//check collisions here:
	
	//draw beats
	beats->draw(shader,p,v);

	//glBindTexture(GL_TEXTURE_2D, 0);
}

