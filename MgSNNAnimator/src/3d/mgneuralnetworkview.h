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
#ifndef MGNEURALNETWORKVIEW_H_
#define MGNEURALNETWORKVIEW_H_

#include <MgCore/MgGlobal>
#include <Mg3D/Mg3dScene>


class MgNeuralNetworkAnimator;
class MgSpikingNeuralNetwork;

class MgNeuron;
class MgSynapse;

class QGLPainter;
class QGLSceneNode;
class MgCurveData;


MG_USE_PRIVATE_CLASS(MgNeuralNetworkView)

class MgNeuralNetworkView : public Mg3dScene
{
	Q_OBJECT

	Q_PROPERTY(bool drawCurve READ isCurveDrawingEnabled WRITE enableCurveDrawing NOTIFY curveDrawEnabled)
	Q_CLASSINFO("drawCurve_uname","Draw neurons activity")

	Q_PROPERTY(double curveLineWidth READ curveLineWidth WRITE setCurveLineWidth NOTIFY curveLineWidthChanged)
	Q_CLASSINFO("curveLineWidth_uname","Curve line width")
	Q_CLASSINFO("curveLineWidth_info","0,10,0.1,4")

	Q_PROPERTY(int neuronToPlot READ neuronToPlot WRITE setNeuronToPlot NOTIFY neuronToPlotChanged)
	Q_CLASSINFO("neuronToPlot_uname","Draw activity of neuron")
	Q_CLASSINFO("neuronToPlot_info","0,0,0")//just for initialization, will be changed dynamically through neuronToPlot_info property


	Q_PROPERTY(double neuronRadius READ neuronRadius WRITE setNeuronRadius NOTIFY neuronRadiusChanged)
	Q_CLASSINFO("neuronRadius_uname","Neuron radius")
	Q_CLASSINFO("neuronRadius_info","0,10,0.001,4")

	Q_PROPERTY(double synapseRadius READ synapseRadius WRITE setSynapseRadius NOTIFY synapseRadiusChanged)
	Q_CLASSINFO("synapseRadius_uname","Synapse radius")
	Q_CLASSINFO("synapseRadius_info","0,10,0.001,4")

	Q_PROPERTY(double spikeRadius READ spikeRadius WRITE setSpikeRadius NOTIFY spikeRadiusChanged)
	Q_CLASSINFO("spikeRadius_uname","Spike radius")
	Q_CLASSINFO("spikeRadius_info","0,2,0.001,4")


	Q_PROPERTY(int blurPassesCount READ blurPassesCount WRITE setBlurPassesCount)
	Q_PROPERTY(double blurFactor READ blurFactor WRITE setBlurFactor)


	Q_PROPERTY(QColor neuronColor READ neuronColor WRITE setNeuronColor NOTIFY neuronColorChanged)
	Q_CLASSINFO("neuronColor_uname","Neuron color")

	Q_PROPERTY(QColor spikeColor READ spikeColor WRITE setSpikeColor NOTIFY spikeColorChanged)
	Q_CLASSINFO("spikeColor_uname","Spike color")

	Q_PROPERTY(QColor inhibitorySynapseColor READ inhibitorySynapseColor WRITE setInhibitorySynapseColor NOTIFY inhibitorySynapseColorChanged)
	Q_CLASSINFO("inhibitorySynapseColor_uname","Inhibitory synapse color")

	Q_PROPERTY(QColor excitatorySynapseColor READ excitatorySynapseColor WRITE setExcitatorySynapseColor NOTIFY excitatorySynapseColorChanged)
	Q_CLASSINFO("excitatorySynapseColor_uname","Excitatory synapse color")

	Q_CLASSINFO("MG_PROPERTIES",
			"drawCurve,"
			"neuronToPlot,"
			"neuronRadius,"
			"synapseRadius,"
			"spikeRadius,"
			"neuronColor,"
			"inhibitorySynapseColor,"
			"excitatorySynapseColor,"
			"spikeColor,"
			"showReference,"
			"showGrid,"
			"cameraPosition,"
			"cameraCenter,"
			"cameraUpVector")

public:
	MgNeuralNetworkView(MgSpikingNeuralNetwork * graph,QWidget * parent=0);
	virtual ~MgNeuralNetworkView();

	MgSpikingNeuralNetwork * graph()const;

	double animationDuration()const;

	void enableCurveDrawing(bool);
	bool isCurveDrawingEnabled()const;

	void setCurveLineWidth(double);
	double curveLineWidth()const;

	int neuronToPlot()const;
	void setNeuronToPlot(int);

	void setNeuronRadius(double);
	double neuronRadius()const;

	double synapseRadius()const;
	void setSynapseRadius(double);

	double spikeRadius()const;
	void setSpikeRadius(double);

	void setBlurFactor(double);
	double blurFactor()const;

	void setBlurPassesCount(int);
	int blurPassesCount()const;

	QColor neuronColor()const;
	void setNeuronColor(const QColor &);
	QColor spikeColor()const;
	void setSpikeColor(const QColor &);
	QColor inhibitorySynapseColor()const;
	void setInhibitorySynapseColor(const QColor &);
	QColor excitatorySynapseColor()const;
	void setExcitatorySynapseColor(const QColor &);


public Q_SLOTS:

	void update(double);
	void setAnimationDuration(double);

Q_SIGNALS:

	void metaClassInfoChanged();
	void curveDrawEnabled(bool);
	void curveLineWidthChanged(double);
	void neuronToPlotChanged(int);
	void neuronRadiusChanged(double);
	void synapseRadiusChanged(double);
	void spikeRadiusChanged(double);
	void neuronColorChanged(const QColor &);
	void spikeColorChanged(const QColor &);
	void inhibitorySynapseColorChanged(const QColor &);
	void excitatorySynapseColorChanged(const QColor &);



protected Q_SLOTS:
	void addNeuron(uint id);
	void addSynapse(uint, uint);

	void removeNeuron(uint id);
	void removeSynapse(uint, uint);


protected:
	enum DrawMode{NoSpikes,WithSpikes};
	void loadSynapses();
	void clearSynapses();
	void triggerDifferedUpdate();

	void mousePressEvent(QMouseEvent *);

protected:
	void paint(QGLPainter * painter);
	void paintGL(QGLPainter * );
	void initGL(QGLPainter * );

	void paintNeurons(QGLPainter *);
	void paintSynapses(QGLPainter *);
	void paintNeuronsSpikeEffect(QGLPainter *);
	void paintSpikes(QGLPainter *);
	void paintCurve(QGLPainter *);


	void paintNeuronSpikeEffect(const MgNeuron *,QGLPainter *);
	void paintCurve(MgCurveData *,QGLPainter * );
	void keyPressEvent(QKeyEvent*);
	void resizeEvent(QResizeEvent* );
	void updateFbosSizes();

private:
	MG_PRIVATE_CLASS(MgNeuralNetworkView)
	Q_DISABLE_COPY(MgNeuralNetworkView)
};

#endif /* MGNEURALNETWORKVIEW_H_ */
