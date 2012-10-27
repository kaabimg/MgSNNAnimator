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
varying highp vec2 qt_TexCoord0;
varying vec3 normal,eyeVec;				

uniform vec4 ambiantColor = vec4(0.2,0.2,.2,1);


void main()													
{			
	vec3 N = normalize(normal);								
	vec3 E = normalize(eyeVec);
	float lambertTerm = max(0,dot(N,E));
	vec4 Ia = ambiantColor;
	vec4 Id = ambiantColor * 2 * lambertTerm;
	gl_FragColor = clamp(Ia + Id ,0,1);
//	gl_FragColor = 1 - gl_FragColor;
	gl_FragColor.a = 1;
}
