#include "qwindowitem.h"
#include "qtoplevelwindow.h"

#include <QTimer>

QWindowItem::QWindowItem()
    : _window(new QTopLevelWindow), _positionIsDefined(false), _delayedVisible(false), _deleteOnClose(true), _closeByUser(true), _x(0), _y(0)
{
    connect(_window, SIGNAL(visibilityChanged()), this, SIGNAL(visibleChanged()));
    connect(_window, SIGNAL(windowStateChanged()), this, SIGNAL(windowStateChanged()));
    connect(_window, SIGNAL(windowFlagsChanged()), this, SIGNAL(windowFlagsChanged()));
    connect(_window, SIGNAL(sizeChanged(QSize)), this, SLOT(updateSize(QSize)));


    view()->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    _window->installEventFilter(this);
    _window->setMinimumSize(1,1);
    _window->setWindowFlags(Qt::Window);
}

QWindowItem::~QWindowItem()
{
    delete _window;
}

bool QWindowItem::eventFilter(QObject *, QEvent *ev)
{
    switch(ev->type()) {
    case QEvent::Close:
        ev->ignore();
        Q_EMIT beforeClosed();
        if (closeByUser()) close();
        _closeByUser = true;
        return true;
    case QEvent::Resize:
        emit sizeChanged();
        emit widthChanged();
        emit heightChanged();
        break;

    case QEvent::Move:
        emit xChanged();
        emit yChanged();
        break;

    default:
        break;
    }
    return false;
}

void QWindowItem::registerChildWindow(QWindowItem *child) {
    _window->registerChildWindow(child->window());
}

void QWindowItem::updateParentWindow() {
    QDeclarativeItem *p = parentItem();
    while (p) {
        if (QWindowItem *w = qobject_cast<QWindowItem*>(p)) {
            w->registerChildWindow(this);
            return;
        }
        p = p->parentItem();
    }
}

void QWindowItem::componentComplete()
{
    updateParentWindow();
    _window->scene()->addItem(this);
    if (!_window->parentWidget())
        _window->initPosition();

    QDeclarativeItem::componentComplete();

    if (_delayedVisible) {
        setVisible(true);
    }
}

void QWindowItem::updateSize(QSize newSize)
{
    QDeclarativeItem::setSize(newSize);
    emit sizeChanged();
}

void QWindowItem::center()
{
    _window->center();
}

void QWindowItem::setX(int x)
{
    _x = x;
    _window->move(x, _y);
}
void QWindowItem::setY(int y)
{
    _y = y;
    _window->move(_x, y);
}



void QWindowItem::moveWindow(int x,int y, int lx, int ly)
{
    QPoint p = _window->mapToGlobal(QPoint(x,y));
    p.setX(p.x() - lx);
    p.setY(p.y() - ly);
    _window->move(p);
}

void QWindowItem::setHeight(int height)
{
    int menuBarHeight = _window->menuBar()->sizeHint().height();
    if (menuBarHeight) menuBarHeight++;
    _window->resize(width(), height+menuBarHeight);
    QDeclarativeItem::setHeight(_window->height());
}

void QWindowItem::setMinimumHeight(int height)
{
    int menuBarHeight = _window->menuBar()->sizeHint().height();
    if (menuBarHeight) menuBarHeight++;
    _window->setMinimumHeight(height+menuBarHeight);
}

void QWindowItem::setMaximumHeight(int height)
{
    int menuBarHeight = _window->menuBar()->sizeHint().height();
    if (menuBarHeight) menuBarHeight++;
    _window->setMaximumHeight(height+menuBarHeight);
}

void QWindowItem::setWidth(int width)
{
    _window->resize(width, height());
    QDeclarativeItem::setWidth(_window->width());
}

void QWindowItem::setTitle(QString title)
{
    _window->setWindowTitle(title);
    emit titleChanged();
}

void QWindowItem::setVisible(bool visible)
{
    //_window->setWindowFlags(_window->windowFlags()); // | Qt::Window
    //int a = _window->windowFlags();
    //bool windowF = a & Qt::Window;
    //bool popupF = a & Qt::Popup;
    if (visible) {
        if (isComponentComplete()) {
            // avoid flickering when showing the widget,
            // by passing the event loop at least once
            //_window->show();
            QTimer::singleShot(1, _window, SLOT(show()));
        } else {
            _delayedVisible = true;
        }
    } else {
        _window->hide();
    }
}

void QWindowItem::setWindowDecoration(bool s)
{
    bool visible = _window->isVisible();


    _window->setWindowFlags(s ? _window->windowFlags() & ~Qt::FramelessWindowHint
                              : _window->windowFlags() | Qt::FramelessWindowHint);
    if (visible)
        _window->show();
    emit windowDecorationChanged();
}

void QWindowItem::setModal(bool modal)
{
    bool visible = _window->isVisible();
    _window->hide();
    _window->setWindowModality(modal ? Qt::WindowModal : Qt::NonModal);

    if (visible)
        _window->show();
    emit modalityChanged();
}

void QWindowItem::setDeleteOnClose(bool deleteOnClose)
{
    if (deleteOnClose == _deleteOnClose)
        return;
    _deleteOnClose = deleteOnClose;
    emit deleteOnCloseChanged();
}

void QWindowItem::setCloseByUser(bool close) {
    if (_closeByUser == close) return;
    _closeByUser = close;
    Q_EMIT closeByUserChanged();
}

void QWindowItem::close()
{
    if (_deleteOnClose)
        deleteLater();
    else
        _window->hide();
}
