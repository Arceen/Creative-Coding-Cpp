#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>
using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

typedef struct Point
{
    double x;
    double y;
    double vx{0};
    double vy{0};
    double ax{0};
    double ay{0};
    double dragFactorX{0.0002};
    double dragFactorY{0.0001};
    double LeftBound;
    double RightBound;
    double TopBound;
    double BottomBound;
    void setBounds(double l, double r, double t, double b)
    {
        LeftBound = l;
        RightBound = r;
        TopBound = t;
        BottomBound = b;
    }
    void applyForce(double _ax, double _ay)
    {
        ax += _ax;
        ay += _ay;
    }
    void update()
    {
        ax += dragFactorX * -ax;
        ay += dragFactorY * -ay;
        vx = ax;
        vy = ay;
        if (((vx + x) < RightBound))
        {
            x += vx;
        }
        if (((vy + y) < BottomBound))
        {
            y += vy;
        }
    }
} Point;

int main(int argc, char *argv[])
{
    int n = 100;
    Point box[n][n];

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            box[i][j].x = i;
            box[i][j].y = j;
            box[i][j].setBounds(i, SCREEN_WIDTH - (n - i - 1), j, SCREEN_HEIGHT - (n - j - 1));
            box[i][j].applyForce(0.0001 * i, 0.002 * i + pow(0.003, 2.0) * j);
        }
    }

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
    }
    else
    {
        window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        }
        else
        {

            renderer = SDL_CreateRenderer(window, -1, 0);
            SDL_Event e;
            bool quit = false;
            while (!quit)
            {
                while (SDL_PollEvent(&e))
                {
                    if (e.type == SDL_QUIT)
                    {
                        quit = true;
                    }
                    if (e.key.type == SDL_KEYUP)
                    {
                        cout << "UP" << endl;
                    }
                }
                cout << SDL_GetError() << endl;

                SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
                SDL_RenderClear(renderer);
                SDL_SetRenderDrawColor(renderer, 0xff, 0xaf, 0x8a, 0xff);
                for (int i = 0; i < n; i++)
                {
                    for (int j = 0; j < n; j++)
                    {
                        Point *bp = &box[i][j];
                        bp->update();
                        SDL_RenderDrawPoint(renderer, box[i][j].x, box[i][j].y);
                    }
                }
                SDL_RenderPresent(renderer);
            }

            SDL_DestroyWindow(window);
            SDL_DestroyRenderer(renderer);
            SDL_Quit();
        }
    }

    return 0;
}