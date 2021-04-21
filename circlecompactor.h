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

#ifndef CIRCLECOMPACTOR_H
#define CIRCLECOMPACTOR_H
#include <QVector>
#include <QPointF>
#include <math.h>
#include <QLinkedList>
class CircleCompactor
{
public:
  class Circle
  {
  public:
    double x() const{return m_center.x();}
    double y() const{return m_center.y();}
    double radius() const{return m_radius;}
    QPointF center() const{return m_center;}
  //protected:
    Circle(double radius=0, double x=0, double y=0):m_radius(radius),m_center(x,y){}
    double m_radius;
    QPointF m_center;
  };
  class BaseGenerator
  {
  public:
    BaseGenerator(double radialRes, double angularRes);
    QPointF generate();
  protected:
    double m_radialRes;
    double m_angularRes;
    double m_statusR;
    double m_statusTheta;
    bool m_first;
  };

  CircleCompactor();
  void addCircle(double radius);
  void optimize();
  // Generate a random number thus that a<=returned value<b
  int random(int a, int b);
  double distance(QPointF p1,QPointF p2);
  bool contains(const Circle &circle, const QPointF &point);
  template <class T> void shuffle(T &array, int index0, int index1)
  {
  for(int i=index0;i<index1;i++)
  {
    int pos=random(i,index1);
    if(i!=pos)
      std::swap(array[i],array[pos]);
    }
  }
  bool available(const QPointF &point, double radius);
  void makeCircle(QPointF center, QLinkedList<QPointF> &freePoints);
  bool contained(QPointF point);

  void preshuffle();
  // Number of divisions of quadrant to categorize circles and
  int m_ndivisions;
  int m_ndone;
  static const double PADDING;
  double SORT_PARAM_1;
  double SORT_PARAM_2;
  QVector<Circle> m_circles;
};

#endif // CIRCLECOMPACTOR_H
