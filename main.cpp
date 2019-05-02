#include "mainwindow.h"
#include <windows.h>
#include <QApplication>

int main(int argc, char *argv[])
{
	HWND hwnd = GetConsoleWindow();
	ShowWindow(hwnd, 0);
    QApplication app(argc, argv);
    MainWindow window;
	window.show();

    return app.exec();
}
