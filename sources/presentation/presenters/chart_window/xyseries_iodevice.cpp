#include "xyseries_iodevice.h"
#include <QtCharts/QXYSeries>

using namespace presentation;

XYSeriesIODevice::XYSeriesIODevice(QXYSeries * series, QXYSeries* series2, QObject *parent) :
    QIODevice(parent),
    m_series(series), m_series2(series2)
{
}

namespace
{
    uint16_t convertFrom8To16(uint8_t dataFirst, uint8_t dataSecond)
    {
        uint16_t dataBoth = 0x0000;
        dataBoth = dataFirst;
        dataBoth = dataBoth << 8;
        dataBoth |= dataSecond;
        return dataBoth;
    }

    uint32_t toUint32(const char* buffer, int index)
    {
        uint32_t converted;
        converted = buffer[index]|buffer[index+1]<<8|buffer[index+2]<<16|buffer[index+3]<<24;
        return converted;
    }
}

qint64 XYSeriesIODevice::readData(char* data, qint64 maxSize)
{
    Q_UNUSED(data)
    Q_UNUSED(maxSize)
    return -1;
}

qint64 XYSeriesIODevice::writeData(const char * data, qint64 maxSize)
{
    qint64 range = 1024;
    QVector<QPointF> oldPoints = m_series->pointsVector();
    QVector<QPointF> points;

    QVector<QPointF> oldPoints2 = m_series2->pointsVector();
    QVector<QPointF> points2;
    int resolution = 4;

    if (oldPoints.count() < range) {
        points = m_series->pointsVector();
    }
    else
    {
        for (int i = maxSize/resolution; i < oldPoints.count(); i++)
            points.append(QPointF(i - maxSize/resolution, oldPoints.at(i).y()));
    }

    if (oldPoints2.count()< range)
    {
        points2 = m_series2->pointsVector();
    }
    else
    {
        for (int i = maxSize/resolution; i < oldPoints2.count(); i++)
            points2.append(QPointF(i - maxSize/resolution, oldPoints2.at(i).y()));
    }

    qint64 size = points.count();
    qint64 size2 = points2.count();
    int j =0;
    int j2=0;
//    for (int k = 0; k < maxSize/resolution; k++)
//        points.append(QPointF(k + size, convertFrom8To16(data[resolution*k], data[resolution*k+1])));

//    for (int i = 0; i<maxSize/4; i++)
//    {
//        uint32_t sample = toUint32(data, i*sizeof(uint32_t));
//        uint32_t sample0 = sample & 0x00000FFF;
//        points.append(QPointF(i+size, sample0));
//        uint32_t sample1 = (sample & 0x0FFF0000) >> 16;
//        points2.append(QPointF(i+size2, sample1));
//    }
    for (int i = 0; i<maxSize; i+=4)
    {
        points.append(QPointF((j++)+size, convertFrom8To16(data[i], data[i+1])));
        points2.append(QPointF((j2++)+size2, convertFrom8To16(data[i+2], data[i+3])));
    }

    m_series->replace(points);
    m_series2->replace(points2);
    return maxSize;
}