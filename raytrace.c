/* 

Alexzander Daniel Lee and Alexander Six
alexzal & asix
CPSC 1020-001
11/24/15
Major Programming Assignment 3

This file contains functions
to find the closest object and
add the illumination onto objects
in the image
*/

#include "ray.h"


object_t *find_closest_object(model_t  *model, vec_t  *base, vec_t  *dir, 
object_t  *last_hit, double  *retdist)     {

	object_t *minobj = NULL;
	double mindist = -1.0;
	double dist = 0.0;
	object_t *obj;
	list_t *PixieStick=model->objs;
	PixieStick->reset();
	
	while(PixieStick->not_end()){
		obj=(object_t *)PixieStick->get_entity();
			if(obj != last_hit){	
				dist=obj->hits(base,dir);
				if(mindist<0 && dist!=-1){
					mindist=dist;
					minobj=obj;
				}
				if(dist<mindist && dist!=-1){
					mindist=dist;
					minobj=obj;
				}
                       }
		PixieStick->next_link();
        }
	// save the mindist to retdist 
	*retdist=mindist;

	// and return the minobj
	return minobj;
}

void add_illuminate(model_t *model, vec_t *base, 
object_t *hitobj, drgb_t *pixel ){
	light_t *light=NULL;
	list_t *list=NULL;

	list = model->lgts;
	list->reset();	

	while(list->not_end()){
		light=(light_t *)list->get_entity();
		light->illuminate(model,base,hitobj,pixel);
		list->next_link();
	}
}


