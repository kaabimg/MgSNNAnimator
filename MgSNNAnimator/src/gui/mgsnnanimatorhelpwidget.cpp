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
#include "mgsnnanimatorhelpwidget.h"
#include <MgGui/MgListView>
#include <MgGui/MgFadeElement>
#include <MgGui/MgNavigationBar>
#include <MgScript/MgJavaScriptEditor>

#include <QStyle>
#include <QFile>

#include <QWindowsStyle>
#include <QImage>
#include <QPainter>
#include <QDebug>


struct HelpPage
{
	QString id;
	QString title;
	QString text;
	QString jsCode;
};


struct MgSNNAnimatorHelpWidgetPrivate
{
	MgListView * examplesListView;
	MgJavaScriptEditor * jsEditor;
	MgWidgetWithNavigationBar * jsEditorWidget;
	QHash<QString,HelpPage> examples;

};

MgSNNAnimatorHelpWidget::MgSNNAnimatorHelpWidget(QWidget *parent)
	:	MgAnimatedStackedWidget(parent)
{
	d_ptr = new MgSNNAnimatorHelpWidgetPrivate;
	d_ptr->examplesListView = new MgListView(100,this);
	d_ptr->jsEditor = new MgJavaScriptEditor();

	d_ptr->jsEditorWidget = new MgWidgetWithNavigationBar(
			d_ptr->jsEditor,
			MgNavigationBar::Backward,this);


	connect(d_ptr->jsEditorWidget->navigationBar(),SIGNAL(backwardRequest()),
			this,SLOT(makeExamplesListCurrent()));


	addWidget(d_ptr->examplesListView);
	addWidget(d_ptr->jsEditorWidget);


	setCurrentWidget(d_ptr->examplesListView);
	d_ptr->jsEditor->setReadOnly(true);

	connect(d_ptr->examplesListView,SIGNAL(elementDoubleClicked(QString)),
			this,SLOT(slideToJsEditor(QString)));

	createExamplesList();
}

MgSNNAnimatorHelpWidget::~MgSNNAnimatorHelpWidget()
{
	delete d_ptr;
}


QIcon MgSNNAnimatorHelpWidget::createNumberIcon(int number)const
{
	QSize size(48,48);
	QImage image(size,QImage::Format_RGB32);
	QPainter painter(&image);
	QFont f = font();
	f.setFamily("Tokyo");
	f.setPointSize(30);

	painter.setRenderHints(QPainter::HighQualityAntialiasing,true);
	painter.fillRect(image.rect(),QColor(50,50,50));
	painter.setFont(f);
	painter.setPen(Qt::white);
	QTextOption options;
	options.setAlignment(Qt::AlignCenter);
	painter.drawText(image.rect(),QString::number(number),options);
	QPixmap pixmap;
	pixmap.convertFromImage(image);
	return QIcon(pixmap);
}



void MgSNNAnimatorHelpWidget::createExampleHelpPage(const HelpPage & page)
{
	MgFadeElement * element = new MgFadeElement(d_ptr->examplesListView);
	element->id = page.id;
	element->title = page.title;
	element->text = page.text;
	element->enabled = true;
	element->leftIcon = createNumberIcon(d_ptr->examples.size()+1);
	element->rightIcon= style()->standardPixmap(QStyle::SP_ArrowForward);
	d_ptr->examplesListView->insertElement(d_ptr->examples.size(),element);
	d_ptr->examples.insert(page.id,page);
}


QString readFileContent(const QString & fileName)
{
	QFile file(fileName);
	file.open(QFile::ReadOnly);
	QString content = file.readAll();
	file.close();
	return content;
}

void MgSNNAnimatorHelpWidget::createExamplesList()
{
	HelpPage page;

	page.id = "tut1";
	page.title = "JS scripting overview";
	page.text = "Introduction to objects properties and slots in MgSNNAnimator";
	page.jsCode =  readFileContent(":/overview.js");
	createExampleHelpPage(page);


	page.id = "tut2";
	page.title = "Tutorial 1";
	page.text = "Getting started : a simple network";
	page.jsCode =  readFileContent(":/example1.js");
	createExampleHelpPage(page);


	page.id = "tut3";
	page.title = "Tutorial 2";
	page.text = "Loading a graph, spikes and neurons activity from a file";
	page.jsCode =  readFileContent(":/example2.js");
	createExampleHelpPage(page);


	page.id = "tut4";
	page.title = "Tutorial 3";
	page.text = "Customizing animation parameters";
	page.jsCode =  readFileContent(":/example3.js");
	createExampleHelpPage(page);

//  TODO complete the animation tutorial
//	page.id = "tut5";
//	page.title = "Tutorial 4";
//	page.text = "Running an animation, and creating custom camera animations";
//	page.jsCode =  readFileContent(":/example4.js");
//	createExampleHelpPage(page);
}

void MgSNNAnimatorHelpWidget::slideToJsEditor(const QString & id)
{
	d_ptr->jsEditor->setPlainText(d_ptr->examples[id].jsCode);
	slideToWidget(d_ptr->jsEditorWidget,LeftToRight);
}

void MgSNNAnimatorHelpWidget::makeExamplesListCurrent()
{
	slideToWidget(d_ptr->examplesListView,RightToLeft);
}

