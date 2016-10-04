/* 

Alexzander Daniel Lee and Alexander Six
alexzal & asix
CPSC 1020-001
11/24/15
Major Programming Assignment 3

This program utilizes the functions listed below to 
initialize a camera object, load in attribute from 
a file and convert the image to do ray tracing. 
Then the program exports the results to a ppm and a log file.
*/


#include "ray.h"

pparm_t camera_parse[] =
{
	{"pixeldim",  2, sizeof(int),    "%d",  0},
	{"worlddim",  2, sizeof(double), "%lf", 0},
	{"viewpoint", 3, sizeof(double), "%lf", 0}
};
#define NUM_ATTRS (sizeof(camera_parse) / (sizeof(pparm_t)))


camera_t::camera_t(FILE *in){
	
	//declare variables and alllocate necessary space
	int mask, x, y;

	char buf[256];

	assert (fscanf(in, "%s", name) == 1);

	fscanf(in, "%s", buf);
	assert(buf[0] == '{');

	//set the value of cookie in cam equal to the global value in ray.h
	cookie=CAM_COOKIE;

	//load camera attributes and assert that three were loaded correctly.
	camera_parse[0].loc = &pixel_dim;
	camera_parse[1].loc = &world_dim;
	camera_parse[2].loc = &view_point;

	mask = parser(in, camera_parse, NUM_ATTRS, 0);
	assert(mask==7);	
	//used to simplify math in pixmap allocation
	x=pixel_dim[0];
	y=pixel_dim[1];

	//allocated enough pixels for the pixmap in the cam object.
	pixmap=(irgb_t *)malloc(sizeof(irgb_t) * x * y);
	
}

//=============================================================================
// ******* camera_print( ) *******
//
// will print to the file specified by "out"
// 	the attributes read in from camera.txt 
//=============================================================================
void camera_t::camera_print (FILE *out) {
	assert(cookie==CAM_COOKIE);

	// print out the values in cam to their respective locations 
	// and format the log file collums correctly.
	fprintf(out, "%-12s %-5s\n","camera",name);

	fprintf(out,"%-12s","pixeldim");
	fprintf(out,"%5.1d %5.1d\n", pixel_dim[0], pixel_dim[1]);

	fprintf(out,"%-12s","worlddim");
	fprintf(out,"%5.1lf %5.1lf %5.1lf\n", world_dim[0], 
		world_dim[1], world_dim[2]);

	fprintf(out,"%-12s","viewpoint");
	fprintf(out,"%5.1lf %5.1lf %5.1lf\n\n", view_point.x, 
		view_point.y, view_point.z);
}


//=============================================================================
// ******* getdir( ) *******
// 
// computes the unit vector from the viewpoint to the 
// 	(x,y) pixel coordinates passed in as parameters
// vec_t *uvec is the resulting unit vector 
//=============================================================================
void camera_t::getdir(int x, int y, vec_t *dir) {
	//declare variables
	vec_t vec;
	vec_t world;
	double dx = x;
	double dy = y;

	if(AA_SAMPLES >=1){
		dx = randomize(dx);
		dy = randomize(dy);
	}

	// convert pixel to world coordinates
	world.x=(world_dim[0]) * ((double)dx)/(pixel_dim[0]-1); 
	world.y=(world_dim[1]) * ((double)dy)/(pixel_dim[1]-1);
	world.z=0;

	// take the difference in the vectors
	vec.x=world.x-view_point.x;
	vec.y=world.y-view_point.y;
	vec.z=world.z-view_point.z;

	//make a unit vecotr out of the calulated vector
	vec_unit(&vec,dir);
}

double randomize(double a){
	double temp;
	temp = random();
	temp = temp / (0x7fffffff);
	temp = temp - 0.5;
	temp = a + temp;
	return temp;
}

//=============================================================================
// ******* store_pixel( ) *******
//
// converts a pixel from drgb_t to irgb_t and
// 	stores it in the image associated with the camera 
//=============================================================================
void camera_t::store_pixel(int x, int y, drgb_t *pix) {
	// declare and initialize variables
	int col;
	int row;
	irgb_t *ipix=NULL;
	// change origin point from top left to bottom left
	row=pixel_dim[1]-y-1;
	col=x;
	//simple math to set up ipix
	ipix=pixmap +(row*pixel_dim[0])+col;
	//Simple math operation to convert drgbt to irgbt
	pix->r=(pix->r*255.0) +.5;
	pix->g=(pix->g*255.0) +.5;
	pix->b=(pix->b*255.0) +.5;
	//scale and clamp the rgb values
	if(pix->r<0.0){
		pix->r=0.0;
	}
	if(pix->g<0.0){
		pix->g=0.0;
	}
	if(pix->b<0.0){
		pix->b=0.0;
	}
	if(pix->r>255.0){
		pix->r=255.0;
	}
	if(pix->g>255.0){
		pix->g=255.0;
	}
	if(pix->b>255.0){
		pix->b=255.0;
	}

	// put pix value into ipix
	ipix->r=pix->r;
	ipix->g=pix->g;
	ipix->b=pix->b;
}


//=============================================================================
// ******* camera_write_image( ) *******
//
// uses the fprintf( ) to write the .ppm header
// and fwrite() to write the image to the file
//=============================================================================
void camera_t::camera_write_image(FILE *out) {

	fprintf(out,"P6\n %d %d 255\n",pixel_dim[0], pixel_dim[1]);

	fwrite(pixmap,sizeof(irgb_t),((pixel_dim[0])*(pixel_dim[1])), out);

}

int camera_t::getxdim(){
	return pixel_dim[0];
}

int camera_t::getydim(){
	return pixel_dim[1];
}

void camera_t::getviewpt(vec_t *view){
	*view=view_point;
}

void camera_t::getpixsize(double *x, double *y){
	*x=world_dim[0];
	*y=world_dim[1];
}
