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
#include "mganimatortimelinewidget.h"
#include <QSlider>
#include <QPushButton>
#include <QHBoxLayout>
#include <QStyle>
#include <QPainter>
#include <QDebug>


#define TO_STRING(a) #a


static const char * MgAnimatorTimeLineWidget_sliderStyleSheet = TO_STRING(
		QSlider::groove:horizontal {
		border: 1px solid #bbb;
		background: white;
		height: 10px;
		border-radius: 16px;
		}

		QSlider::sub-page:horizontal {
		background: qlineargradient(x1: 0, y1: 0,    x2: 0, y2: 1,
		    stop: 0 #66e, stop: 1 #bbf);
		background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1,
		    stop: 0 #bbf, stop: 1 #55f);
		border: 1px solid #777;
		height: 10px;
		border-radius: 16px;
		}

		QSlider::add-page:horizontal {
		background: #fff;
		border: 1px solid #777;
		height: 10px;
		border-radius: 16px;
		}

		QSlider::handle:horizontal {
		background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
		    stop:0 #eee, stop:1 #ccc);
		border: 1px solid #777;
		width: 13px;
		margin-top: -2px;
		margin-bottom: -2px;
		border-radius: 16px;
		}

		QSlider::handle:horizontal:hover {
		background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
		    stop:0 #fff, stop:1 #ddd);
		border: 1px solid #444;
		border-radius: 16px;
		}

		QSlider::sub-page:horizontal:disabled {
		background: #bbb;
		border-color: #999;
		}

		QSlider::add-page:horizontal:disabled {
		background: #eee;
		border-color: #999;
		}

		QSlider::handle:horizontal:disabled {
		background: #eee;
		border: 1px solid #aaa;
		border-radius: 16px;
		}

);



static const char * MgAnimatorTimeLineWidget_buttonStyleSheet = TO_STRING(
		QPushButton {
		color: #333;
		border: 2px solid #555;
		border-radius: 11px;
		padding: 5px;
		background: qradialgradient(cx: 0.3, cy: -0.4,
		fx: 0.3, fy: -0.4,
		radius: 1.35, stop: 0 #fff, stop: 1 #888);
		min-width: 80px;
		}

		QPushButton:hover {
		background: qradialgradient(cx: 0.3, cy: -0.4,
		fx: 0.3, fy: -0.4,
		radius: 1.35, stop: 0 #fff, stop: 1 #bbb);
		}

		QPushButton:pressed {
		background: qradialgradient(cx: 0.4, cy: -0.1,
		fx: 0.4, fy: -0.1,
		radius: 1.35, stop: 0 #fff, stop: 1 #ddd);
		}
);


MgAnimatorTimeLineWidget::MgAnimatorTimeLineWidget(QWidget* parent)
	:QWidget(parent),m_status(Play)
{
	QHBoxLayout * layout = new QHBoxLayout;

	m_slider = new QSlider(Qt::Horizontal,this);
	m_playPauseButton = new QPushButton(this);
	m_stopButton = new QPushButton(this);
	m_forwardButton = new QPushButton(this);
	m_backwardButton = new QPushButton(this);

	m_slider->setStyleSheet(MgAnimatorTimeLineWidget_sliderStyleSheet);
	m_playPauseButton->setStyleSheet(MgAnimatorTimeLineWidget_buttonStyleSheet);
	m_stopButton->setStyleSheet(MgAnimatorTimeLineWidget_buttonStyleSheet);
	m_forwardButton->setStyleSheet(MgAnimatorTimeLineWidget_buttonStyleSheet);
	m_backwardButton->setStyleSheet(MgAnimatorTimeLineWidget_buttonStyleSheet);

	connect(m_slider,SIGNAL(valueChanged(int)),this,SIGNAL(progress(int)));
	connect(m_playPauseButton,SIGNAL(pressed()),this,SLOT(onPlayOrPausePressed()));
	connect(m_stopButton,SIGNAL(pressed()),this,SIGNAL(stopRequest()));


	connect(m_forwardButton,SIGNAL(pressed()),this,SLOT(forward()));
	connect(m_backwardButton,SIGNAL(pressed()),this,SLOT(backward()));

	m_stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
	m_forwardButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
	m_backwardButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));

	layout->addWidget(m_backwardButton);
	layout->addWidget(m_stopButton);
	layout->addWidget(m_playPauseButton);
	layout->addWidget(m_forwardButton);
	layout->addWidget(m_slider);

	setLayout(layout);

	onStop();
}

MgAnimatorTimeLineWidget::~MgAnimatorTimeLineWidget() {}

void MgAnimatorTimeLineWidget::updateProgress(int progress)
{
	disconnect(m_slider,SIGNAL(valueChanged(int)),this,SIGNAL(progress(int)));
	m_slider->setValue(progress);
	connect(m_slider,SIGNAL(valueChanged(int)),this,SIGNAL(progress(int)));
}


void MgAnimatorTimeLineWidget::onPlayOrPausePressed()
{
	if(m_status == Play)
		Q_EMIT pauseRequest();
	else
		Q_EMIT playRequest();

}

void MgAnimatorTimeLineWidget::onPause()
{
	m_status = Pause;
	m_playPauseButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
	m_slider->setEnabled(true);
}
void MgAnimatorTimeLineWidget::onPlay()
{
	m_status = Play;
	m_playPauseButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
	m_slider->setEnabled(true);
}
void MgAnimatorTimeLineWidget::onStop()
{
	onPause();
	updateProgress(0);
	m_slider->setEnabled(false);
}



void MgAnimatorTimeLineWidget::forward()
{
	if(m_status == Play)
		m_slider->setValue(qMin(100,m_slider->value()+7));

}
void MgAnimatorTimeLineWidget::backward()
{
	if(m_status == Play)
		m_slider->setValue(qMax(0,m_slider->value()-7));
}

void MgAnimatorTimeLineWidget::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event)
    QPainter p(this);
	QLinearGradient grad(rect().topLeft(), rect().topRight());
	grad.setColorAt(0, QColor(20, 20, 20));
	grad.setColorAt(1, QColor(75, 75, 75));
	p.fillRect(rect(), grad);
	QWidget::paintEvent(event);
}


