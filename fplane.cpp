/* 

Alexzander Daniel Lee and Alexander Six
alexzal & asix
CPSC 1020-001
11/24/15
Major Programming Assignment 3

This file creates the fplane objects,
calculates their hitpoints, and determines
what to print out to the logs
*/

#include "ray.h"

pparm_t fplane_parse[] =
{
 	{"xdir", 3, sizeof(double), "%lf", 0},
 	{"dimensions", 2, sizeof(double), "%lf", 0}
};
#define NUM_ATTRS (sizeof(fplane_parse) / sizeof(pparm_t))

fplane_t::fplane_t(
FILE *in, 
model_t *model, 
int attrmax) : plane_t(in, model, 2)
{
	int mask = 0;
	strcpy(obj_type, "fplane");

	fplane_parse[0].loc = &xdir;
	fplane_parse[1].loc = &dims;

	mask = parser(in, fplane_parse, NUM_ATTRS, attrmax);
	assert(mask ==3);

	vec_unit(&normal, &normal);
	vec_project(&normal, &xdir, &projxdir);
	assert(projxdir.x != 0.0);

	vec_unit(&projxdir, &projxdir);

	vec_copy(&projxdir, &rot.row[0]);
	vec_copy(&normal, &rot.row[2]);
	vec_cross(&rot.row[2], &rot.row[0], &rot.row[1]);
}

double fplane_t::hits(vec_t *base, vec_t *dir)
{
	vec_t newvec;
	double test = plane_t::hits(base, dir);

	if(test == -1){
		return(-1);
	}
	else{
		vec_diff(&point, &last_hitpt, &newvec);
		vec_xform(&rot, &newvec, &newvec);

		if((0 <= newvec.x && newvec.x <= dims[0]) && 
		(0 <= newvec.y && newvec.y <= dims[1])){
			vec_copy(&newvec, &rothit);
			return(test);
		}
		return(-1);
	}
}

void fplane_t::printer(FILE *out)
{
	plane_t::printer(out);

	fprintf(out,"%-10s%8.1lf%6.1lf%6.1lf \n","xdir",
		xdir.x, xdir.y, xdir.z);

	fprintf(out,"%-10s%8.1lf%6.1lf%6.1lf \n","dimensions",
		dims[0], dims[1], dims[2]);
}
