#ifndef VIDEOSUFACE_H
#define VIDEOSUFACE_H
#include <QAbstractVideoSurface>
#include <QObject>

class VideoSuface : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    explicit VideoSuface(QObject *parent = 0);

    QList<QVideoFrame::PixelFormat>
            supportedPixelFormats(
            QAbstractVideoBuffer::HandleType
            type = QAbstractVideoBuffer::NoHandle) const;


    bool present(const QVideoFrame &frame);


signals:
    void frameChanged(QVideoFrame);
public slots:
};

#endif // VIDEOSUFACE_H
