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
#include "mgneuronspikeeffectshader.h"

MgNeuronSpikeEffectShader::MgNeuronSpikeEffectShader()
{
	setShaderSourceCodeFromFile(QGLShader::Vertex,":/neuronspikeeffect/neuronspikeeffect.vsh");
	setShaderSourceCodeFromFile(QGLShader::Fragment,":/neuronspikeeffect/neuronspikeeffect.fsh");
}

MgNeuronSpikeEffectShader::~MgNeuronSpikeEffectShader() {}

void MgNeuronSpikeEffectShader::setNeuronPosition(const QVector3D & position)
{
	if(program())
	{
		program()->setUniformValue("neuronPosition",position);
	}
}

void MgNeuronSpikeEffectShader::setAmplification(float amp)
{
	if(program())
	{
		program()->setUniformValue("amplification",amp);
	}
}
void MgNeuronSpikeEffectShader::setRotation(const QVector4D & rotation)
{
	if(program())
	{
		program()->setUniformValue("rotationQuaternion",rotation);
	}
}

void MgNeuronSpikeEffectShader::setColor(const QColor & color)
{
	if(program())
	{
		program()->setUniformValue("color",color);
	}

}
