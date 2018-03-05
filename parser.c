#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"


/*======== void parse_file () ==========
Inputs:   char * filename
struct matrix * transform,
struct matrix * pm,
screen s
Returns:

Goes through the file named filename and performs all of the actions listed in that file.
The file follows the following format:
Every command is a single character that takes up a line
Any command that requires arguments must have those arguments in the second line.
The commands are as follows:
line: add a line to the edge matrix -
takes 6 arguemnts (x0, y0, z0, x1, y1, z1)
ident: set the transform matrix to the identity matrix -
scale: create a scale matrix,
then multiply the transform matrix by the scale matrix -
takes 3 arguments (sx, sy, sz)
translate: create a translation matrix,
then multiply the transform matrix by the translation matrix -
takes 3 arguments (tx, ty, tz)
rotate: create an rotation matrix,
then multiply the transform matrix by the rotation matrix -
takes 2 arguments (axis, theta) axis should be x y or z
apply: apply the current transformation matrix to the
edge matrix
display: draw the lines of the edge matrix to the screen
display the screen
save: draw the lines of the edge matrix to the screen
save the screen to a file -
takes 1 argument (file name)
quit: end parsing

See the file script for an example of the file format


IMPORTANT MATH NOTE:
the trig functions int math.h use radian mesure, but us normal
humans use degrees, so the file will contain degrees for rotations,
be sure to conver those degrees to radians (M_PI is the constant
for PI)
====================*/
void parse_file ( char * filename,
	struct matrix * transform,
	struct matrix * edges,
	screen s) {

		FILE *f;
		char line[256];
		clear_screen(s);

		color c;
		c.red = MAX_COLOR;
		c.green = MAX_COLOR;
		c.blue = MAX_COLOR / 2;

		if ( strcmp(filename, "stdin") == 0 )
		f = stdin;
		else
		f = fopen(filename, "r");

		while ( fgets(line, 255, f) != NULL ) {
			int i;

			line[strlen(line)-1]='\0';
			printf(":%s:\n",line);

			//print_matrix(transform);
			//print_matrix(edges);

			if(!strcmp(line, "line")){
				char * args = (char *)malloc(256);
				fgets(args, 255, f);
				args[strlen(args)-1]='\0';
				double arg_array[6];
				for(i = 0; i < 5; i++){
					arg_array[i] = atof(strsep(&args, " "));
				}
				add_edge(
					edges,
					arg_array[0],
					arg_array[1],
					arg_array[2],
					arg_array[3],
					arg_array[4],
					arg_array[5]
				);
			}

			if(!strcmp(line, "ident")){
				ident(transform);
			}

			if(!strcmp(line, "scale")){
				char * args = (char *)malloc(256);
				fgets(args, 255, f);
				args[strlen(args)-1]='\0';
				double arg_array[3];
				for(i = 0; i < 2; i++){
					arg_array[i] = atof(strsep(&args, " "));
				}
				matrix_mult(
					make_scale(
						arg_array[0],
						arg_array[1],
						arg_array[2]
					),
					transform);
			}

			if(!strcmp(line, "move")){
				char * args = (char *)malloc(256);
				fgets(args, 255, f);
				args[strlen(args)-1]='\0';
				double arg_array[3];
				for(i = 0; i < 2; i++){
					arg_array[i] = atof(strsep(&args, " "));
				}
				matrix_mult(
					make_translate(
						arg_array[0],
						arg_array[1],
						arg_array[2]
					),
					transform);
			}

			if(!strcmp(line, "rotate")){
				char * args = (char *)malloc(256);
				fgets(args, 255, f);
				args[strlen(args)-1]='\0';
				//printf("args:%s:\n", args);
				char *axis = strsep(&args, " ");
				double theta = atof(strsep(&args, " "));
				//printf("axis:%s:\n", axis);

				if(!strcmp(axis, "x")){
					matrix_mult(make_rotX(theta), transform);
				}
				else if(!strcmp(axis, "y")){
					matrix_mult(make_rotY(theta), transform);
				}
				else{
					matrix_mult(make_rotZ(theta), transform);
				}
				//print_matrix(transform);
			}

			if(!strcmp(line, "apply")){
				matrix_mult(transform, edges);
				//print_matrix(transform);
			}

			if(!strcmp(line, "display")){
				clear_screen(s);
				draw_lines(edges, s, c);
				display(s);
			}

			if(!strcmp(line, "save")){
				char * args = (char *)malloc(256);
				fgets(args, 255, f);
				args[strlen(args)-1]='\0';
				save_extension(s, args);
			}

			if(!strcmp(line, "quit")){
				fclose(f);
				return;
			}
		}
		fclose(f);
		return;
	}

