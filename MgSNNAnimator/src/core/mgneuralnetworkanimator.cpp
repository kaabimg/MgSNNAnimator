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
#include "mgneuralnetworkanimator.h"
#include "mgneuralnetworkgraph.h"
#include <QDebug>

MgNeuralNetworkAnimator::MgNeuralNetworkAnimator(MgSpikingNeuralNetwork * graph,QObject * parent)
	:QObject(parent),m_graph(graph),m_animationTime(0)
{
	connect(&m_updateTimer,SIGNAL(timeout()),this,SLOT(update()));

	m_refreshInterval = 50;
	m_animationDuration = 20;
	m_animationStep = .05;
	m_animationTime = 0;
}

MgNeuralNetworkAnimator::~MgNeuralNetworkAnimator() {}

int MgNeuralNetworkAnimator::refreshInterval()const
{
	return m_refreshInterval;
}
double MgNeuralNetworkAnimator::animationDuration()const
{
	return m_animationDuration;
}
double MgNeuralNetworkAnimator::animationStep()const
{
	return m_animationStep;
}


void MgNeuralNetworkAnimator::setRefreshInterval(int interval)
{
	if(m_refreshInterval !=interval)
	{
		m_refreshInterval = interval;
		m_updateTimer.setInterval(m_refreshInterval);
		Q_EMIT refreshIntervalChanged(m_refreshInterval);
	}
}

void MgNeuralNetworkAnimator::setAnimationDuration(double ad)
{
	if(m_animationDuration != ad)
	{
		m_animationDuration = ad;
		Q_EMIT animationDurationChanged(m_animationDuration);
	}
}

void MgNeuralNetworkAnimator::setAnimationStep(double as)
{
	if(m_animationStep != as)
	{
		m_animationStep = as;
		Q_EMIT animationStepChanged(m_animationStep);
	}
}


void MgNeuralNetworkAnimator::start()
{
	m_updateTimer.setInterval(m_refreshInterval);
	m_updateTimer.start();
	Q_EMIT started();
}

void MgNeuralNetworkAnimator::pause()
{
	m_updateTimer.stop();
	Q_EMIT paused();
}

void MgNeuralNetworkAnimator::stop()
{
	m_updateTimer.stop();
	Q_EMIT stopped();
	reset();
}

void MgNeuralNetworkAnimator::reset()
{
	m_graph->reset();
	setProgressAndUpdate(0);
}
void MgNeuralNetworkAnimator::setProgress(int p)
{
	m_graph->reset();
	m_animationTime = (m_animationDuration * p )/100;
}

void MgNeuralNetworkAnimator::setProgressAndUpdate(int p)
{
	setProgress(p);
	update();
}


void MgNeuralNetworkAnimator::update()
{
	m_graph->update(m_animationTime);
	Q_EMIT progress(m_animationTime * 100 / m_animationDuration);
	m_animationTime += m_animationStep;
	if( m_animationTime > m_animationDuration)
		stop();
}
