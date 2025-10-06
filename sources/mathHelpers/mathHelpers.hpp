#ifndef __MATH_HELPERS__
#define __MATH_HELPERS__

#include <stdlib.h>

class NMatrix
{
    private:
        double* data;
    
    public:
        NMatrix(int size)
        {
            this->data = (double*)calloc(size, sizeof(double));
        }
        ~NMatrix()
        {
            free(data);
        }
};

#endif