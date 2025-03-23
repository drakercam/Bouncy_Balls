#include "defs.h"

#include <vector>
#include <random>

void CreateBalls(std::vector<Raylib::Circle>&, const int num_balls);
void CreateWindowBarriers(std::vector<Raylib::Line>&);
void Render(const float dt, std::vector<Raylib::Line>& vec, std::vector<Raylib::Circle>& balls);
void Update(const float dt, std::vector<Raylib::Circle>& balls, std::vector<Raylib::Line>& lines);

int main(void)
{
    const int window_height = 512;
    const int window_width = 512;
    const std::string window_name = "Bouncy Balls";
    float delta_time;
    float time;         // for shaders

    InitWindow(window_width, window_height, window_name.c_str());
    SetTargetFPS(120);
    SetExitKey(KEY_Q);

    // create 4 lines to act as the screen barriers
    std::vector<Raylib::Line> window_barriers;

    CreateWindowBarriers(window_barriers);

    // create bouncing balls
    std::vector<Raylib::Circle> balls;

    CreateBalls(balls, 50);

    while(!WindowShouldClose())
    {
        delta_time = GetFrameTime();
        time = GetTime();

        Update(delta_time, balls, window_barriers);

        Render(delta_time, window_barriers, balls);
    }

    CloseWindow();

    return 0;
}

void Render(const float dt, std::vector<Raylib::Line>& vec, std::vector<Raylib::Circle>& balls)
{
    BeginDrawing();

    ClearBackground(BEIGE);

    for (int i = 0; i < vec.size(); ++i)
    {
        vec[i].DrawLineFilled();
    }

    for (int i = 0; i < balls.size(); ++i)
    {
        balls[i].DrawFilledCircle();
        balls[i].DrawCircleOutline(BLACK);
    }

    DrawFPS(2, 2);

    std::string text = "Bouncy Ball Simulation";
    DrawText(text.c_str(), GetScreenWidth() / 2 - 1.5 * GetTextWidth(text.c_str()), 15, 30, BLACK);
    
    EndDrawing();
}

void Update(const float dt, std::vector<Raylib::Circle>& balls, std::vector<Raylib::Line>& lines)
{
    for (int i = 0; i < balls.size(); ++i)
    {
        balls[i].position.x += balls[i].velocity.x * dt;
        balls[i].position.y += balls[i].velocity.y * dt;

        // check for wall collisions
        if ((balls[i].position.x >= (GetScreenWidth() - balls[i].radius)) || (balls[i].position.x <= balls[i].radius))
        {
            balls[i].velocity.x *= -1.0f;
        }

        if ((balls[i].position.y >= (GetScreenHeight() - balls[i].radius)) || (balls[i].position.y <= balls[i].radius))
        {
            balls[i].velocity.y *= -1.0f;
        }
    }
}

void CreateWindowBarriers(std::vector<Raylib::Line>& vec)
{
    Raylib::Line line1;
    Raylib::Line line2;
    Raylib::Line line3;
    Raylib::Line line4;

    line1.CreateLine(1, 1, 511, 1, BLACK);
    line2.CreateLine(1, 1, 1, 511, BLACK);
    line3.CreateLine(1, 511, 511, 511, BLACK);
    line4.CreateLine(511, 1, 511, 511, BLACK);

    vec.push_back(line1);
    vec.push_back(line2);
    vec.push_back(line3);
    vec.push_back(line4);
}

void CreateBalls(std::vector<Raylib::Circle>& balls, const int num_balls)
{
    std::vector<Color> colors;
    colors.push_back(RED);
    colors.push_back(BLUE);
    colors.push_back(GREEN);
    colors.push_back(MAGENTA);
    colors.push_back(MAROON);
    colors.push_back(PINK);
    colors.push_back(PURPLE);
    colors.push_back(ORANGE);
    colors.push_back(YELLOW);
    colors.push_back(LIME);

    const float ball_radius = 20.0f;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist1(ball_radius, GetScreenWidth() - ball_radius);
    std::uniform_int_distribution<int> dist2(ball_radius, GetScreenHeight() - ball_radius);
    std::uniform_int_distribution<int> colorDist(0, colors.size() - 1);
    std::uniform_real_distribution<float> vel(250.0f, 500.0f);

    for (int i = 0; i < num_balls; ++i)
    {
        int x = dist1(gen);
        int y = dist2(gen);
        float speedX = vel(gen) * (rand() % 2 == 0 ? 1 : -1);
        float speedY = vel(gen) * (rand() % 2 == 0 ? 1 : -1);
        Vector2 velocity = {speedX, speedY};

        int index = colorDist(gen);
        Color color = colors[index];

        Raylib::Circle ball;
        ball.CreateCircle(x, y, ball_radius, color);
        ball.velocity = velocity;

        balls.push_back(ball);
    }
}