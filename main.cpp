#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <cmath>

const int SCREEN_HEIGHT = 600;
const int SCREEN_WIDTH = 800;
const double PI = 3.1415926535;
const int QUADRENT_DEGREE = 90;

using namespace std;

void UpdatePixel(void *pixels, int x, int y, int r, int g, int b, int a)
{
    const int index = (y * SCREEN_WIDTH + x) * 4;
    Uint8 *pixel = static_cast<Uint8 *>(pixels);
    pixel[index + 0] = 255;
    pixel[index + 1] = 0;
    pixel[index + 2] = 0;
    pixel[index + 3] = 255;
}

void DrawCircleV3(SDL_Renderer *renderer, int x, int y, int r, int pointCount)
{
    SDL_RenderClear(renderer);
    SDL_Texture *texture;
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
    void *pixels;
    int pitch;
    SDL_LockTexture(texture, NULL, &pixels, &pitch);

    for (int i = 0; i < QUADRENT_DEGREE; i++)
    {
        int perimeterX = r * cos(i * (PI / 180));
        int perimeterY = r * sin(i * (PI / 180));
        for (int j = -perimeterX; j <= perimeterX; j++)
        {
            UpdatePixel(pixels, x + j, y + perimeterY, 255, 0, 0, 255);
            UpdatePixel(pixels, x + j, y - perimeterY, 255, 0, 0, 255);
        }
    }

    SDL_UnlockTexture(texture);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_DestroyTexture(texture);
}
void DrawCircleV2(SDL_Renderer *renderer, int x, int y, int r, int pointCount)
{
    SDL_RenderClear(renderer);
    SDL_Texture *texture;
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
    void *pixels;
    int pitch;
    SDL_LockTexture(texture, NULL, &pixels, &pitch);

    for (int i = 0; i < QUADRENT_DEGREE; i++)
    {
        int perimeterX = r * cos(i * (PI / 180));
        int perimeterY = r * sin(i * (PI / 180));
        double stepX = static_cast<double>(perimeterX) / r;
        double stepY = static_cast<double>(perimeterY) / r;
        for (int j = 0; j <= 2 * r; j++)
        {
            UpdatePixel(pixels, x - perimeterX + round(j * stepX), y + perimeterY - round(j * stepY), 255, 0, 0, 255);
            UpdatePixel(pixels, x - perimeterX + round(j * stepX), y - perimeterY + round(j * stepY), 255, 0, 0, 255);
        }
    }

    SDL_UnlockTexture(texture);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_DestroyTexture(texture);
}

void DrawCircle(SDL_Renderer *renderer, int x, int y, int r, int pointCount)
{
    for (int i = 0; i < QUADRENT_DEGREE; i++)
    {
        int perimeterX = r * cos(i * (PI / 180));
        int perimeterY = r * sin(i * (PI / 180));

        double stepX = static_cast<double>(perimeterX) / r;
        double stepY = static_cast<double>(perimeterY) / r;
        for (int j = 0; j <= 2 * r; j++)
        {
            SDL_RenderDrawPoint(renderer, x - perimeterX + round(j * stepX), y + perimeterY - round(j * stepY));
            SDL_RenderDrawPoint(renderer, x - perimeterX + round(j * stepX), y - perimeterY + round(j * stepY));
        }
        // SDL_RenderDrawPoint(renderer, x + perimeterX, y + perimeterY);
        // SDL_RenderDrawPoint(renderer, x - perimeterX, y + perimeterY);
        // SDL_RenderDrawPoint(renderer, x + perimeterX, y - perimeterY);
        // SDL_RenderDrawPoint(renderer, x - perimeterX, y - perimeterY);
    }
    // SDL_RenderDrawPoints(renderer, &points[0], points.size());
}

class Ball
{
public:
    int x;
    int y;
    int r;
    Ball(int x_, int y_, int r_) : x(x_), y(y_), r(r_) {}
    void update()
    {
    }
    void render(SDL_Renderer *renderer)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

        DrawCircleV3(renderer, x, y, r, 10);
        // SDL_RenderDrawPoint(renderer, x, y);
        // SDL_Rect rect;
        // rect.x = x - r;
        // rect.y = y - r;
        // rect.w = 2 * r;
        // rect.h = 2 * r;
        // SDL_RenderDrawRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 128, 0, 31, 255);
    }
};

int main(int argc, char *argv[])
{
    Ball *ball = new Ball(100, 100, 50);
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_Event event;
    bool shouldQuit = false;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "SDL Failed to initialize" << endl;
    }

    window = SDL_CreateWindow("Vector", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);

    while (!shouldQuit)
    {
        // Processing events
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                shouldQuit = true;
                break;
            }
        }
        // Clear the screen

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        ball->update();
        ball->render(renderer);
        // Tell window that our render is ready
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}