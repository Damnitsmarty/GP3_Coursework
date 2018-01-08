#include "Scene.h"

Scene::Scene(Window * d, const string & filename)
{
	this->display = d;
	this->fileName = filename;
}


Scene::~Scene()
{
	if (loaded) unload();
}

void Scene::unload()
{
	delete shader;

	for (GameObject* go : gos)
		delete go;
	gos.clear();

	loaded = false;
}

void Scene::load()
{
	if (!loaded) {
		//read the file
		string text = loadFile(fileName);
		parseScene(text);
		loaded = true;
	}
}

void Scene::Draw()
{
	//one camera (therefore one vp matrix) is used for all the objects
	glm::mat4 vp = getActiveCamera()->GetViewProjection();
	for (size_t i = 0; i < gos.size(); i++)
	{
		gos[i]->shader->Bind();
		gos[i]->shader->Update(vp, gos[i]->transform.GetModel());
		gos[i]->Draw();
	}
}

string Scene::loadFile(const string & fileName)
{
	string text;
	ifstream file;
	file.open((fileName).c_str());
	if (file.is_open())
	{
		string line;
		while (file.good())
		{
			file >> line;
			text += line;
		}
	}
	file.close();

	//remove all comments
	regex commentRegex("(?:\\{\\{.*?\\}\\})");
	text = regex_replace(text, commentRegex, "");

	return text;
}

void Scene::parseScene(string text)
{
	//regex match
	smatch rMatch;


	//
	//	SHADER
	//
	regex rShaderExtract("SHADER\\((.*?)\\);");
	if (regex_search(text, rMatch, rShaderExtract)) {
		string shaderSettings = rMatch[1].str();
		
		shader = new Shader(nextString(shaderSettings));
	}
	//
	//	CAMERA
	//
	regex rCamExtract("CAM\\((.*?)\\);");

	if (regex_search(text, rMatch, rCamExtract)) {
		string camSettings = rMatch[1].str();


		glm::vec3 camPos = nextVec3(camSettings);
		float camFOV = nextFloat(camSettings);
		float camNearClip = nextFloat(camSettings);
		float camFarClip = nextFloat(camSettings);

		abcam.initCamera(camPos, glm::vec3(0, 0, 30), camFOV, (float)display->getWidth() / display->getHeight(), camNearClip, camFarClip);

		cam.initCamera(
			camPos,												//position
			camFOV,												//fov
			(float)display->getWidth() / display->getHeight(),	//aspect ratio (w/h)
			camNearClip,										//near clip plane
			camFarClip											//far clip plane
			);

	}
	else {
		printf("No camera definition in scene file. Creating default camera");
		cam.initCamera(
			glm::vec3(0, 0, -5),										//position
			70.0f,														//fov
			(float)display->getWidth() / display->getHeight(),			//aspect ratio (w/h)
			0.01f,														//near clip plane
			1000.0f														//far clip plane
			);
	}

	//
	//	GAME OBJECTS
	//
	regex rGOExtract("GO\\((.*?)\\);");

	while (regex_search(text, rMatch, rGOExtract))
	{
		string GOSettings = rMatch[1].str();

		string GOName = nextString(GOSettings);
		string GOPathMesh = nextString(GOSettings);
		string GOPathTex = nextString(GOSettings);
		glm::vec3 GOPosition = nextVec3(GOSettings);
		glm::vec3 GORotation = nextVec3(GOSettings);
		glm::vec3 GOScale = nextVec3(GOSettings);

		gos.push_back(new GameObject(GOName.c_str(), GOPathMesh.c_str(), GOPathTex.c_str(), shader));
		gos.back()->transform.SetPos(GOPosition);
		gos.back()->transform.SetRot(GORotation);
		gos.back()->transform.SetScale(GOScale);
		
		text = rMatch.suffix();
	}

}

float Scene::nextFloat(string & text)
{
	regex rFloat("[+-]?\\d+(?:\\.\\d+)?");
	smatch match;

	//if (!regex_search(text, match, rFloat)) return nullptr;
	regex_search(text, match, rFloat);

	float f = stof(match[0].str());

	text = match.suffix();
	return f;
}

glm::vec3 Scene::nextVec3(string & text)
{
	regex rVec3("\\(([+-]?\\d+(?:\\.\\d+)?),([+-]?\\d+(?:\\.\\d+)?),([+-]?\\d+(?:\\.\\d+)?)\\)");
	smatch match;


	//if (!regex_search(text, match, rVec3)) return nullptr;
	regex_search(text, match, rVec3);

	float x = stof(match[1].str());
	float y = stof(match[2].str());
	float z = stof(match[3].str());

	text = match.suffix();
	return glm::vec3(x, y, z);
}

string Scene::nextString(string & text)
{
	regex rString("\"(.*?)\"");
	smatch match;

	//if (!regex_search(text, match, rString)) return nullptr;
	regex_search(text, match, rString);

	string s = match[1].str();

	text = match.suffix();
	return s;
}
