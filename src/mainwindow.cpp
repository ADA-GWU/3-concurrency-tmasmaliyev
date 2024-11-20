#include "mainwindow.h"
#include "worker.h"
#include "ui_mainwindow.h"

#include <QThread>
#include <QThreadPool>
#include <QLabel>
#include <QScreen>
#include <QDir>

#include <iostream>

//Initializing UI constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    imageLabel = new QLabel(this);
    imageLabel->setAlignment(Qt::AlignCenter);
}

void MainWindow::processParameters(const QString &imagePath, int blockSize, char type) {
    QPixmap pixmap(imagePath);

    QImage image = pixmap.toImage();
    image = image.convertToFormat(QImage::Format_RGB888);

    this->blockSize = blockSize;

    if (pixmap.isNull()) {
        qDebug() << "Failed to load image from path:" << imagePath;
        return;
    }

    // Get the screen size of monitor
    QScreen *screen = QApplication::primaryScreen();
    if (!screen) {
        qDebug() << "Unable to retrieve screen information.";
    }

    QSize monitorSize = screen->availableGeometry().size();

    qDebug() << "Monitor Size:" << monitorSize;
    qDebug() << "Image Size:" << image.size();

    // If the provided image is greater than monitor size, then rescale to window size
    if (image.width() > monitorSize.width() || image.height() > monitorSize.height()) {
        qDebug() << "Image is larger than monitor size. Rescaling...";

        image = image.scaled(monitorSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        qDebug() << "Rescaled Image Size:" << image.size();
    }

    m_pixmap = QPixmap::fromImage(image);

    imageLabel->setPixmap(m_pixmap);
    imageLabel->setScaledContents(true);

    setCentralWidget(imageLabel);

    // Type for runing process
    if (type == 'S')
        this->singleThreadedProcess();
    else if (type == 'M')
        this->multiThreadedProcess();
}

void MainWindow::singleThreadedProcess() {
    // Assign single thread to run process
    // Since Main UI is running on a thread, we need to add another custom thread to do computation,
    // otherwise, it will block the main UI thread
    QThread *thread = new QThread();
    AverageColorWorkerSVersion *worker = new AverageColorWorkerSVersion(m_pixmap, 0, 0, blockSize);

    worker->moveToThread(thread);

    // Connect the function to retrieve output from thread to mainUI thread to display each iteration on GUI
    QObject::connect(thread, &QThread::started, worker, &AverageColorWorkerSVersion::run);
    QObject::connect(worker, &AverageColorWorkerSVersion::averageColorComputed, this, &MainWindow::onAverageColorComputed);

    QObject::connect(worker, &AverageColorWorkerSVersion::workFinished, this, [&]() {

        QPixmap retrievedPixmap = imageLabel->pixmap();
        QString currentDir = QDir::currentPath();
        std::cout << "Current Directory: " << currentDir.toStdString() << std::endl;

        QString fullPath = currentDir + "/" + "result.jpg";
        retrievedPixmap.save(fullPath);
    });

    // Start the thread
    thread->start();
}

void MainWindow::multiThreadedProcess() {
    int width = m_pixmap.width();
    int height = m_pixmap.height();

    // ThreadPool is created so that OS knows how to distribute the thread tasks (It is management tool)
    // If not used, it will lead resource overhead
    // Set the `maxThreadCount` to available thread count in PC

    QThreadPool *threadPool = QThreadPool::globalInstance();
    threadPool->setMaxThreadCount(QThread::idealThreadCount());


    // Iterate on each block and pass that to threadPool
    for (int x = 0; x < width; x += blockSize) {
        for (int y = 0; y < height; y += blockSize) {
            AverageColorWorkerMVersion *worker = new AverageColorWorkerMVersion(m_pixmap, x, y, blockSize);
            worker->setAutoDelete(true);

            // Connect the function to retrieve output from thread to mainUI thread to display each iteration on GUI
            QObject::connect(worker, &AverageColorWorkerMVersion::averageColorComputed, this, &MainWindow::onAverageColorComputed);

            // Start threadpool on worker object
            threadPool->start(worker);
        }
    }
}

// Slot type for I/O between main thread and Worker thread
// In each completion of block, it sends the block pixels, and it updates the UI
void MainWindow::onAverageColorComputed(int xStart, int yStart, QColor avgColor) {

    QImage image = m_pixmap.toImage();
    image = image.convertToFormat(QImage::Format_RGB888);
    QColor color = avgColor;

    // Assign the pixel values by `avgColor`
    for (int x = xStart; x < xStart + blockSize && x < image.width(); ++x) {
        for (int y = yStart; y < yStart + blockSize && y < image.height(); ++y) {
            image.setPixelColor(x, y, color);
        }
    }

    m_pixmap = QPixmap::fromImage(image);

    // Update UI
    imageLabel->setPixmap(m_pixmap);
    imageLabel->update();
}

void MainWindow::saveImage() {

    QString currentDir = QDir::currentPath();
    std::cout << "Current Directory: " << currentDir.toStdString() << std::endl;

    QString fullPath = currentDir + "/" + "result.jpg";
    m_pixmap.save(fullPath);
}

// Destructor for MainWindow UI
MainWindow::~MainWindow()
{
    delete ui;
}
