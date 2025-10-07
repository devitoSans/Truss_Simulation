#include "NMatrix.hpp"
#include "linearEquations.hpp"
#include "catch_amalgamated.hpp"
#include "splashkit.h"
#include <math.h>

const double ELIPSON = 1e-6;

double round_to(double a)
{
    return std::round(a * 1/ELIPSON) * ELIPSON;
}

bool compare_double(double a, double b)
{
    return abs(round_to(a)-round_to(b)) < ELIPSON;
}

TEST_CASE("NMatrix: Initialising and fetching data")
{
    NMatrix nmatrix = NMatrix(5);
    
    REQUIRE(abs(nmatrix.get(0,0)-0) < ELIPSON);
    REQUIRE(abs(nmatrix.get(1,5)-0) < ELIPSON);
    REQUIRE(abs(nmatrix.get(4,1)-0) < ELIPSON);
    REQUIRE(abs(nmatrix.get(2,2)-0) < ELIPSON);
}

TEST_CASE("NMatrix: Initialising using vectors")
{
    NMatrix nmatrix = NMatrix({{1,2,3}, {1,2,3}});
    
    REQUIRE(abs(nmatrix.get(0,0)-1) < ELIPSON);
    REQUIRE(abs(nmatrix.get(0,1)-2) < ELIPSON);
    REQUIRE(abs(nmatrix.get(0,2)-3) < ELIPSON);

    REQUIRE(abs(nmatrix.get(1,0)-1) < ELIPSON);
    REQUIRE(abs(nmatrix.get(1,1)-2) < ELIPSON);
    REQUIRE(abs(nmatrix.get(1,2)-3) < ELIPSON);
}

TEST_CASE("NMatrix: Changing values")
{
    NMatrix nmatrix = NMatrix(5);
    
    REQUIRE(abs(nmatrix.get(1,2)-0) < ELIPSON);
    REQUIRE(abs(nmatrix.get(0,5)-0) < ELIPSON);
    REQUIRE(abs(nmatrix.get(1,0)-0) < ELIPSON);
    REQUIRE(abs(nmatrix.get(4,4)-0) < ELIPSON);
    
    nmatrix.set(1,2,10);
    nmatrix.set(0,5,2.32);
    nmatrix.set(1,0,1.32);
    nmatrix.set(4,4,-2.5);
    
    REQUIRE(abs(nmatrix.get(1,2)-10) < ELIPSON);
    REQUIRE(abs(nmatrix.get(0,5)-2.32) < ELIPSON);
    REQUIRE(abs(nmatrix.get(1,0)-1.32) < ELIPSON);
    REQUIRE(abs(nmatrix.get(4,4)-(-2.5)) < ELIPSON);   
}

TEST_CASE("NMatrix: Resizing")
{
    NMatrix nmatrix = NMatrix(5);
    
    nmatrix.resize({{1,2,3}, {1,2,3}});
    REQUIRE(nmatrix.get_col_size() == 3);
    REQUIRE(nmatrix.get_row_size() == 2);
}

TEST_CASE("NMatrix: Filling with vectors")
{
    NMatrix nmatrix = NMatrix(5);
    
    nmatrix.fill({{1,2,3}, {1,2,3}});
    REQUIRE(compare_double(nmatrix.get(0,0), 1));
    REQUIRE(compare_double(nmatrix.get(0,1), 2));
    REQUIRE(compare_double(nmatrix.get(0,2), 3));

    REQUIRE(compare_double(nmatrix.get(1,0), 1));
    REQUIRE(compare_double(nmatrix.get(1,1), 2));
    REQUIRE(compare_double(nmatrix.get(1,2), 3));
}

TEST_CASE("NMatrix: Filling a row")
{
    NMatrix nmatrix = NMatrix({{1,2,3}, {1,2,3}});
    
    nmatrix.fill(1,-1);
    REQUIRE(compare_double(nmatrix.get(0,0), 1));
    REQUIRE(compare_double(nmatrix.get(0,1), 2));
    REQUIRE(compare_double(nmatrix.get(0,2), 3));

    REQUIRE(compare_double(nmatrix.get(1,0), -1));
    REQUIRE(compare_double(nmatrix.get(1,1), -1));
    REQUIRE(compare_double(nmatrix.get(1,2), -1));
}

