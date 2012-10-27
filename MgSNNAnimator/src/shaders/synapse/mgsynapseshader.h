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
#ifndef MGSYNAPSESHADER_H_
#define MGSYNAPSESHADER_H_

#include <Mg3D/MgAbstractShader>

class MgSynapseShader : public MgAbstractShader
{
public:
	MgSynapseShader();
	void setCameraPosition(const QVector3D &);
	void setActivity(int);
	void setInhibitorySynapseColor(const QColor &);
	void setExcitatorySynapseColor(const QColor &);

};

#endif /* MGSYNAPSESHADER_H_ */
