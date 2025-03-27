#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <cstdlib>
#include <ctime>
#include "graphics.h"
#include "defs.h"
#include "game.h"
#include "sprite_animation.h"
#include <SDL_ttf.h>
#include <string>
using namespace std;

int main(int argc, char* argv[]) {
    srand(time(0));

    Graphics graphics;
    graphics.init();

    // Khởi tạo âm thanh
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cerr << "SDL_mixer Error: " << Mix_GetError() << endl;
        return -1;
    }

    Mix_Music* bgMusic = Mix_LoadMUS("wind.wav");
    if (!bgMusic) {
        cerr << "Failed to load background music: " << Mix_GetError() << endl;
        return -1;
    }
    Mix_PlayMusic(bgMusic, -1); // Phát lặp vô hạn

    ScrollingBackground background;
    background.setTexture(graphics.loadTexture(BACKGROUND_IMG));

    Mouse mouse;
    mouse.x = 100;
    mouse.y = SCREEN_HEIGHT / 2;
    mouse.texture = graphics.loadTexture("mouse.png");

    // Tạo nhiều ống với khoảng cách khác nhau
    Obstacle obstacles[3] = {
        Obstacle(), Obstacle(), Obstacle()
    };

    obstacles[0].init(graphics, "pipe2.png", "pipe.png", SCREEN_WIDTH);
    obstacles[1].init(graphics, "pipe2.png", "pipe.png", SCREEN_WIDTH + 300);
    obstacles[2].init(graphics, "pipe2.png", "pipe.png", SCREEN_WIDTH + 600);

    if (TTF_Init() == -1) {
    cerr << "SDL_ttf Error: " << TTF_GetError() << endl;
    return -1;
}

TTF_Font* font = TTF_OpenFont("WowDino-G33vP.ttf", 24);
if (!font) {
    cerr << "Failed to load font: " << TTF_GetError() << endl;
    return -1;
}

    SDL_Event event;
    int score = 0;
    int lastScoredPipe = -1;
SDL_Color white = {255, 255, 255, 255};

    bool quit = false;

    while (!quit && !gameOver(mouse)) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) quit = true;
        }

        background.scroll(5);
        graphics.render(background);

        const Uint8* keyStates = SDL_GetKeyboardState(NULL);
        mouse.handleInput(keyStates);

        for (auto& obstacle : obstacles) {
            obstacle.update();
            obstacle.render(graphics);
            if (obstacle.checkCollision(mouse)) {
                quit = true;
            }
            if (mouse.x > obstacle.x + obstacle.width) {
        score++;
    }
        }

        render(mouse, graphics);
        string scoreText = "Score: " + to_string(score/20);
graphics.renderText(scoreText.c_str(), 20, 20, white, font);
graphics.presentScene();
        SDL_Delay(10);
    }

    // Giải phóng âm thanh
    Mix_FreeMusic(bgMusic);
    Mix_CloseAudio();

    graphics.quit();
    TTF_CloseFont(font);
TTF_Quit();
    return 0;
}
