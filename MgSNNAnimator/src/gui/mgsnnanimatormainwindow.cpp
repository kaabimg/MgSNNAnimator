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
#include "mgsnnanimatormainwindow.h"
#include "mgvideoexamplewidget.h"
#include "../3d/mgneuralnetworkview.h"
#include "../core/mgneuralnetworkanimator.h"
#include "../core/mgneuralnetworkgraph.h"
#include "../core/mgneuralnetworkio.h"
#include "../core/mgneuron.h"
#include "../core/mgsynapse.h"
#include "mgsnnanimatorhelpwidget.h"
#include "mganimatortimelinewidget.h"

#include <MgScript/MgScriptEngine>
#include <MgScript/MgScriptWidget>
#include <MgScript/MgCommandLine>
#include <MgScript/MgScriptEngineObjectsInfoWidget>

#include <MgGui/MgMessagesAreaWidget>
#include <MgGui/MgCurveData>
#include <MgGui/MgMultiObjectsPropertiesEditor>

#include <Mg3D/MgCameraAnimation>

#include <QStyle>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QDebug>
#include <QWebView>
#include <math.h>
struct MgSNNAnimatorMainWindowPrivate
{
	MgNeuralNetworkView * view;
	MgAnimatorTimeLineWidget * animatorWidget;
	MgNeuralNetworkAnimator * animator;
	MgSpikingNeuralNetwork * graph;
	MgSNNAnimatorHelpWidget * helpWidget;
	MgScriptEngine * scriptEngine;
	MgScriptWidget * scriptWidget;
	MgMessagesAreaWidget * messagesAreaWidget;
	MgMultiObjectsPropertiesEditor * propertiesEditor;
	MgVideoExampleWidget * videoExampleWidget;
	MgCameraAnimation cameraAnimation;
};


MgSNNAnimatorMainWindow::MgSNNAnimatorMainWindow(
		MgScriptEngine * scriptEngine,
		MgSpikingNeuralNetwork * graph)
{
	resize(800,600);

	d_ptr = new MgSNNAnimatorMainWindowPrivate;
	d_ptr->graph = graph;
	d_ptr->animator = new MgNeuralNetworkAnimator(d_ptr->graph);
	d_ptr->animator->setObjectName("animator");
	d_ptr->helpWidget = new MgSNNAnimatorHelpWidget;
	d_ptr->scriptWidget = new MgScriptWidget;

	d_ptr->cameraAnimation.setObjectName("cameraAnimation");


	QWidget * viewAnimatorWidget = new QWidget(this);
	QHBoxLayout  * viewEditorLayout = new QHBoxLayout;
	QVBoxLayout  * viewAnimatorWidgetLayout = new QVBoxLayout;

	viewEditorLayout->setMargin(0);
	viewEditorLayout->setContentsMargins(0,0,0,0);
	viewEditorLayout->setSpacing(0);

	viewAnimatorWidgetLayout->setMargin(0);
	viewAnimatorWidgetLayout->setContentsMargins(0,0,0,0);
	viewAnimatorWidgetLayout->setSpacing(0);

	QSplitter * viewAndEditorContainer = new QSplitter(viewAnimatorWidget);
	viewAndEditorContainer->setOrientation(Qt::Horizontal);
	viewAndEditorContainer->setHandleWidth(5);

	d_ptr->view = new MgNeuralNetworkView(d_ptr->graph,viewAndEditorContainer);
	d_ptr->view->setObjectName("networkView");
	d_ptr->view->setFocusPolicy(Qt::StrongFocus);
	d_ptr->propertiesEditor = new MgMultiObjectsPropertiesEditor(viewAndEditorContainer);
	d_ptr->cameraAnimation.setScene(d_ptr->view);

	viewAndEditorContainer->addWidget(d_ptr->view);
	viewAndEditorContainer->addWidget(d_ptr->propertiesEditor);

	viewAndEditorContainer->setSizes(QList<int>()<<width()-200<<200);

	d_ptr->animatorWidget = new MgAnimatorTimeLineWidget(viewAnimatorWidget);
	d_ptr->animatorWidget->setMaximumHeight(45);


	viewAnimatorWidgetLayout->addWidget(viewAndEditorContainer);
	viewAnimatorWidgetLayout->addWidget(d_ptr->animatorWidget);
	viewAnimatorWidget->setLayout(viewAnimatorWidgetLayout);

	d_ptr->scriptEngine = scriptEngine;
	d_ptr->scriptWidget->setScriptEngine(d_ptr->scriptEngine );

	d_ptr->messagesAreaWidget = new MgMessagesAreaWidget(this);
	registerMsgEmitterInMsgOutput(d_ptr->graph);


	d_ptr->videoExampleWidget = new MgVideoExampleWidget(this);


	addSidebarWidget("Overview video",QIcon(":/home.png"),d_ptr->videoExampleWidget );

	addSidebarWidget("Examples",QIcon(":/documentation.png"),d_ptr->helpWidget);
	addSidebarWidget("JS objects info",QIcon(":/javascript-logo.png"),
			new MgScriptEngineObjectsInfoWidget(scriptEngine,this));
	addSidebarWidget("Animator",QIcon(":/graph.png"),viewAnimatorWidget);


	MgCommandLineWidget * commandLine = new MgCommandLineWidget(this);
	QIcon execIcon(":/exec.png");
	commandLine->setExecButtonIcon(execIcon);
	commandLine->setMaximumWidth(500);
	commandLine->setScriptEngine(scriptEngine);

	addOutputPaneWidget(commandLine);
	addOutputPane("Script",d_ptr->scriptWidget);
	addOutputPane("Messages",d_ptr->messagesAreaWidget);

	d_ptr->scriptEngine->registerObject(d_ptr->view);
	d_ptr->scriptEngine->registerObject(d_ptr->animator);
	d_ptr->scriptEngine->registerObject(&d_ptr->cameraAnimation);
	d_ptr->scriptEngine->registerObject(d_ptr->videoExampleWidget);
	d_ptr->view->setAnimationDuration(d_ptr->animator->animationDuration());

	makeConnections();

	d_ptr->propertiesEditor->appendObject(" Animation options",d_ptr->animator);
	d_ptr->propertiesEditor->appendObject(" 3D Scene options",d_ptr->view);

	setCurrentWidget(1);
}


