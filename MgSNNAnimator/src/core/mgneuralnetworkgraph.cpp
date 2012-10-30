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
#include "mgneuralnetworkgraph.h"
#include "mgneuralnetworkio.h"

#include <QDebug>

MgSpikingNeuralNetwork::MgSpikingNeuralNetwork(QObject * parent ):QObject(parent)
{
	setObjectName("Neural network graph");
}

MgSpikingNeuralNetwork::~MgSpikingNeuralNetwork()
{
	qDeleteAll(m_neurons);
	qDeleteAll(m_synapsesById);
}


MgNeuron  * MgSpikingNeuralNetwork::neuron(uint id)const
{
	return m_neurons.value(id);
}
MgSynapse  * MgSpikingNeuralNetwork::synapse(uint from,uint to)const
{
	return m_synapsesById.value(SynapseId(from,to));
}
MgSynapse  * MgSpikingNeuralNetwork::synapse(SynapseId id)const
{
	return m_synapsesById.value(id);
}


QList<uint> MgSpikingNeuralNetwork::neuronsIds()const
{
	return m_neurons.keys();
}
QList<MgSpikingNeuralNetwork::SynapseId> MgSpikingNeuralNetwork::synapsesIds()const
{
	return m_synapsesById.keys();
}

bool MgSpikingNeuralNetwork::addNeuron(uint id,const QVector3D & position)
{
	if(!m_neurons.contains(id))
	{
		MgNeuron * neuron =new MgNeuron(id);
		neuron->setPosition(position);
		m_neurons.insert(id,neuron);
		Q_EMIT neuronAdded(id);
		return true;

	}
	else
	{
		errorMessage(QString("Can't add neuron with id %0, identifier already exists").arg(id));
	}
	return false;
}

void MgSpikingNeuralNetwork::removeNeuron(uint id)
{
	if(m_neurons.contains(id))
	{
		delete m_neurons.take(id);
		Q_EMIT neuronRemoved(id);
		QList<MgSynapse*> synapses = m_synapsesBySource.values(id);

	}
	else
		errorMessage(QString("Can't remove neuron with id %0, identifier not found").arg(id));
}

void MgSpikingNeuralNetwork::removeSynapse(uint from ,uint to)
{
	SynapseId id(from,to);
	MgSynapse * synapse = m_synapsesById.take(id);
	if(synapse)
	{

		m_synapsesBySource.remove(from,synapse);
		delete synapse;
		Q_EMIT synapseRemoved(from,to);
	}
}

bool MgSpikingNeuralNetwork::addSynapse(uint from,uint to,double deley,double w)
{
	if(!m_neurons.contains(from))
	{
		errorMessage(QString("Can't add synapse from neuron %0, identifier doesn't exist")
				.arg(from));
		return false;
	}
	if(!m_neurons.contains(to))
	{
		errorMessage(QString("Can't add synapse to neuron %0, identifier doesn't exist")
				.arg(to));
		return false;
	}

	MgSynapse * synapse =  new MgSynapse(
			m_neurons[from],
			m_neurons[to],
			deley,
			w > 0 ? MgSynapse::Excitatory : MgSynapse::Inhibitory );
	m_synapsesBySource.insert(from,synapse);
	m_synapsesById.insert(SynapseId(from,to),synapse);
	Q_EMIT synapseAdded(from,to);
	return true;
}


void MgSpikingNeuralNetwork::addNeuronsLine(
		const QVector3D & from,
		const QVector3D & to,
		uint firstId,int total)
{
	QVector3D unit((to-from)/total);
	QVector3D position(from);
	for (int i = 0; i < total; ++i)
	{
		addNeuron(firstId+i,position);
		position += unit;
	}
}


void MgSpikingNeuralNetwork::addNeuronsGrid(
		const QVector3D & base,
		const QVector3D & hExtremity,
		const QVector3D & vExtremity,
		uint firstId,
		int neuronsPerH,
		int neuronsPerV )
{
	QVector3D unit((vExtremity-base)/neuronsPerV);
	QVector3D from(base);
	QVector3D to(hExtremity);
	uint lineFirstId(firstId);


	for (int i = 0; i < neuronsPerV; ++i)
	{
		addNeuronsLine(from,to,lineFirstId,neuronsPerH);
		from += unit;
		to += unit;
		lineFirstId += neuronsPerH;
	}
}


void MgSpikingNeuralNetwork::clear()
{
	foreach(uint id, m_neurons.keys())
		removeNeuron(id);
	foreach(const SynapseId & id,m_synapsesById.keys())
		removeSynapse(id.first,id.second);
	m_synapsesBySource.clear();
	m_spikes.clear();
	m_activeSpikesPositions.clear();
}

