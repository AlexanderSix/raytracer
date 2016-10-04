/* 

Alexzander Daniel Lee and Alexander Six
alexzal & asix
CPSC 1020-001
11/24/15
Major Programming Assignment 3

This file contains all of the 
different functions and constructor
for the creation of generic planes 
for the image
*/

#include "ray.h"

pparm_t plane_parse[] =
{
	{"point", 3, sizeof(double), "%lf", 0},
	{"normal", 3, sizeof(double), "%lf", 0}
};
#define NUM_ATTRS (sizeof(plane_parse) / sizeof(pparm_t))


plane_t::plane_t(FILE  *in, model_t  *model, 
int  attrmax) : object_t(in,model)
{
	int mask;

	strcpy(obj_type,"plane");

	plane_parse[0].loc = &point;
	plane_parse[1].loc = &normal;
	/* Invoke the parser */
	mask = parser(in, plane_parse, NUM_ATTRS, attrmax);
	/* verify required attributes read */
	assert(mask == 3);

	//getlast_normal(temp);
	vec_copy(&normal, &last_normal);

	ndotq = vec_dot(&point, &normal);

}


//=============================================================================
// ******* printer( ) *******
// 
// prints a formatted version of the plane structure
// format should be consistent with camera_print() or material_print()   
//=============================================================================
void plane_t::printer(FILE  *out)
{

	/* print generic attributes */
	object_t::printer(out);
	/* Recover pln pointer from object_t and print */
	/* point and normal in usual format				  */

	fprintf(out,"%-10s","normal");
	fprintf(out,"%8.1lf%6.1lf%6.1lf \n",normal.x, normal.y, normal.z);
	fprintf(out,"%-10s","point");
	fprintf(out,"%8.1lf%6.1lf%6.1lf \n",point.x, point.y, point.z);

}


//=============================================================================
// ******* hits( ) *******
// 
// determine if a vector projected from location *base
//    in direction *dir hits the plane
//    (*dir is the unit direction vector)
// if so, the return value is the distance from the base
//    to the point of contact
//=============================================================================
double  plane_t::hits(vec_t  *base, vec_t  *dir)    
{

	/* other variables here */
	double ndotd;
	double ndotb;
	double dist;
	vec_t hitpoint;

	assert(cookie == OBJ_COOKIE);

	/* We will use the following notation in the comments below */
	/*  Q = point on the plane  (plane attribute )              */
	/*  N = normal to the plane (plane attribute )              */
	/*  B = base of ray (input parameter )                      */
	/*  D = direction of ray                                    */

	/*  ndotd = N dot D                                         */
	/*  ndotb = N dot B                                         */
	/*  ndotq = N dot Q --- which was precomputed in plane_init */
	/*  dist  = signed distance from base to hitpoint location  */
	ndotq=vec_dot(&normal,&point);

	/* Compute ndotd and if 0, return(-1.0); */
	ndotd=vec_dot(&normal,dir);
	if(ndotd==0){
		return (-1.0);
	}

	/* Compute ndotb */
	ndotb=vec_dot(&normal,base);

	/* Compute dist = (ndotq - ndotb) / ndotd  using */
	/* the plane's ndotq value                       */
	dist=(ndotq-ndotb)/ndotd;

	/* Only continue if dist is positive             */
	if (dist <= 0.0){
		return -1.0;
	}

	/* Compute location of hit point by scaling dir  */
	/* by dist, and adding to base                   */
	vec_scale(dist,dir,&hitpoint);
	vec_sum(&hitpoint,base,&hitpoint);

	/* Only continue if hitpoint.z is negative       */
	if (hitpoint.z > 0){
		return -1.0;
	}

	/* Copy hitpoint_location to obj->last_hit       */
	/* and  pln->normal to obj->last_normal          */
	last_hitpt=hitpoint;
	last_normal=normal;

	return(dist);
}


