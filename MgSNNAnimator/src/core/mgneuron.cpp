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
#include "mgneuron.h"
#include <MgGui/MgCurveData>

MgNeuron::MgNeuron(uint id):m_id(id),m_lastEmittedSpike(0),m_activity(0)
{
	m_activityData = new MgCurveData;
	reset();
}
MgNeuron::~MgNeuron()
{
	delete m_activityData;
}

double remapValue(double value,double low1,double high1,double low2,double high2)
{
	return low2 + (high2 - low2) * (value - low1) / (high1 - low1);
}



void MgNeuron::updateActivity(double t)
{
	if(m_activityData->isEmpty())
		return;
	int before = m_activityData->firstIndexBefore(t);
	int after = m_activityData->firstIndexAfter(t);

	m_activity = remapValue(
			remapValue(t,
						m_activityData->at(before).x(),
						m_activityData->at(after).x(),
						m_activityData->at(before).y(),
						m_activityData->at(after).y()
						),
			m_activityData->minY(),
			m_activityData->maxY(),
			0,255
			);
}

