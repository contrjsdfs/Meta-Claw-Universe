#include "TemperatureGauge.h"
#include <QPainter>
#include <QVBoxLayout>
#include <QFontMetrics>
#include <QLinearGradient>
#include <QPainterPath>

TemperatureGauge::TemperatureGauge(QWidget *parent)
    : QWidget(parent),
    m_valueLabel(new QLabel(this))
{
    // 初始化标签（放在上方）
    m_valueLabel->setAlignment(Qt::AlignCenter);
    m_valueLabel->setStyleSheet(
        "color: white;"
        "font-weight: bold;"
        "font-size: 20px;"  // 初始大字体
        "background: transparent;"
        "margin-bottom: 5px;"  // 与柱状图的间距
        "min-width: 40px;"
        );

    // 设置布局（标签在上，柱状图在下）
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(m_valueLabel, 0, Qt::AlignHCenter);
    layout->addStretch();  // 柱状图占据剩余空间

    setSizeMM(m_widthMM, m_heightMM);
}

void TemperatureGauge::setTemperature(double temp)
{
    if (!qFuzzyCompare(m_temperature, temp)) {
        m_temperature = temp;

        // 智能格式化数值
        QString text;
        if (temp < 10 && temp > -10) {
            text = QString::number(temp, 'f', 1) + "%";
        } else {
            text = QString::number(qRound(temp)) + "%";
        }

        m_valueLabel->setText(text);
        updateLabelFont();
        update();
    }
}

void TemperatureGauge::updateLabelFont()
{
    QFont font = m_valueLabel->font();
    QFontMetrics fm(font);

    // 根据可用宽度调整字体大小
    int availableWidth = width() - 10;  // 留出边距
    int fontSize = 14;
    while (fontSize >= 8) {
        font.setPixelSize(fontSize);
        fm = QFontMetrics(font);
        if (fm.horizontalAdvance(m_valueLabel->text()) <= availableWidth) {
            break;
        }
        fontSize--;
    }

    font.setPixelSize(qMax(8, fontSize));
    m_valueLabel->setFont(font);
}

void TemperatureGauge::calculateDimensions()
{
    const double mmToPx = 96.0 / 25.4;

    // 计算柱状图像素尺寸
    m_gaugeWidthPx = qRound(m_columnWidthMM * mmToPx);
    m_gaugeHeightPx = qRound(m_columnHeightMM * mmToPx);

    // 计算标签占用的高度
    int labelHeight = m_valueLabel->height() + m_valueLabel->contentsMargins().bottom();

    // 柱状图位置（居中）
    int gaugeX = (width() - m_gaugeWidthPx) / 2;
    int gaugeY = labelHeight + 2;  // 标签下方留5px间距

    m_gaugeRect = QRect(gaugeX, gaugeY, m_gaugeWidthPx, m_gaugeHeightPx);
}

void TemperatureGauge::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    calculateDimensions();
    updateLabelFont();
}

QSize TemperatureGauge::sizeHint() const
{
    const double mmToPx = 96.0 / 25.4;
    return QSize(
        qRound(m_widthMM * mmToPx),
        qRound(m_heightMM * mmToPx)
    );
}

void TemperatureGauge::setRange(double min, double max)
{
    m_minTemp = min;
    m_maxTemp = max;
    update();
}

void TemperatureGauge::setSizeMM(int widthMM, int heightMM)
{
    m_widthMM = qMax(widthMM, m_columnWidthMM + 6);  // 确保总宽度足够
    m_heightMM = heightMM;

    const double mmToPx = 96.0 / 25.4;
    setFixedSize(
        qRound(m_widthMM * mmToPx),
        qRound(m_heightMM * mmToPx)
    );
}

QColor TemperatureGauge::calculateBarColor(double temp) const
{
    temp = qBound(m_minTemp, temp, m_maxTemp);
    double ratio = (temp - m_minTemp) / (m_maxTemp - m_minTemp);

    if (ratio < 0.3) return QColor(0, 100, 255);   // 低温蓝
    if (ratio < 0.7) return QColor(0, 200, 0);     // 正常绿
    return QColor(255, 60, 60);                    // 高温红
}

void TemperatureGauge::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制背景
    QPainterPath bgPath;
    bgPath.addRoundedRect(m_gaugeRect, 3, 3);
    painter.fillPath(bgPath, QColor(240, 240, 240));

    // 计算填充高度
    double ratio = qBound(0.0, (m_temperature - m_minTemp) / (m_maxTemp - m_minTemp), 1.0);
    int fillHeight = static_cast<int>(m_gaugeHeightPx * ratio);

    // 绘制温度柱
    if (fillHeight > 0) {
        QRect fillRect(
            m_gaugeRect.left(),
            m_gaugeRect.bottom() - fillHeight,
            m_gaugeRect.width(),
            fillHeight
        );

        QPainterPath fillPath;
        fillPath.addRoundedRect(fillRect, 3, 3);

        QLinearGradient gradient(0, fillRect.top(), 0, fillRect.bottom());
        QColor baseColor = calculateBarColor(m_temperature);
        gradient.setColorAt(0.0, baseColor.lighter(120));
        gradient.setColorAt(1.0, baseColor.darker(120));

        painter.fillPath(fillPath, gradient);
    }

    // 绘制边框
    painter.setPen(QPen(Qt::gray, 1));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(m_gaugeRect, 3, 3);
}
