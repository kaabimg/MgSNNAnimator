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
#include "mgneuralnetworkview.h"

#include <math.h>

#include <QHash>
#include <QPair>
#include <QTimer>

#include <Qt3D/qglbuilder.h>
#include <Qt3D/qglsphere.h>
#include <Qt3D/qglpicknode.h>
#include <Qt3D/qgltexture2d.h>
#include <Qt3D/qglframebufferobjectsurface.h>
#include <QQuaternion>

#include <Mg3D/MgLagrangeCurve>
#include <Mg3D/MgParametericLineBuilder>

#include <Mg3D/Mg3DHelpers>
#include <Mg3D/MgHBlurShader>
#include <Mg3D/MgVBlurShader>
#include <Mg3D/MgColorFilterShader>
#include <Mg3D/MgBlendShader>
#include <Mg3D/MgAntialiasingShader>
#include <Mg3D/MgBumpMappingShader>


#include <MgGui/MgCurveData>

#include "mg3dcurveplotter.h"

#include "../core/mgneuralnetworkgraph.h"
#include "../core/mgneuralnetworkio.h"

#include "../shaders/synapse/mgsynapseshader.h"
#include "../shaders/neuron/mgneuronshader.h"
#include "../shaders/filters/mgspikepassfilter.h"
#include "../shaders/neuronspikeeffect/mgneuronspikeeffectshader.h"
#include "../shaders/neuronspikeeffect/mgspikeeffectshader.h"
#include "../shaders/neuronspikeeffect/mgspikecircleshader.h"
#include "../shaders/blend/mgspikesglowblendshader.h"


#include <QKeyEvent>

/// helpers




QGeometryData createCircle(int slices,double radus)
{
	QGeometryData circle;
	int index(0);
	circle.appendVertex(QVector3D(0,0,0));
	circle.appendIndex(0);
	index++;
	qreal d_angle = 2.0 * M_PI/slices;
	qreal angle=0;
	for(double i =0;i<slices;++i)
	{
		circle.appendVertex(QVector3D(cos(angle)*radus,sin(angle)*radus,0));
		circle.appendIndex(index);
		index++;
		angle += d_angle;
	}
	circle.appendVertex(QVector3D(cos(angle)*radus,sin(angle)*radus,0));
	circle.appendIndex(index);
	return circle;
}

QGeometryData createSpikeCircle(int slices,double radus)
{
	QGeometryData circle;
	int index(0);

	qreal d_angle = 2.0 * M_PI/slices;
	qreal angle=0;
	for(double i =0;i<slices;++i)
	{
		circle.appendVertex(QVector3D(cos(angle)*radus,sin(angle)*radus,0));
		circle.appendIndex(index);
		index++;
		angle += d_angle;
	}
	return circle;
}


QQuaternion rotationFromZ(const QVector3D & vect)
{
	static const QVector3D z(0.,0.,1.);
	QVector3D cross_product = QVector3D::crossProduct(z,vect);

	QQuaternion rotation = QQuaternion::fromAxisAndAngle(cross_product,
			acos(QVector3D::dotProduct(z,vect.normalized()))*180/M_PI);
	return rotation;
}

QGeometryData rotateFaceToCamera(
		const QVector3DArray & cercle,
		const QVector3D & position,
		const QVector3D & cameraPosition)
{
	QGeometryData circleFacingCamera;
	QQuaternion rotation = rotationFromZ(position-cameraPosition);

	//center
	circleFacingCamera.appendVertex(position);
	circleFacingCamera.appendIndex(0);

	//circle vertices
	for(int i=1;i<cercle.size();++i)
	{
		circleFacingCamera.appendVertex(rotation.rotatedVector(cercle[i])+position);
		circleFacingCamera.appendIndex(i);
	}


	return circleFacingCamera;
}

///////////////////////////////////::


struct MgNeuralNetworkViewPrivate
{
	MgSpikingNeuralNetwork * graph;
	double updateTime;

