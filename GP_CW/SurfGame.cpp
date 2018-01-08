#include "SurfGame.h"
#include <iostream>
#include <string>

SurfGame::SurfGame()
{
}

SurfGame::~SurfGame()
{
	for (SurfScene* i : scenes)
		delete i;
}

void SurfGame::run()
{
	initSystems();
	_gameState = GameState::PLAY;
	gameLoop();
}

void SurfGame::initSystems()
{
	//init display
	_gameDisplay.initDisplay(1280.0, 980.0);

	float ratio = _gameDisplay.getWidth() / _gameDisplay.getHeight();

	//load scenes from file
	//Baracuda - La Di Da (Nightcore Mix)
	scenes.push_back(new SurfScene(ratio, "../res/Songs/Baracuda - La Di Da (Nightcore Mix)/easy.osu"));
	scenes.push_back(new SurfScene(ratio, "../res/Songs/Baracuda - La Di Da (Nightcore Mix)/normal.osu"));
	scenes.push_back(new SurfScene(ratio, "../res/Songs/Baracuda - La Di Da (Nightcore Mix)/hard.osu"));
	scenes.push_back(new SurfScene(ratio, "../res/Songs/Baracuda - La Di Da (Nightcore Mix)/insane.osu"));
	//Kitsune^2 - Rainbow Tylenol
	scenes.push_back(new SurfScene(ratio, "../res/Songs/Kitsune^2 - Rainbow Tylenol/easy.osu"));
	scenes.push_back(new SurfScene(ratio, "../res/Songs/Kitsune^2 - Rainbow Tylenol/normal.osu"));
	scenes.push_back(new SurfScene(ratio, "../res/Songs/Kitsune^2 - Rainbow Tylenol/hard.osu"));
	scenes.push_back(new SurfScene(ratio, "../res/Songs/Kitsune^2 - Rainbow Tylenol/insane.osu"));
	//P-Light - TRIGGER-HAPPY
	scenes.push_back(new SurfScene(ratio, "../res/Songs/P-Light - TRIGGER-HAPPY/easy.osu"));
	scenes.push_back(new SurfScene(ratio, "../res/Songs/P-Light - TRIGGER-HAPPY/normal.osu"));
	scenes.push_back(new SurfScene(ratio, "../res/Songs/P-Light - TRIGGER-HAPPY/hard.osu"));
	scenes.push_back(new SurfScene(ratio, "../res/Songs/P-Light - TRIGGER-HAPPY/insane.osu"));
	//Noisestorm - Solar
	scenes.push_back(new SurfScene(ratio, "../res/Songs/Noisestorm - Solar/easy.osu"));
	scenes.push_back(new SurfScene(ratio, "../res/Songs/Noisestorm - Solar/normal.osu"));
	scenes.push_back(new SurfScene(ratio, "../res/Songs/Noisestorm - Solar/hard.osu"));
	scenes.push_back(new SurfScene(ratio, "../res/Songs/Noisestorm - Solar/insane.osu"));
	//LeaF - Calamity Fortune
	scenes.push_back(new SurfScene(ratio, "../res/Songs/LeaF - Calamity Fortune/easy.osu"));
	scenes.push_back(new SurfScene(ratio, "../res/Songs/LeaF - Calamity Fortune/normal.osu"));
	scenes.push_back(new SurfScene(ratio, "../res/Songs/LeaF - Calamity Fortune/hard.osu"));
	scenes.push_back(new SurfScene(ratio, "../res/Songs/LeaF - Calamity Fortune/insane.osu"));

	//set and load active SurfScene
	activeScene = 0;
	scenes[activeScene]->load();

}

void SurfGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		
		if (_gameState == GameState::PLAY) {
			//process keyboard input
			processInput();
			//update scene
			scenes[activeScene]->update();
			drawGame();
		}
	}
}

void SurfGame::processInput()
{
	SDL_Event e;

	while (SDL_PollEvent(&e)) //get and process events
	{
		switch (e.type)
		{
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_KEYDOWN:
			HandleKeyboardInput(e.key);
			break;
		case SDL_KEYUP:
			HandleKeyboardInput(e.key);
			break;
		case SDL_MOUSEMOTION:
			//rotate the camera if the left button is pressed,
			if (e.motion.state & SDL_BUTTON_LMASK) {
				scenes[activeScene]->cam()->inputs.mouseMotionX = e.motion.xrel;
				scenes[activeScene]->cam()->inputs.mouseMotionY = e.motion.yrel;
			}
			//rotate the models if the right button is pressed
			/*else if (e.motion.state & SDL_BUTTON_RMASK) {
				for (size_t i = 0; i < scenes[activeScene]->gos.size(); i++) {

					glm::vec3 newRot = scenes[activeScene]->gos[i]->transform.GetRot() + glm::vec3(e.motion.yrel / -500.0, e.motion.xrel / 500.0, 0);
					scenes[activeScene]->gos[i]->transform.SetRot(newRot);

				}
			}*/
			break;
		}
	}

}

void SurfGame::HandleKeyboardInput(SDL_KeyboardEvent e) {

	switch (e.keysym.sym)
	{
	case SDLK_COMMA:
		//prev SurfScene
		if (e.state == SDL_PRESSED) {
			if (activeScene > 0)
			{
				scenes[activeScene - 1]->load();
				activeScene--;
				scenes[activeScene + 1]->unload();
			}
		}
		break;
	case SDLK_PERIOD:
		//next SurfScene
		//exit if this was the last SurfScene
		if (e.state == SDL_PRESSED) {
			if (activeScene < scenes.size() - 1)
			{
				scenes[activeScene + 1]->load();
				activeScene++;
				scenes[activeScene - 1]->unload();
			}
			else _gameState = GameState::EXIT;
		}
		break;
	case SDLK_w:
		scenes[activeScene]->player->inputs.W = (e.state == SDL_PRESSED);
		break;
	case SDLK_s:
		scenes[activeScene]->player->inputs.S = (e.state == SDL_PRESSED);
		break;
	case SDLK_a:
		scenes[activeScene]->player->inputs.A = (e.state == SDL_PRESSED);
		break;
	case SDLK_d:
		scenes[activeScene]->player->inputs.D = (e.state == SDL_PRESSED);
		break;
	case SDLK_SPACE:
		scenes[activeScene]->player->inputs.Space = (e.state == SDL_PRESSED);
		break;
	case SDLK_LCTRL:
		scenes[activeScene]->player->inputs.Ctrl = (e.state == SDL_PRESSED);
		break;
	case SDLK_c: //change camera
		if (e.state == SDL_PRESSED) {
			scenes[activeScene]->activeCam++;
			if (scenes[activeScene]->activeCam >= scenes[activeScene]->cams.size()) 
				scenes[activeScene]->activeCam = 0;
		}
		break;
	case SDLK_p: //pause/resume the song
		if (e.state == SDL_PRESSED)
			scenes[activeScene]->pause();
		break;
	case SDLK_o:
		if (e.state == SDL_PRESSED)
			scenes[activeScene]->restart();
		break;

	case SDLK_ESCAPE:
		_gameState = GameState::EXIT;
	default:
		cout << e.keysym.sym << endl;
		break;
	}
}

void SurfGame::drawGame()
{
	_gameDisplay.clearDisplay(0.3f, 0.3f, 0.5f, 1.0f);

	scenes[activeScene]->draw();

	glEnableClientState(GL_COLOR_ARRAY);
	glEnd();

	_gameDisplay.swapBuffer();
}