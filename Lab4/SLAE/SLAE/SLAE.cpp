#include <iostream>
#include "MatrixMethod.h"
#include "GaussMethod.h"

using namespace std;

int main()
{
    system("chcp 1251");
    system("cls");

    srand(time(NULL));

    int equations_amount;
    cout << "Введите количество уравнений: ";
    cin >> equations_amount;

    cout << "Матричный метод:\n";
    MatrixMethod matrix_method(equations_amount);
    matrix_method.run_matrix_method();

    cout << "Метод Гаусса:\n";
    GaussMethod gauss_method(equations_amount);
    gauss_method.run_gauss_method();
}