	QHash<MgSpikingNeuralNetwork::SynapseId,QGLSceneNode* > synapses;
	QHash<MgCurveData*,QGeometryData*> curves;

	QGLSceneNode * neuronSphere;
	QGLSceneNode * curveNode;
	QGLSceneNode  * spikeNode;

	MgNeuronShader  neuronShader;
	MgSynapseShader synapseShader;
	MgSpikePassFilter spikePassFilter;


	MgNeuronSpikeEffectShader neuronsSpikeEffect;
	MgSpikeEffectShader spikeEffect;
	MgSpikeCircleShader spikeCircleShader;
	MgBumpMappingShader bumpMappingShader;


	MgHBlurShader hBlurShader;
	MgVBlurShader vBlurShader;
	MgBlendShader blendShader;
	MgSpikesGlowBlendShader spikesGlowBlendShader;
	MgAntialiasingShader antialiasingShader;
	MgColorFilterShader colorFilterColor;

	QColor receivedSpikeColor,emittedSpikeColor;
	QColor spikeColor;
	QColor neuronColor;
	QColor inhibitorySynapseColor,excitatorySynapseColor;

	Mg3dCurvePlotter curvesPlotter;

	double animationDuration;
	double curveLineWidth;

	MgNeuron * neuronToPlot;
	uint neuronToPlotId;
	bool drawCurve;

	QGeometryData spikeCircle,spikeEffectCircle;

	double maxLastSpikeElapedTime;
	bool drawNeuronsSpikesAnimation;

	QGLTexture2D emittedSpikeCircleTexture,receivedSpikeCircleTexture;


	double neuronRadius;
	double synapseRadius;
	double spikeRadius;



	QGLFramebufferObjectSurface * paintSurface1_1;
	QGLFramebufferObjectSurface * paintSurface2_1;
	QGLFramebufferObjectSurface * paintSurface1_2;
	QGLFramebufferObjectSurface * paintSurface2_2;
	QGLFramebufferObjectSurface * paintSurface1_4;


	int blurPassesCount;
	double blurFactor;


	MgNeuralNetworkView::DrawMode drawMode;

	QTimer differedUpdateTimer;



};


MgNeuralNetworkView::MgNeuralNetworkView(MgSpikingNeuralNetwork * graph,QWidget * parent)
	: Mg3dScene(parent)
{
	d_ptr = new MgNeuralNetworkViewPrivate;
	d_ptr->graph = graph;
	d_ptr->updateTime = 0;

	d_ptr->neuronRadius = .5;
	d_ptr->synapseRadius = .05;
	d_ptr->spikeRadius = .15;
	d_ptr->updateTime = 0;
	d_ptr->animationDuration = 0;
	d_ptr->curveLineWidth = 2;

	d_ptr->neuronToPlot = 0;
	d_ptr->neuronToPlotId = 0;


	d_ptr->blurPassesCount = 2;
	d_ptr->blurFactor = 1.2;


	QGLBuilder builder;
	builder << QGLSphere(d_ptr->neuronRadius * 2,4);
	d_ptr->neuronSphere  = builder.finalizedSceneNode();
	d_ptr->neuronSphere->setParent(this);

	QGLBuilder builder2;
	builder2 << QGLSphere(d_ptr->spikeRadius * 2,2);
	d_ptr->spikeNode = builder2.finalizedSceneNode();
	loadSynapses();

	connect(d_ptr->graph,SIGNAL(neuronAdded(uint)),this,SLOT(addNeuron(uint)));
	connect(d_ptr->graph,SIGNAL(neuronRemoved(uint)),this,SLOT(removeNeuron(uint)));
	connect(d_ptr->graph,SIGNAL(synapseAdded(uint,uint)),this,SLOT(addSynapse(uint, uint)));
	connect(d_ptr->graph,SIGNAL(synapseRemoved(uint,uint)),this,SLOT(removeSynapse(uint, uint)));

	d_ptr->drawCurve = false;

	d_ptr->spikeCircle =createCircle(30,1);
	d_ptr->maxLastSpikeElapedTime = 2;
	d_ptr->emittedSpikeCircleTexture.setImage(QImage(":/circle.png"));
	d_ptr->receivedSpikeCircleTexture.setImage(QImage(":/circle2.png"));
	d_ptr->spikeEffectCircle = createSpikeCircle(20,d_ptr->spikeRadius * 1.3);




	d_ptr->drawNeuronsSpikesAnimation = true;
	d_ptr->emittedSpikeColor = QColor(86,194,170,70);
	d_ptr->receivedSpikeColor = QColor(247,254,50,70);

	d_ptr->neuronColor = QColor(75,75,75);
	d_ptr->spikeColor = QColor(255,175,25);
	d_ptr->inhibitorySynapseColor = QColor(50,50,200);
	d_ptr->excitatorySynapseColor = QColor(200,50,50);

	d_ptr->curvesPlotter.createBorder();
	d_ptr->paintSurface1_1 = 0;
	d_ptr->paintSurface2_1 = 0;
	d_ptr->paintSurface1_2 = 0;
	d_ptr->paintSurface2_2 = 0;
	d_ptr->paintSurface1_4 = 0;


	//TODO : move it to properties
	d_ptr->curvesPlotter.setCurveColor(QColor(100,100,250));
	d_ptr->curvesPlotter.setBackgroundColor(QColor(80,160,240,130));

	d_ptr->drawCurve = NoSpikes;

	d_ptr->differedUpdateTimer.setInterval(800);
	d_ptr->differedUpdateTimer.setSingleShot(true);
	connect(&d_ptr->differedUpdateTimer,SIGNAL(timeout()),this,SLOT(updateGLContent()));
	setStateTextSize(5);
}


