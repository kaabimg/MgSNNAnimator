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
#include "mgcurvedrawshader.h"

MgCurveDrawShader::MgCurveDrawShader()
{
	setShaderSourceCodeFromFile(QGLShader::Vertex,":/curve/curvedraw.vsh");
	setShaderSourceCodeFromFile(QGLShader::Fragment,":/curve/curvedraw.fsh");
}

void MgCurveDrawShader::setCenter(const QVector2D & center)
{
	if(program())
		program()->setUniformValue("center",center);
}
void MgCurveDrawShader::setSize(const QVector2D & size)
{
	if(program())
		program()->setUniformValue("size",size);

}
void MgCurveDrawShader::setXRange(const QVector2D & xRange)
{
	if(program())
		program()->setUniformValue("xRange",xRange);

}
void MgCurveDrawShader::setYRange(const QVector2D & yRange)
{
	if(program())
		program()->setUniformValue("yRange",yRange);

}

void MgCurveDrawShader::setCurveColor(const QColor & color)
{
	if(program())
		program()->setUniformValue("curveColor",QVector4D(
				color.redF(),
				color.greenF(),
				color.blueF(),
				color.alphaF()
		));
}
