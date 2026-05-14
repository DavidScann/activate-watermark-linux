#ifndef WATERMARKWINDOW_H
#define WATERMARKWINDOW_H

#include <QWidget>
#include <QTimer>
#include "Settings.h"

class WatermarkWindow : public QWidget {
    Q_OBJECT
public:
    explicit WatermarkWindow(QWidget *parent = nullptr);

    void applySettings(const Settings &settings);
    void updatePosition();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Settings m_settings;
    QTimer m_refreshTimer;
    QFont m_line1Font;
    QFont m_line2Font;

    void rebuildFonts();
    void reposition();
    QSize calculateSize() const;
    void setupWayland();
    bool m_waylandInitialized = false;
};

#endif // WATERMARKWINDOW_H
