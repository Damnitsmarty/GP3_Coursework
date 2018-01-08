#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "Window.h"
#include "Scene.h"
enum class GameState{PLAY, PAUSED, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:

	GameState _gameState;
	Window _gameDisplay;
	vector<Scene*> scenes;

	unsigned int activeScene;

	float counter;


	void initSystems();
	void processInput();
	void HandleKeyboardInput(SDL_KeyboardEvent e);
	void gameLoop();
	void drawGame();






};

