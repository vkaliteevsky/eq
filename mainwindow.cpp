#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    cur  = (double**)malloc(desk_size*sizeof(double*));
    next = (double**)malloc(desk_size*sizeof(double*));
    int i,x,y;
    for(i = 0; i < desk_size; i++)
    {
        cur[i]=(double*)malloc(desk_size*sizeof(double));//
    }
    for(i = 0; i < desk_size; i++)
    {
        next[i]=(double*)malloc(desk_size*sizeof(double));//
    }
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
	    bound_cond();


	    double t = 0;
	int step = 0;
	for(;;)
	    {
	        t += Tstep; step++;
	        mScene->clear();
	    //    print(cur);
	   //     print(next);
	    //    printf("\n\n");
	        for (int i = 1; i < Nx-1; i++)
	        {
	            for (int j = 1; j < Ny-1; j++)
	            {
	               // next[i][j]=(i+j)/200;
	                next[i][j] = cur[i][j]+Tstep*( (cur[i+1][j]-2*cur[i][j]+cur[i-1][j])/(hx*hx) + (cur[i][j+1]-2*cur[i][j]+cur[i][j-1])/(hy*hy) );

	                int qwe =color_int(next[i][j]);
	               QColor brush_color(qwe,qwe,qwe);
	                //QColor brush_color(i,j,i+j);
	               mScene->addRect(i*4, j*4, 4, 4, QPen(brush_color), QBrush(brush_color));
	            }
	        }
	        printf("next[1][1]= %f", cur[1][1]);
	        for (int x = 0; x < Nx; x++)
	        {
	            for (int y = 0; y < Ny; y++)
	            {
	                cur[x][y] = next[x][y];
	            }
	        }
	        bound_cond();
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
