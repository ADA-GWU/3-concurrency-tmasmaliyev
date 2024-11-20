#include "mainwindow.h"

#include <QApplication>
#include <iostream>
#include <string>

int main(int argc, char **argv)
{
    // If the input size 3 (arguments) + 1 (default .cpp file) = 4 is not meant, then return error message
    if (argc != 4) {
        std::cerr << "Please, consider the correct input !" << std::endl;
    }

    // Assign file name
    QString fileName = QString(argv[1]);
    int blockSize;
    char type;

    // Check if block size is in correct format and not negative
    try {
        blockSize = std::stoi(argv[2]);

        if (blockSize <= 0)
            throw std::invalid_argument("Block size must be positive.");
    }
    catch (const std::exception& e) {
        std::cerr << "Invalid block size: " << argv[2] << "\n";
        return 1;
    }

    // Check if run type is either `M` or `S`
    if (std::string(argv[3]).length() != 1 || (std::string(argv[3])[0] != 'M' && std::string(argv[3])[0] != 'S')) {
        std::cerr << "Character must be a single character: " << argv[3] << "\n";
        return 1;
    }
    type = argv[3][0];

    // Create Main UI
    QApplication a(argc, argv);
    MainWindow w;

    // Show app in desktop
    w.show();

    // Process parameters
    w.processParameters(fileName, blockSize, type);
    w.saveImage();

    // Stay until the termination of app
    return a.exec();
}
