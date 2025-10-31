#ifndef __MATH_HELPERS__
#define __MATH_HELPERS__

#include <stdlib.h>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <splashkit.h>


class NMatrix
{
    private:
        double* data;
        size_t size;

        void validating_range(int row, int col)
        {
            if(row < 0 || row >= (int)this->size || col < 0 || col >= (int)this->size+1)
            {
                throw std::out_of_range("index (" + std::to_string(row) + ", " + std::to_string(col) + 
                                               ") is out of the range. valid range: 0-" + std::to_string(this->size-1));
            }
        }

        int get_array_index(int row, int col)
        {
            this->validating_range(row,col);
            return this->get_col_size()*row + col;
        }
    
    public:
        /**
         * Create 1-by-2 Matrix.
         */
        NMatrix()
        {
            this->data = (double*)calloc(1*(2), sizeof(double));
            this->size = 1;
        }

        /**
         * Create size-by-(size+1) Matrix.
         */
        NMatrix(int size)
        {
            this->data = (double*)calloc(size*(size+1), sizeof(double));
            this->size = size;
        }
        NMatrix(std::vector<std::vector<double>> matrix)
        {
            this->data = (double*)calloc(matrix.size()*(matrix.size()+1), sizeof(double));
            this->size = matrix.size();
            this->fill(matrix,false);
        }
        ~NMatrix()
        {
            free(this->data);
            this->data = nullptr;
        }

        NMatrix& operator=(const NMatrix& other)
        {
            free(this->data);
            this->data = (double*)calloc(other.size*(other.size+1), sizeof(double)); // TODO: use realloc
            this->size = other.size;
            
            memcpy(this->data, other.data, other.size*(other.size+1)*sizeof(double));
            return *this;
        }

        double get(int row, int col)
        {
            return this->data[this->get_array_index(row,col)];
        }

        double get(int row)
        {
            return this->get(row, this->size);
        }

        void set(int row, int col, double value)
        {
            this->data[this->get_array_index(row,col)] = value;
        }

        void set(int row, double value)
        {
            this->set(row, this->size, value);
        }

        int get_row_size()
        {
            return (int)this->size;
        }
        
        int get_col_size()
        {
            return this->get_row_size()+1;
        }

        void resize(const std::vector<std::vector<double>> matrix)
        {
            if(matrix.size() == 0) 
            {
                return;
            }
            for(int i = 0; i < (int)matrix.size(); i++)
            {
                if(matrix[i].size() != matrix.size()+1)
                {
                    this->~NMatrix();
                    throw std::length_error("matrix must be size-by-(size+1). Found (" + std::to_string(matrix.size()) + ", " + std::to_string(matrix[0].size()) + ")");
                }
            }

            free(this->data);
            this->data = (double*)calloc(matrix.size()*(matrix.size()+1), sizeof(double)); // TODO: use realloc
            this->size = matrix.size();
        }

        void fill(const std::vector<std::vector<double>> matrix, bool doResize = true)
        {
            if(doResize)
            {
                this->resize(matrix);
            }
            
            for(int i = 0; i < this->get_row_size(); i++)
            {
                for(int j = 0; j < this->get_col_size(); j++)
                {
                    this->set(i,j,matrix[i][j]);
                }
            }
        }

        void fill(int row, double value)
        {
            for(int j = 0; j < this->get_col_size(); j++)
            {
                this->set(row,j,value);
            }
        }

        std::string to_string()
        {
            std::string result = "";
            for(int i = 0; i < this->get_row_size(); i++)
            {
                for(int j = 0; j < this->get_col_size(); j++)
                {
                    result.append(std::to_string(this->get(i,j)) + (j+1 == this->get_col_size() && 
                                                                    i+1 == this->get_row_size() ? "" : ", "));
                }
                result.append((i+1 == this->get_row_size() ? "" : "\n"));
            }
            return result;
        }
};

#endif // __MATH__HELPERS__
