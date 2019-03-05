#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include "perlinnoise.h"
#include "qnoise.h"

class QLabel;
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void createPixmap();
    void updateDescription();

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;

    PerlinNoise m_perlineNoise;
    QNoise m_qNoise;
    double m_zPane ;

    double m_scaleFactor = 5;
    double m_offSetX, m_offSetY;

    bool m_useQNoise = true;
    bool m_useContour = false;
    bool m_doAnimation = true;
    bool m_useHighContrast = false;
    bool m_doLoop = false;

    QImage m_image;
    QLabel *m_Desciption = nullptr;
};

#endif // WIDGET_H
