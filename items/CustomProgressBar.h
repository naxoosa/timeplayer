#ifndef CUSTOMPROGRESSBAR_H
#define CUSTOMPROGRESSBAR_H

#include <QQuickPaintedItem>
#include <QPainter>
#include <QColor>
#include <QImage>

class CustomProgressBar : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(qreal startAngle READ getStartAngle WRITE setStartAngle NOTIFY startAngleChanged)
    Q_PROPERTY(qreal spanAngle READ getSpanAngle WRITE setSpanAngle NOTIFY spanAngleChanged)
    Q_PROPERTY(int depth READ getDepth WRITE setDepth NOTIFY depthChanged)
    Q_PROPERTY(QColor color READ getColor WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(int progress READ getProgress WRITE setProgress NOTIFY progressChanged)
    Q_PROPERTY(double echelle READ getEchelle WRITE setEchelle NOTIFY echelleChanged)
    Q_PROPERTY(int duration READ getDuration WRITE setDuration NOTIFY durationChanged)
    Q_PROPERTY(bool isSimplify READ isSimplify WRITE setIsSimplify NOTIFY isSimplifyChanged)


public:
    CustomProgressBar(QQuickPaintedItem *parent=0);

    void paint(QPainter *painter);

    qreal getStartAngle() {return __start_angle;}
    qreal getSpanAngle() {return __span_angle;}
    int getDepth() {return __depth;}
    QColor getColor() {return __color;}
    int getProgress() {return __progress;}
    double getEchelle() { return __echelle;}
    int getDuration() {return __duration;}
    bool isSimplify() {return __issimplify;}

    void setStartAngle(qreal angle){__start_angle=angle;this->update();}
    void setSpanAngle(qreal angle){__span_angle=angle;this->update();}
    void setDepth(int depth){__depth=depth;this->update();}
    void setColor(QColor color){__color=color;this->update();}
    void setProgress(int progress){__progress=progress;this->update();}
    void setEchelle(double echelle) { __echelle=echelle;this->update();}
    void setDuration(int duration){__duration=duration;this->update();}
    void setIsSimplify(bool issimplify) {__issimplify=issimplify;this->update();}

signals:
    void startAngleChanged();
    void spanAngleChanged();
    void depthChanged();
    void colorChanged();
    void progressChanged();
    void echelleChanged();
    void durationChanged();
    void isSimplifyChanged();

public slots:   

private:
    qreal __start_angle;
    qreal __span_angle;
    int __depth;
    QColor __color;
    int __progress;
    double __echelle;
    int __duration;
    QImage __progressImage;
    QPainter *painter;
    bool __issimplify;
};

#endif // CUSTOMPROGRESSBAR_H
