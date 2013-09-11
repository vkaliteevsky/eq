#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QGraphicsScene>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	
private slots:
    void on_pushButton_clicked();

    void bound_cond();

private:
    Ui::MainWindow *ui;
    QGraphicsScene * mScene;
    double mCounter;
//    bool mFlag;
    double **cur;
    double **next;
    double  Xmin;
    double  Xmax;
    double  Ymin;
    double  Ymax;
    int SAVE_STEP;
    int Nx;
    int Ny;

    double Tstep;
    double Tmax;


    double hx, hy;

};

#endif // MAINWINDOW_H
