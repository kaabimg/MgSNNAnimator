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
#ifndef MGNEURALNETWORKIO_H_
#define MGNEURALNETWORKIO_H_

#include <QString>
#include <Qt3D/qgeometrydata.h>

class MgSpikingNeuralNetwork;
class MgCurveData;
class MgNeuralNetworkIO
{
public:
	MgNeuralNetworkIO(){}
	~MgNeuralNetworkIO(){}
	bool loadGraph(MgSpikingNeuralNetwork * ,const QString & fileName);
	bool saveGraph(const MgSpikingNeuralNetwork * ,const QString & fileName);
	bool loadSpikes(MgSpikingNeuralNetwork * ,const QString & fileName);
	bool saveSpikes(const MgSpikingNeuralNetwork * ,const QString & fileName);
	bool loadActivity(MgCurveData * activityArray,const QString & fileName);
	bool loadActivityFromDirectory(MgSpikingNeuralNetwork * ,const QString & dirName);

	static QGeometryData arrayTo3dData(const MgCurveData * activityArray);

	QString errorString()const
	{
		return m_errorString;
	}

private:
	QString m_errorString;

};

#endif /* MGNEURALNETWORKIO_H_ */
