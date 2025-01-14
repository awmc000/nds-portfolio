#include <check.h>
#include "maze.hpp"

START_TEST (test_maze_build) {
    Maze maze;
    maze.build();
    ck_assert_int_eq(maze.getCell(0, 0), 1);
    ck_assert_int_eq(maze.getCell(5, 0), 1);
    ck_assert_int_eq(maze.getCell(1, 1), 0);
}
END_TEST

Suite * maze_suite(void) {
    Suite * s;
    TCase * tc_maze;

    s = suite_create("Maze");

    tc_maze = tcase_create("Maze");

    tcase_add_test(tc_maze, test_maze_build);
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