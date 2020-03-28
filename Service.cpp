#include "Service.h"

Service::Service(const Functor1<byte> &reportProgress, const Functor0 &reportCompletion)
{
    this->reportProgress = reportProgress;
    this->reportCompletion = reportCompletion;
}

bool Service::isRunning()
{
    return this->running;
}