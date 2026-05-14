#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>

struct Settings {
    // 0=bottom-right, 1=bottom-left, 2=top-right, 3=top-left
    int position = 0;
    // 0-255 alpha for text color
    int alpha = 130;
    int fontSize1 = 22;
    int fontSize2 = 17;
    QString line1 = QStringLiteral("Activate Linux");
    QString line2 = QStringLiteral("Go to Settings to activate Linux.");
};

#endif // SETTINGS_H
