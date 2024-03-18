#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

typedef struct
{
    int x;
    int y;
    int height;
    int width;
    int xSpeed;
    int ySpeed;
    int r;
    int g;
    int b;
    int a;
} Ball;

void drawBall(Ball ball, SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, ball.r, ball.g, ball.b, ball.a);
    SDL_Rect ball_rect = {ball.x, ball.y, ball.width, ball.height};
    SDL_RenderFillRect(renderer, &ball_rect);
}

void ballBoundary(Ball *ball)
{
    if (ball->y < 0 || ball->y > SCREEN_HEIGHT - ball->height)
    {
        ball->ySpeed = -ball->ySpeed;
    }
}

void ballReset(Ball *ball)
{
    if (ball->x < 0 || ball->x > SCREEN_WIDTH - ball->width)
    {
        Ball newBall = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 20, 20, 3, 3, 0, 0, 0, 255};
        *ball = newBall;
        SDL_Delay(75);
    }
}

void updateBall(Ball *ball)
{
    ball->x += ball->xSpeed;
    ball->y += ball->ySpeed;
}

typedef struct
{
    int x;
    int y;
    int height;
    int width;
    int ySpeed;
    int r;
    int g;
    int b;
    int a;
} Paddle;

void drawPaddle(Paddle paddle, SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, paddle.r, paddle.g, paddle.b, paddle.a);
    SDL_Rect paddle_rect = {paddle.x, paddle.y, paddle.width, paddle.height};
    SDL_RenderFillRect(renderer, &paddle_rect);
}

void paddleBoundary(Paddle *paddle)
{
    if (paddle->y < 0)
    {
        paddle->y = 0;
    }
    else if (paddle->y > SCREEN_HEIGHT - paddle->height)
    {
        paddle->y = SCREEN_HEIGHT - paddle->height;
    }
}

void rightPaddleCollisons(Ball *ball, Paddle rightPaddle)
{
    if (ball->x + ball->width >= rightPaddle.x - abs(ball->xSpeed) && (ball->y >= rightPaddle.y && ball->y <= rightPaddle.y + rightPaddle.height ||
                                                                       ball->y + ball->height >= rightPaddle.y && ball->y + ball->height < rightPaddle.y + rightPaddle.height))
    {
        if (ball->x + ball->width >= rightPaddle.x + abs(ball->xSpeed))
            ball->ySpeed = -ball->ySpeed;
        else
            ball->xSpeed = -ball->xSpeed;
    }
}

void leftPaddleCollisions(Ball *ball, Paddle leftPaddle)
{
    if (ball->x <= leftPaddle.x + leftPaddle.width + abs(ball->xSpeed) && (ball->y >= leftPaddle.y && ball->y <= leftPaddle.y + leftPaddle.height ||
                                                                           ball->y + ball->height >= leftPaddle.y && ball->y + ball->height < leftPaddle.y + leftPaddle.height))
    {
        if (ball->x <= leftPaddle.x + leftPaddle.width - abs(ball->xSpeed))
            ball->ySpeed = -ball->ySpeed;
        else
            ball->xSpeed = -ball->xSpeed;
    }
}

void handleInput(Paddle *leftPaddle, Paddle *rightPaddle)
{
    const uint8_t *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_W])
        leftPaddle->y -= leftPaddle->ySpeed;
    if (state[SDL_SCANCODE_S])
        leftPaddle->y += leftPaddle->ySpeed;
    if (state[SDL_SCANCODE_UP])
        rightPaddle->y -= rightPaddle->ySpeed;
    if (state[SDL_SCANCODE_DOWN])
        rightPaddle->y += rightPaddle->ySpeed;
}

int main(int argc, char *argv[])
{

    (void)argc;
    (void)argv;

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    Ball ball = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 20, 20, 3, 3, 0, 0, 0, 255};
    Paddle leftPaddle = {0, SCREEN_HEIGHT / 2, 100, 20, 7, 0, 0, 0, 255};
    Paddle rightPaddle = {SCREEN_WIDTH - 20, SCREEN_HEIGHT / 2, 100, 20, 7, 0, 0, 0, 255};

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
        window = SDL_CreateWindow("SDL2 Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

        if (window == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }
        else
        {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

            if (renderer == NULL)
            {
                printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
            }
            else
            {

                SDL_Event e;
                bool quit = false;
                while (quit == false)
                {
                    while (SDL_PollEvent(&e))
                    {
                        if (e.type == SDL_QUIT)
                            quit = true;
                    }
                    handleInput(&leftPaddle, &rightPaddle);
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderClear(renderer);
                    drawBall(ball, renderer);
                    updateBall(&ball);
                    ballBoundary(&ball);
                    drawPaddle(leftPaddle, renderer);
                    drawPaddle(rightPaddle, renderer);
                    paddleBoundary(&leftPaddle);
                    paddleBoundary(&rightPaddle);
                    rightPaddleCollisons(&ball, rightPaddle);
                    leftPaddleCollisions(&ball, leftPaddle);
                    ballReset(&ball);
                    SDL_RenderPresent(renderer);
                    SDL_Delay(10);
                }
            }
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
