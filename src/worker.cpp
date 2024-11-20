#include "worker.h"

#include <QThread>

// Constructor for Multi Threaded process
AverageColorWorkerMVersion::AverageColorWorkerMVersion(const QPixmap &pixmap, int xStart, int yStart, int blockSize)
    : m_pixmap(pixmap), m_xStart(xStart), m_yStart(yStart), m_blockSize(blockSize) {}


// Run function for Multi-Threaded process
void AverageColorWorkerMVersion::run()
{
    int xEnd = m_xStart + m_blockSize;
    int yEnd = m_yStart + m_blockSize;

    int rSum = 0, gSum = 0, bSum = 0;
    int count = 0;

    // Iterate over the block's pixels and calculate R, G, B colors
    for (int x = m_xStart; x < xEnd && x < m_pixmap.width(); ++x) {
        for (int y = m_yStart; y < yEnd && y < m_pixmap.height(); ++y) {
            QColor color(m_pixmap.toImage().pixel(x, y));
            rSum += color.red();
            gSum += color.green();
            bSum += color.blue();
            ++count;
        }
    }

    // Added sleep of 1 second to visually describe how multi-threading works, otherwise , it will be so fast that
    // it won't be recognizeable
    QThread::sleep(1);

    // If the count is different than 1, then send it to emittable function to call main UI thread
    if (count > 0) {
        QColor avgColor(rSum / count, gSum / count, bSum / count);
        emit averageColorComputed(m_xStart, m_yStart, avgColor);
    }
}

// Constructor for Single Threaded process
AverageColorWorkerSVersion::AverageColorWorkerSVersion(const QPixmap &pixmap, int xStart, int yStart, int blockSize)
    : m_pixmap(pixmap), m_xStart(xStart), m_yStart(yStart), m_blockSize(blockSize) {}


// Run function for Single-Threaded process
void AverageColorWorkerSVersion::run()
{
    QImage img = m_pixmap.toImage();

    // Iterate over all blocks of image at one time
    for (int x = 0; x < m_pixmap.width(); x += m_blockSize) {
        for (int y = 0; y < m_pixmap.height(); y += m_blockSize) {
            int rSum = 0, gSum = 0, bSum = 0;
            int count = 0;

            // Iterate over the block's pixels and calculate R, G, B colors
            for (int dx = x; dx < x + m_blockSize && dx < m_pixmap.width(); ++dx) {
                for (int dy = y; dy < y + m_blockSize && dy < m_pixmap.height(); ++dy) {
                    QColor color(img.pixel(dx, dy));
                    rSum += color.red();
                    gSum += color.green();
                    bSum += color.blue();
                    ++count;
                }
            }

            // If the count is different than 1, then send it to emittable function to call main UI thread
            if (count > 0) {
                int avgR = rSum / count;
                int avgG = gSum / count;
                int avgB = bSum / count;
                QColor avgColor(avgR, avgG, avgB);

                emit averageColorComputed(x, y, avgColor);


                // In order to be faster, I add 20 msecond to see how single-threading works
                QThread::msleep(20);
            }
        }
    }
}
