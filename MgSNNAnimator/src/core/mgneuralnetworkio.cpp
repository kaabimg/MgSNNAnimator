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
#include "mgneuralnetworkio.h"
#include "mgneuralnetworkgraph.h"
#include "mgneuron.h"
#include "mgsynapse.h"

#include <QFile>
#include <QDir>
#include <QStringList>
#include <QTextStream>

#include <MgGui/MgCurveData>

#define CLOSE_FILE_AND_EXIT_FALSE file.close();return false

bool MgNeuralNetworkIO::loadGraph(MgSpikingNeuralNetwork * graph,const QString & fileName)
{
	m_errorString.clear();
	QFile file(fileName);
	if(!file.exists())
	{
		m_errorString = QString("Error when loading graph from %0 : file not found").arg(fileName);
		return false;
	}
	if(!file.open(QFile::ReadOnly | QFile::Text))
	{
		m_errorString = QString("Error when loading activity from %0 : can't open file").arg(fileName);
		return false;
	}

	QStringList data;
	int line(0);
	QString lineStr;
	while( ! file.atEnd())
	{
		++line;
		lineStr = file.readLine();
		lineStr = lineStr.simplified();
		if(lineStr.isEmpty())
			continue;
		data = lineStr.split(' ');

		if(data.size()!=5)
		{
			m_errorString = QString("Error when loading graph from %0 : invalid line %1").arg(fileName).arg(line);
			CLOSE_FILE_AND_EXIT_FALSE;
		}

		if(data.first() == "neuron")
		{
			uint id;
			double x,y,z;
			bool ok;
			id = data[1].toUInt(&ok);
			if(!ok)
			{
				m_errorString = QString("Error when loading neuron id from %0 : invalid line %1").arg(fileName).arg(line);
				CLOSE_FILE_AND_EXIT_FALSE;
			}
			x = data[2].toDouble(&ok);
			if(!ok)
			{
				m_errorString = QString("Error when loading neuron x position from %0 : invalid line %1").arg(fileName).arg(line);
				CLOSE_FILE_AND_EXIT_FALSE;
			}
			y = data[3].toDouble(&ok);
			if(!ok)
			{
				m_errorString = QString("Error when loading neuron y position from %0 : invalid line %1").arg(fileName).arg(line);
				CLOSE_FILE_AND_EXIT_FALSE;
			}
			z = data[4].toDouble(&ok);
			if(!ok)
			{
				m_errorString = QString("Error when loading neuron z position from %0 : invalid line %1").arg(fileName).arg(line);
				CLOSE_FILE_AND_EXIT_FALSE;
			}
			graph->addNeuron(id,QVector3D(x,y,z));

		}
		else if(data.first() == "synapse")
		{

			uint from,to;
			double deley;
			bool ok;
			double w;
			from = data[1].toUInt(&ok);
			if(!ok)
			{
				m_errorString = QString("Error when loading synapse from %0 : invalid line %1").arg(fileName).arg(line);
				CLOSE_FILE_AND_EXIT_FALSE;
			}
			to = data[2].toUInt(&ok);
			if(!ok)
			{
				m_errorString = QString("Error when loading synapse from %0 : invalid line %1").arg(fileName).arg(line);
				CLOSE_FILE_AND_EXIT_FALSE;
			}
			deley = data[3].toDouble(&ok);
			if(!ok)
			{
				m_errorString = QString("Error when loading synapse from %0 : invalid line %1").arg(fileName).arg(line);
				CLOSE_FILE_AND_EXIT_FALSE;
			}
			w = data[4].toDouble(&ok);
			if(!ok)
			{
				m_errorString = QString("Error when loading synapse from %0 : invalid line %1").arg(fileName).arg(line);
				CLOSE_FILE_AND_EXIT_FALSE;
			}

			if(w>0)
				graph->addSynapse(from,to,deley,MgSynapse::Excitatory);
			else
				graph->addSynapse(from,to,deley,MgSynapse::Inhibitory);
		}
	}

	file.close();
	return true;
}


bool MgNeuralNetworkIO::loadSpikes(MgSpikingNeuralNetwork * graph,const QString & fileName)
{
	m_errorString.clear();
	QFile file(fileName);
	if(!file.exists())
	{
		m_errorString = QString("Error when loading spikes from %0 : file not found").arg(fileName);
		return false;
	}
	if(!file.open(QFile::ReadOnly | QFile::Text))
	{
		m_errorString = QString("Error when loading spikes from %0 : can't open file").arg(fileName);
		return false;
	}

	QString lineStr;
	QStringList data;
	int line(0);

	while( !file.atEnd())
	{
		++line;
		lineStr = file.readLine();
		lineStr = lineStr.simplified();
		if(lineStr.isEmpty())
			continue;

		data = lineStr.split(' ');
		if(data.size()!= 2)
		{
			m_errorString = QString("Error when loading spikes from %0 : invalid line %1 : line format error").arg(fileName).arg(line);
			CLOSE_FILE_AND_EXIT_FALSE;
		}

		bool ok;
		double time = data.first().toDouble(&ok);
		if(!ok)
		{
			m_errorString = QString("Error when loading spikes from %0 : invalid line %1: time data format error").arg(fileName).arg(line);
			CLOSE_FILE_AND_EXIT_FALSE;
		}


		uint id = data.last().toUInt(&ok);
		if(!ok)
		{
			m_errorString = QString("Error when loading spikes from %0 : invalid line %1 : id data format error").arg(fileName).arg(line);
			CLOSE_FILE_AND_EXIT_FALSE;
		}


		graph->addSpike(time,id);

	}

	file.close();
	return true;
}



