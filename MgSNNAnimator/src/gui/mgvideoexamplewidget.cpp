#include "mgvideoexamplewidget.h"
#include <QDebug>
MgVideoExampleWidget::MgVideoExampleWidget(QWidget * parent) : QWebView(parent)
{
	setObjectName("mgWebBrowser");
//	setPageUrl("https://sites.google.com/site/kaabimg/");
	setPageUrl("http://youtube.googleapis.com/v/zQhSrKATP2Y");
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

