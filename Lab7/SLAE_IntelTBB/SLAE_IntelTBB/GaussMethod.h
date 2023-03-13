#include <iostream>
#include <vector>
#include <time.h>
#include <tbb/tbb.h>

using namespace std;
using namespace tbb;

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
    double random(const int min, const int max)
    {
        if (min == max)
            return min;
        return min + rand() % (max - min);
    }

    vector<double> generate_random_double_array(int size)
    {
        vector<double> random_array;
        for (int i = 0; i < size; i++)
        {
            double random_number = random(0, 100);
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
//#pragma omp parallel for
            for (int col = 0; col < size; col++)
            {
                input_matrix[i][col] = input_matrix[i][col] / scale;
                I[i][col] = I[i][col] / scale;
            }
            if (i < size - 1)
            {
//#pragma omp parallel for
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
//#pragma omp parallel for
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
        // Наша матрица
        double** A;
        A = new double* [equations_amount];
        for (int i = 0; i < equations_amount; i++)
            A[i] = new double[equations_amount];
        // Вектор правой части
        double* B = new double[equations_amount];

        // Заполняем матрицу коэффициентов и B
        for (int i = 0; i < equations_amount; i++)
        {
            for (int j = 0; j < equations_amount; j++)
                A[i][j] = random(0, 100);
            B[i] = random(0, 100);
        }

        tick_count beg = tick_count::now();
        // Создаем параллельный контекст
        parallel_for(0, equations_amount, 1, [&](int i) {
            // Находим максимальный элемент в столбце
            double max = 0.0;
            for (int j = 0; j < equations_amount; j++) {
                if (A[i][j] > max)
                    max = A[i][j];
            }

            // Делим строку на максимальный элемент
            for (int j = 0; j < equations_amount; j++) {
                A[i][j] /= max;
            }
            B[i] /= max;
            });

        // Вычитаем строку из всех последующих строк
        parallel_for(0, equations_amount - 1, 1, [&](int i) {
            for (int j = i + 1; j < equations_amount; j++) {
                double m = A[j][i] / A[i][i];
                for (int k = 0; k < equations_amount; k++) {
                    A[j][k] -= m * A[i][k];
                }
                B[j] -= m * B[i];
            }
            });

        // Находим решение
        double* X = new double[equations_amount];
        for (int i = equations_amount - 1; i >= 0; i--) {
            X[i] = B[i];
            for (int j = i + 1; j < equations_amount; j++) {
                X[i] -= A[i][j] * X[j];
            }
            X[i] /= A[i][i];
        }

        tick_count end = tick_count::now();
        printf("Время, затраченное на вычисление: %f с.\n", (end - beg).seconds());

        // Выводим результат
        //cout << endl << "Решение:" << endl;
        //for (int i = 0; i < equations_amount; i++) {
        //    cout << X[i] << endl;
        //}
    }
};
