#include "videowindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    // Create a QApplication instance to manage the application's GUI
    QApplication a(argc, argv);

    // Create a videowindow instance, which is the main application window
    videowindow *app = new videowindow();

    // Set the window title
    app->setWindowTitle("CineSync Leeds");

    // Show the application window
    app->show();

    // Start the event loop, allowing the application to respond to user interactions
    return a.exec();
}