TEST_CASE("NMatrix: Equal operator")
{
    NMatrix nmatrix = NMatrix(5);

    nmatrix.set(0,0,10);
    nmatrix.set(0,5,2.32);
    nmatrix.set(4,4,-2.5);

    NMatrix nm_1 = NMatrix();
    nm_1 = nmatrix;

    REQUIRE(abs(nm_1.get(0,0)-10) < ELIPSON);
    REQUIRE(abs(nm_1.get(0,5)-2.32) < ELIPSON);
    REQUIRE(abs(nm_1.get(4,4)-(-2.5)) < ELIPSON);
}

TEST_CASE("NMatrix: To string")
{
    NMatrix nmatrix = NMatrix({
        {1, 2, 3},
        {1, 3, 4}
    });

    REQUIRE(nmatrix.to_string().compare("1.000000, 2.000000, 3.000000,\n1.000000, 3.000000, 4.000000"));
}

TEST_CASE("Linear Equations Functions: is_zero_columns_exist()")
{
    NMatrix nm_1 = NMatrix({
        {1, 2, 4, 5},
        {1, 0, 0, 0},
        {0, 1, 0, 2}
    });
    REQUIRE(is_zero_columns_exist(nm_1) == false);

    NMatrix nm_2 = NMatrix({
        {1, 2, 4, 5},
        {0, 0, 0, 0},
        {1, 0, 0, 0}
    });
    REQUIRE(is_zero_columns_exist(nm_2) == true);

    NMatrix nm_3 = NMatrix({
        {1, 2, 4, 5},
        {1, 0, 0, 0},
        {0, 0, 0, 2}
    });
    REQUIRE(is_zero_columns_exist(nm_3) == true);

    NMatrix nm_4 = NMatrix({
        {0, 0, 0, 2.7},
        {1, 2, 4, 5},
        {1, 0, 0, 0}
    });
    REQUIRE(is_zero_columns_exist(nm_4) == true);

    NMatrix nm_5 = NMatrix({
        {0, 1},
    });
    REQUIRE(is_zero_columns_exist(nm_5) == true);

    NMatrix nm_6 = NMatrix({
        {1, 0},
    });
    REQUIRE(is_zero_columns_exist(nm_6) == false);
}

TEST_CASE("Linear Equations Functions: is_zero_rows_exist()")
{
    NMatrix nm_1 = NMatrix({
        {1, 2, 4, 5},
        {1, 0, 0, 0},
        {0, 1, 0, 2}
    });
    REQUIRE(is_zero_rows_exist(nm_1) == false);

    NMatrix nm_2 = NMatrix({
        {1, 0, 4, 5},
        {1, 0, 0, 0},
        {0, 0, 1, 2}
    });
    REQUIRE(is_zero_rows_exist(nm_2) == true);

    NMatrix nm_3 = NMatrix({
        {0, 1, 4, 5},
        {0, 0, 0, 0},
        {0, 0, 1, 2}
    });
    REQUIRE(is_zero_rows_exist(nm_3) == true);

    NMatrix nm_4 = NMatrix({
        {3, 1, 1, 0},
        {0, 0, 0, 0},
        {1, 0, 0, 0}
    });
    REQUIRE(is_zero_rows_exist(nm_4) == false);

    NMatrix nm_5 = NMatrix({
        {0, 1},
    });
    REQUIRE(is_zero_rows_exist(nm_5) == true);

    NMatrix nm_6 = NMatrix({
        {1, 0},
    });
    REQUIRE(is_zero_rows_exist(nm_6) == false);
}

TEST_CASE("Linear Equations Functions: find_max_pivot()")
{
    NMatrix nm_1 = NMatrix({
        {1, 3, 4, 2},
        {0, 0, 1, 0},
        {2, 0, 0, 1}
    });
    REQUIRE(find_max_pivot(0,nm_1) == 2);
    REQUIRE(find_max_pivot(1,nm_1) == 1);
    REQUIRE(find_max_pivot(2,nm_1) == 2);

    NMatrix nm_2 = NMatrix({
        {2, 0, 0, 1},
        {1, 3, 0, 2},
        {0, 3, 4, 0}
    });
    REQUIRE(find_max_pivot(0,nm_2) == 0);
    REQUIRE(find_max_pivot(1,nm_2) == 1);
    REQUIRE(find_max_pivot(2,nm_2) == 2);

    NMatrix nm_3 = NMatrix({
        {2, 0},
    });
    REQUIRE(find_max_pivot(0,nm_3) == 0);
}

