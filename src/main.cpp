#include <QApplication>
#include "WatermarkWindow.h"
#include "TrayManager.h"

int main(int argc, char *argv[])
{
    if (qEnvironmentVariableIsEmpty("QT_WAYLAND_SHELL_INTEGRATION"))
        qputenv("QT_WAYLAND_SHELL_INTEGRATION", "layer-shell");

    QApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("activate-watermark"));
    app.setQuitOnLastWindowClosed(false);

    Settings defaultSettings;
    defaultSettings.line1 = QStringLiteral("Activate Linux");
    defaultSettings.line2 = QStringLiteral("Go to Settings to activate Linux.");

    WatermarkWindow watermark;
    watermark.applySettings(defaultSettings);
    watermark.show();

    TrayManager tray;
    tray.show();

    QObject::connect(&tray, &TrayManager::settingsChanged,
                     &watermark, &WatermarkWindow::applySettings);
    QObject::connect(&tray, &TrayManager::exitRequested,
                     &app, &QApplication::quit);

    return app.exec();
}
