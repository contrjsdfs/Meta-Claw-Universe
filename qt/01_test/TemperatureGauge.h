#ifndef TEMPERATUREGAUGE_H
#define TEMPERATUREGAUGE_H

#include <QWidget>
#include <QLabel>

class TemperatureGauge : public QWidget
{
    Q_OBJECT
public:
    explicit TemperatureGauge(QWidget *parent = nullptr);

    void setTemperature(double temp);
    void setRange(double min, double max);
    void setSizeMM(int widthMM, int heightMM);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    QSize sizeHint() const override;

private:
    void updateLabelFont();
    void calculateDimensions();
    QColor calculateBarColor(double temp) const;

    double m_temperature = 20.0;
    double m_minTemp = 0.0;
    double m_maxTemp = 100.0;
    int m_widthMM = 15;    // 控件总宽度
    int m_heightMM = 80;   // 控件总高度
    int m_columnWidthMM = 4; // 柱状图宽度9mm
    int m_columnHeightMM = 60; // 柱状图高度60mm
    QLabel *m_valueLabel;

    // 尺寸缓存
    int m_gaugeWidthPx;
    int m_gaugeHeightPx;
    QRect m_gaugeRect;
};

#endif // TEMPERATUREGAUGE_H