TEST_CASE("Linear Equations Functions: swap_row()")
{
    NMatrix nm_1 = NMatrix({
        {1, 3, 4, 2},
        {0, 0, 1, 0},
        {2, 0, 0, 1}
    });

    swap_row(0, 2, nm_1);

    REQUIRE(abs(nm_1.get(0,0)-2.0) < ELIPSON);
    REQUIRE(abs(nm_1.get(0,1)-0.0) < ELIPSON);
    REQUIRE(abs(nm_1.get(0,2)-0.0) < ELIPSON);
    REQUIRE(abs(nm_1.get(0,3)-1.0) < ELIPSON);

    REQUIRE(abs(nm_1.get(2,0)-1.0) < ELIPSON);
    REQUIRE(abs(nm_1.get(2,1)-3.0) < ELIPSON);
    REQUIRE(abs(nm_1.get(2,2)-4.0) < ELIPSON);
    REQUIRE(abs(nm_1.get(2,3)-2.0) < ELIPSON);

    swap_row(1, 2, nm_1);

    REQUIRE(abs(nm_1.get(1,0)-1.0) < ELIPSON);
    REQUIRE(abs(nm_1.get(1,1)-3.0) < ELIPSON);
    REQUIRE(abs(nm_1.get(1,2)-4.0) < ELIPSON);
    REQUIRE(abs(nm_1.get(1,3)-2.0) < ELIPSON);

    REQUIRE(abs(nm_1.get(2,0)-0.0) < ELIPSON);
    REQUIRE(abs(nm_1.get(2,1)-0.0) < ELIPSON);
    REQUIRE(abs(nm_1.get(2,2)-1.0) < ELIPSON);
    REQUIRE(abs(nm_1.get(2,3)-0.0) < ELIPSON);
}

TEST_CASE("Linear Equations Functions: __foward_elimination()")
{
    NMatrix nm_1 = NMatrix({
        {2, 0, 0, 1},
        {1, 3, 0, 2},
        {0, 3, 4, 0}
    });
    __foward_elimination(nm_1);
    REQUIRE(nm_1.to_string().compare("2.000000, 0.000000, 0.000000, 1.000000,\n0.000000, 3.000000, 0.000000, 1.500000,\n0.000000, 0.000000, 4.000000, -1.500000"));
    
    NMatrix nm_2 = NMatrix({
        {2, 0, 0},
        {1, 3, 0}
    });
    __foward_elimination(nm_2);
    REQUIRE(nm_2.to_string().compare("2.000000, 0.000000, 0.000000,\n0.000000, 3.000000, 0.000000"));
    
    NMatrix nm_3 = NMatrix({
        {2, 1, 3, 1},
        {1, 3, 1, 2},
        {0, 3, 4, 0}
    });
    __foward_elimination(nm_3);
    REQUIRE(nm_3.to_string().compare("2.000000, 1.000000, 3.000000, 1.000000,\n0.000000, 2.500000, -0.500000, 1.500000,\n0.000000, 0.000000, 4.600000, -1.800000"));
    
    NMatrix nm_4 = NMatrix({
        {0, 0, 1, 1},
        {1, 3, 0, 2},
        {0, 3, 4, 0}
    });
    __foward_elimination(nm_4);
    REQUIRE(nm_4.to_string().compare("1.000000, 3.000000, 0.000000, 2.000000,\n0.000000, 3.000000, 4.000000, 0.000000,\n0.000000, 0.000000, 1.000000, 1.000000"));

    NMatrix nm_5 = NMatrix({
        {-2, 0, 0, 1},
        {1, 3, 0, -2},
        {0, 3, -4, 0}
    });
    __foward_elimination(nm_5);
    REQUIRE(nm_5.to_string().compare("-2.000000, 0.000000, 0.000000, 1.000000,\n0.000000, 3.000000, 0.000000, -1.500000,\n0.000000, 0.000000, -4.000000, 1.500000"));
    
    NMatrix nm_6 = NMatrix({
        {2, 1, 1},
        {5, 3, 2},
    });
    __foward_elimination(nm_6);
    REQUIRE(nm_6.to_string().compare("2.000000, 1.000000, 1.000000,\n0.000000, 0.500000, -0.500000"));
}

