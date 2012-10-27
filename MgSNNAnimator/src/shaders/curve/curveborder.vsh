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
attribute vec4 qt_Vertex;
uniform vec2 center;
uniform vec2 size;


float remapVal(float value,float origin_low,float origin_high,float dest_low,float dest_high) 
{																		
	return dest_low + (dest_high - dest_low) * (value - origin_low) / (origin_high - origin_low);		
}


void main(void)
{
    gl_Position.x = remapVal(qt_Vertex.x,0,1,center.x - size.x,center.x + size.x);
    gl_Position.y = remapVal(qt_Vertex.y,0,1,center.y - size.y,center.y + size.y);
    gl_Position.z = 0;
    gl_Position.w = 1;
};