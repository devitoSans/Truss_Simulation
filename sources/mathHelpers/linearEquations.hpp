#ifndef __LINEAR_EQUATIONS__
#define __LINEAR_EQUATIONS__

#include <stdlib.h>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <cmath>
#include <splashkit.h>
#include "NMatrix.hpp"


inline int find_max_pivot(int currentPivotRow, NMatrix& nmatrix)
{
    int pivotCol = currentPivotRow;

    int potentialPivot = nmatrix.get(currentPivotRow,pivotCol);
    int pivotIndex = currentPivotRow;

    for(int j = currentPivotRow; j < nmatrix.get_row_size(); j++)
    {
        if(potentialPivot < nmatrix.get(j,pivotCol))
        {
            potentialPivot = nmatrix.get(j,pivotCol);
            pivotIndex = j;
        }
    }
    return pivotIndex;
}

inline void swap_row(int row1, int row2, NMatrix& nmatrix)
{
    for(int i = 0; i < nmatrix.get_col_size(); i++)
    {
        double rubbish = nmatrix.get(row1,i);
        nmatrix.set(row1,i, nmatrix.get(row2,i));
        nmatrix.set(row2,i, rubbish);
    }
}

inline void __foward_elimination(NMatrix& nmatrix)
{
    // Looping through the columns, except for the last one.
    for(int pivotCol = 0; pivotCol < nmatrix.get_col_size()-1; pivotCol++)
    {
        int pivotRow = pivotCol;

        // Avoiding pivot with zero value
        if(nmatrix.get(pivotRow,pivotCol) == 0)
        {
            int newPivotRow = find_max_pivot(pivotRow,nmatrix);
            swap_row(pivotRow,newPivotRow,nmatrix);
        }

        // Making the element under pivot zero by 
        // substracting pivot's equation (row) times a factor 
        // (so that the pivot willl be equal to an element under it)
        // with the equations under it.
        for(int r = pivotRow+1; r < nmatrix.get_row_size(); r++)
        {
            double factor = nmatrix.get(r,pivotCol)/nmatrix.get(pivotRow,pivotCol);
            
            for(int c = pivotCol; c < nmatrix.get_col_size(); c++)
            {
                nmatrix.set(r,c, nmatrix.get(r,c)-factor*nmatrix.get(pivotRow,c));
            }
        }
    }
}

// Assuming nmatrix is in Row Echelon Form (ref)
inline void __back_substitution(NMatrix& nmatrix, std::vector<double>& roots)
{
    int last_index_row = nmatrix.get_row_size()-1, last_index_col = nmatrix.get_col_size()-1;

    // Solve the last equations
    roots[last_index_row] = nmatrix.get(last_index_row) / nmatrix.get(last_index_row,last_index_col-1);

    // Solve the rest of the equations
    for(int r = last_index_row-1; r >= 0; r--)
    {
        double rubbish = 0;
        for(int c = r+1; c <= last_index_col-1; c++)
        {
            rubbish += roots[c]*nmatrix.get(r,c);
        }
        roots[r] = (nmatrix.get(r) - rubbish) / nmatrix.get(r,r);
    }
}

// Solve using gauss algorithm, forward elimination and backward subsitution.
inline bool solve_linear_equations(NMatrix& nmatrix, std::vector<double>& roots)
{
    if(roots.size() < (int)nmatrix.get_row_size())
    {
        roots.resize(nmatrix.get_row_size(), 0);
    }
    
    // FORWARD ELIMINATION
    __foward_elimination(nmatrix);
    // BACK SUBSTITUTION
    __back_substitution(nmatrix, roots);

    // Division by zero in floating point will result in either NaN, -inf, or inf. Not a crash.
    for(auto r : roots)
    {
        if(!std::isfinite(r))
        {
            return false;
        }
    }

    return true;
}

#endif // __LINEAR_EQUATIONS__