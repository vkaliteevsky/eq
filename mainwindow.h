#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QGraphicsScene>
#include <complex>

typedef std::complex<double> complex;

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

	void on_pushButton_2_clicked();



private:
	complex secderivative(complex a, complex b, complex c, complex delta);
    Ui::MainWindow *ui;
    QGraphicsScene * mScene;
    double mCounter;
//    bool mFlag;
	complex cur[100][100];
	complex next[100][100];
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
