/* 

Alexzander Daniel Lee and Alexander Six
alexzal & asix
CPSC 1020-001
11/24/15
Major Programming Assignment 3

This file initializes the lights and
holds all of the functions having to do
with the lighting of the scenes
*/

#include "ray.h"


// for the generalized parsher
static pparm_t lgt_parse[] =
{
   {"emissivity",      3, sizeof(double), "%lf", 0},
   {"location",      3, sizeof(double), "%lf", 0},
};

#define NUM_ATTRS (sizeof(lgt_parse) / sizeof(pparm_t))

light_t::light_t(FILE *in, model_t *model, int attrmax)
{
	//parse the required attribute values
	char attrname[NAME_LEN];
	int count;
	int mask;

	/* Create a new material structure and initialize it */
	cookie = LGT_COOKIE;

	/* Read the descriptive name of the material */
	/* (dark_red, light_blue, etc.               */
	count = fscanf(in, "%s", name);
	assert(count == 1);

	count = fscanf(in, "%s", attrname);
	assert(*attrname == '{');

	lgt_parse[0].loc = &emissivity;
	lgt_parse[1].loc = &location;

	// calls the generic parser
	mask = parser(in, lgt_parse, NUM_ATTRS, 0);
	assert(mask != 0);

	//put the light_t class instance into the light list
	model->lgts->add((void *)this);
}	

void light_t::getemiss(drgb_t * emiss){
	pix_copy(&emissivity, emiss);
}

void light_t::getloc(vec_t *loc){
	vec_copy(&location,loc);
}

void light_t::printer(FILE *out){
	fprintf(out, "\n%-12s %s \n", "light", name);

	fprintf(out, "%-12s %5.1lf %5.1lf %5.1lf \n", "emissivity",
		emissivity.r, emissivity.g, emissivity.b);

	fprintf(out, "%-12s %5.1lf %5.1lf %5.1lf \n", "location",
		location.x, location.y, location.z);
}

void light_list_print(model_t *model, FILE *out){
	light_t *lgt;
	list_t *list = model->lgts;
	list->reset();
	while (list->not_end())
	{
		lgt = (light_t *)list->get_entity();
		lgt->printer(out);
		list->next_link();
	}
}

void light_t::illuminate(model_t *model, vec_t *base, 
object_t * hitobj, drgb_t *pixel){
	vec_t dir; // unit direction to light from hitpt
	object_t *obj; // closest object in dir to light
	double close; // dist to closest object in dir to light
	double cos=0; // of angle between normal and dir to light
	double dist=0; // to the light from hitpoint
	drgb_t diffuse = {0.0, 0.0, 0.0};
	vec_t *lasthit=(vec_t *)malloc(sizeof(vec_t *));
	vec_t *lastnorm=(vec_t *)malloc(sizeof(vec_t *));
	drgb_t emiss={0.0,0.0,0.0};
	this->getemiss(&emiss);
	object_t *baseobj = hitobj;

	//Compute distance from hit to the light and and a unit vector in the direction of the light from hitpt
	hitobj->getlast_hitpt(lasthit);

	vec_diff(lasthit, &this->location, &dir);
	dist=vec_len(&dir);
	vec_unit(&dir,&dir);

	hitobj->getlast_normal(lastnorm);

	//check the object for self occlusion and return if occluded	
	cos=vec_dot(&dir, lastnorm);
	double workdist = dist;
	if(cos<=0.0){
		return;
	}
	drgb_t *diffcolor=(drgb_t *)malloc(sizeof(drgb_t*));
	double *maxpix=NULL;

	obj=(object_t *) find_closest_object(model, lasthit, &dir, hitobj, &close);

	if(obj!=NULL && close<dist){
		return;
	}

	while(1){
		double trans=0;

		pix_copy(pixel,diffcolor);
		baseobj=(object_t *) find_closest_object(model, lasthit, &dir, baseobj, &close);

		if(baseobj==NULL || workdist>close){
			break;
		}
		baseobj->gettrans(&trans);

		if(trans==0.0){
			return;
		}
		baseobj->getdiffuse(diffcolor);
		*maxpix=diffcolor->r;

		if(*maxpix<=diffcolor->g){
			*maxpix=diffcolor->g;
		}

		if(*maxpix<=diffcolor->b){
			*maxpix=diffcolor->b;
		}

		pix_scale((trans/ *maxpix),diffcolor,diffcolor);
		pix_prod(&emiss,diffcolor,&emiss);
		baseobj=hitobj;
		workdist=dist; 
	}

	hitobj->getdiffuse(&diffuse);
	pix_scale((cos/workdist),&diffuse,&diffuse);
	pix_prod(&emiss,&diffuse,&diffuse);

	pix_sum(pixel,&diffuse,pixel);

	add_glint(hitobj, base, &dir, pixel);
}

void light_t::add_glint(
object_t *hitobj,
vec_t *base,
vec_t *dir,
drgb_t *pixel)
{
	vec_t last_hitpt, last_normal;
	vec_t tot_light, res_light;
	double shininess, specular, dot;
	drgb_t emiss, glint;

	hitobj->getshine(&shininess);
	hitobj->getlast_hitpt(&last_hitpt);
	hitobj->getlast_normal(&last_normal);
	hitobj->getspecular(&specular);

	if(shininess == 0){
		return;
	}

	//Find unit vector from object to viewpoint
	vec_diff(&last_hitpt, base, &res_light);
	vec_unit(&res_light, &res_light);

	//Find the sum of previous vectors
	vec_sum(dir, &res_light, &tot_light);
	vec_unit(&tot_light, &tot_light);

	//Get dot prod & raise to shininess (pow)
	dot = vec_dot(&tot_light, &last_normal);
	shininess = pow(dot, shininess);

	//Scale emissivity by dot times spec reflectivity
	pix_copy(&emissivity, &emiss);
	pix_scale((shininess*specular), &emiss, &glint);

	//Add scaled value to pixel
	pix_sum(&glint, pixel, pixel);
}


