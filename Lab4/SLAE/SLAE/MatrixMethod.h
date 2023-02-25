#include <iostream>
#include <time.h>
#include <omp.h>
#include <vector>

using namespace std;

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
    bool search_reverse_matrix(vector <vector<double>>& matrix)
    {
        int size = matrix.size();
        vector <vector<double>> E(size, vector<double>(size));

        //���������� ��������� �������
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (i == j) E[i][j] = 1.0;
                else E[i][j] = 0.0;
            }
        }

        //������������� �������� ������� � �����������������
        for (int k = 0; k < size; k++)
        {
            if (abs(matrix[k][k]) < 1e-8)
            {
                bool changed = false;

                for (int i = k + 1; i < size; i++)
                {
                    if (abs(matrix[i][k]) > 1e-8)
                    {
                        swap(matrix[k], matrix[i]);
                        swap(E[k], E[i]);
                        changed = true;
                        break;
                    }
                }

                if (!changed)
                    return false;
            }

            double div = matrix[k][k];

#pragma omp parallel
            {
#pragma omp for
                for (int j = 0; j < size; j++)
                {
                    matrix[k][j] /= div;
                    E[k][j] /= div;
                }
            }

#pragma omp parallel
            {
#pragma omp for
                for (int i = k + 1; i < size; i++)
                {
                    double multi = matrix[i][k];


                    for (int j = 0; j < size; j++)
                    {
                        matrix[i][j] -= multi * matrix[k][j];
                        E[i][j] -= multi * E[k][j];
                    }
                }
            }
        }

        //������������ ��������� ������� �� ��������
        //� �������� �� ���������
        for (int k = size - 1; k > 0; k--)
        {
#pragma omp parallel
            {
#pragma omp for
                for (int i = k - 1; i > -1; i--)
                {
                    double multi = matrix[i][k];

                    for (int j = 0; j < size; j++)
                    {
                        matrix[i][j] -= multi * matrix[k][j];
                        E[i][j] -= multi * E[k][j];
                    }
                }
            }
        }
        matrix = E;
        return true;
    }

    double random(const int min, const int max)
    {
        if (min == max)
            return min;
        return min + rand() % (max - min);
    }

public:
    void run_matrix_method()
    {
        vector<vector<double>> matrix(equations_amount, vector<double>(equations_amount));
        vector<double> B(equations_amount);

        // ��������� ������� ������������� � B
        for (int i = 0; i < equations_amount; i++)
        {
            for (int j = 0; j < equations_amount; j++)
                matrix[i][j] = random(0, 100);
            B[i] = random(0, 100);
        }

        // ����� ������� ���������
        /*cout << "\n������� ���������:\n";
        for (int i = 0; i < equations_amount; i++)
        {
            for (int j = 0; j < equations_amount; j++)
            {

                if (j != 0 && matrix[i][j] >= 0)
                    cout << " +";
                cout << " " << matrix[i][j] << "x" << j + 1;
            }
            cout << " = " << B[i] << endl;
        }*/

        double seconds = clock();

        // ���������� �������� �������
        if (!search_reverse_matrix(matrix))
        {
            cout << "\n���������� ����� �������� �������!\n";
            exit(1);
        }

        // �������-������� ����������� X � ���������� �������������� ����������
        vector<double> X(equations_amount);
#pragma omp parallel
        {
#pragma omp for
            for (int i = 0; i < equations_amount; i++)
            {
                X[i] = 0;
                for (int j = 0; j < equations_amount; j++)
                    X[i] += matrix[i][j] * B[j];
            }
        }

        // ����� �������������� ����������
        /*cout << "\n������� ������� ���������:";
        for (int i = 0; i < equations_amount; i++)
            cout << "\nx" << i + 1 << " = " << X[i];*/

        seconds = (clock() - seconds) / 1000;
        printf("�����, ����������� �� ����������: %f �.\n", seconds);
    }
};