MgNeuralNetworkView::~MgNeuralNetworkView()
{

	delete d_ptr->paintSurface1_1;
	delete d_ptr->paintSurface2_1;

	delete d_ptr->paintSurface1_2;
	delete d_ptr->paintSurface2_2;

	delete d_ptr->paintSurface1_4;

	delete d_ptr;
}

MgSpikingNeuralNetwork * MgNeuralNetworkView::graph()const
{
	return d_ptr->graph;
}


double MgNeuralNetworkView::animationDuration()const
{
	return d_ptr->animationDuration;
}
void MgNeuralNetworkView::setAnimationDuration(double d)
{
	d_ptr->animationDuration = d;
	updateGLContent();

}

void MgNeuralNetworkView::setNeuronToPlot(int id)
{
	if(id>=0 &&d_ptr->graph->neuron(id) != d_ptr->neuronToPlot)
	{
		d_ptr->neuronToPlot =d_ptr->graph->neuron(id);
		d_ptr->neuronToPlotId = id;
		Q_EMIT neuronToPlotChanged(id);
	}
}

int MgNeuralNetworkView::neuronToPlot()const
{
	if(d_ptr->neuronToPlot)
		return d_ptr->neuronToPlotId;
	return 0;
}



void MgNeuralNetworkView::enableCurveDrawing(bool draw)
{
	if(d_ptr->drawCurve != draw)
	{
		d_ptr->drawCurve = draw;
		updateGLContent();
		Q_EMIT curveDrawEnabled(draw);
	}
}

bool MgNeuralNetworkView::isCurveDrawingEnabled()const
{
	return d_ptr->drawCurve;
}



void MgNeuralNetworkView::setCurveLineWidth(double w)
{
	if(d_ptr->curveLineWidth != w)
	{
		d_ptr->curveLineWidth = w;
		Q_EMIT curveLineWidthChanged(w);
		updateGLContent();
	}
}
double MgNeuralNetworkView::curveLineWidth()const
{
	return d_ptr->curveLineWidth;
}


void MgNeuralNetworkView::setNeuronRadius(double r)
{
	if(d_ptr->neuronRadius !=r)
	{
		d_ptr->neuronRadius = r;
		delete d_ptr->neuronSphere ;
		QGLBuilder builder;
		builder << QGLSphere(d_ptr->neuronRadius * 2,6);
		d_ptr->neuronSphere = builder.finalizedSceneNode();
		d_ptr->neuronSphere->setParent(this);
		triggerDifferedUpdate();
		Q_EMIT neuronRadiusChanged(r);
	}
}
double MgNeuralNetworkView::neuronRadius()const
{
	return d_ptr->neuronRadius;
}