bool MgNeuralNetworkIO::loadActivity(MgCurveData * activityArray,const QString & fileName)
{
	m_errorString.clear();
	QFile file(fileName);
	if(!file.exists())
	{
		m_errorString = QString("Error when loading activity from %0 : file not found").arg(fileName);
			return false;
	}
	if(!file.open(QFile::ReadOnly | QFile::Text))
	{
		m_errorString = QString("Error when loading activity from %0 : can't open file").arg(fileName);
		return false;
	}

	QString lineStr;
	QStringList data;
	int line(0);
	qreal x,y;
	activityArray->clear();
	while(!file.atEnd())
	{
		++line;
		lineStr = file.readLine();
		lineStr = lineStr.simplified();
		if(lineStr.isEmpty())
			continue;

		data = lineStr.split(' ');
		if(data.size()!= 2)
		{
			m_errorString = QString("Error when loading activity from %0 : invalid line %1").arg(fileName).arg(line);
			CLOSE_FILE_AND_EXIT_FALSE;
		}

		bool ok;
		x  = data.first().toDouble(&ok);
		if(!ok)
		{
			m_errorString = QString("Error when loading activity from %0 : invalid line %1").arg(fileName).arg(line);
			CLOSE_FILE_AND_EXIT_FALSE;
		}
		y  = data.last().toDouble(&ok);
		if(!ok)
		{
			m_errorString = QString("Error when loading activity from %0 : invalid line %1").arg(fileName).arg(line);
			CLOSE_FILE_AND_EXIT_FALSE;
		}

		activityArray->addPoint(x,y);
	}

	file.close();
	return true;

}

bool MgNeuralNetworkIO::loadActivityFromDirectory(
		MgSpikingNeuralNetwork * graph,
		const QString & dirName)
{
	QDir dir(dirName);
	if(!dir.exists())
	{
		m_errorString = QString("Error when loading activity from %0 : directory not found").arg(dirName);
		return false;
	}
	bool ok = true;
	QFileInfo info;
	QString errors;
	foreach(const uint id, graph->neuronsIds())
	{
		bool localOk;
		info.setFile(dirName+"/"+QString("neuron_%1").arg(id));
		if(info.exists())
		{
			localOk = loadActivity(
				graph->neuron(id)->activityData(),
				info.filePath()
			);

			if(!localOk)
			{
				errors += errorString() + "\n";
				ok = false;
			}
		}
	}
	if(!ok)
		m_errorString = errors;
	return ok;
}


bool MgNeuralNetworkIO::saveGraph(const MgSpikingNeuralNetwork * graph,const QString & fileName)
{
	QFile file(fileName);
	if(!file.open(QFile::WriteOnly | QFile::Text))
	{
		m_errorString = QString("Error when saving graph in %0 : can't open file").arg(fileName);
		return false;
	}

	QTextStream dataStream(&file);

	foreach(uint id,graph->neuronsIds())
	{
		MgNeuron * neur = graph->neuron(id);
		dataStream<<"neuron "<<QString::number(neur->id())<<" "
				<<QString::number(neur->position().x())<<" "
				<<QString::number(neur->position().y())<<" "
				<<QString::number(neur->position().z())<<"\n";
	}
	foreach(const MgSpikingNeuralNetwork::SynapseId & id,graph->synapsesIds())
	{
		MgSynapse * synapse =  graph->synapse(id);
		dataStream<<"synapse "<<QString::number(synapse->from()->id())<<" "
				<<QString::number(synapse->to()->id())<<" "
				<<QString::number(synapse->deley())<<" "
				<<QString::number(synapse->type())<<"\n";

	}

	file.close();
	return true;

}


bool MgNeuralNetworkIO::saveSpikes(const MgSpikingNeuralNetwork * graph ,const QString & fileName)
{
	QFile file(fileName);
	if(!file.open(QFile::WriteOnly | QFile::Text))
	{
		m_errorString = QString("Error when saving spikes in %0 : can't open file").arg(fileName);
		return false;
	}

	QTextStream dataStream(&file);

	foreach(MgSpikingNeuralNetwork::Spike spike,graph->spikes())
		dataStream<<QString::number(spike.first)<<" "<<QString::number(spike.second)<<"\n";

	file.close();
	return true;
}


QGeometryData MgNeuralNetworkIO::arrayTo3dData(const MgCurveData * activityArray)
{
	QGeometryData data;
	for (int index = 0; index < activityArray->size(); ++index)
	{
		data.appendVertex(QVector3D(
				activityArray->at(index).x(),
				activityArray->at(index).y(),
				0));
		data.appendIndex(index);
	}
	return data;
}
