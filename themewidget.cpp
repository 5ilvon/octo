#include "themewidget.h"
#include "ui_themewidget.h"
#include "NetworkManagerWrapper.hpp"

#include <QtCharts/QChartView>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QAreaSeries>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtCore/QRandomGenerator>
#include <QtWidgets/QApplication>
#include <QDate>
#include <QTimeZone>

#include "qcustomplot.h"

ThemeWidget::ThemeWidget(QWidget* parent) :
    QWidget(parent),
    m_ui(new Ui_ThemeWidgetForm),
    m_net(new NetworkManagerWrapper) {
    m_ui->setupUi(this);
    populateThemeBox();

    m_net->Start();

    m_ui->widget->setVisible(false); // hide right sideframe
                                     //m_ui->widget->

    connect(m_net, &NetworkManagerWrapper::resultReady, this, &ThemeWidget::handleResults);

    connect(m_ui->pushButton_2, &QAbstractButton::clicked, this, [=] {
        m_ui->widget->setVisible(!m_ui->widget->isVisible());
    });

    m_ui->qcpWidget->setBackground(QColor(0, 0, 255, 0)); // test for change background color

    // Set the colors from the light theme as default ones
    QPalette pal = qApp->palette();
    pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
    pal.setColor(QPalette::WindowText, QRgb(0x404044));
    qApp->setPalette(pal);

    updateUI();
}

void ThemeWidget::handleResults(const OHLCData& data) {
    setupSimpleDemo(m_ui->qcpWidget, data);
}

ThemeWidget::~ThemeWidget() {
    delete m_ui;
}

void ThemeWidget::populateThemeBox() {
    // add items to theme combobox
    m_ui->themeComboBox->addItem("Light", QChart::ChartThemeLight);
    m_ui->themeComboBox->addItem("Blue Cerulean", QChart::ChartThemeBlueCerulean);
    m_ui->themeComboBox->addItem("Dark", QChart::ChartThemeDark);
    m_ui->themeComboBox->addItem("Brown Sand", QChart::ChartThemeBrownSand);
    m_ui->themeComboBox->addItem("Blue NCS", QChart::ChartThemeBlueNcs);
    m_ui->themeComboBox->addItem("High Contrast", QChart::ChartThemeHighContrast);
    m_ui->themeComboBox->addItem("Blue Icy", QChart::ChartThemeBlueIcy);
    m_ui->themeComboBox->addItem("Qt", QChart::ChartThemeQt);
}

