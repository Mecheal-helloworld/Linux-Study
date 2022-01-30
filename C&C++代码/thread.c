#include "header.h"

#define THREAD_POOL_SIZE       ( ( const int )20 )


static volatile int g_run = 1;
static volatile int g_pool_run = 1;
static volatile int g_send = 0;
static pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_cond = PTHREAD_COND_INITIALIZER;

static void sig_handler( int sig )
{
	if ( sig == SIGTERM )g_run = 0;
}

static void task_dispatch( int sig )
{
	if ( sig == SIGINT )
	{
		pthread_mutex_lock( &g_mutex );
		g_send ++;
		pthread_mutex_unlock( &g_mutex );
		pthread_cond_broadcast( &g_cond );
		usleep( 10 );
	}
}

void* thr_routine( void* arg )
{
	int idx;

	idx = *( int* )arg;

	while ( 1 )
	{
		pthread_mutex_lock( &g_mutex );
		while ( g_pool_run == 1 && g_send == 0 )
		{
			pthread_cond_wait( &g_cond, &g_mutex );
		}
		if ( g_pool_run == 0 )
		{
			pthread_mutex_unlock( &g_mutex );
			break;
		}
		pthread_mutex_unlock( &g_mutex );

		printf( "%d-th thread service you!\n", idx );
		usleep( 20000 );

		pthread_mutex_lock( &g_mutex );
		g_send --;
		pthread_mutex_unlock( &g_mutex );
	}

	return NULL;
}

int main()
{
	int i, j;
	pthread_t tid[THREAD_POOL_SIZE];

	signal( SIGINT, task_dispatch );
	signal( SIGTERM, sig_handler );

	g_run = 1;
	g_send = 0;

	for ( i = 0; i < THREAD_POOL_SIZE; i ++ )
	{
		if ( pthread_create( tid + i, NULL, thr_routine, &i ) != 0 )
		{
			g_run = 0;
			pthread_cond_broadcast( &g_cond );
			for ( j = 0; j < i; j ++ )pthread_join( tid[j], NULL );
			return -1;
		}
	}

	while ( g_run )usleep( 10000 );

	g_pool_run = 0;
	pthread_cond_broadcast( &g_cond );
	for ( i = 0; i < THREAD_POOL_SIZE; i ++ )pthread_join( tid[i], NULL );

	printf( "System Will Quit ...\n" );

	return 0;
}
