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

#include "circlecompactor.h"
#include <QPointF>
#include <math.h>
const double CircleCompactor::PADDING=0;
CircleCompactor::CircleCompactor()
{
  m_ndone=0;
  SORT_PARAM_1=0.1;
  SORT_PARAM_2=0.8;
}
void CircleCompactor::addCircle(double radius)
{
  m_circles.append(Circle(radius,0,0));
}
void CircleCompactor::optimize()
{
  QLinkedList<QPointF> freePoints;
  preshuffle();
  BaseGenerator base_points(.4, .4);
  for(m_ndone=0;m_ndone<m_circles.size();m_ndone++)
  {
    QLinkedList<QPointF>::iterator cur_free;
    bool found=false;
    for (cur_free = freePoints.begin(); cur_free != freePoints.end(); ++cur_free)
    {
      if(available(*cur_free,m_circles[m_ndone].radius()))
      {
        QPointF p=*cur_free;
        freePoints.erase(cur_free);
        makeCircle(p,freePoints);
        found=true;
        break;
      }
    }
    if(!found)
    {
      while(true)
      {
        QPointF cur_p=base_points.generate();
        if(available(cur_p,m_circles[m_ndone].radius()))
        {
          makeCircle(cur_p,freePoints);
          break;
        }
        else if(!contained(cur_p))
          freePoints.append(cur_p);
      }
    }
  }
}
bool CircleCompactor::available(const QPointF &point, double radius)
{
  for(int i=0;i<m_ndone;i++)
  {
    if(distance(point, m_circles[i].center()) < radius + m_circles[i].radius()+ PADDING)
      return false;
  }
  return true;
}

void CircleCompactor::makeCircle(QPointF center, QLinkedList<QPointF> &freePoints)
{
  m_circles[m_ndone]=Circle(m_circles[m_ndone].radius(),center.x(),center.y());
  QLinkedList<QPointF>::iterator cur_free;
  for (cur_free = freePoints.begin(); cur_free != freePoints.end(); )
  {
    if(contains(m_circles[m_ndone],*cur_free))
    {
      cur_free=freePoints.erase(cur_free);
    }
    else
      cur_free++;
  }
}

#include <QDebug>
static bool sortCircles(const CircleCompactor::Circle &a, const CircleCompactor::Circle &b){
  if(a.radius()>b.radius()) return true;
  //else if(a.radius()>b.radius()) return 1;
  else return false;
}
void CircleCompactor::preshuffle()
{
  // (1, 0) = totally sorted   - appealing border, very dense center, sparse midradius
  // (0, 1), (1, 1) = totally randomized  - well packed center, ragged border
  //const double SORT_PARAM_1 = .10,  SORT_PARAM_2 = .80;
  qSort(m_circles.begin(),m_circles.end(),sortCircles);
  shuffle(m_circles,0,(int)m_circles.size()*SORT_PARAM_1);
  shuffle(m_circles,(int)m_circles.size()*SORT_PARAM_2,m_circles.size());

}
int CircleCompactor::random(int a, int b)
{
  return qrand() % (b - a) + a;
}
double CircleCompactor::distance(QPointF p1, QPointF p2)
{
  return sqrt((p1.x()-p2.x())*(p1.x()-p2.x())+(p1.y()-p2.y())*(p1.y()-p2.y()));

}
bool CircleCompactor::contains(const Circle &circle, const QPointF &point)
{
  return distance(circle.center(), point) < circle.radius() + PADDING;
}
bool CircleCompactor::contained(QPointF point)
{
  for(int i=0;i<m_ndone;i++)
    if(contains(m_circles[i],point))
      return true;
  return false;

}

CircleCompactor::BaseGenerator::BaseGenerator(double radialRes, double angularRes)
{
    m_radialRes=radialRes;
    m_angularRes=angularRes;

    m_first=true;
}

  QPointF CircleCompactor::BaseGenerator::generate()
  {
    const double circle_angle = 2 * M_PI;
    if(!m_first)
      goto next;
    m_first=false;
    m_statusR=0;
    for(;;)
    {
      m_statusTheta = 0;
      while(m_statusTheta <= circle_angle)
      {
        return QPointF(m_statusR * cos(m_statusTheta), m_statusR *sin(m_statusTheta));
      next:
        double r_ = m_statusR>1?sqrt(m_statusR):1;
        m_statusTheta += m_angularRes/r_;
      }
      m_statusR += m_radialRes;
    }
  }
