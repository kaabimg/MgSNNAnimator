#ifndef MGVIDEOEXAMPLEWIDGET_H_
#define MGVIDEOEXAMPLEWIDGET_H_

#include <QWebView>

class MgVideoExampleWidget : public QWebView
{
	Q_OBJECT
	Q_PROPERTY(QString pageUrl READ pageUrl WRITE setPageUrl)
	Q_CLASSINFO("MG_PROPERTIES","pageUrl")
public:
	MgVideoExampleWidget(QWidget * parent = 0);
	virtual ~MgVideoExampleWidget();
	QString pageUrl()const;
	void setPageUrl(const QString & );
};

#endif /* MGVIDEOEXAMPLEWIDGET_H_ */
