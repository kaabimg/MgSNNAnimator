/*************************************************************************
** 
** Copyright 2011 Mohamed-Ghaïth KAABI (mohamedghaith.kaabi@gmail.com)
** This file is part of MgSNNAnimator.
** 
** MgSNNAnimator is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** any later version.
** 
** MgSNNAnimator is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with MgSNNAnimator.  If not, see <http://www.gnu.org/licenses/>.
** 
***************************************************************************/
#ifndef MGNEURON_H_
#define MGNEURON_H_

#include <QVector3D>
#include <QPointF>
#include <QVector>
class MgCurveData;

class MgNeuron
{

public:
	MgNeuron(uint id);
	~MgNeuron();
	static const double NO_SPIKE = -1;
	uint id()const{return m_id;}

	QVector3D position()const{return m_position;}
	void setPosition(const QVector3D& pos)
	{
		m_position=pos;
	}

	void updateActivity(double t);
	uint activity()const{return m_activity;}

	MgCurveData * activityData()const{return m_activityData;}


	double lastEmittedSpike()const{return m_lastEmittedSpike;}
	void setLastEmittedSpike(double t)
	{
		if(t>m_lastEmittedSpike)
			m_lastEmittedSpike = t;
	}

	double lastReceivedSpike()const{return m_lastReceivedSpike;}
	void setLastReceivedSpike(double t)
	{
		if(t>m_lastReceivedSpike)
			m_lastReceivedSpike = t;
	}

	void reset()
	{
		m_lastEmittedSpike = NO_SPIKE;
		m_lastReceivedSpike = NO_SPIKE;
	}

private:
	uint m_id;
	QVector3D m_position;
	double m_lastEmittedSpike,m_lastReceivedSpike;
	uint m_activity;
	qreal m_activityRange,m_minActivity;
	MgCurveData * m_activityData;
};



#endif /* MGNEURON_H_ */
