#include "maze.hpp"

Maze::Maze() {
    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            cell[y][x] = 0;
            visited[y][x] = false;
        }
    }
}

Maze::~Maze() {

}

void Maze::build() {
    boxOutside();

    // Some random dots
    cell[10][10] = 1;
    cell[8][10] = 1;
    cell[20][20] = 1;
    cell[50][10] = 1;
    cell[19][8] = 1;
}

bool Maze::cellVisited(const int &y, const int &x) const
{
    return visited[y][x];
}

void Maze::visit(const int &y, const int &x)
{
    visited[y][x] = true;
}

int Maze::getCell(const int &y, const int &x) const {
    if (y > MAZE_HEIGHT - 1 || y < 0) {
        return -1;
    }

    if (x > MAZE_WIDTH - 1 || x < 0) {
        return -1;
    }

    return cell[y][x];
}

void Maze::drawLine(int y, int x, int length, bool vertical) {
    int sy = y, sx = x;
    if (vertical) {
        for (y; y < sy + length; y++) {
            cell[y][x] = 1;
        }
    } else {
        for (x; x < sx + length; x++) {
            cell[y][x] = 1;
        }
    }
}

void Maze::boxOutside() {
    // Top <---->
    drawLine(0, 0, MAZE_WIDTH, false);
    
    // Left |
    drawLine(0, 0, MAZE_HEIGHT, true);
    
    // Bottom <---->
    drawLine(MAZE_HEIGHT - 1, 0, MAZE_HEIGHT, false);
    
    // Right |
    drawLine(0, MAZE_WIDTH - 1, MAZE_HEIGHT, true);
}
