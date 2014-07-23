#include <stdio.h>
#include <stdlib.h>

#define offset( type, member ) ( (size_t) &( (type *)0)->member)

struct struct_1 {
	int a;
};

struct struct_2 {
	int b;
	int c;
	struct struct_1 s_1;
};

int main( void )
{
	struct struct_2 *s_2 = ( struct struct_2 * ) malloc ( sizeof( struct struct_2 ) );
	struct struct_2 *aux;
	struct struct_1 *p;

		s_2->b = 1;
		s_2->c = 2;
		s_2->s_1.a = 3;

		p = &(s_2->s_1);

		printf( "Pointers: p: %p | s_2->s_1: %p\n", p, &(s_2->s_1) );
		
//		aux = get_me_p();
		printf( "Offset: %d\n", &( ( struct struct_2 * )0 )->s_1 );
		
		printf( "Offset: %d\n", offset( struct struct_2, s_1 ));

		return 0;
}
		

		
