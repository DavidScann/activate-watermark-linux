#ifndef TRAYMANAGER_H
#define TRAYMANAGER_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include "Settings.h"

class TrayManager : public QObject {
    Q_OBJECT
public:
    explicit TrayManager(QObject *parent = nullptr);

    void show();

signals:
    void settingsChanged(const Settings &settings);
    void exitRequested();

private slots:
    void onTrayActivated(QSystemTrayIcon::ActivationReason reason);

private:
    QSystemTrayIcon *m_trayIcon;
    QMenu *m_menu;
    Settings m_settings;

    void rebuildMenu();
    void showEditDialog(int lineIndex);
};

#endif // TRAYMANAGER_H
