/*
 * Raycaster making use of LibNDS gl2d.
 */
#ifndef RAYCASTER_HPP
#define RAYCASTER_HPP

#include "maze.hpp"
#include "vector.hpp"

class Raycaster {
public:
    /**
     * @brief Constructor
     * @param maze
     * @param pos
     * @param dir
     * @param plane
    */
    Raycaster(Maze * maze, struct v2f * pos, struct v2f * dir, struct v2f * plane) 
        : maze(maze), pos(pos), dir(dir), plane(plane) {}
    
    /**
     * @brief Renders a frame by untextured raycasting, using GL2d libnds functions
    */
    void drawFrame();
private:
    Maze * maze;

    // Player position
    struct v2f * pos;

    // Direction player is looking
    struct v2f * dir;

    // Camera plane
    struct v2f * plane;
};

#endif // RAYCASTER_HPP