// ClusterSimulation2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include <omp.h>
#include "Cluster.hpp"
#include "DynamicArray.hpp"

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

using namespace std;

void printResults(double** buf, uint64_t simulations, uint64_t intervals, const char* ms, const char *ds)
{
    // Память под матожидание и дисперсию
    double* M = new double[intervals];
    double* D = new double[intervals];

    // Вычислим матожидание
    for (int k = 0; k < intervals; k++) {
        M[k] = 0;
        for (int i = 0; i < simulations; i++) {
            M[k] += buf[k][i];
        }
        M[k] /= simulations;
    }
    // вычислим дисперсию
    for (int k = 0; k < intervals; k++) {
        D[k] = 0;
        for (int i = 0; i < simulations; i++) {
            D[k] += (buf[k][i] - M[k]) * (buf[k][i] - M[k]);
        }
        D[k] /= simulations - 1;
    }
    //Выведим на экран:
    cerr << ms << " = c(";
    for (int k = 0; k < intervals - 1; k++) {
        cerr << M[k] << ", ";
    }
    cerr << M[intervals - 1] << ")\n";
    cerr << ds << " = c(";
    for (int k = 0; k < intervals - 1; k++) {
        cerr << D[k] << ", ";
    }
    cerr << D[intervals - 1] << ")\n";
    delete[] M;
    delete[] D;
}

int main()
{
    cerr << fixed;
    cerr.precision(6);
    // Параметры симуляции
    uint64_t warmingCount, intervalLen, intervalCount, simCounts;
    // Колво потоков
    int threadsCount = 1;
    // Параметры системы
    double lambda, mu_h, mu_l, p_h, p_l;
    uint64_t serversCount;
    // распределение вероятностей
    double* p;
    // Энергопотребление
    double e_0, e_l, e_h;

    // Переключаемся на utf-8 под виндой
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif 

    cout << u8"Введите кол-во потоков для расчётов: ";
    cin >> threadsCount;

    cout << u8"Введите кол-во симуляций (должно быть чётным): ";
    cin >> simCounts;
    cout << u8"Введите длину прогрева (число событий с начала симуляции, для которых статистика не собирается): ";
    cin >> warmingCount;
    cout << u8"Введите длину одного интервала симуляции: ";
    cin >> intervalLen;
    cout << u8"Введите кол-во интервалов. Статистика будет выводится по завершеню каждого интервала: ";
    cin >> intervalCount;

    cout << u8"Введите кол-во серверов в кластере: ";
    cin >> serversCount;
    if (serversCount > 1) {
        p = new double(serversCount - 1);
        cout << u8"Введите распределение для числа заявок через пробел\n NB: количество чисел должно быть на 1 меньше, чем серверов: ";
        for (int k = 0; k < (serversCount - 1); k++) {
            cin >> p[k];
        }
    }else{
        p = NULL;
    }
    cout << u8"Введите интенсивность прихода заявок: ";
    cin >> lambda;
    cout << u8"Введите скорость в повышеном режиме: ";
    cin >> mu_h;
    cout << u8"Введите скорость в энергосберегающем режиме: ";
    cin >> mu_l;
    cout << u8"Введите вероятность переключения в повышенный режим: ";
    cin >> p_h;
    cout << u8"Введите вероятность переключения в энергосберегающий режим: ";
    cin >> p_l;

    cout << u8"Введите потребляемую мощность в простое: ";
    cin >> e_0;
    cout << u8"Введите потребляемую мощность в энергосберегающем режиме: ";
    cin >> e_l;
    cout << u8"Введите потребляемую мощность в повышенном режиме: ";
    cin >> e_h;

    // Выделим память под результаты опытов
    // Собираем среднее число пользователей в системе и очереди
    // среднее время обработки и в очереди, средняя потребляемая энергия
    double** Sr = new double*[intervalCount];
    for (int k = 0; k < intervalCount; k++) {
        Sr[k] = new double[simCounts];
    }
    double** Tqr = new double* [intervalCount];
    for (int k = 0; k < intervalCount; k++) {
        Tqr[k] = new double[simCounts];
    }
    double** Mr = new double* [intervalCount];
    for (int k = 0; k < intervalCount; k++) {
        Mr[k] = new double[simCounts];
    }
    double** Mqr = new double* [intervalCount];
    for (int k = 0; k < intervalCount; k++) {
        Mqr[k] = new double[simCounts];
    }
    double** Wr = new double* [intervalCount];
    for (int k = 0; k < intervalCount; k++) {
        Wr[k] = new double[simCounts];
    }

    // Запустим симуляцию
    Cluster cl;
    #pragma omp parallel for schedule(dynamic) private(cl)
    for (int k = 0; k < simCounts / 2; k++) {
        cl.syncGenerators();
        cl.init(lambda, mu_h, mu_l, p_h, p_l, serversCount, p);
        cl.simulate(warmingCount, intervalLen, intervalCount);
        cl.calculatePower(e_0, e_l, e_h);
        for (int i = 0; i < intervalCount; i++) {
            Mqr[i][k] = cl.meanAppsInQueue[i];
            Mr[i][k] = cl.meanAppsInSystem[i];
            Sr[i][k] = cl.meanProcessingTime[i];
            Tqr[i][k] = cl.meanWaitingTime[i];
            Wr[i][k] = cl.meanPower[i];
        }
        cl.init(lambda, mu_h, mu_l, p_h, p_l, serversCount, p);
        cl.useAlternativeGenertors();
        cl.simulate(warmingCount, intervalLen, intervalCount);
        cl.calculatePower(e_0, e_l, e_h);
        for (int i = 0; i < intervalCount; i++) {
            Mqr[i][k + simCounts / 2] = cl.meanAppsInQueue[i];
            Mr[i][k + simCounts / 2] = cl.meanAppsInSystem[i];
            Sr[i][k + simCounts / 2] = cl.meanProcessingTime[i];
            Tqr[i][k + simCounts / 2] = cl.meanWaitingTime[i];
            Wr[i][k + simCounts / 2] = cl.meanPower[i];
        }
    }
    
    // TODO: Надо ещё вывести распределение для системы.
    printResults(Mqr, simCounts, intervalCount, "E[Mq]", "D[Mq]");
    printResults(Mr, simCounts, intervalCount, "E[M]", "D[M]");
    printResults(Sr, simCounts, intervalCount, "E[S]", "D[S]");
    printResults(Tqr, simCounts, intervalCount, "E[Tq]", "D[Tq]");
    printResults(Wr, simCounts, intervalCount, "E[W]", "D[W]");
    
    return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.

