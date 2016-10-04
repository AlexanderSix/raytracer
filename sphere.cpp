/* 

Alexzander Daniel Lee and Alexander Six
alexzal & asix
CPSC 1020-001
11/24/15
Major Programming Assignment 3

This file contains all of the 
different functions and constructor
for the creation of spheres within
the image
*/


#include "ray.h"
#include <math.h>


pparm_t sphere_parse[] =
{
	{"center", 3, sizeof(double), "%lf", 0},
	{"radius", 1, sizeof(double), "%lf", 0}
};
#define NUM_ATTRS (sizeof(sphere_parse) / sizeof(pparm_t))

//**
// Create a new sphere object and initialize it 
sphere_t::sphere_t(FILE  *in, model_t  *model, 
int  attrmax) : object_t(in,model)
{
	int mask;

	strcpy(obj_type, "sphere");

	sphere_parse[0].loc = &center;
	sphere_parse[1].loc = &radius;

	/* Invoke the parser */
	mask = parser(in, sphere_parse, NUM_ATTRS, attrmax);

	/* verify required attributes read */
	assert(mask > 0);
}

void sphere_t::printer(FILE  *out) 
{
	// Print generic attributes
	object_t::printer(out);

	fprintf(out,"%-12s","center");
	fprintf(out,"%5.1lf %5.1lf %5.1lf\n", center.x, center.y, center.z);
	fprintf(out,"%-12s %5.1lf\n","radius",radius);
}

double sphere_t::hits(vec_t  *base, vec_t  *dir) 
{
	vec_t C,V,hitpoint,NU,temp1;
	double a,b,c,disc,t;


	assert (cookie == OBJ_COOKIE);

	// extract sph pointer from object structure 
	// see notes for sphere_hits function

	/* We will use the following notation in the comments below */
	/*  C= Center of the sphere  (plane attribute )              */
	/*  V = viewpoint or start of the ray             */
	/*  R = Radius of Sphere                      */
	/*  D = unit vector in direction ray travels in  */

	/*  ndotd = N dot D                                         */
	/*  ndotb = N dot B                                         */
	/*  ndotq = N dot Q --- which was precomputed in plane_init */
	/*  dist  = signed distance from base to hitpoint location  */

	// move center of sphere to the origin

	vec_diff(&center, &center, &C);
	vec_diff(&center,base,&V);

	a=vec_dot(dir,dir);
	b=2*vec_dot(&V,dir);
	c=((vec_dot(&V,&V))-((radius)*(radius)));

	disc=((b*b)-(4*a*c));

	if(disc<0){
		return -1.0;
	}

	else if(disc==0){
		return -1.0;
	}

	else if(disc>0){
		t=((-1*(b))-(sqrt(disc)))/(2*a);
	}

	hitpoint.x=base->x+(dir->x*t);
	hitpoint.y=base->y+(dir->y*t);
	hitpoint.z=base->z+(dir->z*t);

	object_t::last_hitpt=hitpoint;
	getlast_normal(&NU);

	vec_diff(&center,&hitpoint,&temp1);
	NU.x=((1.0/(vec_len(&temp1)))*temp1.x);
	NU.y=((1.0/(vec_len(&temp1)))*temp1.y);
	NU.z=((1.0/(vec_len(&temp1)))*temp1.z);

	object_t::last_normal=NU;

	// return distance
	return t;
}
