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
#ifndef MGNEURALNETWORKGRAPH_H_
#define MGNEURALNETWORKGRAPH_H_
#include <QObject>
#include <QHash>
#include <QPair>
#include <QVector3D>
#include <QVector>

#include "mgneuron.h"
#include "mgsynapse.h"

#include <MgCore/MgMessagesEmitter>
#include <MgCore/MgLockableObject>


class MgSpikingNeuralNetwork : public QObject, public MgMessagesEmitter, public MgLockableObject
{
	Q_OBJECT

	Q_CLASSINFO("MG_SLOTS",""
			"addNeuron,"
			"removeNeuron,"
			"addSynapse,"
			"removeSynapse,"
			"addNeuronsLine,"
			"addNeuronsGrid,"
			"clear,"
			"addSpike,"
			"reset,"
			"loadGraph,"
			"loadSpikes,"
			"loadActivity,"
			"loadActivityFromDirectory,"
			"saveGraph,"
			"saveSpikes")

public:
	typedef QPair<uint,uint> SynapseId;
	typedef QPair<double,uint> Spike;

	MgSpikingNeuralNetwork(QObject * parent = 0);
	virtual ~MgSpikingNeuralNetwork();

	QVector<QVector3D> activeSpikesPositions()const{return m_activeSpikesPositions;}

	int neuronsCount()const{return m_neurons.count();}
	MgNeuron  * neuron(uint id)const;
	QHash<uint,MgNeuron*>::const_iterator neuronsConstBegin()const
	{
		return m_neurons.constBegin();
	}
	QHash<uint,MgNeuron*>::const_iterator neuronsConstEnd()const
	{
		return m_neurons.constEnd();
	}


	int synapsesCount()const{return m_synapsesById.count();}
	MgSynapse  * synapse(SynapseId)const;
	MgSynapse  * synapse(uint,uint)const;
	QList<uint> neuronsIds()const;
	QList<SynapseId> synapsesIds()const;
	bool hasSpikes()const{return !m_activeSpikesPositions.isEmpty();}
	QList<Spike> spikes()const{return m_spikes;}

public Q_SLOTS:

	bool addNeuron(uint,const QVector3D &);
	void removeNeuron(uint);
	bool addSynapse(uint,uint,double,double w);
	void removeSynapse(uint,uint);
	void addNeuronsLine(const QVector3D & from,const QVector3D & to,uint firstId,int total);
	void addNeuronsGrid(const QVector3D &,const QVector3D & ,const QVector3D &,uint firstId,int,int);

	void clear();

	void addSpike(double,uint);
	void update(double t);
	void reset();


	bool loadGraph(const QString & fileName);
	bool loadSpikes(const QString & fileName);
	bool loadActivityFromDirectory(const QString & dirName);
	bool loadActivity(uint id,const QString & fileName);
	bool saveGraph(const QString & fileName);
	bool saveSpikes(const QString & fileName);


Q_SIGNALS:

	void neuronAdded(uint);
	void neuronRemoved(uint);

	void synapseAdded(uint,uint);
	void synapseRemoved(uint,uint);

	void updateFinished(double);


private:
	void addSpike(double,uint,int,int);
	bool isSpikeAlive(const Spike &,double t);
	void resetState();

private:

	QHash<uint,MgNeuron*> m_neurons;
	QMultiHash<uint,MgSynapse*> m_synapsesBySource;
	QHash<SynapseId,MgSynapse*> m_synapsesById;
	QList<Spike> m_spikes;
	QVector<QVector3D> m_activeSpikesPositions;

};

#endif /* MGNEURALNETWORKGRAPH_H_ */
