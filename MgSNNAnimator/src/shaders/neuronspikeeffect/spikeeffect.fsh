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

//uniform vec4 ambiantColor = vec4(1,.7,.2,1);
//uniform vec4 diffuseColor = vec4(0,.3,.8,1);

uniform vec4 spikeColor = vec4(0,.3,.8,1);


varying vec3 normal,eyeVec;	

void main()
{
//	vec3 N = normalize(normal);								
//	vec3 E = normalize(eyeVec);
//	float lambertTerm = max(0,dot(N,E));
//	gl_FragColor = clamp(ambiantColor + diffuseColor * exp(-1/lambertTerm),0,1);
//	gl_FragColor.a = 1.0;
	
	gl_FragColor = spikeColor;
	
	
}