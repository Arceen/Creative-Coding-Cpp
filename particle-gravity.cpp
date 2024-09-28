#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib>

using namespace std;

const int SCREEN_WIDTH = 1400;
const int SCREEN_HEIGHT = 800;
const int BARRIER_WIDTH = 2;
const double MATERIAL_BOUNCE_FACTOR = 0.9;
const double GRAVITY = 9.8;

typedef struct Point
{
    double x;
    double y;
    double vx{0};
    double vy{0};
    double ax{0};
    double ay{0};
    double dragFactorX{0.0000};
    double dragFactorY{0.0000};
    double LeftBound{BARRIER_WIDTH};
    double RightBound{SCREEN_WIDTH - BARRIER_WIDTH};
    double TopBound{BARRIER_WIDTH};
    double BottomBound{SCREEN_HEIGHT - BARRIER_WIDTH};
    Uint8 r{0xFF};
    Uint8 g{0xFF};
    Uint8 b{0xFF};
    void setLocation(double _x, double _y)
    {
        x = _x;
        y = _y;
    }
    void setBounds(double l, double r, double t, double b)
    {
        LeftBound = l;
        RightBound = r;
        TopBound = t;
        BottomBound = b;
    }
    void setColor(Uint8 _r, Uint8 _g, Uint8 _b)
    {
        r = _r;
        g = _g;
        b = _b;
    }
    void applyForce(double _ax, double _ay)
    {
        ax += _ax;
        ay += _ay;
    }
    void update(double time)
    {
        // cout << "TIME: " << time << endl;
        // cout << "Y: " << y << endl;
        auto tax = ax + dragFactorX * -ax;
        auto tay = ay + (dragFactorY * -ay);
        auto tvx = vx + tax * time;
        auto tvy = vy + (tay * time) + (GRAVITY);
        auto tx = x + tvx * time;
        auto ty = y + tvy * time;
        // cout << "ACC: " << tay << endl;
        // cout << "VLC: " << tvy << endl;
        // cout << "TY: " << ty << endl;

        if ((tx < RightBound) && (tx > LeftBound))
        {
            x = tx;
            ax = tax;
            vx = tvx;
        }
        else
        {
            double timeUntilBorderHit;
            if (tx > RightBound)
            {
                x = RightBound;
                timeUntilBorderHit = (RightBound - x) / tvx;
            }
            else
            {
                x = LeftBound;
                timeUntilBorderHit = (x - LeftBound) / tvx;
            }
            tvx = -1 * (vx + tax * timeUntilBorderHit) * MATERIAL_BOUNCE_FACTOR;
            // cout << "PREV -> ACC: " << tax << " & VELOCITY: " << tvx << endl;

            double timeAfterBorderHit = time - timeUntilBorderHit;
            ax = 0;
            vx = tvx + ax;
            y += vx * timeAfterBorderHit;
        }
        if ((ty <= BottomBound) && (ty >= TopBound))
        {
            y = ty;
            vy = tvy;
            ay = tay;
        }
        else
        {
            double timeUntilBorderHit;
            if (ty >= BottomBound)
            {
                timeUntilBorderHit = (BottomBound - y) / tvy;
                y = BottomBound;
            }
            else
            {
                y = TopBound;
                timeUntilBorderHit = (y - TopBound) / tvy;
            }
            // cout << "Time before collision: " << timeUntilBorderHit << endl;
            // cout << "Time after collision: " << (time - timeUntilBorderHit) << endl;
            // cout << "PREV -> ACC: " << tay << " & VELOCITY: " << tvy << endl;

            tvy = -1 * (vy + tay * timeUntilBorderHit) * MATERIAL_BOUNCE_FACTOR;
            // cout << "PREV -> ACC: " << tay << " & VELOCITY: " << tvy << endl;

            double timeAfterBorderHit = time - timeUntilBorderHit;
            ay = 0;
            vy = tvy + ay;
            y += vy * timeAfterBorderHit;
            // cout << "New -> ACC: " << ay << " & VELOCITY: " << vy << endl;
            // cout << "NEW Y: " << y << endl;
        }
    }
    void render(SDL_Renderer *renderer)
    {

        SDL_SetRenderDrawColor(renderer, r, g, b, 0xff);
        SDL_RenderDrawPoint(renderer, x, y);
    }
} Point;

typedef struct Random
{
    static double GetRandomDouble()
    {
        return static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
    }
    static Point *GetRandomPoint(long long int frame, double min_a, double max_a, int dir = -1)
    {
        Point *p = new Point{};
        double rand_ay = min_a + (max_a - min_a) * GetRandomDouble();
        double tay = dir * rand_ay;
        double tax = static_cast<double>((GetRandomDouble() - 0.5) * (16000 + rand() % 16000));
        // double tax = 0;
        p->applyForce(tax, tay);
        p->setColor(static_cast<Uint8>(rand() % 256), static_cast<Uint8>(rand() % 256), static_cast<Uint8>(rand() % 256));
        return p;
    }
} Random;

int main(int argc, char *argv[])
{
    vector<Point *> points;
    long long int frames = 0;
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    Uint64 now = SDL_GetPerformanceCounter();
    Uint64 last = 0;
    double deltaTime = 0;
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
                last = now;
                now = SDL_GetPerformanceCounter();

                deltaTime = (now - last) / static_cast<double>(SDL_GetPerformanceFrequency());
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
                // cout << SDL_GetError() << endl;

                SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
                SDL_RenderClear(renderer);

                // Every 60 frames (1 second) one point gets added to points

                if (!(frames % 60))
                {
                    Point *newPoint = Random::GetRandomPoint(frames, 12000, 12000);
                    newPoint->setLocation(SCREEN_WIDTH / 2, SCREEN_HEIGHT - BARRIER_WIDTH - 1);
                    // newPoint->setColor(0xAF, 0x12, 0x54);
                    // newPoint->applyForce(0.2, -8.3);
                    points.push_back(newPoint);
                }
                ++frames;
                for (auto &p : points)
                {
                    p->update(deltaTime);
                    p->render(renderer);
                }
                // SDL_Delay(200);
                SDL_RenderPresent(renderer);
            }

            SDL_DestroyWindow(window);
            SDL_DestroyRenderer(renderer);
            SDL_Quit();
        }
    }

    return 0;
}