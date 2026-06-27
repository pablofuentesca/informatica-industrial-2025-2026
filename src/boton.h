#pragma once

struct Boton {
    float x1, y1, x2, y2;
    const char* texto;

    void dibuja() const;
    bool contiene(float mx, float my) const {
        if (mx < x1 || mx > x2) return false;
        if (my < y1 || my > y2) return false;
        return true;
    }
};
