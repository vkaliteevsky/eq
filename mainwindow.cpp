#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <complex>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


#include <QColor>
#include <QWaitCondition>
#include <QMutex>
#include <QGraphicsScene>
#include <QTimer>



MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	mScene = new QGraphicsScene;
	ui->setupUi(this);

	ui->graphicsView->setScene(mScene);
	Nx = 100;
	Ny = 100;
	int desk_size = Nx;
	mScene->setSceneRect(0, 0, 400, 400);

	int i,x,y;


	for (x = 0; x < desk_size; x++)
	{
		for (y = 0; y < desk_size; y++)
		{
			cur[x][y] = 0;
			next[x][y] = 0;
		}
	}
	Xmin = 0.0;
	Xmax = 1.0;
	Ymin = 0.0;
	Ymax = 1.0;
	Tmax = 10;
	Tstep = 0.01;
	SAVE_STEP = 10;
}

MainWindow::~MainWindow()
{
	delete ui;
}


int color_int(double x)
{

    if (x == 1){return 255;}
    double step = 0.00390625;
    return floor(x/step);
}

int color_int2(double x)
{

	if (x > 20){return 255;}
	if (x < -40){return 0;}
	x+=40;
    double step = 0.234375;
    return floor(x/step);
}

void print (double ** board)
{
    int i;
    int j;
    for (i = 0; i < 100; i++)
    {
        for (j = 0; j < 100; j++)
        {
            printf ("%4.4f ", board [i][j]);
        }
//        printf ("\n");
    }
}

void MainWindow::bound_cond()
{
    for (int x = 0; x < Nx; x++)
    {
        cur[x][0]  = 0;
        cur[x][Ny-1] = 0;
    }
    for (int y = 0; y < Ny; y++)
    {
        cur[0][y]  = 1;
        cur[Nx-1][y] = 0;
    }
    return;
}


void MainWindow::on_pushButton_clicked()
{
	//    hx = (Xmax-Xmin)/Nx;
	//    hy = (Ymax-Ymin)/Ny;
	hx = 0.5; hy = 0.5;
	//bound_cond();

	int cell = 4;
	int count = 0;

	double t = 0;
	int step = 0;

	complex  A( 2,0);
	complex x0( 2,0);
	complex sigm( 2,0);
	complex kx( 2,0);
	complex  U( 2,0);
	qreal alpha = 2;
	complex i1(0, 1);

	complex deltax(1,0);

	for (int i = 0; i < Nx; i++)
	{
        next[i][0] = A * exp( -(pow((next[i][0]-x0),2)) / (complex(2,0)*(pow(sigm,2)) )) * exp(kx*(next[i][0])*i1);
    }

	for(;;)
	{
		t += Tstep; step++;
		mScene->clear();




		for (int k = 0; k < Nx; k++)
		{
			for (int j = 0; j < Ny; j++)
			{



				if ((j==0)||(j==(Ny-1))) {
					complex t;
					t =next[j][k] + (Tstep/i1)*(next[j][k]*alpha*pow((abs(next[j][k])),2) + U*next[j][k]);
					if (t.real() == NAN) {
						qreal a = 4;
						a = a + 1;
					}
					next[j][k+1] = t;
				} else {
					complex t;
					complex sec = -(secderivative(next[j-1][k], next[j][k], next[j+1][k], deltax));
					t = next[j][k] + (Tstep/i1)*(sec + next[j][k]*alpha*pow(abs(next[j][k]), 2) + U*next[j][k]);
					if (t.real() == NAN) {
						qreal a = 4;
						a = a + 1;
					}
					next[j][k+1] =  t;
				}



				/*

				// next[i][j]=(i+j)/200;
				next[i][j] = cur[i][j]+Tstep*( (cur[i+1][j]-2*cur[i][j]+cur[i-1][j])/(hx*hx) + (cur[i][j+1]-2*cur[i][j]+cur[i][j-1])/(hy*hy) );



				//QColor brush_color(i,j,i+j);
				mScene->addRect(i*4, j*4, 4, 4, QPen(brush_color), QBrush(brush_color));
				*/

				int qwe =color_int2(real(next[k][j]));
				QColor brush_color(qwe,qwe,qwe);

				mScene->addRect(cell * j, cell * (Ny - k), cell, cell, QPen(brush_color), QBrush(brush_color));
			}
		}

		count++;
		if (count == 10)
			printf("Hello world");
		//printf("next[1][1]= %f", cur[1][1]);
		for (int x = 0; x < Nx; x++)
		{
			for (int y = 0; y < Ny; y++)
			{
				cur[x][y] = next[x][y];
			}
		}
		//bound_cond();
		if(step % SAVE_STEP)
		{
			ui->graphicsView->show();
			QEventLoop loop;
			QTimer::singleShot(10, &loop, SLOT(quit()));
			loop.exec();
		}
	}


	free(cur);
	free(next);
	return;
}

complex MainWindow::secderivative(complex a, complex b, complex c, complex delta)
{
    complex res = (a- complex(2,0) * b + c)/(delta * delta);
	return res;
}

void MainWindow::on_pushButton_2_clicked()
{

}


