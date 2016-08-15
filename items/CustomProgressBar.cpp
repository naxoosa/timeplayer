#include "items/CustomProgressBar.h"

CustomProgressBar::CustomProgressBar(QQuickPaintedItem *parent):
    QQuickPaintedItem(parent)
{
    __depth = 10;
    __start_angle=0;
    __span_angle=0;
    __color= "#ce8d1b";
    __progressImage = QImage(":/contents/rotor.png");
    __echelle= 8;

    //connect(this, SIGNAL(startAngleChanged()), this, SLOT(update()));
    //connect(this, SIGNAL(spanAngleChanged()), this, SLOT(update()));
    //connect(this, SIGNAL(depthChanged()), this, SLOT(update()));
    //connect(this, SIGNAL(colorChanged()), this, SLOT(update()));
    //connect(this, SIGNAL(colorChanged()), this, SLOT(changeColor()));
    //connect(this, SIGNAL(progressChanged()), this, SLOT(update()));
}

void CustomProgressBar::paint(QPainter *painter)
{    
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::HighQualityAntialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);    


    QRectF rect = this->boundingRect();
    //painter->setPen(Qt::NoPen);
    painter->setPen(QPen(__color));
    painter->setBrush(QBrush(__color));

    //painter->drawArc(rect.width()/2, rect.height()/2,320,320,__start_angle, __span_angle);
    painter->drawPie(rect,-__start_angle*16*270/100-135*16, -__span_angle*16*270/100);

    /*QPainterPath slicePath(QPointF(rect.width()/2, rect.height()/2));
    slicePath.lineTo(0,rect.height()/2);
    slicePath.arcTo(rect, __start_angle, __span_angle);
    slicePath.lineTo(rect.width()/2, rect.height()/2);
    slicePath.closeSubpath();*/

    //painter->setClipRegion(slicePath.toFillPolygon().toPolygon());
    //painter->drawImage(rect, __progressImage);

};

