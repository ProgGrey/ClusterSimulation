#pragma once
#include "Timer.hpp"
#include "Statistic.hpp"
#include <deque>
#include "xoshiro/ExponentialDistribution.hpp"
#include "ProbabilityTest.hpp"

struct Application
{
    // Насколько давно пришла заявка
    double time = 0;
    // Сколько нужно узлов
    uint64_t cores;
};


class Cluster
{
private:
    // Генераторы случайных чисел
    // Тест вероятности переключиться в другой режим
    ProbabilityTest probTest;
    // Генераторы времени до прихода в систему
    ExponentialDistribution timeArrival;

    // Генераторы времени до ухода из системы
    ExponentialDistribution timeLeave;

    // Очередь заявок, прибывших в систему
    std::deque<Application>* queue;

    // Прогревается-ли симуляция?
    bool isWarming = false;
    // Таймер, который срабатывает, когда происходят безусловные события
    // Если id события равно нулю, то это приход новой заявки в систему.
    // Если id больше нуля, то это уход обработанной заявки, а id равно 
    // количеству серверов, которое занимала эта заявка при обработке
    Timer* eventsB;

    // Безусловные события
    void applicationArrival();
    void applicationLeave();
    // Условые события
    void applicationProcessing();
    void toLowMode();
    void toHighMode();

    // Параметры симуляции
    double lambda;
    double mu_h;
    double mu_l;
    double p_h;
    double p_l;
    uint64_t servers;
    double* p;
    // Сколько серверов свободно
    uint64_t freeServers;

    // Генерация кол-ва серверов, необходимых заявке
    xoshiro256plus coresGen;
    uint64_t calculateCores();

    // Подсчёт необходимых метрик
    void calculateMetrics();

    // Находится ли сервер в энергосберегающем режиме
    bool isLowMode;

    // Статистика
    Statistic *stat;

public:
    Cluster();
    Cluster(double lambda, double mu_h, double mu_l, double p_h, double p_l, uint64_t servers, const double* p);
    ~Cluster();

    // Запуск симуляции
    // Второй параметр - прогрев: сколько из SimCount не собирать статистику
    void simulate(uint64_t warmingCount, uint64_t simCount, uint64_t intervalsCount);

    // Проверка условия стационарности
    //bool isStable();

    // Вывод метрик на экран. Нужна, в первую очередь, для отладки.
    void printMetrics();
    
    // Указатель на вектор среднего числа заявок в очереди в разные моменты времени
    const double* meanAppsInQueue;
    // Среднее число заявок в системе
    const double* meanAppsInSystem;
    // Среднее время обслуживания заявки
    const double* meanProcessingTime;
    // Среднее время нахождения заявки в очереди
    const double* meanWaitingTime;
    // Средняя потребляемая мощность. Перед считыванием необходимо вызвать calculatePower
    const double* meanPower;

    // Расчёт потребляемой мощности
    void calculatePower(double e_0, double e_l, double e_h);

    // Переключение на альтернативный генератор
    void useAlternativeGenertors();
    // Переключение на основной генератор.
    void useStandartGenertors();
    // Синхронизация генераторов случайных чисел
    void syncGenerators();
    // Инициализация симуляции
    void init(double lambda, double mu_h, double mu_l, double p_h, double p_l, uint64_t servers, const double *p);
};