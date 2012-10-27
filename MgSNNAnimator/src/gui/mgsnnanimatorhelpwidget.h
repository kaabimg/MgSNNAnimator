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
#ifndef MGSNNANIMATORHELPWIDGET_H_
#define MGSNNANIMATORHELPWIDGET_H_

#include <MgCore/MgGlobal>
#include <MgGui/MgAnimatedStackedWidget>


MG_USE_PRIVATE_CLASS(MgSNNAnimatorHelpWidget)

struct HelpPage;
class MgSNNAnimatorHelpWidget : public MgAnimatedStackedWidget
{
	Q_OBJECT
public:
	MgSNNAnimatorHelpWidget(QWidget * parent=0);
	virtual ~MgSNNAnimatorHelpWidget();

protected Q_SLOTS:
	void makeExamplesListCurrent();
	void slideToJsEditor(const QString &);
protected:
	void createExamplesList();
	QIcon createNumberIcon(int number)const;
	void createExampleHelpPage(const HelpPage &);


private:
	MG_PRIVATE_CLASS(MgSNNAnimatorHelpWidget)
	Q_DISABLE_COPY(MgSNNAnimatorHelpWidget)

};

#endif /* MGSNNANIMATORHELPWIDGET_H_ */
