/* 

Alexzander Daniel Lee and Alexander Six
alexzal & asix
CPSC 1020-001
11/24/15
Major Programming Assignment 3

This file contains all of the 
different functions involved in
the creation of the image
*/


#include "ray.h"


//=============================================================================
// ******* ray_trace( ) *******
// 
// 
//=============================================================================
void ray_trace (
   	model_t *model,
   	vec_t *base,                // view point
   	vec_t *dir,                 // unit direction vector
   	drgb_t *pix,                // pixel return location
  	double total_dist,          // distance ray has travelled so far
   	object_t *last_hit)         // most recently hit object
{
	double specref = 0.0;
	double mindist;
	drgb_t thispix = {0.0, 0.0, 0.0};
	object_t *closest=(object_t *)malloc(sizeof(object_t *));
	vec_t *ref_dir=(vec_t *)malloc(sizeof(vec_t *));
	drgb_t specint = {0.0, 0.0, 0.0};
	drgb_t specintholder = {0.0,0.0,0.0};
	vec_t *lastnormal=(vec_t *)malloc(sizeof(vec_t *));
	vec_t *hitpt=(vec_t *)malloc(sizeof(vec_t *));
	double trans;

	if(total_dist > MAX_DIST){
		return;
	}
	closest= find_closest_object(model, base, dir, last_hit, &mindist);

	/* get the distance and the color */
	if(closest != NULL){
		closest->getlast_normal(lastnormal);
		closest->getlast_hitpt(hitpt);
		#ifdef DBG_HIT
		fprintf(stderr, "%-12s HIT:(%5.1lf, %5.1lf, %5.1lf)",
		closest->obj_name, closest->last_hit.x,
		closest->last_hit.y, closest->last_hit.z);
		#endif
		closest->getambient(&thispix);
		add_illuminate(model,base,closest,&thispix);	

		if(mindist>0){	
			total_dist=total_dist+mindist;
			pix_scale(1/total_dist,&thispix,&thispix);
			closest->gettrans(&trans);

			if(trans != 0.0 && &trans != NULL ){
				drgb_t transint = {0.0,0.0,0.0};
				drgb_t diffcolor = {0.0,0.0,0.0};
				double maxpix;
				pix_scale((1.0 - trans),&thispix,&thispix);
				ray_trace(model, hitpt, dir, &transint, 
					total_dist, closest);
				pix_scale(trans,&transint,&transint);
				add_illuminate(model, base, closest, &diffcolor);
				maxpix=diffcolor.r;
				if(maxpix<diffcolor.g){
					maxpix=diffcolor.g;
				}
				if(maxpix<diffcolor.b){
					maxpix=diffcolor.b;
				}
				maxpix=trans/maxpix;
				pix_scale(maxpix,&diffcolor,&diffcolor);
				pix_prod(&transint,&diffcolor,&transint);
				pix_sum(&thispix,&transint,&thispix);
			}
			closest->getspecular(&specref); 
			if(specref != 0){
				vec_reflect(lastnormal,dir,ref_dir);
				ray_trace(model, hitpt, ref_dir, &specint, 
					total_dist, closest);
				pix_scale(specref,&specint,&specintholder);
				pix_sum(&thispix,&specintholder,&thispix);
			}
			pix_sum(pix,&thispix,pix);	
		}
	}
	#ifdef DBG_DRGB
	fprintf(stderr, "%-12s DRGB:(%5.2lf, %5.2lf, %5.2lf)",
	closest->objname, dpix->r, dpix->g, dpix->b);	
	#endif  

}


//=============================================================================
// ******* make_pixel( ) *******
// 
// 
//=============================================================================
void make_pixel(model_t  *model, int  x, int  y) {
	vec_t raydir;
	drgb_t pix = {0.0, 0.0, 0.0};
	camera_t *cam = model->cam;
	vec_t viewpthldr;
	int i;

	cam->getviewpt(&viewpthldr);

	for(i=0;i<AA_SAMPLES;i++){
		cam->getdir(x, y, &raydir); 
		ray_trace(model, &viewpthldr, &raydir, &pix, 0.0, NULL);
	}

	pix_scale(1.0/ AA_SAMPLES, &pix, &pix);
	cam->store_pixel(x, y, &pix);

	return;
}
 

//=============================================================================
// ******* make_row( ) *******
// 
// 
//=============================================================================
void make_row(model_t  *model, int  y) 
{
	int x,HoldMe;
	camera_t *cam = model->cam;

	HoldMe=cam->getxdim();
	for(x=0;x<HoldMe;x++){
		make_pixel(model,x,y);
	}
}


//=============================================================================
// ******* image_create( ) *******
// 
// 
//=============================================================================
void image_create(model_t  *model, FILE  *out) 
{
	int y, HoldMeTghtr;
	camera_t *cam = model->cam;

	HoldMeTghtr=cam->getydim();

	for(y=0;y<HoldMeTghtr;y++){
		make_row(model,y);
	}

	/*  write the image                           */
	cam->camera_write_image(out);
}