void MgSNNAnimatorMainWindow::makeConnections()
{
	connect(d_ptr->animator,SIGNAL(started()),d_ptr->animatorWidget,SLOT(onPlay()));
	connect(d_ptr->animator,SIGNAL(paused()),d_ptr->animatorWidget,SLOT(onPause()));
	connect(d_ptr->animator,SIGNAL(stopped()),d_ptr->animatorWidget,SLOT(onStop()));
	connect(d_ptr->animator,SIGNAL(progress(int)),d_ptr->animatorWidget,SLOT(updateProgress(int)));

	connect(d_ptr->animator,SIGNAL(animationDurationChanged(double)),d_ptr->view,SLOT(setAnimationDuration(double)));

	connect(d_ptr->animatorWidget,SIGNAL(playRequest()),d_ptr->animator,SLOT(start()));
	connect(d_ptr->animatorWidget,SIGNAL(pauseRequest()),d_ptr->animator,SLOT(pause()));
	connect(d_ptr->animatorWidget,SIGNAL(stopRequest()),d_ptr->animator,SLOT(stop()));
	connect(d_ptr->animatorWidget,SIGNAL(progress(int)),d_ptr->animator,SLOT(setProgressAndUpdate(int)));


	connect(d_ptr->graph,SIGNAL(updateFinished(double)),d_ptr->view,SLOT(update(double)));
	connect(d_ptr->animator,SIGNAL(animationDurationChanged(double)),
			d_ptr->view,SLOT(setAnimationDuration(double)));
}

MgSNNAnimatorMainWindow::~MgSNNAnimatorMainWindow()
{
	delete d_ptr;
}
void MgSNNAnimatorMainWindow::registerMsgEmitterInMsgOutput(MgMessagesEmitter * msgEmitter)
{
	d_ptr->messagesAreaWidget->registerSender(msgEmitter);
}
