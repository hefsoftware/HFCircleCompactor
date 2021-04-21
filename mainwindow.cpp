/*
Copyright (C) 2021 - Marzocchi Alessandro

This file is part of HFCircleCompactor.

HFCircleCompactor is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Foobar is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with HFCircleCompactor.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <circlecompactor.h>
#include <QDebug>
#include <QElapsedTimer>
#include <QGraphicsEllipseItem>
QDebug operator<<(QDebug dbg, const CircleCompactor::Circle &c)
{
  dbg.nospace() << "(r" <<c.radius()<<"("<< c.x() << ", " << c.y() << "))";
  return dbg.space();
}
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->formOptions->hide();
    QVector<int> v;
    v.append(0);
    v.append(1);
    v.append(2);
    v.append(3);
    v.append(4);
    v.append(5);
    v.append(6);
    v.append(7);
    v.append(8);
    v.append(9);
}



MainWindow::~MainWindow()
{
    delete ui;
}
static int limitColor(double v)
{
  return qMax(qMin((int)v,255),0);
}

QColor MainWindow::colorFor(double radius)
{
  const double maxradius=4;
  return QColor(limitColor(255*radius/maxradius),limitColor(255-255*radius/maxradius),0);
}

void MainWindow::on_pushButton_clicked()
{
  QElapsedTimer t;
  t.start();
  CircleCompactor c;
  //c.shuffle(v,7,v.size()-1);
  //qDebug()<<v;
//  for(int i=0;i<ui->numCircles->value();i++)
//  {
//    const double a=1,b=4;
//    double randomV=(double)qrand()/RAND_MAX;
//    randomV*=randomV*randomV*randomV;
//    c.addCircle((b-a)*randomV+a);
//  }
  for(int i=0;i<30;i++)
    c.addCircle(1.7);
  for(int i=0;i<5;i++)
    c.addCircle(1.9);
  c.addCircle(2.2*.5);
  for(int i=0;i<2;i++)
    c.addCircle(4.4);
  c.SORT_PARAM_1=ui->sortParam1->value();
  c.SORT_PARAM_2=ui->sortParam2->value();
  qDebug()<<c.m_circles;
  c.optimize();
  qDebug()<<c.m_circles;
  double scale=5;
  ui->gView->setScene(new QGraphicsScene(this));
  for(int i=0;i<c.m_circles.size();i++)
  {
    const CircleCompactor::Circle &cc=c.m_circles[i];
    ui->gView->scene()->addEllipse((cc.x()-cc.radius())*scale,(cc.y()-cc.radius())*scale,2*cc.radius()*scale,2*cc.radius()*scale,QPen(),QBrush(colorFor(cc.radius())));
  }
  ui->statusBar->showMessage(tr("%1ms elapsed").arg(t.elapsed()));
}

void MainWindow::on_toolButton_clicked()
{
  if(ui->formOptions->isVisible())
    ui->formOptions->hide();
  else
    ui->formOptions->show();
}
