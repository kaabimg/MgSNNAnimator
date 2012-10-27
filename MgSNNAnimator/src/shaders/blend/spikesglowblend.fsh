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
uniform sampler2D qt_Texture0;
uniform sampler2D qt_Texture1;
varying highp vec2 qt_TexCoord0;
uniform vec2 bendSize; 

void main(void)
{
	gl_FragColor = clamp(
			texture2D(qt_Texture0, qt_TexCoord0)+
			texture2D(qt_Texture1, qt_TexCoord0)+
			texture2D(qt_Texture1, qt_TexCoord0 + vec2(-bendSize.x,0))+
			texture2D(qt_Texture1, qt_TexCoord0 + vec2(bendSize.x,0))+
			texture2D(qt_Texture1, qt_TexCoord0 + vec2(0,bendSize.y))+
			texture2D(qt_Texture1, qt_TexCoord0 + vec2(0,-bendSize.y))+
			texture2D(qt_Texture1, qt_TexCoord0 + vec2(bendSize.x,bendSize.y))+
			texture2D(qt_Texture1, qt_TexCoord0 + vec2(bendSize.x,-bendSize.y))+
			texture2D(qt_Texture1, qt_TexCoord0 + vec2(-bendSize.x,bendSize.y))+
			texture2D(qt_Texture1, qt_TexCoord0 + vec2(-bendSize.x,-bendSize.y)),
			0,1);
};