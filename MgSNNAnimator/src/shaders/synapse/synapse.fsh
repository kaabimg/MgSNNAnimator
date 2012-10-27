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
varying vec3 normal,eyeVec;		


const vec4 inactiveAmbiantColor = vec4(0.4,0.4,.4,1);
const vec4 inactiveDiffuseColor =  vec4(0.4,0.4,0.4,1);

uniform vec4 excitatoryAmbiantColor = vec4(0.8,0.2,0.2,1);
uniform vec4 inhibitoryAmbiantColor = vec4(0.2,0.2,0.8,1);

uniform int activity;

void main()													
{			
	vec3 N = normalize(normal);								
	vec3 E = normalize(eyeVec);
	float lambertTerm = max(0,dot(N,E));
	
	vec4 ambiant,diffuse;
	float alpha;
	if(activity == 1)
	{
		ambiant = excitatoryAmbiantColor;
		diffuse = excitatoryAmbiantColor * 2;
		alpha = .5;
	}
	else if(activity == -1)
	{
		ambiant = inhibitoryAmbiantColor;
		diffuse = inhibitoryAmbiantColor * 2;
		alpha = .5;
	}
	else
	{
		ambiant = inactiveAmbiantColor;
		diffuse = inactiveDiffuseColor;
		alpha = .3;
	}
		
	vec4 Ia = ambiant;
	vec4 Id = diffuse * lambertTerm;
	gl_FragColor = clamp(Ia + Id ,0,1);
//	if(activity == 0)
//		gl_FragColor = 1 - gl_FragColor;
	gl_FragColor.a = alpha;
}