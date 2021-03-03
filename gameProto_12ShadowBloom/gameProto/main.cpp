#include <SDL/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <string>
#include "Game.h"
#include "Renderer.h"
#include "TestScene.h"

int main(int argc, char** argv)
{
	if (!GAMEINSTANCE.Initialize(1280, 768, false))
	{
		return -1;
	}
	GAMEINSTANCE.setFirstScene(new TestScene);
	GAMEINSTANCE.GetRenderer()->SetWindowTitle("Game Proto");
	GAMEINSTANCE.Run();
	GAMEINSTANCE.Shutdown();
	return 0;
}