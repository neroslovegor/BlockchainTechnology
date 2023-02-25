#include <iostream>
#include <vector>
#include <time.h>
#include <omp.h>

using namespace std;

#pragma once
class GaussMethod
{
private:
    int equations_amount;

public:
    GaussMethod(int n)
    {
        equations_amount = n;
    }

private:
    vector<double> generate_random_double_array(int size)
    {
        vector<double> random_array;
        for (int i = 0; i < size; i++)
        {
            double random_number = (rand() % 99) + 10;
            random_array.push_back(random_number);
        }
        return random_array;
    }

    vector<vector<double>> generate_random_double_matrix(int size)
    {
        // declare 2D vector
        vector<vector<double>> matrix;
        for (int i = 0; i < size; i++)
        {
            vector<double> row = generate_random_double_array(size);
            matrix.push_back(row);
        }

        return matrix;
    }

    vector<vector<double>> generate_identity(int size)
    {
        vector<vector<double>> I;
        for (int i = 0; i < size; i++)
        {
            vector<double> row;
            for (int j = 0; j < size; j++)
            {
                if (i == j)
                {
                    row.push_back(1);
                    continue;
                }
                row.push_back(0);
            }
            I.push_back(row);
        }
        return I;
    }

    vector<vector<double>> generate_inverse_parallel(vector<vector<double>> input_matrix)
    {
        int size = input_matrix.size();
        vector<vector<double>> I = generate_identity(size);
        for (int i = 0; i < size; i++)
        {
            if (input_matrix[i][i] == 0)
            {
                // swap nearest subsequent row s.t input_matrix[i][i] != 0 after swapping
                for (int j = i + 1; j < size; j++)
                {
                    if (input_matrix[j][i] != 0.0)
                    {
                        swap(input_matrix[i], input_matrix[j]);
                        break;
                    }
                    if (j == size - 1)
                    {
                        cout << "Inverse does not exist for this matrix";
                        exit(0);
                    }
                }
            }
            double scale = input_matrix[i][i];
#pragma omp parallel for
            for (int col = 0; col < size; col++)
            {
                input_matrix[i][col] = input_matrix[i][col] / scale;
                I[i][col] = I[i][col] / scale;
            }
            if (i < size - 1)
            {
#pragma omp parallel for
                for (int row = i + 1; row < size; row++)
                {
                    double factor = input_matrix[row][i];
                    for (int col = 0; col < size; col++)
                    {
                        input_matrix[row][col] -= factor * input_matrix[i][col];
                        I[row][col] -= factor * I[i][col];
                    }
                }
            }
        }
        for (int zeroing_col = size - 1; zeroing_col >= 1; zeroing_col--)
        {
#pragma omp parallel for
            for (int row = zeroing_col - 1; row >= 0; row--)
            {
                double factor = input_matrix[row][zeroing_col];
                for (int col = 0; col < size; col++)
                {
                    input_matrix[row][col] -= factor * input_matrix[zeroing_col][col];
                    I[row][col] -= factor * I[zeroing_col][col];
                }
            }
        }
        return I;
    }

    void print_matrix(vector<vector<double>> matrix)
    {
        for (size_t i = 0; i < matrix.size(); i++)
        {
            cout << endl;
            for (size_t j = 0; j < matrix.size(); j++)
            {
                cout << matrix[i][j] << " ";
            }
        }
        cout << endl
            << endl;
    }

public:
    void run_gauss_method()
    {
        double dtime;
        vector<vector<double>> matrix;
        vector<vector<double>> matrix_parallel;

        matrix = generate_random_double_matrix(equations_amount);

        double seconds = clock();
        matrix_parallel = generate_inverse_parallel(matrix);
        seconds = (clock() - seconds) / 1000;
        printf("Время, затраченное на вычисление: %f с.\n", seconds);
    }
};
