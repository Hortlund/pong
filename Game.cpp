#include "Game.h"

const int thickness = 15;
const int paddleH = 100.f;
const int width = 1024;
const int height = 768;

Game::Game()
:mWindow(nullptr),
mIsRunning(true),
mTicksCount(0)
{

}

bool Game::Initialize(){
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
    	mWindow = SDL_CreateWindow(
		"Game Programming in C++ (Chapter 1)",
		100,
		100,
		width,
		height,
		0
	);
    if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
    mRenderer = SDL_CreateRenderer(
		mWindow, 
		-1,		 
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

    if(!mRenderer){
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
    }

    mPaddlePos.x = 10.0f;
	mPaddlePos.y = 768.0f/2.0f;
	mPaddlePos2.x = width - (thickness + 10.0f);
	mPaddlePos2.y = height/2.0f;
	
	balls.push_back(Ball());
	balls.push_back(Ball());
	balls.push_back(Ball());
	balls[0].pos.x = 1024.0f / 2.0f;
	balls[0].pos.y = 768.0f / 2.0f;
	balls[0].vel.x = 250.0f;
	balls[0].vel.y = -235.0f;
	balls[1].pos.x = 1024.0f / 2.0f;
	balls[1].pos.y = 768.0f / 2.0f;
	balls[1].vel.x = 200.0f;
	balls[1].vel.y = -235.0f;
	balls[2].pos.x = 300.0f;
	balls[2].pos.y = 768.0f / 2.0f;
	balls[2].vel.x = -200.0f;
	balls[2].vel.y = 235.0f;
	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{

		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}
void Game::UpdateGame(){
	while(!SDL_TICKS_PASSED(SDL_GetTicks(),mTicksCount + 16))
		;
	float deltaTime = (SDL_GetTicks()-mTicksCount) / 1000.0f;

	if(deltaTime > 0.05f){
		deltaTime = 0.05f;
	}

	mTicksCount = SDL_GetTicks();

	if(mPaddleDir != 0){
		mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;
		if(mPaddlePos.y < (paddleH/2.0f + thickness)){
			mPaddlePos.y = paddleH/2.0f + thickness;
		}else if(mPaddlePos.y > (768.0f - paddleH/2.0f - thickness)){
			mPaddlePos.y = 768.0f - paddleH/2.0f - thickness;
		}
	}
	if (mPaddleDir2 != 0)
	{
		mPaddlePos2.y += mPaddleDir2 * 300.0f * deltaTime;
		if (mPaddlePos2.y < (paddleH / 2.0f + thickness))
		{
			mPaddlePos2.y = paddleH / 2.0f + thickness;
		}
		else if (mPaddlePos2.y > (768.0f - paddleH / 2.0f - thickness))
		{
			mPaddlePos2.y = 768.0f - paddleH / 2.0f - thickness;
		}
	}

	for (std::vector<Ball>::iterator i = balls.begin(); i != balls.end(); ++i)
	{

		i->pos.x += i->vel.x * deltaTime;
		i->pos.y += i->vel.y * deltaTime;

		if (i->pos.y <= thickness && i->vel.y < 0.0f)
		{
			i->vel.y *= -1;
		}
		if (i->pos.y >= (768 - thickness) && i->vel.y > 0.0f)
		{
			i->vel.y *= -1;
		}

		float diff = mPaddlePos.y - i->pos.y;
		diff = (diff > 0.0f) ? diff : -diff;
		float diff2 = mPaddlePos2.y - i->pos.y;
		diff2 = (diff2 > 0.0f) ? diff2 : -diff2;
		if (diff <= paddleH / 2.0f && i->pos.x <= 25.0f && i->pos.x >= 20.0f && i->vel.x < 0.0f)
		{
			i->vel.x *= -1.0f;
		}
		if (diff2 <= paddleH / 2.0f && i->pos.x >= (width - 25.0f) && i->pos.x <= (width - 20.0f) && i->vel.x > 0.0f)
		{
			i->vel.x *= -1.0f;
		}
	}

	
}

void Game::ProcessInput(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_QUIT:
                mIsRunning = false;
                break;
        }
    }
    const Uint8* state = SDL_GetKeyboardState(NULL);

	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	mPaddleDir = 0;
	mPaddleDir2 = 0;
	if(state[SDL_SCANCODE_W]){
		mPaddleDir -= 1;
	}
	if(state[SDL_SCANCODE_S]){
		mPaddleDir += 1;
	}
	if(state[SDL_SCANCODE_I]){
		mPaddleDir2 -= 1;
	}
	if (state[SDL_SCANCODE_K])
	{
		mPaddleDir2 += 1;
	}
}
void Game::GenerateOutput()
{
	
	SDL_SetRenderDrawColor(
		mRenderer,
		0,
		100,
		200,
		125	
	);
	
	SDL_RenderClear(mRenderer);
    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

    SDL_Rect wall{0,0,1024,thickness};
    SDL_RenderFillRect(mRenderer, &wall);
    wall.y = 768 - thickness;
    SDL_RenderFillRect(mRenderer,&wall);

    SDL_Rect paddle{static_cast<int>(mPaddlePos.x), static_cast<int>(mPaddlePos.y - paddleH/2),thickness,static_cast<int>(paddleH)};
    SDL_RenderFillRect(mRenderer, &paddle);

	SDL_Rect paddle2{static_cast<int>(mPaddlePos2.x), static_cast<int>(mPaddlePos2.y - paddleH / 2), thickness, static_cast<int>(paddleH)};
	SDL_RenderFillRect(mRenderer, &paddle2);
	for (std::vector<Ball>::iterator i = balls.begin(); i != balls.end(); ++i){
		SDL_Rect ball{static_cast<int>(i->pos.x - thickness / 2), static_cast<int>(i->pos.y - thickness / 2), thickness, thickness};
		SDL_RenderFillRect(mRenderer, &ball);
	}
		

    SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
    SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}