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
#include <QApplication>
#include <QThread>
#include <QTime>

#include "src/gui/mgsnnanimatormainwindow.h"
#include "src/core/mgneuralnetworkgraph.h"

#include <MgCore/MgGlobal>
#include <MgCore/MgApplicationArgumentsManager>
#include <MgGui/MgCurveData>
#include <MgScript/MgScriptEngine>
#include <MgScript/MgCommandLine>
#include <MgScript/MgMathScriptBindings>
#include <MgScript/MgMessagesScriptBindings>

#include <Mg3dScene>
#include <math.h>
#include <QDebug>

//QGLView known bug segfault
#ifdef Q_OS_UNIX
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void segfault_sigaction(int , siginfo_t *, void *)
{
    exit(0);
}
#endif


//================================================================================


int main(int argc, char *argv[])
{
	qInstallMsgHandler(&MgMessagesHandler::mgMessagesHandler);
//Handle QGLView segfault
#ifdef Q_OS_UNIX
	struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = segfault_sigaction;
	sa.sa_flags   = SA_SIGINFO;
	sigaction(SIGSEGV, &sa, NULL);
#endif


	QApplication a(argc, argv);
	qsrand(QTime::currentTime().msec());
	//////
	a.setApplicationName("MgSNNAnimator");
	a.setApplicationVersion("1.0.0");
	//////


	MgApplicationArgumentsManager argumentsManager(a.arguments());

	MgSpikingNeuralNetwork graph;
	graph.setObjectName("network");
	MgScriptEngine scriptEngine;
	scriptEngine.registerObject(&graph);

	MgMathScriptBindings mathBingings(&scriptEngine);
	MgMessagesScriptBindings messagesBindings(&scriptEngine);

	MgSNNAnimatorMainWindow mainWindow(&scriptEngine,&graph);
	mainWindow.registerMsgEmitterInMsgOutput(&messagesBindings);
	mainWindow.showMaximized();

	if(argumentsManager.optionArgumentsSize("-script")>0)
		scriptEngine.execFile(argumentsManager.optionArguments("-script").first());

	return a.exec();
}
