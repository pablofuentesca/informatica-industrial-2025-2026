#pragma once

template <class T, int N>
class Grid {
    T datos_[N][N]{};

public:
    static constexpr int DIM = N;

    T& at(int x, int y) { return datos_[x][y]; }
    const T& at(int x, int y) const { return datos_[x][y]; }
};