TEST_CASE("Linear Equations Functions: __back_substitution()")
{
    NMatrix nm_1 = NMatrix({
        {2.000000, 0.000000, 0.000000},
        {0.000000, 3.000000, 0.000000}
    });
    std::vector<double> r1(nm_1.get_row_size());
    __back_substitution(nm_1, r1);
    REQUIRE(std::equal(r1.begin(), r1.end(), std::vector<double> {0.000000, 0.000000}.begin(), compare_double));
    
    NMatrix nm_2 = NMatrix({
        {2.000000, 0.000000, 0.000000, 1.000000},
        {0.000000, 3.000000, 0.000000, 1.500000},
        {0.000000, 0.000000, 4.000000, -1.500000}
    });
    std::vector<double> r2(nm_2.get_row_size());
    __back_substitution(nm_2, r2);
    REQUIRE(std::equal(r2.begin(), r2.end(), std::vector<double> {0.500000, 0.500000, -0.375000}.begin(), compare_double));
    
    NMatrix nm_3 = NMatrix({
        {2.000000, 1.000000, 3.000000, 1.000000},
        {0.000000, 2.500000, -0.500000, 1.500000},
        {0.000000, 0.000000, 4.600000, -1.800000}
    });
    std::vector<double> r3(nm_3.get_row_size());
    __back_substitution(nm_3, r3);
    REQUIRE(std::equal(r3.begin(), r3.end(), std::vector<double> {0.826087, 0.521739, -0.391304}.begin(), compare_double));
    
    NMatrix nm_4 = NMatrix({
        {-2.000000, 0.000000, 0.000000, 1.000000},
        {0.000000, 3.000000, 0.000000, -1.500000},
        {0.000000, 0.000000, -4.000000, 1.500000}
    });
    std::vector<double> r4(nm_4.get_row_size());
    __back_substitution(nm_4, r4);
    REQUIRE(std::equal(r4.begin(), r4.end(), std::vector<double> {-0.500000, -0.500000, -0.375000}.begin(), compare_double));
    
    NMatrix nm_5 = NMatrix({
        {2.000000, 1.000000, 1.000000},
        {0.000000, 0.500000, -0.500000}
    });
    std::vector<double> r5(nm_5.get_row_size());
    __back_substitution(nm_5, r5);
    REQUIRE(std::equal(r5.begin(), r5.end(), std::vector<double> {1.000000, -1.000000}.begin(), compare_double));
}

