#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "Window.h"
#include "SurfScene.h"
enum class GameState { PLAY, PAUSED, EXIT };

class SurfGame
{
public:
	SurfGame();
	~SurfGame();

	void run();

private:

	GameState _gameState;
	Window _gameDisplay;
	vector<SurfScene*> scenes;

	unsigned int activeScene;

	void initSystems();
	void processInput();
	void HandleKeyboardInput(SDL_KeyboardEvent e);
	void gameLoop();
	void drawGame();






};
