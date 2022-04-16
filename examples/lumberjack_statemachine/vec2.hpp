#pragma once

#include <cmath>


class vec2
{
public:
    float x, y;


public:
    float length()
    {
        return std::sqrt(x * x + y * y);
    }

    vec2 normalized()
    {
        float len = length();
        return {x / len, y / len};
    }

    vec2 operator+(const vec2& v)
    {
        return {this->x + v.x, this->y + v.y};
    }

    vec2& operator+=(const vec2& v)
    {
        this->x += v.x;
        this->y += v.y;
        return *this;
    }

    vec2 operator-(const vec2& v)
    {
        return {this->x - v.x, this->y - v.y};
    }

    vec2& operator-=(const vec2& v)
    {
        this->x -= v.x;
        this->y -= v.y;
        return *this;
    }

    vec2 operator*(float s)
    {
        return {this->x * s, this->y * s};
    }
};

