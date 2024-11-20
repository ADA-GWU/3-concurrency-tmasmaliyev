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

    void processParameters(const QString &imagePath, int blockSize, char type);
    void singleThreadedProcess();
    void multiThreadedProcess();

public slots:
    void onAverageColorComputed(int xStart, int yStart, QColor avgColor);

private:
    Ui::MainWindow *ui;
    QLabel *imageLabel;
    QPixmap m_pixmap;
    int blockSize;
};


#endif // MAINWINDOW_H
