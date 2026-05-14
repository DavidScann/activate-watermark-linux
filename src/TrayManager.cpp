#include "TrayManager.h"
#include <QApplication>
#include <QInputDialog>
#include <QActionGroup>
#include <QStyle>

static const char *kPositionLabels[] = {
    "Bottom Right", "Bottom Left", "Top Right", "Top Left"
};
static const int kAlphaPresets[]  = { 64, 130, 191, 255 };
static const char *kAlphaLabels[] = { "25%", "50%", "75%", "100%" };
static const float kFontSizePresets[][2] = {
    {16, 13}, {22, 17}, {30, 23}, {40, 30}
};
static const char *kFontSizeLabels[] = {
    "Small", "Medium", "Large", "Extra Large"
};

TrayManager::TrayManager(QObject *parent)
    : QObject(parent)
    , m_trayIcon(new QSystemTrayIcon(this))
    , m_menu(new QMenu())
{
    m_trayIcon->setIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation));
    m_trayIcon->setToolTip(QStringLiteral("Activate Linux Watermark"));
    m_trayIcon->setContextMenu(m_menu);

    connect(m_trayIcon, &QSystemTrayIcon::activated,
            this, &TrayManager::onTrayActivated);

    rebuildMenu();
}

void TrayManager::show()
{
    m_trayIcon->show();
}

void TrayManager::rebuildMenu()
{
    m_menu->clear();

    // Position submenu
    QMenu *posMenu = m_menu->addMenu(QStringLiteral("Position"));
    QActionGroup *posGroup = new QActionGroup(posMenu);
    posGroup->setExclusive(true);
    for (int i = 0; i < 4; ++i) {
        QAction *act = posMenu->addAction(QString::fromLatin1(kPositionLabels[i]));
        act->setCheckable(true);
        act->setChecked(m_settings.position == i);
        act->setData(i);
        posGroup->addAction(act);
        connect(act, &QAction::triggered, this, [this, i]() {
            m_settings.position = i;
            rebuildMenu();
            emit settingsChanged(m_settings);
        });
    }

    // Opacity submenu
    QMenu *opacityMenu = m_menu->addMenu(QStringLiteral("Opacity"));
    QActionGroup *opacityGroup = new QActionGroup(opacityMenu);
    opacityGroup->setExclusive(true);
    for (int i = 0; i < 4; ++i) {
        QAction *act = opacityMenu->addAction(QString::fromLatin1(kAlphaLabels[i]));
        act->setCheckable(true);
        act->setChecked(m_settings.alpha == kAlphaPresets[i]);
        act->setData(kAlphaPresets[i]);
        opacityGroup->addAction(act);
        connect(act, &QAction::triggered, this, [this, i]() {
            m_settings.alpha = kAlphaPresets[i];
            rebuildMenu();
            emit settingsChanged(m_settings);
        });
    }

    // Font Size submenu
    QMenu *fontMenu = m_menu->addMenu(QStringLiteral("Font Size"));
    QActionGroup *fontGroup = new QActionGroup(fontMenu);
    fontGroup->setExclusive(true);
    for (int i = 0; i < 4; ++i) {
        QAction *act = fontMenu->addAction(QString::fromLatin1(kFontSizeLabels[i]));
        act->setCheckable(true);
        act->setChecked(m_settings.fontSize1 == static_cast<int>(kFontSizePresets[i][0]));
        act->setData(i);
        fontGroup->addAction(act);
        connect(act, &QAction::triggered, this, [this, i]() {
            m_settings.fontSize1 = static_cast<int>(kFontSizePresets[i][0]);
            m_settings.fontSize2 = static_cast<int>(kFontSizePresets[i][1]);
            rebuildMenu();
            emit settingsChanged(m_settings);
        });
    }

    m_menu->addSeparator();

    // Edit Line 1
    QAction *edit1 = m_menu->addAction(QStringLiteral("Edit Line 1..."));
    connect(edit1, &QAction::triggered, this, [this]() { showEditDialog(0); });

    // Edit Line 2
    QAction *edit2 = m_menu->addAction(QStringLiteral("Edit Line 2..."));
    connect(edit2, &QAction::triggered, this, [this]() { showEditDialog(1); });

    m_menu->addSeparator();

    QAction *exitAct = m_menu->addAction(QStringLiteral("Exit"));
    connect(exitAct, &QAction::triggered, this, &TrayManager::exitRequested);
}

void TrayManager::showEditDialog(int lineIndex)
{
    QString title = (lineIndex == 0)
        ? QStringLiteral("Edit Line 1")
        : QStringLiteral("Edit Line 2");
    QString current = (lineIndex == 0) ? m_settings.line1 : m_settings.line2;

    bool ok = false;
    QString text = QInputDialog::getText(
        nullptr, title, QStringLiteral("Enter text:"),
        QLineEdit::Normal, current, &ok);

    if (ok && !text.isEmpty()) {
        if (lineIndex == 0)
            m_settings.line1 = text;
        else
            m_settings.line2 = text;
        emit settingsChanged(m_settings);
    }
}

void TrayManager::onTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Context || reason == QSystemTrayIcon::Trigger) {
        rebuildMenu();
        m_menu->popup(QCursor::pos());
    }
}
