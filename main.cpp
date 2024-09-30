#include <SDL2/SDL.h>
#include "helpers/Random.h"
#include <iostream>
using namespace std;
#define WINDOW_CREATE_ERROR -1;
#define SDL_INIT_ERROR -2;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 400;

int main(int argc, char *argv[])
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    bool shouldQuit{false};
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "SDL could not be initialized!" << endl;
        return SDL_INIT_ERROR;
    }

    window = SDL_CreateWindow("Random Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (window == NULL)
    {
        cout << "SDL could not create Window!" << endl;
        return WINDOW_CREATE_ERROR;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    int frames = 0;
    int fx = 0;
    int fy = 0;
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
    while (!shouldQuit)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                shouldQuit = true;
            }
        }

        SDL_RenderClear(renderer);

        void *pixels;
        int pitch;
        SDL_LockTexture(texture, NULL, &pixels, &pitch);

        Uint32 *pixel_ptr = (Uint32 *)pixels;
        for (int y = 0; y < SCREEN_HEIGHT; ++y)
        {
            for (int x = 0; x < SCREEN_WIDTH; ++x)
            {
                pixel_ptr[y * (pitch / 4) + x] = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888), (++fx + y) % 256, (++fy + x) % 256, (y + x) % 256, 255);
            }
        }
        fy++;
        SDL_UnlockTexture(texture);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        cout << SDL_GetError() << endl;
        ++frames;
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}