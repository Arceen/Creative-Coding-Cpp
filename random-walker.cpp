#include <SDL2/SDL.h>
#include "helpers/Random.h"
#include <iostream>
using namespace std;
#define WINDOW_CREATE_ERROR -1;
#define SDL_INIT_ERROR -2;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 400;

class Walker
{
public:
    Walker(double _x, double _y) : x{_x}, y{_y} {};
    void step()
    {
        int choice = Random::randRangeInt(1, 4);
        if (choice == 1)
        {
            ++x;
        }
        else if (choice == 2)
        {
            --x;
        }
        else if (choice == 3)
        {
            ++y;
        }
        else
        {
            --y;
        }
    }
    void render(SDL_Renderer *renderer)
    {
        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderDrawPoint(renderer, x, y);
    }

private:
    double x;
    double y;
};

main(int argc, char *argv[])
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    bool shouldQuit{false};

    Walker walker{SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
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
    double posX = 0;
    double posY = 0;
    int fx = 0;
    int fy = 0;
    while (!shouldQuit)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                shouldQuit = true;
            }
        }
        // SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

        // void *pixels;
        // int pitch;
        // SDL_LockTexture(texture, NULL, &pixels, &pitch);

        // Uint32 *pixel_ptr = (Uint32 *)pixels;
        // for (int y = 0; y < SCREEN_HEIGHT; ++y)
        // {
        //     for (int x = 0; x < SCREEN_WIDTH; ++x)
        //     {
        //         pixel_ptr[y * (pitch / 4) + x] = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888), (++fx + y) % 256, (++fy + x) % 256, (y + x) % 256, 255);
        //     }
        // }
        // fy++;

        // SDL_UnlockTexture(texture);
        // SDL_RenderClear(renderer);
        // SDL_RenderCopy(renderer, texture, NULL, NULL);
        // SDL_RenderPresent(renderer);

        // SDL_DestroyTexture(texture);
        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
        // SDL_Rect *rect = new SDL_Rect{posX, posY, 4, 4};
        // SDL_RenderFillRect(renderer, rect);
        walker.step();
        walker.render(renderer);
        SDL_RenderPresent(renderer);
        // SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
        // posX = posX > SCREEN_WIDTH ? 0 : posX + 1;
        // posY = posY > SCREEN_HEIGHT ? 0 : posY + 0.2;
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}