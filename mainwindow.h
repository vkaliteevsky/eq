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



	void on_MainWindow_destroyed();

	void on_pushButton_3_clicked();

private:
	void print (double ** board);
	int color_int(double x);
	int color_int2(double x);
	int color_int3(double x);
	QColor color_int5(double x);
	complex secderivative(complex a, complex b, complex c, complex delta);
    Ui::MainWindow *ui;
    QGraphicsScene * mScene;
	QGraphicsScene * mScene2;
    double mCounter;
//    bool mFlag;
	complex cur[200][200];
	complex next[200][200];
    double  Xmin;
    double  Xmax;
    double  Ymin;
    double  Ymax;
    int SAVE_STEP;
    int Nx;
    int Ny;

	double mMaxi;

    double Tstep;
    double Tmax;

	bool isOpen;
    double hx, hy;

};

#endif // MAINWINDOW_H
