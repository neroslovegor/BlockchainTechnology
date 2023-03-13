#include <iostream>
#include <time.h>
#include <vector>
#include <tbb/tbb.h>

using namespace std;
using namespace tbb;

#pragma once
class MatrixMethod
{
private:
    int equations_amount;

public:
    MatrixMethod(int n)
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
        vector<vector<double>> matrix;
        for (int i = 0; i < size; i++)
        {
            vector<double> row = generate_random_double_array(size);
            matrix.push_back(row);
        }

        return matrix;
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


    void print_array(vector<double> array)
    {
        for (size_t i = 0; i < array.size(); i++)
        {
            cout << array[i] << endl;
        }
    }


    // Функция вычисления обратной матрицы
    vector<vector<double>> search_reverse_matrix(vector<vector<double>> matrix, int n) {
        // Объявляем матрицу для хранения результата
        vector<vector<double>> result(n, vector<double>(n));

        // Обратная матрица равна единичной матрице
        for (int i = 0; i < n; i++)
            result[i][i] = 1;

        // Находим обратную матрицу
        for (int i = 0; i < n; i++) {
            // Находим детерминант матрицы
            double det = matrix[i][i];
            // Инвертируем детерминант
            det = 1.0 / det;

            // Находим обратную матрицу
            for (int j = 0; j < n; j++) {
                matrix[i][j] *= det;
                result[i][j] *= det;
            }

            // Делим строку на детерминант
            parallel_for(tbb::blocked_range<int>(0, n),
                [&](const tbb::blocked_range<int>& r) {
                    for (int j = r.begin(); j != r.end(); ++j) {
                        matrix[i][j] *= det;
                        result[i][j] *= det;
                    }
                });

            // Вычитаем строку из остальных строк
            parallel_for(tbb::blocked_range<int>(0, n),
                [&](const tbb::blocked_range<int>& r) {
                    for (int k = r.begin(); k != r.end(); ++k) {
                        if (k != i) {
                            double c = matrix[k][i];
                            for (int j = 0; j < n; j++) {
                                matrix[k][j] -= matrix[i][j] * c;
                                result[k][j] -= result[i][j] * c;
                            }
                        }
                    }
                });
        }
        return matrix;
    }

public:
    void run_matrix_method()
    {
        vector<vector<double>> a = generate_random_double_matrix(equations_amount);
        vector<double> b = generate_random_double_array(equations_amount);
        vector<double> x(equations_amount);
        //print_matrix(a);
        //print_array(b);

        // Нахождение обратной матрицы
        vector<vector<double>> E(equations_amount, vector<double>(equations_amount));
        for (int i = 0; i < equations_amount; ++i) {
            E[i][i] = 1;    
        }

        tick_count beg = tick_count::now();

        E = search_reverse_matrix(a, equations_amount);

        // Нахождение ответа
        parallel_for(blocked_range<size_t>(0, equations_amount),
            [&](const blocked_range<size_t>& r) {
                for (size_t i = r.begin(); i < r.end(); ++i) {
                    double sum = 0;
                    for (int j = 0; j < equations_amount; ++j) {
                        sum += E[i][j] * b[j];
                    }
                    x[i] = sum;
                }
            });

        //for (int i = 0; i < equations_amount; ++i) {
        //    cout << x[i] << ' ';
        //}
        //cout << endl;

        tick_count end = tick_count::now();
        printf("Время, затраченное на вычисление: %f с.\n", (end - beg).seconds());
    }
};
