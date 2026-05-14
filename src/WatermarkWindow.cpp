#include "WatermarkWindow.h"
#include <QPainter>
#include <QFontMetrics>
#include <QScreen>
#include <QGuiApplication>
#include <QFontDatabase>

#ifdef HAVE_LAYERSHELL
#include <LayerShellQt/window.h>
#endif
#ifdef HAVE_WAYLAND_CLIENT
#include <QtGui/qpa/qplatformnativeinterface.h>
#include <wayland-client.h>
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

    QTimer::singleShot(0, this, &WatermarkWindow::updatePosition);

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

void WatermarkWindow::setupWayland()
{
    if (m_waylandInitialized) return;

    auto *handle = windowHandle();
    if (!handle || !isVisible())
        return;

#ifdef HAVE_WAYLAND_CLIENT
    auto *native = QGuiApplication::platformNativeInterface();
    if (!native) return;

    auto *display = static_cast<wl_display*>(
        native->nativeResourceForIntegration("display"));
    auto *surface = static_cast<wl_surface*>(
        native->nativeResourceForWindow("surface", handle));
    if (!display || !surface) return;

    struct wl_registry *registry = wl_display_get_registry(display);
    struct Listener {
        wl_compositor *compositor = nullptr;
        static void global(void *data, struct wl_registry *reg,
                           uint32_t name, const char *iface, uint32_t ver) {
            auto *l = static_cast<Listener*>(data);
            if (strcmp(iface, "wl_compositor") == 0) {
                auto *proxy = wl_registry_bind(reg, name, &wl_compositor_interface, 4);
                if (proxy)
                    l->compositor = static_cast<wl_compositor*>(proxy);
            }
        }
        static void globalRemove(void*, struct wl_registry*, uint32_t) {}
    };
    Listener listener;
    wl_registry_listener rl;
    rl.global = Listener::global;
    rl.global_remove = Listener::globalRemove;
    wl_registry_add_listener(registry, &rl, &listener);
    wl_display_roundtrip(display);
    wl_registry_destroy(registry);

    if (!listener.compositor) return;

    struct wl_region *empty = wl_compositor_create_region(listener.compositor);
    wl_surface_set_input_region(surface, empty);
    wl_region_destroy(empty);
    wl_surface_commit(surface);
    m_waylandInitialized = true;
#endif
}

QSize WatermarkWindow::calculateSize() const
{
    QFontMetrics fm1(m_line1Font);
    QFontMetrics fm2(m_line2Font);
    int w1 = fm1.horizontalAdvance(m_settings.line1);
    int w2 = fm2.horizontalAdvance(m_settings.line2);
    int h1 = fm1.ascent() + fm1.descent();
    int h2 = fm2.ascent() + fm2.descent();

    int maxW = qMax(w1, w2);
    int totalH = h1 + 2 + h2;
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
        if (layerWin) {
            layerWin->setDesiredSize(size);
            layerWin->setLayer(LayerShellQt::Window::LayerOverlay);

            using A = LayerShellQt::Window::Anchor;
            switch (m_settings.position) {
            case 0:
                layerWin->setAnchors(LayerShellQt::Window::Anchors(A::AnchorBottom) | A::AnchorRight);
                layerWin->setMargins(QMargins(0, 0, 20, 20));
                break;
            case 1:
                layerWin->setAnchors(LayerShellQt::Window::Anchors(A::AnchorBottom) | A::AnchorLeft);
                layerWin->setMargins(QMargins(20, 0, 0, 20));
                break;
            case 2:
                layerWin->setAnchors(LayerShellQt::Window::Anchors(A::AnchorTop) | A::AnchorRight);
                layerWin->setMargins(QMargins(0, 20, 20, 0));
                break;
            case 3:
                layerWin->setAnchors(LayerShellQt::Window::Anchors(A::AnchorTop) | A::AnchorLeft);
                layerWin->setMargins(QMargins(20, 20, 0, 0));
                break;
            }
            layerWin->setKeyboardInteractivity(LayerShellQt::Window::KeyboardInteractivityNone);
            layerWin->setExclusiveZone(0);
        }

        setupWayland();
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

    QFontMetrics fm1(m_line1Font);
    QFontMetrics fm2(m_line2Font);
    int h1 = fm1.ascent() + fm1.descent();

    painter.setFont(m_line1Font);
    painter.setPen(textColor);
    painter.drawText(0, fm1.ascent(), m_settings.line1);

    painter.setFont(m_line2Font);
    painter.setPen(textColor);
    painter.drawText(0, h1 + 2 + fm2.ascent(), m_settings.line2);
}
