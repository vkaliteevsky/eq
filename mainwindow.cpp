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

	mScene2 = new QGraphicsScene;
	ui->graphicsView_2->setScene(mScene2);
	mScene2->setSceneRect(0, 0, 10, 600);
	double Mas[600];
	qreal a1 = 4.0/600.0;
	Mas[0] = -1.0;

	for (int g = 1; g < 600; g++) {
		Mas[g] = Mas[g-1] +a1;
	}

	for (int g = 0; g < 600; g++) {
		QColor qwe = color_int5(Mas[g]);
		mScene2->addRect(0,(600 - g), 10, 1, QPen(qwe), QBrush(qwe));
	}
	ui->graphicsView_2->show();


	Nx = 200;
	Ny = 200;
	int desk_size = Nx;
	mScene->setSceneRect(0, 0, 600, 600);

	int i,x,y;
	isOpen = true;

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
	mMaxi = 0;
}

MainWindow::~MainWindow()
{
	delete ui;
}


int MainWindow::color_int(double x)
{

    if (x == 1){return 255;}
    double step = 0.00390625;
    return floor(x/step);
}

int MainWindow::color_int2(double x)
{

	if (x > 20){return 255;}
	if (x < -40){return 0;}
	x+=40;
    double step = 0.234375;
    return floor(x/step);
}

int MainWindow::color_int3(double x)
{
	if (abs(x) > abs(mMaxi)) {
		mMaxi = x;
	}
	double step = mMaxi*2/256;

	x+=mMaxi;

    return floor(x/step);
}

QColor MainWindow::color_int5(double x)
{
	int r = 0, g = 0, b = 0;
	if (x<-1) x = -1;
	if (x>4) {
		x =4;
		return  QColor(255, 255, 255);
	}
	x+=1;
	qreal step = 0.00390625;
	if ((x >= 0) && (x < 1)) {
		b = floor(x/step);
		return  QColor(r, g, b);
	} else if ((x >= 1) && (x < 2)) {
		x-=1;
		g = floor(x/step);
		b = 255 - floor(x/step);
		return  QColor(r, g, b);
	} else if ((x >= 2) && (x < 3)) {
		x-=2;
		r = floor(x/step);
		g = 255 - floor(x/step);
		return  QColor(r, g, b);
	} else if ((x >= 3) && (x < 4)) {
		x-=3;
		r = 255;
		g = floor(x/step);
		b = floor(x/step);
		return  QColor(r, g, b);
	}




	return floor(x/step);
}

void MainWindow::print (double ** board)
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
        next[x][0]  = 0;
        next[x][Ny-1] = 0;
    }
    for (int y = 0; y < Ny; y++)
    {
        next[0][y]  = 1;
        next[Nx-1][y] = 0;
    }
    return;
}


