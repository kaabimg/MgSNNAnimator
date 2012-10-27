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
#include "mgspikeeffectshader.h"

MgSpikeEffectShader::MgSpikeEffectShader()
{
	setShaderSourceCodeFromFile(QGLShader::Vertex,":/neuronspikeeffect/spikeeffect.vsh");
	setShaderSourceCodeFromFile(QGLShader::Fragment,":/neuronspikeeffect/spikeeffect.fsh");
}


void MgSpikeEffectShader::setPosition(const QVector3D & position)
{
	if(program())
	{
		program()->setUniformValue("spikePosition",position);
	}
}
void MgSpikeEffectShader::setCameraPosition(const QVector3D & pos)
{
	if(program())
		program()->setUniformValue("cameraPosition",pos);
}
void MgSpikeEffectShader::setSpikeColor(const QColor & color)
{
	if(program())
		program()->setUniformValue("spikeColor",color);
}

