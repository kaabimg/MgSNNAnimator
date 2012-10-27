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
#include "mgsynapse.h"
#include "mgneuron.h"


MgSynapse::MgSynapse(MgNeuron* from,MgNeuron *to,double deley,Type type)
	:m_type(type),m_from(from),m_to(to),m_active(false)
{
	m_curve.init(deley,from->position(),to->position());
}

double MgSynapse::deley()const
{
	return m_curve.period();
}

QVector3D MgSynapse::spikePosition(double trigerTime,double t)const
{
	return m_curve.position(t-trigerTime);
}
bool MgSynapse::isSpikeAlive(double triggerTime,double t)const
{
	return triggerTime + m_curve.period() > t;
}
