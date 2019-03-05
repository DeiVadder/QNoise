/*
A simple example class, that uses Perlin & Simplex Noise to create an Image
The Image is than drawn on the surface of the Widget

Pressing different Keys will effect the displayed Image:

- wasd "moves the window over the map", ctrl key will amplify that movement
- +/- used to "zoom in on the map"
- Q-Key switches between Perlin Noise  and OpenSimplex Noise (in this example called QNoise)
- C-Key switches between "normal" and contour shapes (can be interpretaed as rings on a tree or hight map)
- T-Key toggles the "animation"
- H-Key toggles the high contrast mode, 2 colors compared to greyscale
- L-Key "loops the animation" - only works with QNoise, as it requieres 4D-Noise generation

QNoise and PerlinNoise classes are adaptations of open source java classes.
More information to that in the relating class files.
*/


#include "widget.h"
#include <cmath>

#include <QKeyEvent>
#include <QPainter>
#include <QLabel>

const QString description("PosX: %1 | PosY: %2 | Scale: %3 | QNoise: %4 | Contour: %5 | Animated: %6 | High contrast: %7 | Looping: %8");

Widget::Widget(QWidget *parent)
    : QWidget(parent), m_perlineNoise(2437), m_qNoise(2437), m_zPane(0), m_offSetX(50), m_offSetY(50)
{
    resize(400,400);
    createPixmap();

    //Info window that shows the current applied settings
    m_Desciption = new QLabel();
    m_Desciption->resize(400, 30);
    m_Desciption->show();
    m_Desciption->setWordWrap(true);

    updateDescription();
}

Widget::~Widget()
{
    if(m_Desciption)
        m_Desciption->deleteLater();
}

void Widget::createPixmap()
{
    if(m_doAnimation)
        m_zPane += 0.012;

    double u(0), v(0);
    if(m_doLoop){
        u = std::sin(m_zPane);
        v = std::cos(m_zPane);
        //changing cos to sin will result in a "bounce" effect instead of a loop
    }

    //Increasing the img size only increases the noise resolution, does not the scale the resulting image
    //400 x 400 is decently fast, < 50ms in my tests
    QImage img(400,400,QImage::Format_RGB888);

    for(uint i(0), h(400); i < h; i++) {
        uchar *index = img.scanLine(i);
        for(uint j(0), bbl(img.bytesPerLine()); j < bbl; j+=3){
            // x and y for NoiseMap
            double x = j/static_cast<double>(bbl);
            double y = i/static_cast<double>(h);

            double noiseValue(0);

            if(m_useQNoise) {
                if(m_useContour) {
                    if(!m_doLoop)
                        noiseValue = 10 * m_qNoise.noise(m_scaleFactor*x+m_offSetX,m_scaleFactor*y+m_offSetY, m_zPane);
                    else
                        noiseValue = 10 * m_qNoise.noise(m_scaleFactor*x+m_offSetX,m_scaleFactor*y+m_offSetY, u, v);
                    noiseValue = noiseValue - std::floor(noiseValue);
                } else {
                    if(!m_doLoop)
                        noiseValue = m_qNoise.noise(m_scaleFactor * x + m_offSetX, m_scaleFactor * y + m_offSetY, m_zPane);
                    else
                        noiseValue = m_qNoise.noise(m_scaleFactor * x + m_offSetX, m_scaleFactor * y + m_offSetY, u,v);

                    //Constrains the noise value, that is between -1 and 1 to 0 and 1
                    noiseValue = (noiseValue + 1) /2;
                }
            } else {
                if(m_useContour) {
                    noiseValue = 10*m_perlineNoise.noise(m_scaleFactor * x + m_offSetX, m_scaleFactor * y + m_offSetY, m_zPane);
                    noiseValue = noiseValue - std::floor(noiseValue);
                } else {
                    noiseValue = m_perlineNoise.noise(m_scaleFactor * x + m_offSetX, m_scaleFactor * y + m_offSetY, m_zPane);
                }
            }

            uchar grey;
            if(!m_useHighContrast)
                grey = std::floor(255 * noiseValue);
            else
                grey = noiseValue < 0.5 ? 0 : 255;

            for(int a(0); a < 3; a++)
                memcpy(index+j+a, &grey, 1);
        }
    }

    m_image.swap(img);

    //request repaint
    update();

    //Shedule new noise calculation for next event loop cycle
    QMetaObject::invokeMethod(this, &Widget::createPixmap, Qt::QueuedConnection);
}

void Widget::updateDescription()
{
    //PosX: %1 | PosY: %2 | Scale: %3 | QNoise: %4 | Contour: %5 | Animated: %6 | High contrast: %7 | Looping: %8
    if(m_Desciption)
        m_Desciption->setText(description.arg(m_offSetX).arg(m_offSetY).arg(m_scaleFactor)
                              .arg(m_useQNoise).arg(m_useContour).arg(m_doAnimation).arg(m_useHighContrast).arg(m_doLoop));
}

void Widget::paintEvent(QPaintEvent *event)
{
    //Paint default stuff
    QWidget::paintEvent(event);

    //Draw the created image, streched to fit the window
    if(!m_image.isNull()) {
        QPainter p(this);
        p.drawImage(0,0,m_image.scaled(size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    }
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    QWidget::keyPressEvent(event);

    switch (event->key()) {
    //Zoom
    case Qt::Key_Plus:
        if(event->modifiers() ==  Qt::ControlModifier)
            m_scaleFactor -= 0.1;
        else
            m_scaleFactor -= 0.01;
        if (m_scaleFactor <= 0)
            m_scaleFactor = 0.01;
        break;
    case Qt::Key_Minus:
        if(event->modifiers() ==  Qt::ControlModifier)
            m_scaleFactor += 0.1;
        else
            m_scaleFactor += 0.01;
        break;

    //Movement
    case Qt::Key_Up:
    case Qt::Key_W:
        if(event->modifiers() ==  Qt::ControlModifier)
            m_offSetY -= 0.1;
        else
            m_offSetY -= 0.01;
        if(m_offSetY < 0)
            m_offSetY = 0;
        break;

    case Qt::Key_Left:
    case Qt::Key_A:
        if(event->modifiers() ==  Qt::ControlModifier)
            m_offSetX -= 0.1;
        else
            m_offSetX -= 0.01;
        if(m_offSetX < 0)
            m_offSetX = 0;
        break;

    case Qt::Key_Down:
    case Qt::Key_S:
        if(event->modifiers() ==  Qt::ControlModifier)
            m_offSetY += 0.1;
        else
            m_offSetY += 0.01;
        break;

    case Qt::Key_Right:
    case Qt::Key_D:
        if(event->modifiers() ==  Qt::ControlModifier)
            m_offSetX += 0.1;
        else
            m_offSetX += 0.01;
        break;

    //Mode
    case Qt::Key_Q: m_useQNoise = !m_useQNoise;break;
    case Qt::Key_C: m_useContour = !m_useContour; break;
    case Qt::Key_T: m_doAnimation = !m_doAnimation;break;
    case Qt::Key_H: m_useHighContrast = !m_useHighContrast;break;
    case Qt::Key_L: m_doLoop = !m_doLoop; break;
    default:break;
    }
    updateDescription();
}
