#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main( void )
{
	int i;
	char *buff;
	char *t_buff;
        int size = sizeof( "Line ( 1 )\n" );

		buff = ( char * ) malloc ( 1024 );
                buff2 = ( char * ) malloc ( 14 );
           
                for( i = 0 ; i < 10 ; i++ )
                {
                        snprintf( t_buff, size, "Line ( %d )\n", i );
                        printk( "tbuff-> %s\n", t_buff );

                        strcat( buff, t_buff );
                        printf( "Kbuff-> %s\n", buff );
                }


		printf( "Buff1-> %s", buff );
		free( buff );
		free( t_buff );


		return 0;
}
	
		
