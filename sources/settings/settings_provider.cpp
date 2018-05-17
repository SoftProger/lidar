#include "settings_provider.h"

#include <QMap>

using namespace settings;

namespace
{
    QMap <QString, QVariant> defaultSettings =
    {
        //ad5302armz used
        {dac::maxVal, 255},
        {dac::vRef, 3.3},
        {dac::channelCount, 2},

        //si5351 used
        {freq_generator::channelCount, 2},
        {freq_generator::minFrequency, 2500},
        {freq_generator::maxFrequency, 200000000},

        //mcp41xx used
        {digital_potentiometer::maxVal, 255},
        {digital_potentiometer::vRef, 3.3},
        {digital_potentiometer::channelCount, 1},

        //custom HID device
        {communication::usb::VID, 1155},
        {communication::usb::PID, 22352},
        {communication::usb::packetSize, 64}
    };
}

Provider* Provider::instance()
{
    static Provider self;
    return &self;
}

Provider::Provider()
{

}

Provider::~Provider()
{

}

QVariant Provider::value(const QString &key)
{
    return defaultSettings.value(key);
}
