#include <iostream>
#include <cstdlib>
#include <omp.h>

using namespace std;

int main()
{
    system("chcp 1251");
    system("cls");

    srand(time(NULL));

    int row1 = 2000, row2 = 2000, col1 = 2000, col2 = 2000;
    int** a, ** b, ** c;

    // Ввод элементов первой матрицы
    a = new int* [row1];
    for (int i = 0; i < row1; i++)
    {
        a[i] = new int[col1];
        for (int j = 0; j < col1; j++)
        {
            a[i][j] = rand() % 100 + 1;
        }
    }
    // Вывод элементов первой матрицы
    //for (int i = 0; i < row1; i++)
    //{
    //    for (int j = 0; j < col1; j++)
    //        cout << a[i][j] << "\t";
    //    cout << endl;
    //}
    // Ввод элементов второй матрицы
    b = new int* [row2];
    for (int i = 0; i < row2; i++)
    {
        b[i] = new int[col2];
        for (int j = 0; j < col2; j++)
        {
            b[i][j] = rand() % 100 + 1;
        }
    }
    // Вывод элементов второй матрицы
    //for (int i = 0; i < row2; i++)
    //{
    //    for (int j = 0; j < col2; j++)
    //    {
    //        cout << b[i][j] << "\t";
    //    }
    //    cout << endl;
    //}
    
    // Умножение матриц
    c = new int* [row1];
    for (int i = 0; i < row1; i++)
    {
        c[i] = new int[col2];
        for (int j = 0; j < col2; j++)
        {
            c[i][j] = 0;
        }
    }
    double seconds = clock();
    for (int i = 0; i < row1; i++)
    {
        for (int j = 0; j < col2; j++)
        {
            for (int k = 0; k < col1; k++)
                c[i][j] += a[i][k] * b[k][j];
        }
    }
    seconds = (clock() - seconds) / 1000;
    printf("Время без ускорением: %f seconds\n", seconds);

    // Умножение матриц
    c = new int* [row1];
    for (int i = 0; i < row1; i++)
    {
        c[i] = new int[col2];
        for (int j = 0; j < col2; j++)
        {
            c[i][j] = 0;
        }
    }
    seconds = clock();
    for (int i = 0; i < row1; i++)
    {
        for (int k = 0; k < col1; k++)
        {
            for (int j = 0; j < col2; j++)
                c[i][j] += a[i][k] * b[k][j];
        }
    }
    seconds = (clock() - seconds) / 1000;
    printf("Время с ускорением: %f seconds\n", seconds);

    // Умножение матриц
    c = new int* [row1];
    for (int i = 0; i < row1; i++)
    {
        c[i] = new int[col2];
        for (int j = 0; j < col2; j++)
        {
            c[i][j] = 0;
        }
    }

    seconds = clock();
    int i, j, k;
    #pragma omp parallel for  shared(a, b, c) private(i, j, k)
    for (i = 0; i < row1; i++)
    {
        for (k = 0; k < col1; k++)
        {
            for (j = 0; j < col2; j++)
                c[i][j] += a[i][k] * b[k][j];
        }
    }
    seconds = (clock() - seconds) / 1000;
    printf("Время с ускорением и с распараллеливанием: %f с.\n", seconds);
    // Вывод матрицы произведения
    //cout << "Матрица произведения" << endl;
    //for (int i = 0; i < row1; i++)
    //{
    //    for (int j = 0; j < col2; j++)
    //        cout << c[i][j] << "\t";
    //    cout << endl;
    //}
}
