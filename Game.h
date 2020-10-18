
#include "SDL2/SDL.h"
#include <vector>

struct Vector2{
        float x;
        float y;
};

struct Ball{

        Vector2 pos;
        Vector2 vel;
};

class Game{

public:
    Game();

    bool Initialize();

    void RunLoop();

    void Shutdown();

private:

    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();

    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;

    bool mIsRunning;

    Uint32 mTicksCount;

    int mPaddleDir;
    int mPaddleDir2;

    std::vector<Ball> balls;

    Vector2 mBallVel;
    Vector2 mBallPos;
    Vector2 mPaddlePos;
    Vector2 mPaddlePos2;

};