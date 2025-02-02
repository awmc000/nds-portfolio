#include <check.h>
#include <vector>
#include <set>
#include <utility>
#include "stdio.h"
#include "maze.hpp"

START_TEST (test_maze_build) {
    Maze maze;
    maze.build();
}
END_TEST

START_TEST (test_maze_get_cell) {
    Maze maze;
    maze.build();
    ck_assert_int_eq(maze.getCell(0, 0), 1);
}
END_TEST

START_TEST (test_maze_get_cell_out_of_bounds) {
    Maze maze;
    maze.build();
    ck_assert_int_eq(maze.getCell(0, MAZE_WIDTH), -1);
}
END_TEST

START_TEST (test_maze_generate_cells) {
    Maze maze;
    std::set<std::pair<int, int>> cells;
    cells = maze.mazeCells();
    fprintf(stderr, "[Cells]\n");
    for (const std::pair<int, int>& c : cells) {
        fprintf(stderr, "{%d, %d}\n", c.first, c.second);
    }

    ck_assert_msg(
        cells.count({0, MAZE_WIDTH-1}) == 0 &&
        cells.count({MAZE_WIDTH-1, 0}) == 0, 
        "Contains an outer tile."
    );

    for (const std::pair<int, int>& c : cells) {
        ck_assert_msg(((c.first % 2) == 1) && ((c.second % 2) == 1),
            "Pair found in cells that is not odd/odd");
    }
}
END_TEST

START_TEST (test_maze_generate_walls) {
    Maze maze;
    std::vector<std::pair<int, int>> walls;
    walls = maze.mazeWalls();
    fprintf(stderr, "[Walls]\n");
    for (const std::pair<int, int>& w : walls) {
        fprintf(stderr, "{%d, %d}\n", w.first, w.second);
    }

    for (const std::pair<int, int>& w : walls) {
        ck_assert_msg(((w.first % 2) == 1) || ((w.second % 2) == 1),
            "Pair found in walls that is not diff parity");
    }
}
END_TEST

Suite * maze_suite(void) {
    Suite * s;
    TCase * tc_maze;

    s = suite_create("Maze");

    tc_maze = tcase_create("Maze");

    tcase_add_test(tc_maze, test_maze_build);
    tcase_add_test(tc_maze, test_maze_get_cell);
    tcase_add_test(tc_maze, test_maze_get_cell_out_of_bounds);
    tcase_add_test(tc_maze, test_maze_generate_cells);
    tcase_add_test(tc_maze, test_maze_generate_walls);
    
    suite_add_tcase(s, tc_maze);

    return s;
}

int main(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = maze_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}