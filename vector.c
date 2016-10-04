/* 

Alexzander Daniel Lee and Alexander Six
alexzal & asix
CPSC 1020-001
11/24/15
Major Programming Assignment 3

This file contains all of the 
different functions used to
manipulate the vectors used in
creating the image
*/

#include "vector.h"

/*  This file contains function definitions for    */
/*  for 3D vector operations                       */

/* Compute the sum of two vectors */
/* v3 = v2 + v1; */
void vec_sum(
vec_t *v1,   /* addend */
vec_t *v2,   /* addend */
vec_t *v3)   /* result */
{
	v3->x=v2->x+v1->x;
	v3->y=v2->y+v1->y;
	v3->z=v2->z+v1->z;
}

/* Compute the difference of two vectors */
/* v3 = v2 - v1 */
void vec_diff(
vec_t *v1,   /* subtrahend */
vec_t *v2,   /* minuend    */
vec_t *v3)   /* result     */
{
	v3->x=v2->x-v1->x;
	v3->y=v2->y-v1->y;
	v3->z=v2->z-v1->z;  
}

/* Compute the componentwise product of two vectors */
/* v3 = v2 * v1; */
void vec_mult( 
vec_t *v1,   /* Input vector 1 */
vec_t *v2,   /* Input vector 2 */
vec_t *v3)   /* result         */
{
	v3->x=v2->x*v1->x;
	v3->y=v2->y*v1->y;
	v3->z=v2->z*v1->z;

}

/* Return the (dot) inner product of two input vectors     */
double vec_dot(
vec_t *v1,   /* Input vector 1 */
vec_t *v2)   /* Input vector 2 */
{
	double dprod=((v1->x*v2->x)+(v1->y*v2->y)+(v1->z*v2->z));
	return dprod;
}

/* Return length of a 3d vector */
double vec_len(
vec_t *v1)  /* Vector whose length is desired */
{
	double len=sqrt(vec_dot(v1,v1));
	return len;
}

/* Scale a 3d vector by multiplying each element by scale factor */
void vec_scale(
double fact,   /* Scale factor */
vec_t *v1,      /* Input vector */
vec_t *v2)      /* Output vector */
{
v2->x=fact*v1->x;
v2->y=fact*v1->y;
v2->z=fact*v1->z;
}

/* Construct a unit vector v2 in direction of input vector v1 */
void vec_unit(
vec_t *v1,    /* Input vector      */
vec_t *v2)    /* output unit vec   */
{
	//double len=(vec_len(*v1));
	//v2->x=v1->x(1/len);
	//v2->y=v1->y(1/len);
	//v2->z=v1->z(1/len);
	double scale = 1.0 / vec_len(v1);
        vec_scale(scale, v1, v2);
}

/* Copy vector v1 to v2 another */
void vec_copy(
vec_t *v1,   /* input vector */
vec_t *v2)   /* output vector */
{
	v2->x=v1->x;
	v2->y=v1->y; 
	v2->z=v1->z;    
}


/* Read in values of vector from file */
/* Return the # of items read	*/
int vec_read(
FILE *in,     /* input file  */
vec_t *v1)    /* vector from input file */
{
	int count=fscanf(in, "%lf%lf%lf ", &v1->x, &v1->y, &v1->z);
	assert(count==3);
	return count;
}

/* Print values of vector to file */
void vec_print(
FILE *out,     /* output file */
char *label,   /* label string */
vec_t *v1)      /* vector to print */
{
	fprintf(out, "%s %8.3lf %8.3lf %8.3lf\n", label, v1->x, v1->y, v1->z);
}

void vec_cross(vec_t *v1, vec_t *v2, vec_t *v3){

        vec_t temp;
        temp.x=((v1->y * v2->z)-(v1->z * v2->y));
        temp.y=((v1->z * v2->x)-(v1->x * v2->z));
        temp.z=((v1->x * v2->y)-(v1->y * v2->x));
        vec_copy(&temp,v3);
}

/* project a vector onto a plane */
void vec_project(
        vec_t *n,       /* plane normal     */
        vec_t *v,       /* input vector     */
        vec_t *p){      /* projected vector */
        double dprod;
        vec_t temp;

        dprod=vec_dot(n,v);

        vec_scale(dprod,n,&temp);
        vec_diff(&temp,v,p);

}

/* reflect a vector from a surface plane */
void vec_reflect(
        vec_t *n,       /* surface normal      */
        vec_t *w,       /* incoming ray vector */
        vec_t *v){      /* reflected vector    */

        vec_t  u, temp;

        vec_scale(-1.0,w,&u);

        double dprod;
        dprod=vec_dot(&u,n);


        dprod=dprod*2;

        vec_scale(dprod,n,&temp);
        vec_diff(&u,&temp,v);
}


void vec_xform(
        mtx_t *m,
        vec_t *v1,
        vec_t *v2){

        vec_t tempx=m->row[0];
        vec_t tempy=m->row[1];
        vec_t tempz=m->row[2];

        vec_t v2temp;
        v2temp.x = vec_dot(&tempx,v1);
        v2temp.y = vec_dot(&tempy,v1);
        v2temp.z = vec_dot(&tempz,v1);

        vec_copy(&v2temp,v2);
}

void mtx_transpose(
        mtx_t *m1,
        mtx_t *m2){

        mtx_t m3;

        m3.row[0].x=m1->row[0].x;
        m3.row[0].y=m1->row[1].x;
        m3.row[0].z=m1->row[2].x;

        m3.row[1].x=m1->row[0].y;
        m3.row[1].y=m1->row[1].y;
        m3.row[1].z=m1->row[2].y;

        m3.row[2].x=m1->row[0].z;
        m3.row[2].y=m1->row[1].z;
        m3.row[2].z=m1->row[2].z;

        vec_copy(&m3.row[0],&m2->row[0]);
        vec_copy(&m3.row[1],&m2->row[1]);
        vec_copy(&m3.row[2],&m2->row[2]);
}
