/*  == material.c  ==  */

material_t *material_getbyname(model_t  *, char  *);

void material_list_print(model_t  *, FILE  *);

/*  == object.c ==  */
void object_list_print(model_t  *, FILE  *);
object_t *find_closest_object(model_t  *model, vec_t  *base, vec_t  *dir, object_t  *last_hit, double  *retdist);

/*  == image.c == */
void ray_trace (
   model_t *model,
   vec_t *base,             //  view point
   vec_t *dir,              //  unit direction vector
   drgb_t *pix,             //  pixel return location
   double total_dist,       //  distance ray has travelled so far
   object_t *last_hitobj);  //  most recently hit object

void make_pixel(model_t  *model, int  x, int  y);

void make_row(model_t  *, int );

void image_create(model_t  *, FILE  *);

double randomize(double);


/*==  parser.c ==  */
int parser(
FILE    *in,
pparm_t *pct,         /* parser control table                */
int      numattrs,    /* number of legal attributes          */
int      attrmax);    /* Quit after this many attrs if not 0 */

/*== light.cpp == */
void light_list_print(model_t *, FILE *);
void add_illuminate(model_t *, vec_t *, object_t *, drgb_t *);