double MgNeuralNetworkView::synapseRadius()const
{
	return d_ptr->synapseRadius;
}
void MgNeuralNetworkView::setSynapseRadius(double r)
{
	if(d_ptr->synapseRadius !=r)
	{

		d_ptr->synapseRadius = r;
		clearSynapses();
		loadSynapses();
		triggerDifferedUpdate();
		Q_EMIT synapseRadiusChanged(r);
	}
}

double MgNeuralNetworkView::spikeRadius()const
{
	return d_ptr->spikeRadius;
}
void MgNeuralNetworkView::setSpikeRadius(double r)
{
	if(d_ptr->spikeRadius != r)
	{
		d_ptr->spikeRadius = r;
		delete d_ptr->spikeNode ;
		QGLBuilder builder;
		builder << QGLSphere(d_ptr->spikeRadius * 2,6);
		d_ptr->spikeNode = builder.finalizedSceneNode();
		d_ptr->spikeNode->setParent(this);
		triggerDifferedUpdate();
		Q_EMIT spikeRadiusChanged(r);
	}
}


void MgNeuralNetworkView::setBlurFactor(double f)
{
	if(d_ptr->blurFactor != f)
	{
		d_ptr->blurFactor = f;
		updateGLContent();
	}
}
double MgNeuralNetworkView::blurFactor()const
{
	return d_ptr->blurFactor;
}

void MgNeuralNetworkView::setBlurPassesCount(int p)
{
	if(d_ptr->blurPassesCount != p)
	{
		d_ptr->blurPassesCount = p;
		updateGLContent();
	}
}

int MgNeuralNetworkView::blurPassesCount()const
{
	return d_ptr->blurPassesCount;
}


QColor MgNeuralNetworkView::neuronColor()const
{
	return d_ptr->neuronColor;
}

void MgNeuralNetworkView::setNeuronColor(const QColor & color)
{
	if(d_ptr->neuronColor != color)
	{
		d_ptr->neuronColor = color;
		updateGLContent();
		Q_EMIT neuronColorChanged(color);
	}
}
QColor MgNeuralNetworkView::spikeColor()const
{
	return d_ptr->spikeColor;
}
void MgNeuralNetworkView::setSpikeColor(const QColor & color)
{
	if(d_ptr->spikeColor != color)
	{
		d_ptr->spikeColor = color;
		updateGLContent();
		Q_EMIT spikeColorChanged(color);
	}
}
QColor MgNeuralNetworkView::inhibitorySynapseColor()const
{
	return d_ptr->inhibitorySynapseColor;
}
void MgNeuralNetworkView::setInhibitorySynapseColor(const QColor & color)
{
	if(d_ptr->inhibitorySynapseColor != color)
	{
		d_ptr->inhibitorySynapseColor = color;
		updateGLContent();
		Q_EMIT inhibitorySynapseColorChanged(color);
	}
}
QColor MgNeuralNetworkView::excitatorySynapseColor()const
{
	return d_ptr->excitatorySynapseColor;
}
void MgNeuralNetworkView::setExcitatorySynapseColor(const QColor & color)
{
	if(d_ptr->excitatorySynapseColor != color)
	{
		d_ptr->excitatorySynapseColor = color;
		updateGLContent();
		Q_EMIT excitatorySynapseColorChanged(color);
	}
}


//////////////////////////////////////////////////////////////////////////:



void MgNeuralNetworkView::addNeuron(uint)
{
	setProperty("neuronToPlot_info",QString("0,%0,1").arg(d_ptr->graph->neuronsCount()));
	Q_EMIT metaClassInfoChanged();
	triggerDifferedUpdate();
}

