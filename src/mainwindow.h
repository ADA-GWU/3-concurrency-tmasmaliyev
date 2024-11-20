#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPixmap>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Accepting input parameters for UI as three arguments
    void processParameters(const QString &imagePath, int blockSize, char type);

    // Single Threaded Process
    void singleThreadedProcess();

    // Multi Threaded Process
    void multiThreadedProcess();

    // Save image
    void saveImage();

    // ImageLabel
    QLabel *imageLabel;

public slots:
    // Slot for `thread` connection
    void onAverageColorComputed(int xStart, int yStart, QColor avgColor);

private:
    // Additional parameters
    Ui::MainWindow *ui;
    QPixmap m_pixmap;
    int blockSize;
};


#endif // MAINWINDOW_H
