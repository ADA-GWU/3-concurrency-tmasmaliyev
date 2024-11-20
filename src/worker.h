#ifndef WORKER_H
#define WORKER_H

#include <QRunnable>
#include <QPixmap>
#include <QObject>

// Multi-threaded version Worker Class
class AverageColorWorkerMVersion : public QObject, public QRunnable
{
    Q_OBJECT

public:
    // Constructor
    AverageColorWorkerMVersion(const QPixmap &pixmap, int xStart, int yStart, int blockSize);

    // Run function for Thread start operation
    void run() override;

signals:
    // Signal type function that emits
    void averageColorComputed(int xStart, int yStart, QColor avgColor);

private:
    QPixmap m_pixmap;
    int m_xStart, m_yStart, m_blockSize;
};

// Single-threaded version Worker Class
class AverageColorWorkerSVersion : public QObject {
    Q_OBJECT

public:
    // Constructor
    AverageColorWorkerSVersion(const QPixmap &pixmap, int xStart, int yStart, int blockSize);

public slots:
    // Slot for `thread` connection
    void run();

signals:
    // Signal type function that emits
    void averageColorComputed(int xStart, int yStart, QColor avgColor);

private:
    QPixmap m_pixmap;
    int m_xStart, m_yStart, m_blockSize;
};

#endif // WORKER_H
