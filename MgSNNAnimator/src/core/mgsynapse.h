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
#ifndef MGSYNAPSE_H_
#define MGSYNAPSE_H_

#include <Mg3D/MgLagrangeCurve>
class MgNeuron;

class MgSynapse
{
public:
	enum Type
	{
		Inhibitory = -1,
		Excitatory = 1
	};

	MgSynapse(MgNeuron* from,MgNeuron *to,double deley,Type type);

	QVector3D spikePosition(double trigerTime,double t)const;
	double deley()const;
	bool isSpikeAlive(double triggerTime,double t)const;

	bool isActive()const{return m_active;}
	void setActive(bool active){m_active = active;}

	Type type()const{return m_type;}
	void setType(Type t){m_type = t;}

	const MgNeuron* from()const{return m_from;}
	const MgNeuron* to()const{return m_to;}

	MgLagrangeCurve curve()const{return m_curve;}

	void reset()
	{
		m_active = false;
	}

private:
	Type m_type;
	MgNeuron* m_from, *m_to;
	MgLagrangeCurve m_curve;
	bool m_active;
};

#endif /* MGSYNAPSE_H_ */
