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
#ifndef MGNEURALNETWORKANIMATOR_H_
#define MGNEURALNETWORKANIMATOR_H_

#include <QObject>
#include <QTimer>
#include <QThread>

class MgSpikingNeuralNetwork;

class MgNeuralNetworkAnimator : public QObject
{
	Q_OBJECT
	Q_PROPERTY(int refreshInterval READ refreshInterval WRITE setRefreshInterval NOTIFY refreshIntervalChanged SCRIPTABLE true)
	Q_CLASSINFO("refreshInterval_uname","Refresh interval (ms)")
	Q_CLASSINFO("refreshInterval_info","10,100,5")

	Q_PROPERTY(double animationDuration READ animationDuration WRITE setAnimationDuration NOTIFY animationDurationChanged SCRIPTABLE true)
	Q_CLASSINFO("animationDuration_uname","Animation duration")
	Q_CLASSINFO("animationDuration_info","0,100000,10,4")

	Q_PROPERTY(double animationStep READ animationStep WRITE setAnimationStep NOTIFY animationStepChanged SCRIPTABLE true)
	Q_CLASSINFO("animationStep_uname","Animation step")
	Q_CLASSINFO("animationStep_info","0,10,0.001,4")

	Q_CLASSINFO("MG_PROPERTIES",""
			"refreshInterval,"
			"animationDuration,"
			"animationStep")


	Q_CLASSINFO("MG_SLOTS",""
			"start,"
			"pause,"
			"stop,"
			"reset")

public:
	MgNeuralNetworkAnimator(MgSpikingNeuralNetwork *,QObject * =0);
	virtual ~MgNeuralNetworkAnimator();

	int refreshInterval()const;
	double animationDuration()const;
	double animationStep()const;

	void setRefreshInterval(int);
	void setAnimationDuration(double);
	void setAnimationStep(double);

public Q_SLOTS:
	void start();
	void pause();
	void stop();
	void setProgress(int);
	void setProgressAndUpdate(int);
	void reset();

Q_SIGNALS:
	void started();
	void stopped();
	void paused();
	void progress(int);

	void refreshIntervalChanged(int);
	void animationDurationChanged(double);
	void animationStepChanged(double);

protected Q_SLOTS:
	void update();

private:
	MgSpikingNeuralNetwork * m_graph;
	QTimer m_updateTimer;
	int m_refreshInterval;
	double m_animationDuration;
	double m_animationStep;
	double m_animationTime;
};

#endif /* MGNEURALNETWORKANIMATOR_H_ */
