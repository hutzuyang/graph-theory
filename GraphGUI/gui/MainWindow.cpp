#include "MainWindow.hpp"
#include "GraphWidget.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    GraphWidget *widget = new GraphWidget(this);
    setCentralWidget(widget);
    setWindowTitle("Graph MDS Visualizer");
}
