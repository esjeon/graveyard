// kwindowsystem.cpp - a simple demo that detects compistor using KWindowSystem
//
// compile with:
//   g++ $(pkg-config --cflags --libs Qt5Widgets) -I /usr/include/KF5/KWindowSystem/ -fPIC -lKF5WindowSystem test.cpp
//

#include <QGuiApplication>
#include <KWindowSystem>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
	// KWindowSystem internally relies on QGuiApplication to detect
	// graphics backend, so it should be initialized first.
	QGuiApplication app(argc, argv);

	cout << KWindowSystem::compositingActive() << endl;
	return 0;
}
