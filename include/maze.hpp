/**
 * Maze representation used by Raycaster.
*/
#ifndef MAZE_HPP
#define MAZE_HPP

#define MAZE_WIDTH          64
#define MAZE_HEIGHT         64

#define MAZE_CELL_FLOOR     0
#define MAZE_CELL_WALL      1

/**
 * Represents a maze. Maze is initially empty.
*/
class Maze {
public:
    
    /**
     * Constructor
    */
    Maze();

    /**
     * Destructor
    */
    ~Maze();

    /**
     * Generate maze.
    */
    void build();

    /**
     * Returns the contents of a cell.
    */
    int getCell(int y, int x) const;

private:

    /**
     * Draws a horizontal or vertical line in the maze.
    */
    void drawLine(int y, int x, int length, bool vertical);

    /**
     * Draws a box around the maze.
    */
    void boxOutside();

    /**
     * Grid itself as 2D array of ints.
     **/ 
    int cell[MAZE_HEIGHT][MAZE_WIDTH];
};

#endif // MAZE_HPP