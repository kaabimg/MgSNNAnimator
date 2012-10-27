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
#ifndef MGSNNANIMATORMAINWINDOW_H_
#define MGSNNANIMATORMAINWINDOW_H_

#include <MgGui/MgMainWindow>
#include <MgCore/MgGlobal>

class MgScriptEngine;
class MgSpikingNeuralNetwork;
class MgMessagesEmitter;

MG_USE_PRIVATE_CLASS(MgSNNAnimatorMainWindow)
class MgSNNAnimatorMainWindow : public MgMainWindow
{
	Q_OBJECT
public:
	MgSNNAnimatorMainWindow(MgScriptEngine * ,MgSpikingNeuralNetwork *);
	virtual ~MgSNNAnimatorMainWindow();
	void registerMsgEmitterInMsgOutput(MgMessagesEmitter *);

protected:
	void makeConnections();

private:
	MG_PRIVATE_CLASS(MgSNNAnimatorMainWindow)
	Q_DISABLE_COPY(MgSNNAnimatorMainWindow)
};

#endif /* MGSNNANIMATORMAINWINDOW_H_ */
