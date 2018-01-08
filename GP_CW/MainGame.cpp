#include "MainGame.h"
#include <iostream>
#include <string>
#include "Scene.h"


MainGame::MainGame()
{
}

MainGame::~MainGame()
{
	for (Scene* i : scenes)
		delete i;
}

void MainGame::run()
{
	initSystems();
	_gameState = GameState::PLAY;
	gameLoop();
}

void MainGame::initSystems()
{
	//init display
	_gameDisplay.initDisplay(800.0,600.0);
	

	//load scenes from file
	scenes.push_back(new Scene(&_gameDisplay, "../res/level1.txt"));
	scenes.push_back(new Scene(&_gameDisplay, "../res/level2.txt"));
	//set and load active scene
	activeScene = 0;
	scenes[activeScene]->load();

}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		if (_gameState == GameState::PLAY) {
			processInput();
			scenes[activeScene]->getActiveCamera()->Update();
			drawGame();
		}
	}
}

void MainGame::processInput()
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
				scenes[activeScene]->getActiveCamera()->inputs.mouseMotionX = e.motion.xrel;
				scenes[activeScene]->getActiveCamera()->inputs.mouseMotionY = e.motion.yrel;
			}
			//rotate the models if the right button is pressed
			else if(e.motion.state & SDL_BUTTON_RMASK) {
				for (size_t i = 0; i < scenes[activeScene]->gos.size();i++) {

					glm::vec3 newRot = scenes[activeScene]->gos[i]->transform.GetRot() + glm::vec3(e.motion.yrel / -500.0, e.motion.xrel / 500.0, 0);
					scenes[activeScene]->gos[i]->transform.SetRot(newRot);

				}
			}
			break;
		}
	}

}


///<summary>
/// Processes Keyboard Input events
///</summary>
///<param name="e"> The KeyboardEvent to process </param>
void MainGame::HandleKeyboardInput(SDL_KeyboardEvent e) {
	
	switch (e.keysym.sym) 
	{
		case SDLK_COMMA:
			//prev scene
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
			//next scene
			//exit if this was the last scene
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
			scenes[activeScene]->getActiveCamera()->inputs.W = (e.state == SDL_PRESSED);
			break;
		case SDLK_s:
			scenes[activeScene]->getActiveCamera()->inputs.S = (e.state == SDL_PRESSED);
			break;
		case SDLK_a:
			scenes[activeScene]->getActiveCamera()->inputs.A = (e.state == SDL_PRESSED);
			break;
		case SDLK_d:
			scenes[activeScene]->getActiveCamera()->inputs.D = (e.state == SDL_PRESSED);
			break;
		case SDLK_SPACE:
			scenes[activeScene]->getActiveCamera()->inputs.Space = (e.state == SDL_PRESSED);
			break;
		case SDLK_LCTRL:
			scenes[activeScene]->getActiveCamera()->inputs.Ctrl = (e.state == SDL_PRESSED);
			break;
		default:
			cout << e.keysym.sym << endl;
			break;
	}
}

void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(0.3f, 0.3f, 0.5f, 1.0f);

	//scenes[activeScene]->getActiveCamera().
	scenes[activeScene]->Draw();
	counter = counter + 0.01f;
				
	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	_gameDisplay.swapBuffer();
} 