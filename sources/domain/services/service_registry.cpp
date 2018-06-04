#include "service_registry.h"

#include <QDebug>

#include "presentation_manager.h"
#include "freq_generator_service.h"
#include "da_converter_service.h"
#include "digital_potentiometer_service.h"
#include "temperature_service.h"
#include "transfer_service.h"

using namespace domain;

ServiceRegistry* ServiceRegistry::self = nullptr;

class ServiceRegistry::Impl
{
public:
    TransferService transferService;
    FreqGeneratorService freqGenService;
    DAConverterService daConverterService;
    DigitalPotentiometerService digitalPotentiometerSerivce;
    TemperatureService temperatureSerive;
};

ServiceRegistry::ServiceRegistry() : d(new Impl)
{
    ServiceRegistry::self = this;
}

ServiceRegistry* ServiceRegistry::instance()
{
    return ServiceRegistry::self;
}

ServiceRegistry::~ServiceRegistry()
{
    qDebug()<<"service registry destroyed";
}

FreqGeneratorService* ServiceRegistry::freqGeneratorService()
{
    return &d->freqGenService;
}

DAConverterService* ServiceRegistry::daConverterService()
{
    return &d->daConverterService;
}

DigitalPotentiometerService*ServiceRegistry::digitalPotentiometerSerivce()
{
    return &d->digitalPotentiometerSerivce;
}

TemperatureService*ServiceRegistry::temperatureService()
{
    return &d->temperatureSerive;
}

TransferService* ServiceRegistry::transferService()
{
    return &d->transferService;
}
