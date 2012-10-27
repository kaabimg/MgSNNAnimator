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
#include "mgcurveborderdrawshader.h"

MgCurveBorderDrawShader::MgCurveBorderDrawShader()
{
	setShaderSourceCodeFromFile(QGLShader::Vertex,":/curve/curveborder.vsh");
	setShaderSourceCodeFromFile(QGLShader::Fragment,":/curve/curveborder.fsh");
}


void MgCurveBorderDrawShader::setCenter(const QVector2D & center)
{
	if(program())
		program()->setUniformValue("center",center);
}


void MgCurveBorderDrawShader::setSize(const QVector2D & size)
{
	if(program())
		program()->setUniformValue("size",size);
}

void MgCurveBorderDrawShader::setBackgroundColor(const QColor & color)
{
	if(program())
		program()->setUniformValue("backgroundColor",QVector4D(
				color.redF(),
				color.greenF(),
				color.blueF(),
				color.alphaF()
		));
}
