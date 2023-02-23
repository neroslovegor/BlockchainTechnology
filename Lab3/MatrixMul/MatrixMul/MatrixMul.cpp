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
    clock_t start = clock();
    for (int i = 0; i < row1; i++)
    {
        for (int j = 0; j < col2; j++)
        {
            for (int k = 0; k < col1; k++)
                c[i][j] += a[i][k] * b[k][j];
        }
    }
    clock_t end = clock();
    double seconds = difftime(end, start);
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
    start = clock();
    for (int i = 0; i < row1; i++)
    {
        for (int k = 0; k < col1; k++)
        {
            for (int j = 0; j < col2; j++)
                c[i][j] += a[i][k] * b[k][j];
        }
    }
    end = clock();
    seconds = difftime(end, start);
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
    int threadsNum = 3;
    omp_set_num_threads(threadsNum);
    start = clock();
    #pragma omp parallel for  shared(a, b, c) private(i, j, k)
    for (int i = 0; i < row1; i++)
    {
        for (int k = 0; k < col1; k++)
        {
            for (int j = 0; j < col2; j++)
                c[i][j] += a[i][k] * b[k][j];
        }
    }
    end = clock();
    seconds = difftime(end, start);
    printf("Время с ускорением и с распараллеливанием: %f seconds\n", seconds);
    // Вывод матрицы произведения
    //cout << "Матрица произведения" << endl;
    //for (int i = 0; i < row1; i++)
    //{
    //    for (int j = 0; j < col2; j++)
    //        cout << c[i][j] << "\t";
    //    cout << endl;
    //}
}
