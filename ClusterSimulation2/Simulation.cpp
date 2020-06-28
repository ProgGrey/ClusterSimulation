#include "Cluster.hpp"

using namespace std;

uint64_t Cluster::calculateCores()
{
    if (p != NULL) {
        // Нормализуем генерируемые значения
        double tmp = coresGen();
        tmp /= coresGen.max();
        for (uint64_t k = 0; k < servers - 1; k++){
            tmp -= p[k];
            if (tmp <= 0.0) {
                return k + 1;
            }
            return servers;
        }

    }
    return 1;
}

// Приход новой заявки в систему
void Cluster::applicationArrival()
{
    // Добавим заявку в очередь на обработку
    Application tmp;
    tmp.time = 0;
    tmp.cores = calculateCores();
    queue->push_back(tmp);
    // Добавим событие прибытия новой заявки
    eventsB->addEvent(timeArrival(), 0);
    applicationProcessing();
    // Попробуем переключить кластер в повышенный режим
    if ((isLowMode) && (probTest(p_h))) {
        toHighMode();
    }
}

// Уход заявки из системы
void Cluster::applicationLeave()
{
    freeServers += eventsB->eventId();
    applicationProcessing();
    // Попробуем переключить кластер в пониженный режим
    if ((!isLowMode) && (probTest(p_l))) {
        toLowMode();
    }
}

// Отправка заявки в обработку
void Cluster::applicationProcessing()
{
    if (queue->size() > 0) {
        Application tmp = queue->front();
        if (tmp.cores <= freeServers) {
            // Отправим заявку на обработку
            freeServers -= tmp.cores;
            eventsB->addEvent(timeLeave(), tmp.cores);
            queue->pop_front();
            // Соберём метрику по среднему времени ожидания в очереди
            if (!isWarming) {
                *stat->meanWaitingTime_p += tmp.time;
                stat->applicationsInQueueCountSum++;
            }
            // Попытаемся отправить ещё одну заявку на обработку
            applicationProcessing();
        }
    }
}

void Cluster::toLowMode()
{
    isLowMode = true;
    eventsB->recalculateTime(mu_l);
    timeLeave.lambda = mu_l;
}

void Cluster::toHighMode()
{
    isLowMode = false;
    eventsB->recalculateTime(mu_h);
    timeLeave.lambda = mu_h;
}

// Симуляция
void Cluster::simulate(uint64_t warmingCount, uint64_t simCount, uint64_t intervalsCount)
{
    // Добавим событие прибытия первой заявки
    eventsB->addEvent(timeArrival(), 0);
    // Прогрев. Метрики не собираем
    isWarming = true;
    for (uint64_t k = 0; k < warmingCount; k++) {
        // Перейдём к следующему событию
        eventsB->nextEvent();
        // Пересчитаем время ожидания заявок в очереди
        // Мы всё равно должны это делать не смотря на прогрев,
        // чтобы не испортить статистику когда он кончится
        for (auto i = queue->begin(); i != queue->end(); i++) {
            i->time += eventsB->passedTime();
        }
        // Определим тип этого события
        if (eventsB->eventId() == 0) {
            // В систему пребыла новая заявка
            applicationArrival();
        }
        else {
            // Из системы ушла заявка
            applicationLeave();
        }
    }
    isWarming = false;
    delete stat;
    stat = new Statistic(intervalsCount);
    // Запустим симуляцию
    for (uint64_t i = 0; i < intervalsCount; i++) {
        for (uint64_t k = 0; k < simCount; k++) {
            // Перейдём к следующему событию
            eventsB->nextEvent();
            // Пересчитаем время ожидания заявок в очереди
            for (auto i = queue->begin(); i != queue->end(); i++) {
                i->time += eventsB->passedTime();
            }
            // Посчитаем метрики
            calculateMetrics();
            // Определим тип этого события
            if (eventsB->eventId() == 0) {
                // В систему пребыла новая заявка
                applicationArrival();
            }
            else {
                // Из системы ушла заявка
                applicationLeave();
            }
        }
        // Начнём собирать статистику для следующего интервала
        stat->nextInterval();
    }
    stat->finalizeCalculation();
    meanAppsInQueue = stat->getMeanAppsInQueue();
    meanAppsInSystem = stat->getMeanAppsInSystem();
    meanProcessingTime = stat->getMeanProcessingTime();
    meanWaitingTime = stat->getMeanWaitingTime();
    meanPower = stat->getMeanPower();
}

void Cluster::calculatePower(double e_0, double e_l, double e_h)
{
    stat->calculatePower(e_0, e_l, e_h);
}