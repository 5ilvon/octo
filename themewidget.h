#ifndef THEMEWIDGET_H
#define THEMEWIDGET_H

#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include "NetworkManagerWrapper.hpp"

#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
class Ui_ThemeWidgetForm;
QT_END_NAMESPACE

QT_BEGIN_NAMESPACE
class QChart;
QT_END_NAMESPACE

QT_USE_NAMESPACE

struct OHLCData;

class ThemeWidget : public QWidget {
    Q_OBJECT
public:
    explicit ThemeWidget(QWidget* parent = nullptr);
    ~ThemeWidget();

private Q_SLOTS:
    void updateUI();

private:
    void populateThemeBox();
    void setupSimpleDemo(QCustomPlot* customPlot, const OHLCData& data);

private:
    QCustomPlot* m_plot;
    NetworkManagerWrapper* m_net;
    Ui_ThemeWidgetForm* m_ui;

public slots:
    void handleResults(const OHLCData&);

    void mouseWheel();
    void mousePress();
};

#endif /* THEMEWIDGET_H */
