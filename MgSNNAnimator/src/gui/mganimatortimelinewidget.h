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
#ifndef MGANIMATORTIMELINEWIDGET_H_
#define MGANIMATORTIMELINEWIDGET_H_
#include <QWidget>

class QPushButton;
class QSlider;
class MgAnimatorTimeLineWidget : public QWidget
{
	Q_OBJECT
public:
	MgAnimatorTimeLineWidget(QWidget* =0);
	virtual ~MgAnimatorTimeLineWidget();

public Q_SLOTS:
	void updateProgress(int);
	void onPause();
	void onPlay();
	void onStop();


protected:
	void paintEvent(QPaintEvent *);


Q_SIGNALS:
	void progress(int);
	void stopRequest();
	void playRequest();
	void pauseRequest();

private Q_SLOTS:
	void onPlayOrPausePressed();

	void forward();
	void backward();

private:
	enum Status{Play,Pause};
	Status m_status;
	QSlider * m_slider;
	QPushButton * m_playPauseButton;
	QPushButton * m_stopButton;

	QPushButton * m_forwardButton;
	QPushButton * m_backwardButton;

};

#endif /* MGANIMATORTIMELINEWIDGET_H_ */
