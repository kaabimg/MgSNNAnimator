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
attribute highp vec4 qt_Vertex;	

uniform mediump mat4 qt_ModelViewProjectionMatrix;
uniform vec3 spikePosition;
uniform vec4 rotationQuaternion;

void main()
{
	vec3 temp = cross(rotationQuaternion.xyz, qt_Vertex.xyz) + rotationQuaternion.w * qt_Vertex.xyz;
	vec3 position = (cross(temp, -rotationQuaternion.xyz) + 
			dot(rotationQuaternion.xyz,qt_Vertex.xyz) * rotationQuaternion.xyz + rotationQuaternion.w * temp);
	gl_Position = qt_ModelViewProjectionMatrix * vec4(position+spikePosition,1);
}