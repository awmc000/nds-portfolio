#include "maze.hpp"

Maze::Maze() {
    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            cell[y][x] = 0;
        }
    }
}

Maze::~Maze() {

}

void Maze::build() {
    boxOutside();
}

int Maze::getCell(int y, int x) const {
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