void MgNeuralNetworkView::addSynapse(uint from, uint to)
{

	MgSpikingNeuralNetwork::SynapseId id(from,to);
	if(!d_ptr->synapses.contains(id))
	{
		MgSynapse * synapse = d_ptr->graph->synapse(from,to);
		MgParametericLineBuilder builder;
		builder.setRadius(d_ptr->synapseRadius);
		builder.setAdditiveRandomRadius(d_ptr->synapseRadius/2);
		builder.setLagrangeCurve(synapse->curve());

		QGLSceneNode * synapseNode = builder.buildNode();
		d_ptr->synapses.insert(id,synapseNode);
		triggerDifferedUpdate();
	}
}


void MgNeuralNetworkView::removeNeuron(uint)
{
	setProperty("neuronToPlot_info",QString("0,%0,1").arg(d_ptr->graph->neuronsCount()));
	Q_EMIT metaClassInfoChanged();
	triggerDifferedUpdate();


}
void MgNeuralNetworkView::removeSynapse(uint from, uint to)
{
	MgSpikingNeuralNetwork::SynapseId id(from,to);
	if(d_ptr->synapses.contains(id))
		delete d_ptr->synapses.take(id);
	triggerDifferedUpdate();
}


void MgNeuralNetworkView::clearSynapses()
{
	qDeleteAll(d_ptr->synapses);
	d_ptr->synapses.clear();

}


void MgNeuralNetworkView::loadSynapses()
{
	foreach(const MgSpikingNeuralNetwork::SynapseId & id,d_ptr->graph->synapsesIds())
		addSynapse(id.first,id.second);
	triggerDifferedUpdate();
}


void MgNeuralNetworkView::triggerDifferedUpdate()
{
	d_ptr->differedUpdateTimer.stop();
	d_ptr->differedUpdateTimer.start();
}

///////////////////////////////////////////////////////////////////////////:

void MgNeuralNetworkView::updateFbosSizes()
{
	resizeFbo(d_ptr->paintSurface1_1,size());
	resizeFbo(d_ptr->paintSurface2_1,size());

	resizeFbo(d_ptr->paintSurface1_2,size()/2);
	resizeFbo(d_ptr->paintSurface2_2,size()/2);

	resizeFbo(d_ptr->paintSurface1_4,size()/4);

}


void MgNeuralNetworkView::initGL(QGLPainter * painter)
{
	Mg3dScene::initializeGL(painter);
	updateFbosSizes();
}


void MgNeuralNetworkView::update(double t)
{
	d_ptr->updateTime = t;
	updateGLContent();
}


QString vectorToString(const QVector3D & vec)
{
	return  "["+QString::number(vec.x(),'g',4)+","+
			QString::number(vec.y(),'g',4)+","+
			QString::number(vec.z(),'g',4)+
			"]";
}

void MgNeuralNetworkView::paintGL(QGLPainter * painter)
{
	d_ptr->drawMode = d_ptr->graph->hasSpikes() ? WithSpikes : NoSpikes;
	paint(painter);
	updateFps();


	setStateText(1,"Camera position : "+vectorToString(cameraPosition()));
	setStateText(2,"Camera center : "+vectorToString(cameraCenter()));
	setStateText(3,"Current animation time : "+QString::number(d_ptr->updateTime));
	setStateText(4,"Animation duration : "+QString::number(animationDuration()));
	drawStateText(painter);
}


