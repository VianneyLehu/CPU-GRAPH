#include "MainWindow.h"
#include "pdh.h"
#include "cpu.h"

#define KB 1024
#define MB (KB*1024)
#define GB (MB*1024)
#define TB (GB *1024)


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
    setupCpuGraph();

    init();
    setGeometry(100, 100, 1000, 800);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateCpuGraph()));
    timer->start(1000); // Update every second
}

MainWindow::~MainWindow()
{

}

void MainWindow::setupCpuGraph()
{
    chart = new QChart();
    chart->legend()->hide();
    chart->setTitle("CPU Usage Over Time");

    series = new QLineSeries();

    chart->addSeries(series);

    axisX = new QValueAxis;
    axisX->setRange(0, 60); // Assuming 60 seconds
    axisX->setLabelFormat("%i");
    axisX->setTitleText("Time (s)");

    axisY = new QValueAxis;
    axisY->setRange(0, 100); // CPU usage in percentage
    axisY->setLabelFormat("%i");
    axisY->setTitleText("CPU Usage (%)");


    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    chart->addSeries(series);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    setCentralWidget(chartView);
}

void MainWindow::updateCpuGraph()
{

    double cpuUsage = getCurrentValue();

    count ++;

    if(count >= 60){
        series->remove(0);
        axisX->setRange(count - 59, count);
    }

    if (cpuUsage != -1.0) {
        series->append(count, cpuUsage);
        ++count;
        axisX->setMax(count > 60 ? count : 60); // Adjust maximum X value if needed
    } else {
        std::cerr << "Failed to retrieve CPU usage." << std::endl;
    }


}