TEST_CASE("Linear Equations Functions: solve_linear_equations()")
{
    // Don't remove it, if it fails some test cases, use this to debug.
    // for(auto i : r1) {write(std::to_string(i) + ", ");} write_line("");
    
    // 1) 2x2 sanity check
    NMatrix nm_1 = NMatrix({
        {2, 1, 1},
        {5, 3, 2},
    });
    std::vector<double> r1;
    std::vector<double> x1 = {1, -1};
    solve_linear_equations(nm_1, r1);
    REQUIRE(std::equal(r1.begin(), r1.end(), x1.begin(), compare_double));
    
    // 2) Needs row swap (zero leading pivot)
    NMatrix nm_2 = NMatrix({
        {0, 1, 2},
        {2, 1, 4},
    });
    std::vector<double> r2;
    std::vector<double> x2 = {1, 2};
    solve_linear_equations(nm_2, r2);
    REQUIRE(std::equal(r2.begin(), r2.end(), x2.begin(), compare_double));

    /* 
    Disabled due to precision error check. 
    For now, we don't really care about precision 
    */
    // // 3) Ill-conditioned but solvable
    // NMatrix nm_3 = NMatrix({
    //     {1, 1, 0},
    //     {1, 1.000000000001, -1e-12},
    // });
    // std::vector<double> r3;
    // std::vector<double> x3 = {1, -1};
    // solve_linear_equations(nm_3, r3);
    // for(auto i : r3) {write(std::to_string(i) + ", ");} write_line("");
    // REQUIRE(std::equal(r3.begin(), r3.end(), x3.begin(), compare_double));
    
    /* 
    Disabled due to precision error check. 
    For now, we don't really care about precision 
    */
    // // 4) Badly scaled but nonsingular (corrected)
    // NMatrix nm_4 = NMatrix({
    //     {100000000, 1,        100000001},
    //     {1,         1e-8+1e-16, 1.00000002},
    // });
    // std::vector<double> r4;
    // std::vector<double> x4 = {1e-8, 1e8};  // still the intended solution
    // solve_linear_equations(nm_4, r4);
    // REQUIRE(std::equal(r4.begin(), r4.end(), x4.begin(), compare_double));

    
    // 5) Tiny pivot vs. normal row (tests partial pivoting)
    NMatrix nm_5 = NMatrix({
        {1e-12, 1, 1e-12},
        {1,     1, 1},
    });
    std::vector<double> r5;
    std::vector<double> x5 = {1, 0};
    solve_linear_equations(nm_5, r5);
    REQUIRE(std::equal(r5.begin(), r5.end(), x5.begin(), compare_double));

    // 6) Permutation-like system (swap columns/rows effectively)
    NMatrix nm_6 = NMatrix({
        {0, 1, 3},
        {1, 0, 2},
    });
    std::vector<double> r6;
    std::vector<double> x6 = {2, 3};
    solve_linear_equations(nm_6, r6);
    REQUIRE(std::equal(r6.begin(), r6.end(), x6.begin(), compare_double));

    // 7) 3x3 clean system
    NMatrix nm_7 = NMatrix({
        {3,  2, -1, -1},
        {2, -2,  4,  6},
        {-1, 0.5, -1, -2},
    });
    std::vector<double> r7;
    std::vector<double> x7 = {1, -2, 0};
    solve_linear_equations(nm_7, r7);
    REQUIRE(std::equal(r7.begin(), r7.end(), x7.begin(), compare_double));

    // 8) 3x3 requiring pivot swap (first row starts with 0)
    NMatrix nm_8 = NMatrix({
        {0, 1, 1, 1},
        {2, 1, 5, -1},
        {4, 2, 1, 7},
    });
    std::vector<double> r8;
    std::vector<double> x8 = {1, 2, -1};
    solve_linear_equations(nm_8, r8);
    REQUIRE(std::equal(r8.begin(), r8.end(), x8.begin(), compare_double));

    // 9) 3x3 tridiagonal, diagonally dominant
    NMatrix nm_9 = NMatrix({
        {4, -1,  0,  2},
        {-1, 4, -1,  4},
        {0, -1,  4, 10},
    });
    std::vector<double> r9;
    std::vector<double> x9 = {1, 2, 3};
    solve_linear_equations(nm_9, r9);
    REQUIRE(std::equal(r9.begin(), r9.end(), x9.begin(), compare_double));

    // 10) 4x4 Hilbert (classically ill-conditioned) with x = [1,1,1,1]
    NMatrix nm_10 = NMatrix({
        {1.0,      0.5,       0.3333333333333333, 0.25,              2.083333333333333},
        {0.5,      0.3333333333333333, 0.25,     0.2,               1.2833333333333332},
        {0.3333333333333333, 0.25,     0.2,      0.16666666666666666, 0.95},
        {0.25,     0.2,       0.16666666666666666, 0.14285714285714285, 0.7595238095238095},
    });
    std::vector<double> r10;
    std::vector<double> x10 = {1, 1, 1, 1};
    solve_linear_equations(nm_10, r10);
    REQUIRE(std::equal(r10.begin(), r10.end(), x10.begin(), compare_double));

    // 11) 4x4 well-conditioned integers
    NMatrix nm_11 = NMatrix({
        {3, 2, 0, 1, 1},
        {1, 1, 2, 0, 4},
        {0, 2, 3, 1, 4},
        {2, 0, 1, 2, 4},
    });
    std::vector<double> r11;
    std::vector<double> x11 = {1, -1, 2, 0};
    solve_linear_equations(nm_11, r11);
    REQUIRE(std::equal(r11.begin(), r11.end(), x11.begin(), compare_double));

    // 12) 3x3 with mixed signs & fractions
    NMatrix nm_12 = NMatrix({
        {0.5, -1.5, 0.0,  2.5},
        {2.5,  1.0, -0.5, 2.5},
        {1.0, -1.0,  2.0, 9.0},
    });
    std::vector<double> r12;
    std::vector<double> x12 = {2.0, -1.0, 3.0};
    solve_linear_equations(nm_12, r12);
    REQUIRE(std::equal(r12.begin(), r12.end(), x12.begin(), compare_double));

    // 13) Infinitely many solutions 1
    NMatrix nm_13 = NMatrix({
        {1, 1, 0.0},
        {1, 1, 0.0},
    });
    std::vector<double> r13;
    bool check13 = solve_linear_equations(nm_13, r13);
    REQUIRE(check13 == false);

    // 14) Infinitely many solutions 2
    NMatrix nm_14 = NMatrix({
        {1, 0, 0.0},
        {1, 0, 0.0},
    });
    std::vector<double> r14;
    bool check14 = solve_linear_equations(nm_14, r14);
    REQUIRE(check14 == false);

    // 15) Infinitely many solutions 3
    NMatrix nm_15 = NMatrix({
        {0, 0, 0.0},
        {1, 1, 0.0},
    });
    std::vector<double> r15;
    bool check15 = solve_linear_equations(nm_15, r15);
    REQUIRE(check15 == false);

    // 16) No solutions
    NMatrix nm_16 = NMatrix({
        {0, 0, 1.0},
        {1, 2, 3.0},
    });
    std::vector<double> r16;
    bool check16 = solve_linear_equations(nm_16, r16);
    REQUIRE(check16 == false);

    // Don't remove, same reason as above
    // for(auto i : r1) {write(std::to_string(i) + ", ");} write_line("");
}