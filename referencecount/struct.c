#include <stdio.h>
#include <stdlib.h>

struct priv_data
{
	char str[12];
	int nr;
};

struct priv_data *alloc( void )
{
	return ( struct priv_data * ) malloc( sizeof( struct priv_data ) );
}

int main( void )
{
	int a = 5;
	struct priv_data *p_data;
	void *p;
	int *b;

		p_data = alloc();
		p = ( void * )p_data;
		p_data->nr = 5;
		b = 1000;

		p = &a;
		*(int *)p = 3;
		printf( "Void: %d\n", a);
		printf( "Void: %d\n", *( int *)p);

		return 0;
}
