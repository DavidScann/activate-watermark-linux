#include "WatermarkWindow.h"
#include <QPainter>
#include <QFontMetrics>
#include <QScreen>
#include <QGuiApplication>
#include <QFontDatabase>

#ifdef HAVE_LAYERSHELL
#include <LayerShellQt/window.h>
#endif

WatermarkWindow::WatermarkWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint
                   | Qt::WindowStaysOnTopHint
                   | Qt::Tool);
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
    reposition();
    update();
}

QSize WatermarkWindow::calculateSize() const
{
    QFontMetrics fm1(m_line1Font);
    QFontMetrics fm2(m_line2Font);
    QRect bounds1 = fm1.boundingRect(m_settings.line1);
    QRect bounds2 = fm2.boundingRect(m_settings.line2);

    int maxW = qMax(bounds1.width(), bounds2.width());
    int totalH = bounds1.height() + 2 + bounds2.height();
    return QSize(maxW + 8, totalH + 4);
}

void WatermarkWindow::reposition()
{
    QSize size = calculateSize();

#ifdef HAVE_LAYERSHELL
    if (QGuiApplication::platformName() == "wayland") {
        auto *handle = windowHandle();
        if (!handle) return;
        auto *layerWin = LayerShellQt::Window::get(handle);
        if (!layerWin) return;

        layerWin->setDesiredSize(size);
        layerWin->setLayer(LayerShellQt::Window::LayerOverlay);

        auto anch = [&]() -> LayerShellQt::Window::Anchors {
            using A = LayerShellQt::Window::Anchor;
            switch (m_settings.position) {
            case 0: return LayerShellQt::Window::Anchors(A::AnchorBottom) | A::AnchorRight;
            case 1: return LayerShellQt::Window::Anchors(A::AnchorBottom) | A::AnchorLeft;
            case 2: return LayerShellQt::Window::Anchors(A::AnchorTop) | A::AnchorRight;
            case 3: return LayerShellQt::Window::Anchors(A::AnchorTop) | A::AnchorLeft;
            }
            return {};
        }();
        layerWin->setAnchors(anch);
        layerWin->setMargins(QMargins(20, 20, 20, 20));
        layerWin->setKeyboardInteractivity(LayerShellQt::Window::KeyboardInteractivityNone);
        layerWin->setExclusiveZone(0);
        return;
    }
#endif

    raise();
    QScreen *screen = QGuiApplication::primaryScreen();
    if (!screen) return;

    QRect avail = screen->availableGeometry();
    int x, y;
    switch (m_settings.position) {
    case 0:
        x = avail.right() - size.width() - 20;
        y = avail.bottom() - size.height() - 20;
        break;
    case 1:
        x = avail.left() + 20;
        y = avail.bottom() - size.height() - 20;
        break;
    case 2:
        x = avail.right() - size.width() - 20;
        y = avail.top() + 20;
        break;
    case 3:
    default:
        x = avail.left() + 20;
        y = avail.top() + 20;
        break;
    }

    setGeometry(x, y, size.width(), size.height());
}

void WatermarkWindow::updatePosition()
{
    reposition();
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
