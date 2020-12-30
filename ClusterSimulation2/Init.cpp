#include "Cluster.hpp"
using namespace std;

Cluster::Cluster()
{
    init(1, 1, 1, 1, 1, 1, NULL);
}

Cluster::Cluster(double lambda, double mu_h, double mu_l, double p_h, double p_l, uint8_t servers, const double* p)
{
    init(lambda, mu_h, mu_l, p_h, p_l, servers, p);
}

Cluster::~Cluster() {
    delete queue;
    delete eventsB;
    delete stat;
    delete[] p;
}

void Cluster::init(double lambda, double mu_h, double mu_l, double p_h, double p_l, uint8_t servers, const double* p)
{
    queue = NULL;
    eventsB = NULL;
    stat = NULL;
    meanAppsInQueue = NULL;
    meanAppsInSystem = NULL;
    meanProcessingTime = NULL;
    meanWaitingTime = NULL;
    meanPower = NULL;
    this->p = NULL;
    // запомним параметры симул€ции
    this->lambda = lambda;
    this->mu_h = mu_h;
    this->mu_l = mu_l;
    this->p_h = p_h;
    this->p_l = p_l;
    this->servers = servers;
    freeServers = servers;
    // –асчитаем первоначальное состо€ние системы
    isLowMode = probTest(0.5);
    double mu = mu_h;
    if (isLowMode) {
        mu = mu_l;
    }
    // »нициализируем генераторы случайных чисел дл€ времени
    timeArrival.lambda = lambda;
    timeLeave.lambda = mu;

    useStandartGenertors();
    
    // ¬ыделим пам€ть под безусловные событи€ и очередь
    delete queue;
    delete eventsB;
    
    // —копируем распределение веро€тностей
    if (servers > 1) {
        this->p = new double[servers - 1];
        for (int k = 0; k < servers - 1; k++) {
            this->p[k] = p[k];
        }
    }
    queue = new deque<Application>();
    eventsB = new Timer(mu);
    
}


void Cluster::useAlternativeGenertors()
{
    timeLeave.useAlternativeGenerator();
    timeArrival.useAlternativeGenerator();
}

void Cluster::useStandartGenertors()
{
    timeLeave.useStandartGenerator();
    timeArrival.useStandartGenerator();
}

void Cluster::syncGenerators()
{
    timeArrival.syncGenerators();
    timeLeave.syncGenerators();
}