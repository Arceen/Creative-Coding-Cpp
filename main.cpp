#include <SDL2/SDL.h>
#include "helpers/Random.h"
#include <iostream>
#include <vector>
using namespace std;
#define WINDOW_CREATE_ERROR -1;
#define SDL_INIT_ERROR -2;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

class Box
{

public:
    int x;
    int y;
    int h;
    int w;
    Box(int _x, int _y, int _h, int _w) : x{_x}, y{_y}, h{_h}, w{_w} {}
    void update(bool shouldUpdate)
    {
        if (((y + h) < SCREEN_HEIGHT) && shouldUpdate)
        {
            ++y;
        }
    }

    void render(SDL_Renderer *renderer)
    {

        // SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, w, h);
        // SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        // void *pixels;
        // int pitch;
        // SDL_LockTexture(texture, NULL, &pixels, &pitch);
        // Uint32 *pixel_ptr = (Uint32 *)pixels;

        // for (int j = y; j < h; ++j)
        // {
        //     for (int i = x; i < w; ++i)
        //     {
        //         pixel_ptr[j * (pitch / 4) + i] = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888), 255, 255, 255, 255);
        //     }
        // }

        // SDL_UnlockTexture(texture);
        // SDL_Rect rect;
        // rect.x = x;
        // rect.y = y;
        // rect.w = w;
        // rect.h = h;
        // SDL_RenderCopy(renderer, texture, &rect, &rect);
        // SDL_DestroyTexture(texture);
        SDL_Rect rect;
        rect.x = x;
        rect.y = y;
        rect.w = w;
        rect.h = h;
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    }
};

bool isColliding(Box &a, Box &b)
{
    return (a.x < (b.x + b.w)) && ((a.x + a.w) > b.x) && ((a.y < (b.y + b.h)) && ((a.y + a.h) > b.y));
}

int main(int argc, char *argv[])
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    bool shouldQuit{false};
    Box *a = new Box(70, 10, 100, 100);
    Box *b = new Box(80, 300, 100, 100);
    vector<Box *> boxes;
    boxes.push_back(a);
    boxes.push_back(b);
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
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
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
        for (int i = 0; i < boxes.size(); i++)
        {
            bool shouldUpdate = true;
            for (int j = 0; j < boxes.size(); j++)
            {
                if (i == j)
                    continue;
                if (isColliding(*boxes[i], *boxes[j]))
                {
                    shouldUpdate = false;
                }
            }
            boxes[i]->update(shouldUpdate);
            boxes[i]->render(renderer);
        }
        SDL_RenderPresent(renderer);
        ++frames;
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}