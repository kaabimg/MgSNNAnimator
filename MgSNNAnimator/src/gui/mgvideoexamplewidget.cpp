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
#include "mgvideoexamplewidget.h"
#include <QDebug>
MgVideoExampleWidget::MgVideoExampleWidget(QWidget * parent) : QWebView(parent)
{
	setObjectName("mgWebBrowser");
//	setPageUrl("https://sites.google.com/site/kaabimg/");
//	setPageUrl("http://youtube.googleapis.com/v/zQhSrKATP2Y");
}

QString MgVideoExampleWidget::pageUrl()const
{
	return url().toString();
}
void MgVideoExampleWidget::setPageUrl(const QString & _url)
{
	if(url() != _url)
	{
		load(QUrl(_url));
	}
}

MgVideoExampleWidget::~MgVideoExampleWidget() {}