void MgNeuralNetworkView::paint(QGLPainter * painter)
{

	if(d_ptr->drawCurve)
	{
		MG_START_SURFACE(painter,d_ptr->paintSurface1_4);
			paintCurve(painter);
		MG_END_SURFACE(painter);
	}

	if(d_ptr->drawMode == WithSpikes)
	{
		MG_START_SURFACE(painter,d_ptr->paintSurface1_1);
			glClearColor(.0f, .0f, .0f, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
			glDisable(GL_BLEND);
			drawGridAndReference(painter);
			paintNeurons(painter);
			paintSpikes(painter);
			glEnable(GL_BLEND);
			paintSynapses(painter);
			paintNeuronsSpikeEffect(painter);
			if(d_ptr->drawCurve)
				drawFboSurface(
						d_ptr->paintSurface1_4,
						QVector2D(.5,.75),
						QVector2D(.5,.25),
						painter
						);
		MG_END_SURFACE(painter);

		MG_START_SURFACE(painter,d_ptr->paintSurface1_2);
			d_ptr->colorFilterColor.setActive(painter,true);
			d_ptr->colorFilterColor.setColor(d_ptr->spikeColor);
			glClearColor(.0f, .0f, .0f, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D,d_ptr->paintSurface1_1->framebufferObject()->texture());
			drawFboVertices(painter);
			d_ptr->paintSurface1_1->framebufferObject()->release();
			d_ptr->colorFilterColor.setActive(painter,false);
		MG_END_SURFACE(painter);


		for(int i=0;i<d_ptr->blurPassesCount;++i)
		{
			MG_START_SURFACE(painter,d_ptr->paintSurface2_2);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
				d_ptr->hBlurShader.setActive(painter,true);
				d_ptr->hBlurShader.setFrameWidth(width()/(2*d_ptr->blurFactor));
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D,d_ptr->paintSurface1_2->framebufferObject()->texture());
				drawFboVertices(painter);
				d_ptr->paintSurface1_2->framebufferObject()->release();
				d_ptr->hBlurShader.setActive(painter,false);
			MG_END_SURFACE(painter);

			MG_START_SURFACE(painter,d_ptr->paintSurface1_2);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
				d_ptr->vBlurShader.setActive(painter,true);
				d_ptr->vBlurShader.setFrameHeight(height()/(2*d_ptr->blurFactor));
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D,d_ptr->paintSurface2_2->framebufferObject()->texture());
				drawFboVertices(painter);
				d_ptr->paintSurface2_2->framebufferObject()->release();
				d_ptr->vBlurShader.setActive(painter,false);
			MG_END_SURFACE(painter);
		}


		MG_START_SURFACE(painter,d_ptr->paintSurface2_1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
			d_ptr->spikesGlowBlendShader.setActive(painter,true);
			d_ptr->spikesGlowBlendShader.setBlendSize(QVector2D(
					.5/width(),
					.5/height()
					));
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D,d_ptr->paintSurface1_1->framebufferObject()->texture());
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D,d_ptr->paintSurface1_2->framebufferObject()->texture());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			drawFboVertices(painter);
			d_ptr->paintSurface1_2->framebufferObject()->release();
			d_ptr->paintSurface1_1->framebufferObject()->release();
			d_ptr->spikesGlowBlendShader.setActive(painter,false);
		MG_END_SURFACE(painter);

		drawFboSurface(d_ptr->paintSurface2_1,painter);

	}
	else
	{
		glClearColor(.0f, .0f, .0f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glDisable(GL_BLEND);
		drawGridAndReference(painter);
		paintNeurons(painter);
		paintSpikes(painter);
		glEnable(GL_BLEND);
		paintSynapses(painter);
		paintNeuronsSpikeEffect(painter);
		if(d_ptr->drawCurve)
			drawFboSurface(
					d_ptr->paintSurface1_4,
					QVector2D(.5,.75),
					QVector2D(.5,.25),
					painter
					);
	}
}

