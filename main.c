#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"

int main(int argc, char **argv) {

	screen s;
	struct matrix * edges;
	struct matrix * transform;

	clear_screen(s);

	edges = new_matrix(4, 0);
	transform = new_matrix(4, 4);
	ident(transform);

	if ( argc == 2 )
	parse_file( argv[1], transform, edges, s );
	else
	parse_file( "stdin", transform, edges, s );

	//print_matrix(edges);

	free_matrix( edges );
	free_matrix( transform );
}
