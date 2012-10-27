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
#include "mg3dcurveplotter.h"
#include <math.h>
#include <Qt3D/qglpainter.h>
#include <QDebug>

Mg3dCurvePlotter::Mg3dCurvePlotter()
{
	m_curveLineWidth = 1.5;

	setCurveColor(QColor(40,40,220,200));
	setBackgroundColor(QColor(37,218,227,80));
}

void Mg3dCurvePlotter::setCurveColor(const QColor & color)
{
	if(m_curveColor != color)
	{
		m_curveColor = color;
		m_updateCurveColor = true;
	}
}
void Mg3dCurvePlotter::setBackgroundColor(const QColor & color)
{
	if(m_backgroundColor != color)
	{
		m_backgroundColor = color;
		m_updateBackgroundColor = true;
	}
}



void Mg3dCurvePlotter::createBorder()
{
	m_borderData.clear();
	int index(0);

	m_borderData.appendVertex(QVector3D(0,1,0));
	m_borderData.appendIndex(index);index++;

	m_borderData.appendVertex(QVector3D(0,0,0));
	m_borderData.appendIndex(index);index++;


	m_borderData.appendVertex(QVector3D(1,0,0));
	m_borderData.appendIndex(index);index++;


	m_borderData.appendVertex(QVector3D(0,1,0));
	m_borderData.appendIndex(index);index++;


	m_borderData.appendVertex(QVector3D(1,0,0));
	m_borderData.appendIndex(index);index++;


	m_borderData.appendVertex(QVector3D(1,1,0));
	m_borderData.appendIndex(index);

}


void Mg3dCurvePlotter::drawBorder(const QVector2D & center,const QVector2D & size,QGLPainter* painter)
{
	m_curveBorderDrawShader.setActive(painter,true);
	if(m_updateBackgroundColor)
	{
		m_curveBorderDrawShader.setBackgroundColor(m_backgroundColor);
		m_updateBackgroundColor = false;
	}
	m_curveBorderDrawShader.setCenter(center);
	m_curveBorderDrawShader.setSize(size);
	m_borderData.draw(painter,0,m_borderData.indexCount(),QGL::Triangles);
	m_curveBorderDrawShader.setActive(painter,false);
}

void Mg3dCurvePlotter::drawCurve(
		QGeometryData & curve,
		const QVector2D & center,
		const QVector2D & size,
		const QVector2D & xRange,
		const QVector2D & yRange,
		int lastIndex,
		double curveLineWidth,
		QGLPainter* painter)
{
	m_curveDrawShader.setActive(painter,true);
	if(m_updateCurveColor)
	{
		m_curveDrawShader.setCurveColor(m_curveColor);
		m_updateCurveColor = false;
	}
	m_curveDrawShader.setCenter(center);
	m_curveDrawShader.setSize(size);
	m_curveDrawShader.setXRange(xRange);
	m_curveDrawShader.setYRange(yRange);

	curve.draw(painter,0,lastIndex,QGL::LineStrip,curveLineWidth);

	m_curveDrawShader.setActive(painter,false);
}
