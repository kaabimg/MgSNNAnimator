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
#ifndef MG3DCURVEPLOTTER_H_
#define MG3DCURVEPLOTTER_H_

#include <Qt3D/qgeometrydata.h>
#include "../shaders/curve/mgcurveborderdrawshader.h"
#include "../shaders/curve/mgcurvedrawshader.h"

class QGLPainter;

class Mg3dCurvePlotter
{
public:
	Mg3dCurvePlotter();

	void createBorder();

	// center in [-1,1]
	// size in [0,1]
	void drawBorder(const QVector2D & center,const QVector2D & size,QGLPainter*);
	void drawCurve(
			QGeometryData & curve,
			const QVector2D & center,
			const QVector2D & size,
			const QVector2D & xRange,
			const QVector2D & yRange,
			int lastIndex,
			double,
			QGLPainter*);


	void setCurveColor(const QColor & color);
	void setBackgroundColor(const QColor & color);


	double curveLineWidth()const{return m_curveLineWidth;}
	void setCurveLinewidth(double w){m_curveLineWidth = w;}

private:
	MgCurveBorderDrawShader m_curveBorderDrawShader;
	MgCurveDrawShader m_curveDrawShader;
	QGeometryData m_borderData;
	QColor m_curveColor,m_backgroundColor;
	bool m_updateCurveColor,m_updateBackgroundColor;

	double m_curveLineWidth;
};

#endif /* MG3DCURVEPLOTTER_H_ */
