#ifndef THERMOMETERWIDGET_H
#define THERMOMETERWIDGET_H

#include <QWidget>
#include <QLabel>

class ThermometerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ThermometerWidget(QWidget *parent = nullptr);

    void setTemperature(double temp);
    void setRange(double min, double max);
    void setSizeMM(int widthMM, int totalHeightMM);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void updateLayout();
    QColor getFillColor(double temp) const;
    void drawScale(QPainter &painter, const QRect &columnRect);

    double m_temperature = 20.0;
    double m_minTemp = -20.0;
    double m_maxTemp = 50.0;
    int m_widthMM = 5;
    int m_totalHeightMM = 200;
    QLabel *m_valueLabel;
};

#endif // THERMOMETERWIDGET_H