void MgNeuralNetworkView::paintNeurons(QGLPainter * painter)
{
	d_ptr->neuronShader.setActive(painter,true);
	d_ptr->neuronShader.setCameraPosition(cameraPosition());
	d_ptr->neuronShader.setNeuronColor(d_ptr->neuronColor);

	painter->projectionMatrix().push();
	painter->modelViewMatrix().push();
	QVector3D lastPosition(0,0,0);
	MgNeuron * neuron;
	QHash<uint,MgNeuron*>::const_iterator iterator = d_ptr->graph->neuronsConstBegin();
	QHash<uint,MgNeuron*>::const_iterator end = d_ptr->graph->neuronsConstEnd();

	while(iterator != end)
	{
		neuron = iterator.value();
		painter->modelViewMatrix().translate(neuron->position()-lastPosition);
		d_ptr->neuronSphere->draw(painter);
		lastPosition = neuron->position();
		++iterator;
	}

	painter->projectionMatrix().pop();
	painter->modelViewMatrix().pop();
	d_ptr->neuronShader.setActive(painter,false);
}
void MgNeuralNetworkView::paintSynapses(QGLPainter * painter)
{
	QHash<MgSpikingNeuralNetwork::SynapseId,QGLSceneNode* >::iterator
		synapses_iterator =	d_ptr->synapses.begin();

	d_ptr->synapseShader.setActive(painter,true);
	d_ptr->synapseShader.setCameraPosition(cameraPosition());
	d_ptr->synapseShader.setInhibitorySynapseColor(d_ptr->inhibitorySynapseColor);
	d_ptr->synapseShader.setExcitatorySynapseColor(d_ptr->excitatorySynapseColor);

	MgSynapse * synapse;
	while(synapses_iterator != d_ptr->synapses.end())
	{
		synapse = d_ptr->graph->synapse(synapses_iterator.key());
		d_ptr->synapseShader.setActivity(synapse->isActive() ? synapse->type() : 0);

		glCullFace(GL_FRONT);
	    glEnable(GL_CULL_FACE);
	    synapses_iterator.value()->draw(painter);
	    glCullFace(GL_BACK);
	    synapses_iterator.value()->draw(painter);
	    glDisable(GL_CULL_FACE);

		++synapses_iterator;
	}

	d_ptr->bumpMappingShader.setActive(painter,false);
}

void MgNeuralNetworkView::paintNeuronsSpikeEffect(QGLPainter * painter)
{
	for(int i=0;i<d_ptr->graph->neuronsCount();++i)
		paintNeuronSpikeEffect(d_ptr->graph->neuron(i),painter);

}
void MgNeuralNetworkView::paintSpikes(QGLPainter * painter)
{
	QVector<QVector3D> spikes = d_ptr->graph->activeSpikesPositions();
	d_ptr->spikeEffect.setActive(painter,true);
	d_ptr->spikeEffect.setCameraPosition(cameraPosition());
	d_ptr->spikeEffect.setSpikeColor(d_ptr->spikeColor);
	for (int i = 0; i < spikes.size(); ++i)
	{
		d_ptr->spikeEffect.setPosition(spikes[i]);
		d_ptr->spikeNode->draw(painter);
	}
	d_ptr->spikeEffect.setActive(painter,false);
}
void MgNeuralNetworkView::paintCurve(QGLPainter * painter)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	d_ptr->curvesPlotter.drawBorder(
			QVector2D(0,0),
			QVector2D(1,1),
			painter);
	d_ptr->neuronToPlot = d_ptr->graph->neuron(d_ptr->neuronToPlotId);
	if(d_ptr->neuronToPlot != NULL)
		paintCurve(d_ptr->neuronToPlot->activityData(),painter);
}


