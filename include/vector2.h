#pragma once

class Vector2 {
    public:
        Vector2(){};
        Vector2(int x, int y): x(x), y(y) {}
        int x;
        int y;
    Vector2 operator+(const Vector2& vector) {
        return Vector2(x + vector.x, y + vector.y);
    }
    void operator+=(const Vector2& vector) {
        x += vector.x;
        y += vector.y;
    }

    bool operator==(const Vector2& vector) const {
        return x == vector.x && y == vector.y;
    }
     bool operator!=(const Vector2& vector) const {
        return x != vector.x || y != vector.y;
    }
    static Vector2 LEFT;
    static Vector2 RIGHT;
    static Vector2 UP;
    static Vector2 DOWN;
};

Vector2 Vector2::LEFT = Vector2(-1, 0);
Vector2 Vector2::RIGHT = Vector2(1, 0);
Vector2 Vector2::UP = Vector2(0, -1);
Vector2 Vector2::DOWN = Vector2(0, 1);