void MgSpikingNeuralNetwork::addSpike(double time,uint source)
{

	if(m_spikes.isEmpty())
	{
		m_spikes.append(Spike(time,source));
		return;
	}

	if(m_spikes.size() == 1)
	{
		if(m_spikes.first().first<time)
		{
			m_spikes.append(Spike(time,source));
			return;
		}
		else
		{
			m_spikes.prepend(Spike(time,source));
			return;
		}
	}

	addSpike(time,source,0,m_spikes.size());
}

void MgSpikingNeuralNetwork::addSpike(double t,uint source ,int begin,int end)
{
	if(begin + 1 == end)
	{
		m_spikes.insert(begin,Spike(t,source));
		return;
	}
	int middle((begin+end)/2);
	if(t<m_spikes[middle].first)
		addSpike(t,source,begin,middle);
	else
		addSpike(t,source,middle,end);
}


bool MgSpikingNeuralNetwork::isSpikeAlive(const Spike & spike,double t)
{
	QList<MgSynapse *> synapses = m_synapsesBySource.values(spike.second);
	if(synapses.isEmpty())
		return false;
	foreach(const MgSynapse * synapse,synapses)
	{
		if(!synapse->isSpikeAlive(spike.first,t))
			return false;
	}
	return true;
}

void MgSpikingNeuralNetwork::update(double t)
{
	resetState();
	QHash<uint,MgNeuron*>::iterator i;
	for (i = m_neurons.begin(); i != m_neurons.end(); ++i)
		i.value()->updateActivity(t);

	QList<MgSynapse*> synapses;

	for (int spike = 0; spike < m_spikes.size(); ++spike)
	{
		if(m_spikes[spike].first>t)
			break;
		synapses = m_synapsesBySource.values(m_spikes[spike].second);
		for (int synapse = 0; synapse < synapses.size(); ++synapse)
		{
			if(synapses[synapse]->isSpikeAlive(m_spikes[spike].first,t))
			{
				synapses[synapse]->setActive(true);
				m_activeSpikesPositions.append(synapses[synapse]->spikePosition(m_spikes[spike].first,t));
				m_neurons[m_spikes[spike].second]->setLastEmittedSpike(m_spikes[spike].first);

			}
			else
				m_neurons[synapses[synapse]->to()->id()]->setLastReceivedSpike(
					m_spikes[spike].first + synapses[synapse]->deley());
		}
	}
	Q_EMIT updateFinished(t);
}
void  MgSpikingNeuralNetwork::resetState()
{
	QHash<uint,MgSynapse*>::iterator i;
	for (i = m_synapsesBySource.begin(); i != m_synapsesBySource.end(); ++i)
		i.value()->setActive(false);

	m_activeSpikesPositions.clear();
}

void MgSpikingNeuralNetwork::reset()
{
	m_activeSpikesPositions.clear();
	QHash<uint,MgNeuron*>::iterator ni;
	for (ni = m_neurons.begin(); ni != m_neurons.end(); ++ni)
		ni.value()->reset();

	QHash<uint,MgSynapse*>::iterator si;
	for (si = m_synapsesBySource.begin(); si != m_synapsesBySource.end(); ++si)
		si.value()->reset();
}


bool MgSpikingNeuralNetwork::loadGraph(const QString & fileName)
{
	MgNeuralNetworkIO ioMgr;
	if(!ioMgr.loadGraph(this,fileName))
	{
		errorMessage(ioMgr.errorString());
		return false;
	}
	return true;
}
bool MgSpikingNeuralNetwork::loadSpikes(const QString & fileName)
{
	MgNeuralNetworkIO ioMgr;
	if(!ioMgr.loadSpikes(this,fileName))
	{
		errorMessage(ioMgr.errorString());
		return false;
	}
	return true;

}

bool MgSpikingNeuralNetwork::loadActivityFromDirectory(const QString & dirName)
{
	MgNeuralNetworkIO ioMgr;
	if(!ioMgr.loadActivityFromDirectory(this,dirName))
	{
		errorMessage(ioMgr.errorString());
		return false;
	}
	return true;

}

bool MgSpikingNeuralNetwork::loadActivity(uint id,const QString & fileName)
{
	MgNeuron * n = neuron(id);
	if(n == 0)
	{
		errorMessage(QString("Can't load activity of neuron %0 : neuron not found").arg(id));
		return false;
	}
	MgNeuralNetworkIO ioMgr;
	if(!ioMgr.loadActivity(n->activityData(),fileName))
	{
		errorMessage(ioMgr.errorString());
		return false;
	}
	return true;
}


bool MgSpikingNeuralNetwork::saveGraph(const QString & fileName)
{
	MgNeuralNetworkIO ioMgr;
	if(!ioMgr.saveGraph(this,fileName))
	{
		errorMessage(ioMgr.errorString());
		return false;
	}
	return true;
}
bool MgSpikingNeuralNetwork::saveSpikes(const QString & fileName)
{
	MgNeuralNetworkIO ioMgr;
	if(!ioMgr.saveSpikes(this,fileName))
	{
		errorMessage(ioMgr.errorString());
		return false;
	}
	return true;
}
