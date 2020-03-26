#include "Service.h"

Service::Service(const Functor1<byte> &reportProgress, const Functor1<byte> &reportCompletion)
{
    this->reportProgress = reportProgress;
    this->reportCompletion = reportCompletion;
}

bool Service::isRunning()
{
    return this->running;
}