void MainWindow::on_pushButton_clicked()
{


	//    hx = (Xmax-Xmin)/Nx;
	//    hy = (Ymax-Ymin)/Ny;
	hx = 0.5; hy = 0.5;
	//bound_cond();

	qreal cell = 3;


	qreal t = 5;
	int step = 0;

	complex  A( 1,0);
	complex x0( 0,0);
	complex sigm( 2,0);
	complex kx( 1,0);
	complex  U( 0,0);
	qreal alpha = 0;
	complex i1(0, 1);

	complex deltax(1,0);
	complex deltat(1,0);

	mScene->clear();
	complex masX[Nx];


	qreal nX = Nx;
	qreal nY = Ny;

	complex a = -40;
	complex b = 40;
    deltax = (b-a)/(complex(nX,0));
	deltat = complex(t/nY,0);
    x0 = (b+a)/2.0;
	//x0 = -30;

	a     = (ui->textEdit->toPlainText().toDouble() );
	b     = (ui->textEdit_2->toPlainText().toDouble() );
	t     = (ui->textEdit_3->toPlainText().toDouble() );
	A     = (ui->textEdit_4->toPlainText().toDouble() );
	U     = (ui->textEdit_5->toPlainText().toDouble() );
	kx    = (ui->textEdit_6->toPlainText().toDouble() );
	alpha = (ui->textEdit_7->toPlainText().toDouble() );
	sigm  = (ui->textEdit_8->toPlainText().toDouble() );

	ui->label_2->setText("a= "+QString::number(a.real()));
	ui->label_3->setText("b= "+QString::number(b.real()));
	ui->label_4->setText("t= "+QString::number(t));

	ui->graphicsView->show();



	masX[0] = a;
	for (int j = 1; j < Nx; j++)
	{
		masX[j] = masX[j-1] + deltax;
	}

	for (int j = 0; j < Nx; j++)
	{
		complex numerator = -(pow((masX[j]-x0),2.0));
		complex denominator = (2.0 *(pow(sigm,2.0)));
		complex nd = numerator / denominator;
		complex exp1 = exp (nd);
		complex exp2 = exp(kx*(masX[j])*i1);

        next[j][0] = A * exp1 * exp2;
		QColor qwe =color_int5(real(next[j][0]));

	}
	ui->graphicsView->show();





	for (int l = 0; l<2; l++)
	{


		t += Tstep;


		int k1;
		if (step == 0){
			k1 = 1;
		} else {
			k1 = 0;
		}
		step++;
		for (int k = k1; k < Ny - 1; k++)
		{
			for (int j = 0; j < Nx; j++)
			{

				if ((j == (39))&&(k == (3))) {
					qreal tmp123;
					tmp123 += 0;
				}

				if ((j==0)||(j==(Ny-1))) {
					complex expr;
					complex nextjk = next[j][k];
					complex deli1 = deltat/i1;
					complex abs_next_jk = (abs(nextjk));
					complex	pow2 = pow(abs_next_jk,2.0);
					complex Unext = U*next[j][k];
					complex Skob = nextjk*alpha*pow2 + Unext;

					expr = nextjk + deli1 * Skob;

					next[j][k+1] = expr;
				} else {
					complex expr;
					complex nextjk = next[j][k];
					complex deli1 = deltat/i1;
					complex	pow2 = pow((abs(nextjk)),2.0);
					complex Unext = U*next[j][k];
					complex Skob = nextjk*alpha*pow2 + Unext;



					complex sec = -(secderivative(next[j-1][k], next[j][k], next[j+1][k], deltax));
					expr = nextjk + deli1*(sec + Skob);

					next[j][k+1] =  expr;
				}

				if ((j == (Ny - 1))&&(k == (Ny - 2))) {
					qreal tmp123;
					tmp123 += 0;
				}

			}
		}

		//count++;
		//if (count == 10)
		//	printf("Hello world");
		//for (int x = 0; x < Nx; x++)
		//{
		//	for (int y = 0; y < Ny; y++)
		//	{
		//		cur[x][y] = next[x][y];
		//	}
		//}
		//bound_cond();
		mScene->clear();

		if (step == 2) {
			for (int k = 0; k < Ny; k++)
			{
				for (int j = 0; j < Nx; j++)
				{
					QColor qwe = color_int5(real(next[j][k]));
					mScene->addRect(cell * j, cell * (Ny - k), cell, cell, QPen(qwe), QBrush(qwe));
				}
			}
		}
		//ui->label->setText("Итерация: "+QString::number(step));
		//if(step % SAVE_STEP)
		//{
			ui->graphicsView->show();
			QEventLoop loop;
			QTimer::singleShot(10, &loop, SLOT(quit()));
			loop.exec();
		//}

	}




	free(cur);
	free(next);
	return;
}

complex MainWindow::secderivative(complex a, complex b, complex c, complex delta)
{
    complex res = (a- 2.0 * b + c)/(delta * delta);
	return res;
}

void MainWindow::on_pushButton_2_clicked()
{
	ui->textEdit->clear();
	ui->textEdit_2->clear();
	ui->textEdit_3->clear();
	ui->textEdit_4->clear();
	ui->textEdit_5->clear();
	ui->textEdit_6->clear();
	ui->textEdit_7->clear();
	ui->textEdit_8->clear();

}



void MainWindow::on_MainWindow_destroyed()
{
    //isOpen = false;
}

void MainWindow::on_pushButton_3_clicked()
{
	ui->textEdit->clear();
	ui->textEdit_2->clear();
	ui->textEdit_3->clear();
	ui->textEdit_4->clear();
	ui->textEdit_5->clear();
	ui->textEdit_6->clear();
	ui->textEdit_7->clear();
	ui->textEdit_8->clear();
	ui->label_2->clear();
	ui->label_3->clear();
	ui->label_4->clear();




	ui->textEdit->insertPlainText("-40");
	ui->textEdit_2->insertPlainText("40");
	ui->textEdit_3->insertPlainText("5");
	ui->textEdit_4->insertPlainText("1");
	ui->textEdit_5->insertPlainText("0");
	ui->textEdit_6->insertPlainText("1");
	ui->textEdit_7->insertPlainText("0");
	ui->textEdit_8->insertPlainText("2");


	ui->label_2->setText("a= -40");
	ui->label_3->setText("b= 40");
	ui->label_4->setText("t= 5");


}
