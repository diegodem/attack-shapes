#include "GameScene.h"

GameScene::GameScene(SDL_Renderer *renderer)
{
	nextScene = -1;
	energyPlayer = 1;
	generateFigure(0);
	generateFigure(1);
	generateFigure(2);
	for (int i = 0; i < 3; i++)
	{
		shapes[i].increaseSpawnTime();
	}
	rectPlayer = { 96, 336, 96, 96 };
	this->renderer = renderer;
	loadMedia();
	srand(time(NULL));

	levelRect = { 96, 48, 96, 96 };
	color = { 0, 0, 0, 0 };
	
	level = 1;

	if (Mix_PlayingMusic() == 0)
	{
		//Play the music
		Mix_PlayMusic(music, -1);
	}

}


void GameScene::update(Timer deltaTime, std::vector<SDL_Keycode> keysPressed)
{
	for (int i = 0; i < keysPressed.size(); i++)
	{
		if (keysPressed[i] == SDLK_UP)
		{
			moveUp();
		}
		else if (keysPressed[i] == SDLK_DOWN)
		{
			moveDown();
		}
	}
	for (int i = 0; i < 3; i++)
	{
		shapes[i].getRect()->x -= (3 + level) * (int)(125.f * deltaTime.getTime());
		if (shapes[i].getRect()->x > 10000)
		{

			if (shapes[i].getSpawnTime() <= shapes[i].getSpawnTimer().getTime())
			{
				shapes[i].getRect()->x = 1024;
				shapes[i].spawned();
			}

		}
		else if (shapes[i].getRect()->x < -96)
		{
			generateFigure(i);
		}
		if (checkCollision(&rectPlayer, shapes[i].getRect()))
		{
			if (energyPlayer > shapes[i].getEnergy())
			{
				energyPlayer++;
				Mix_PlayChannel(-1, growSound, 0);
				shapes[i].getRect()->x = -96;
			}
			else if (energyPlayer < shapes[i].getEnergy())
			{
				energyPlayer--;
				Mix_PlayChannel(-1, hurtSound, 0);
				shapes[i].getRect()->x = -96;
			}
		}
	}


	if (energyPlayer == 10)
	{
		level++;
		Mix_PlayChannel(-1, levelUpSound, 0);
		energyPlayer = 1;
		generateFigure(0);
		generateFigure(1);
		generateFigure(2);
		for (int i = 0; i < 3; i++)
		{
			shapes[i].increaseSpawnTime();
		}
	}
	else if (energyPlayer < 1)
	{
		Mix_HaltMusic();
		nextScene = (int)SceneList::GAME_OVER_SCENE;
	}
	

}

void GameScene::draw()
{
	SDL_SetRenderDrawColor(renderer, 0xA0, 0xC5, 0xD0, 0xFF);
	SDL_RenderClear(renderer);
	for (int i = 0; i < 3; i++)
	{
		SDL_RenderCopy(renderer, textureGeo[shapes[i].getEnergy()], NULL, shapes[i].getRect());
	}
	SDL_RenderCopy(renderer, textureGeo[energyPlayer], NULL, &rectPlayer);

	levelStr.assign("Level ");
	levelStr.append(std::to_string(level));

	surfaceLevel = TTF_RenderText_Solid(generalFont, levelStr.c_str(), color);
	textureLevel = SDL_CreateTextureFromSurface(renderer, surfaceLevel);

	levelRect.w = surfaceLevel->w;
	levelRect.h = surfaceLevel->h;

	SDL_RenderCopy(renderer, textureLevel, NULL, &levelRect);

	SDL_RenderPresent(renderer);

	SDL_DestroyTexture(textureLevel);
	SDL_FreeSurface(surfaceLevel);
}