void MgNeuralNetworkView::paintNeuronSpikeEffect(const MgNeuron * neuron,QGLPainter * painter)
{
	if(	d_ptr->drawNeuronsSpikesAnimation &&
		neuron->lastEmittedSpike() != MgNeuron::NO_SPIKE &&
		d_ptr->updateTime - neuron->lastEmittedSpike() <= d_ptr->maxLastSpikeElapedTime)
	{
		d_ptr->neuronsSpikeEffect.setActive(painter,true);
		QQuaternion rotation = rotationFromZ(cameraPosition()-neuron->position());
		d_ptr->neuronsSpikeEffect.setRotation(rotation.toVector4D());
		d_ptr->neuronsSpikeEffect.setNeuronPosition(neuron->position());
		d_ptr->neuronsSpikeEffect.setAmplification(
				d_ptr->neuronRadius * 1.5 * (2 - (d_ptr->updateTime - neuron->lastEmittedSpike())/d_ptr->maxLastSpikeElapedTime));

		d_ptr->neuronsSpikeEffect.setColor(d_ptr->emittedSpikeColor);
		glActiveTexture(GL_TEXTURE0);
		d_ptr->emittedSpikeCircleTexture.bind();
		d_ptr->spikeCircle.draw(painter,0,d_ptr->spikeCircle.indexCount(),QGL::TriangleFan);
		d_ptr->emittedSpikeCircleTexture.release();
		d_ptr->neuronsSpikeEffect.setActive(painter,false);
	}

	if(	d_ptr->drawNeuronsSpikesAnimation &&
		neuron->lastReceivedSpike() != MgNeuron::NO_SPIKE &&
		neuron->lastReceivedSpike() <= d_ptr->updateTime &&
		d_ptr->updateTime - neuron->lastReceivedSpike() <= d_ptr->maxLastSpikeElapedTime )
	{
		d_ptr->neuronsSpikeEffect.setActive(painter,true);
		QQuaternion rotation = rotationFromZ(cameraPosition()-neuron->position());
		d_ptr->neuronsSpikeEffect.setRotation(rotation.toVector4D());
		d_ptr->neuronsSpikeEffect.setNeuronPosition(neuron->position());
		d_ptr->neuronsSpikeEffect.setAmplification(
				d_ptr->neuronRadius * 2 *
				(2 - (d_ptr->updateTime - neuron->lastReceivedSpike())/d_ptr->maxLastSpikeElapedTime));
		d_ptr->neuronsSpikeEffect.setColor(d_ptr->receivedSpikeColor);
		glActiveTexture(GL_TEXTURE0);
		d_ptr->receivedSpikeCircleTexture.bind();
		d_ptr->spikeCircle.draw(painter,0,d_ptr->spikeCircle.indexCount(),QGL::TriangleFan);
		d_ptr->receivedSpikeCircleTexture.release();
		d_ptr->neuronsSpikeEffect.setActive(painter,false);
	}
}


void MgNeuralNetworkView::paintCurve(MgCurveData * data,QGLPainter * painter)
{
	QGeometryData * curve;
	if(!d_ptr->curves.contains(data))
	{
		curve = new QGeometryData(MgNeuralNetworkIO::arrayTo3dData(data));
		curve->upload();
		d_ptr->curves.insert(data,curve);
	}
	else
	{
		curve = d_ptr->curves[data];
		if(curve->indexCount()!=data->size())//re upload the data
		{
			d_ptr->curves.remove(data);
			delete curve;
			curve = new QGeometryData(MgNeuralNetworkIO::arrayTo3dData(data));
			curve->upload();
			d_ptr->curves.insert(data,curve);
		}
	}

	int index = data->firstIndexAfter(d_ptr->updateTime);
	if(index <1)
		return;

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH,GL_LINE_SMOOTH_HINT);
	glDisable(GL_DEPTH_TEST);
	double yOffset = (data->maxY()-data->minY())/30;

	d_ptr->curvesPlotter.drawCurve(
			*curve,
			QVector2D(0,0),
			QVector2D(1,1),
			QVector2D(data->minX(),d_ptr->animationDuration),
			QVector2D(data->minY()-yOffset,data->maxY()+yOffset),
			index,
			d_ptr->curveLineWidth,
			painter);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_MULTISAMPLE);

}


///////////////////////////////////////////////////////////////////////////

void MgNeuralNetworkView::keyPressEvent(QKeyEvent* event)
{
	Mg3dScene::keyPressEvent(event);
	if(event->key() == Qt::Key_C && event->modifiers().testFlag(Qt::ControlModifier))
		enableCurveDrawing(!isCurveDrawingEnabled());
}
void MgNeuralNetworkView::resizeEvent(QResizeEvent* e)
{
	Mg3dScene::resizeEvent(e);
	updateFbosSizes();
}
void MgNeuralNetworkView::mousePressEvent(QMouseEvent * event)
{
	if(event->button() == Qt::RightButton)
	{
		qDebug()<<event->pos()<<mapPoint(event->pos());

	}

	Mg3dScene::mousePressEvent(event);

}


