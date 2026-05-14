#include "WatermarkWindow.h"
#include <QPainter>
#include <QFontMetrics>
#include <QScreen>
#include <QGuiApplication>
#include <QFontDatabase>

WatermarkWindow::WatermarkWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint
                   | Qt::WindowStaysOnTopHint
                   | Qt::Tool
                   | Qt::X11BypassWindowManagerHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_ShowWithoutActivating);

    QFont sysFont = QFontDatabase::systemFont(QFontDatabase::GeneralFont);
    m_line1Font = sysFont;
    m_line2Font = sysFont;

    connect(&m_refreshTimer, &QTimer::timeout, this, &WatermarkWindow::updatePosition);
    m_refreshTimer.setInterval(5000);
    m_refreshTimer.start();
}

void WatermarkWindow::rebuildFonts()
{
    m_line1Font.setPixelSize(m_settings.fontSize1);
    m_line2Font.setPixelSize(m_settings.fontSize2);
}

void WatermarkWindow::applySettings(const Settings &settings)
{
    m_settings = settings;
    rebuildFonts();
    updatePosition();
    update();
}

void WatermarkWindow::updatePosition()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    if (!screen) return;

    QRect avail = screen->availableGeometry();
    const int margin = 20;

    QFontMetrics fm1(m_line1Font);
    QFontMetrics fm2(m_line2Font);
    QRect bounds1 = fm1.boundingRect(m_settings.line1);
    QRect bounds2 = fm2.boundingRect(m_settings.line2);

    int lineSpacing = 2;
    int maxW = qMax(bounds1.width(), bounds2.width());
    int totalH = bounds1.height() + lineSpacing + bounds2.height();
    int winW = maxW + 8;
    int winH = totalH + 4;

    int x, y;
    switch (m_settings.position) {
    case 0: // bottom-right
        x = avail.right() - winW - margin;
        y = avail.bottom() - winH - margin;
        break;
    case 1: // bottom-left
        x = avail.left() + margin;
        y = avail.bottom() - winH - margin;
        break;
    case 2: // top-right
        x = avail.right() - winW - margin;
        y = avail.top() + margin;
        break;
    case 3: // top-left
    default:
        x = avail.left() + margin;
        y = avail.top() + margin;
        break;
    }

    setGeometry(x, y, winW, winH);
}

void WatermarkWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    QColor textColor(255, 255, 255, m_settings.alpha);

    painter.setFont(m_line1Font);
    painter.setPen(textColor);
    QRectF line1Rect = painter.fontMetrics().boundingRect(m_settings.line1);
    painter.drawText(0, line1Rect.height(), m_settings.line1);

    painter.setFont(m_line2Font);
    int line2Y = line1Rect.height() + 2;
    QRectF line2Rect = painter.fontMetrics().boundingRect(m_settings.line2);
    painter.drawText(0, line2Y + line2Rect.height(), m_settings.line2);
}
