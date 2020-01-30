#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "../config.hpp"
#include "vector2d.hpp"
#include <array>
#include <fstream>

namespace ssecs::component
{
struct Position
{
    Vector2D position;
    inline static bool hasDebugDraw = false;
};
struct Velocity
{
    float x;
    float y;
};
struct MovementSpeed
{
    float speed;
};

struct Player
{
    bool isAttacking = true;
    bool isHit = true;
    Vector2D attack_direction;
    float attack_rate = 0.5;
    float dt = 0;
    int score = 0;
    std::string name = "abc";
};
struct Dash
{
    bool canDashing = true;
    float duration = 0.1f;
    float power = 3.f;
    float cd = 2.f;
    float dt = 0;
};
struct ScoreTable
{
    inline static constexpr const auto TABLE_SIZE = 3;
    std::array<std::pair<std::string, int>, TABLE_SIZE> table{};

    void Open(std::string_view path)
    {
        std::ifstream file;
        file.open(path.data());

        if (!file.is_open())
        {
            FAST_THROW("Could't open file.");
        }
        for (auto &row : table)
        {
            std::string line;
            if (std::getline(file, line))
            {
                auto space = line.find(" ");
                row.first = line.substr(0, space);
                row.second = std::stoi(line.substr(space + 1));
            };
        }
        file.close();
    }
    void Save(std::string_view path)
    {
        std::ofstream file;
        file.open(path.data(), std::ofstream::trunc);
        for (auto &row : table)
        {
            if (row.second == 0)
            {
                break;
            }
            file << row.first;
            file << " ";
            file << std::to_string(row.second);
            file << "\n";
        }
        file.close();
    }
};

class Transform
{
public:
private:
    Vector2D position;
    Vector2D scale;
    // Vector2D rotation;
};
} // namespace ssecs::component

#endif // TRANSFORM_HPP
