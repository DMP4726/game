#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include "graphics.h"

struct Mouse {
    int x, y;
    int dx = 0, dy = 0;
    SDL_Texture* texture = nullptr;

    void handleInput(const Uint8* keyStates) {
        dx = dy = 0;
        if (keyStates[SDL_SCANCODE_W] || keyStates[SDL_SCANCODE_UP]) dy -= 5;
        if (keyStates[SDL_SCANCODE_S] || keyStates[SDL_SCANCODE_DOWN]) dy += 5;

        x += dx;
        y += dy;

        if (y < 0) y = 0;
        if (y > SCREEN_HEIGHT - 60) y = SCREEN_HEIGHT - 60;
    }
};

void render(const Mouse& mouse, Graphics& graphics) {
    SDL_Rect dest = { mouse.x, mouse.y, 60, 60 };
    SDL_RenderCopy(graphics.renderer, mouse.texture, NULL, &dest);
}

bool gameOver(const Mouse& mouse) {
    return mouse.y >= SCREEN_HEIGHT - 60;
}

#endif

