#include "maze.hpp"
#include <cassert>
#include <cstdio>

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

/**
 * Generate list of walls for maze generation.
 */
vector<pair<int, int>> Maze::mazeWalls() {
    vector<pair<int, int>> walls;
    // We want the walls with odd X xor odd Y
    // So we could add Odd X / Even Y
    for (int y = 2; y < MAZE_HEIGHT - 1; y += 2) {
        for (int x = 1; x < MAZE_WIDTH - 1; x += 2) {
            walls.push_back({y, x});
        }
    }

    // Then add Even X / Odd Y
    for (int y = 1; y < MAZE_HEIGHT - 1; y += 2) {
        for (int x = 2; x < MAZE_WIDTH - 1; x += 2) {
            walls.push_back({y, x});
        }
    }

    return walls;
}

/**
 * Generate set of cells for maze generation.
 */
set<pair<int, int>> Maze::mazeCells() {
    set<pair<int, int>> cells;

    for (int y = 1; y < MAZE_HEIGHT - 1; y += 2) {
        for (int x = 1; x < MAZE_WIDTH - 1; x += 2) {
            cells.insert({y, x});
        }
    }

    return cells;
}

void Maze::build() {
    boxOutside();

    // Fill
    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            cell[y][x] = 1;
        }
    }

    /* Tailored Kruskal's Algorithm
     * Cells: Odd X and Odd Y
     * Walls: Odd X xor Odd Y
     * Maze dimensions must be a multiple of 3
     * Initial testing done with 12
     * Does not consider outer tiles, so generates a maze in [1, max-2]
     */
    // List of all walls
    
    vector<pair<int, int>> walls = mazeWalls();
    
    // TODO: randomize

    set<pair<int, int>> cells = mazeCells();

    // For each wall...
    for (const auto &wall : walls) {

        // Get the two cells it separates
        int wallY = wall.first;
        int wallX = wall.second;

        // There is either a left/right or up/down connection.
        vector<pair<int, int>> connectedCells;

        bool lr = cells.count({wallX-1, wallY}) == 1 &&
                  cells.count({wallX+1, wallY}) == 1;
        bool ud = cells.count({wallX, wallY+1}) == 1 &&
                  cells.count({wallX, wallY-1}) == 1;
        
        // It cannot be both so we should never have both eval. to true.
        assert(!(lr && ud));

        if (lr) {
            connectedCells.push_back({wallX-1, wallY});
            connectedCells.push_back({wallX+1, wallY});
        } else if (ud) {
            connectedCells.push_back({wallX, wallY-1});
            connectedCells.push_back({wallX, wallY+1});
        }

        // Join their sets: remove each, combine, re-insert
        // For each connected cell, find the set it belongs to
        set<pair<int, int>> &aBelongsTo;
        set<pair<int, int>> &bBelongsTo;
        for (const auto &connCell : connectedCells) {

        }
        // Modify the maze to reflect this, carving out the cells and space between
    }

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
