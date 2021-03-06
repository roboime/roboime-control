#include "move_action.h"

#include <algorithm>

namespace roboime
{
    move_action::move_action(robot r,
            float vx,
            float vy,
            float va,
            float kick,
            bool is_chip,
            bool dribble) :
        action(r),
        vx(vx),
        vy(vy),
        va(va),
        is_chip(is_chip),
        dribble(dribble)
    {}

    char
    move_action::to_byte(float x)
    {
        if (std::abs(x) > r.max_speed)
            x = x / std::abs(x);
        else
            x = x / r.max_speed;

        return (char) (127 * x);
    }

    char
    move_action::to_byte_kick(float x)
    {
        if (std::abs(x) > r.max_speed)
            x = 1;
        return (char) (127 * x);
    }

    std::vector<char>
    move_action::as_buffer()
    {
        if (std::isnan(vx) || std::isnan(vy) || std::isnan(va))
        {
            return std::vector<char>{(char) r.id, 0, 0, 0, 0, 0, 0};
        }
        std::vector<float> speeds = {
            (vy * cosf(robot::angles[0]) - vx * sinf(robot::angles[0]) + va * r.radius) / r.wheel_radius,
            (vy * cosf(robot::angles[1]) - vx * sinf(robot::angles[1]) + va * r.radius) / r.wheel_radius,
            (vy * cosf(robot::angles[2]) - vx * sinf(robot::angles[2]) + va * r.radius) / r.wheel_radius,
            (vy * cosf(robot::angles[3]) - vx * sinf(robot::angles[3]) + va * r.radius) / r.wheel_radius
        };
        auto largest = *std::max_element(speeds.begin(),
            speeds.end(),
            [](float x, float y) {
                return std::fabs(x) < std::fabs(y);
            }
        );
        if (largest > r.max_speed && largest != 0)
            for (auto it = speeds.begin(), end_it = speeds.end(); it < end_it; ++it)
                *it = *it * r.max_speed / largest;

        return std::vector<char> {
            (char) r.id,
            to_byte(-speeds[0]),
            to_byte(-speeds[1]),
            to_byte(-speeds[2]),
            to_byte(-speeds[3]),
            to_byte(dribble? 255 : 0),
            to_byte(is_chip? to_byte_kick(kick) : to_byte_kick(-kick))
        };
    }
}