bool GameScene::loadMedia()
{
	bool success = true;

	// Load sprites
	textureGeo[0] = loadTexture("Sprites/triangle.png");
	if (textureGeo[0] == NULL)
	{
		printf("Failed to load texture image!\n");
		success = false;
	}

	textureGeo[1] = loadTexture("Sprites/square.png");
	if (textureGeo[1] == NULL)
	{
		printf("Failed to load texture image!\n");
		success = false;
	}

	textureGeo[2] = loadTexture("Sprites/pentagon.png");
	if (textureGeo[2] == NULL)
	{
		printf("Failed to load texture image!\n");
		success = false;
	}

	textureGeo[3] = loadTexture("Sprites/hexagon.png");
	if (textureGeo[3] == NULL)
	{
		printf("Failed to load texture image!\n");
		success = false;
	}

	textureGeo[4] = loadTexture("Sprites/heptagon.png");
	if (textureGeo[4] == NULL)
	{
		printf("Failed to load texture image!\n");
		success = false;
	}

	textureGeo[5] = loadTexture("Sprites/octogon.png");
	if (textureGeo[5] == NULL)
	{
		printf("Failed to load texture image!\n");
		success = false;
	}

	textureGeo[6] = loadTexture("Sprites/eneagon.png");
	if (textureGeo[6] == NULL)
	{
		printf("Failed to load texture image!\n");
		success = false;
	}

	textureGeo[7] = loadTexture("Sprites/decagon.png");
	if (textureGeo[7] == NULL)
	{
		printf("Failed to load texture image!\n");
		success = false;
	}

	textureGeo[8] = loadTexture("Sprites/hendecagon.png");
	if (textureGeo[8] == NULL)
	{
		printf("Failed to load texture image!\n");
		success = false;
	}

	textureGeo[9] = loadTexture("Sprites/dodecagon.png");
	if (textureGeo[9] == NULL)
	{
		printf("Failed to load texture image!\n");
		success = false;
	}

	//Load fonts
	generalFont = TTF_OpenFont("Fonts/SigmarOne-Regular.ttf", 40);

	//Load sounds
	changeSound = Mix_LoadWAV("Sounds/change.wav");
	if (changeSound == NULL)
	{
		printf("Failed to load change sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	hurtSound = Mix_LoadWAV("Sounds/hurt.wav");
	if (hurtSound == NULL)
	{
		printf("Failed to load hurt sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	levelUpSound = Mix_LoadWAV("Sounds/levelup.wav");
	if (levelUpSound == NULL)
	{
		printf("Failed to load levelup sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	growSound = Mix_LoadWAV("Sounds/grow.wav");
	if (growSound == NULL)
	{
		printf("Failed to load grow sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	music = Mix_LoadMUS("Music/music_trim.wav");
	if (music == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}
	return true;
}

void GameScene::generateFigure(int position)
{
	shapes[position] = Shape(energyPlayer, position);
}

void GameScene::moveUp()
{
	if (rectPlayer.y > 192)
	{
		rectPlayer.y -= 144;
		Mix_PlayChannel(-1, changeSound, 0);
	}
}

void GameScene::moveDown()
{
	if (rectPlayer.y < 480)
	{
		rectPlayer.y += 144;
		Mix_PlayChannel(-1, changeSound, 0);
	}
}

bool GameScene::checkCollision(SDL_Rect *rectA, SDL_Rect *rectB)
{
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	leftA = rectA->x;
	rightA = rectA->x + rectA->w;
	topA = rectA->y;
	bottomA = rectA->y + rectA->h;

	leftB = rectB->x;
	rightB = rectB->x + rectB->w;
	topB = rectB->y;
	bottomB = rectB->y + rectB->h;

	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	return true;
}

void GameScene::close()
{
	for (int i = 0; i < 10; i++)
	{
		SDL_DestroyTexture(textureGeo[i]);
		textureGeo[i] = NULL;
	}

	TTF_CloseFont(generalFont);
	generalFont = NULL;

	Mix_FreeChunk(changeSound);
	changeSound = NULL;

	Mix_FreeChunk(hurtSound);
	hurtSound = NULL;

	Mix_FreeChunk(levelUpSound);
	levelUpSound = NULL;

	Mix_FreeChunk(growSound);
	growSound = NULL;

	Mix_FreeMusic(music);
	music = NULL;
}