// TODO: Make wrapper for qcustomplot with support autoscaling, crosshair cursor and etc.
void ThemeWidget::setupSimpleDemo(QCustomPlot* customPlot, const OHLCData& data) {
    double binSize = 3600; // bin data in 1 minute intervals

    // create candlestick chart:
    auto candlesticks = new QCPFinancial(customPlot->xAxis, customPlot->yAxis);
    candlesticks->setName("Candlestick");
    candlesticks->setChartStyle(QCPFinancial::csCandlestick);
    candlesticks->addData(data.timeKeys, data.open, data.high, data.low, data.close, true);
    candlesticks->setWidth(binSize * 0.8);
    candlesticks->setTwoColored(true);
    candlesticks->setBrushPositive(QColor(8, 153, 129));
    candlesticks->setBrushNegative(QColor(242, 54, 69));
    candlesticks->setPenPositive(QPen(QColor(8, 153, 129)));
    candlesticks->setPenNegative(QPen(QColor(242, 54, 69)));

    // create ohlc chart:
    auto ohlc = new QCPFinancial(customPlot->xAxis, customPlot->yAxis);
    ohlc->setName("OHLC");
    ohlc->setChartStyle(QCPFinancial::csOhlc);
    ohlc->addData(data.timeKeys, data.open, data.high, data.low, data.close, true);
    ohlc->setWidth(0); // for candlesticks view force to zero width of ohlc bars -> remove horizontal aliasing lines
    ohlc->setTwoColored(true);
    ohlc->setPenPositive(QPen(QColor(8, 153, 129)));
    ohlc->setPenNegative(QPen(QColor(242, 54, 69)));

    // create bottom axis rect for volume bar chart:
    auto volumeAxisRect = new QCPAxisRect(customPlot);
    customPlot->plotLayout()->addElement(1, 0, volumeAxisRect);
    volumeAxisRect->setMaximumSize(QSize(QWIDGETSIZE_MAX, 100));
    volumeAxisRect->axis(QCPAxis::atBottom)->setLayer("axes");
    volumeAxisRect->axis(QCPAxis::atBottom)->grid()->setLayer("grid");
    // bring bottom and main axis rect closer together:
    customPlot->plotLayout()->setRowSpacing(-115);
    volumeAxisRect->setAutoMargins(QCP::msLeft | QCP::msRight | QCP::msBottom);
    volumeAxisRect->setMargins(QMargins(0, 0, 0, 0));
    // create two bar, for positive (green) and negative (red) volume bars:
    customPlot->setAutoAddPlottableToLegend(false);
    auto volumePos = new QCPBars(volumeAxisRect->axis(QCPAxis::atBottom), volumeAxisRect->axis(QCPAxis::atLeft));
    auto volumeNeg = new QCPBars(volumeAxisRect->axis(QCPAxis::atBottom), volumeAxisRect->axis(QCPAxis::atLeft));
    for (int i = 1; i < data.volume.size(); ++i) {
        (data.close[i] < data.close[i - 1] ? volumeNeg : volumePos)->addData(data.timeKeys[i], qAbs(data.volume[i])); // add data to either volumeNeg or volumePos, depending on sign of v
    }
    volumePos->setWidth(binSize * 0.998);
    volumePos->setPen(Qt::NoPen);
    volumePos->setBrush(QColor(8, 153, 129, 127));
    volumeNeg->setWidth(binSize * 0.998);
    volumeNeg->setPen(Qt::NoPen);
    volumeNeg->setBrush(QColor(242, 54, 69, 127));

    //interconnect x-axis ranges of main and bottom axis rects
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), volumeAxisRect->axis(QCPAxis::atBottom), SLOT(setRange(QCPRange)));
    connect(volumeAxisRect->axis(QCPAxis::atBottom), SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis, SLOT(setRange(QCPRange)));

    //configure axes of both main and bottom axis rect
    QSharedPointer<QCPAxisTickerDateTime> dateTimeTicker(new QCPAxisTickerDateTime);
    dateTimeTicker->setDateTimeSpec(Qt::OffsetFromUTC);
    auto timezoneMoscow = new QTimeZone(3600 * 3);
    dateTimeTicker->setTimeZone(*timezoneMoscow);
    dateTimeTicker->setDateTimeFormat("dd. MMMM\nHH:mm");
    volumeAxisRect->axis(QCPAxis::atBottom)->setTicker(dateTimeTicker);
    volumeAxisRect->axis(QCPAxis::atLeft)->setSubTicks(false);
    volumeAxisRect->axis(QCPAxis::atLeft)->setTicks(false);
    volumeAxisRect->axis(QCPAxis::atLeft)->grid()->setVisible(false);
    customPlot->xAxis->setBasePen(Qt::NoPen);
    customPlot->xAxis->setTickLabels(false);
    customPlot->xAxis->setTicks(false); // only want vertical grid in main axis rect, so hide xAxis backbone, ticks, and labels
    customPlot->xAxis->setTicker(dateTimeTicker);
    customPlot->rescaleAxes();
    customPlot->xAxis->scaleRange(1.025, customPlot->xAxis->range().center());
    customPlot->yAxis->scaleRange(1.1, customPlot->yAxis->range().center());

    // make axis rects left side line up:
    auto group = new QCPMarginGroup(customPlot);
    customPlot->axisRect()->setMarginGroup(QCP::msLeft | QCP::msRight, group);
    volumeAxisRect->setMarginGroup(QCP::msLeft | QCP::msRight, group);

    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    volumeAxisRect->setRangeDrag(Qt::Horizontal); // disable free drag for volume plot
    volumeAxisRect->setRangeZoom(Qt::Horizontal); // disable vertical zoom vor volume plot

    connect(customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
    connect(customPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));

    // Test crosshair binned to x-values
    auto t = new QCPItemTracer(customPlot);
    t->setGraph(reinterpret_cast<QCPGraph*>(candlesticks));
    t->setGraphKey(data.timeKeys[0]);
    t->setStyle(QCPItemTracer::tsCrosshair);
    //t->setPen(QPen(Qt::red));
    //t->setBrush(Qt::red);

    customPlot->replot(); // replot after get data and initial setup
}

void ThemeWidget::updateUI() {
    QChart::ChartTheme theme = static_cast<QChart::ChartTheme>(
        m_ui->themeComboBox->itemData(m_ui->themeComboBox->currentIndex()).toInt());

    // Set palette colors based on selected theme
    QPalette pal = window()->palette();
    if (theme == QChart::ChartThemeDark) {
        pal.setColor(QPalette::Window, QRgb(0x121218));
        pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
    } else if (theme == QChart::ChartThemeBlueCerulean) {
        pal.setColor(QPalette::Window, QRgb(0x40434a));
        pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
    } else if (theme == QChart::ChartThemeBrownSand) {
        pal.setColor(QPalette::Window, QRgb(0x9e8965));
        pal.setColor(QPalette::WindowText, QRgb(0x404044));
    } else if (theme == QChart::ChartThemeBlueNcs) {
        pal.setColor(QPalette::Window, QRgb(0x018bba));
        pal.setColor(QPalette::WindowText, QRgb(0x404044));
    } else if (theme == QChart::ChartThemeHighContrast) {
        pal.setColor(QPalette::Window, QRgb(0xffab03));
        pal.setColor(QPalette::WindowText, QRgb(0x181818));
    } else if (theme == QChart::ChartThemeBlueIcy) {
        pal.setColor(QPalette::Window, QRgb(0xcee7f0));
        pal.setColor(QPalette::WindowText, QRgb(0x404044));
    } else {
        pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
        pal.setColor(QPalette::WindowText, QRgb(0x404044));
    }
    window()->setPalette(pal);
}

void ThemeWidget::mousePress() {
    // TODO: auto rescale plot when mouse wheel or press button
}

void ThemeWidget::mouseWheel() {
    // TODO: auto rescale plot when mouse wheel or press button
}