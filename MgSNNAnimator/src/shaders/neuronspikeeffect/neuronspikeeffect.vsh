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
uniform vec3 neuronPosition;
uniform float amplification;
uniform vec4 rotationQuaternion;
varying highp vec2 qt_TexCoord0;

void main()
{
	vec3 v = qt_Vertex.xyz * amplification;
	vec3 temp = cross(rotationQuaternion.xyz, v) + rotationQuaternion.w * v;
	vec3 position = (cross(temp, -rotationQuaternion.xyz) + 
			dot(rotationQuaternion.xyz,v) * rotationQuaternion.xyz + rotationQuaternion.w * temp);
	
	gl_Position = qt_ModelViewProjectionMatrix * vec4(position+neuronPosition,1);
	qt_TexCoord0 = (qt_Vertex.xy + 1)/2;
}