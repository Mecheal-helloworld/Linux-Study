#include "header.h"


static volatile int g_run = 1;

void sig_handler( int sig )
{
	if ( sig == SIGTERM || sig == SIGINT )
	{
		g_run = 0;
	}
}


int daemonize()
{
	int i, fd;
	pid_t pid;

	fd = open( "/dev/zero", O_RDWR );
	dup( fd );
	dup( fd );

	/* 是否已启动 */
	if ( ( fd = open( "/var/lock/test.lck", O_RDWR | O_CREAT | O_CLOEXEC | O_NOCTTY, 0666 ) ) == -1 || \
	        flock( fd, LOCK_EX | LOCK_NB ) == -1 )
	{
		fprintf( stderr, "Another Instance is Exist... Quit now!\n" );
		return -1;
	}

	/* 避免僵尸进程 */
	signal( SIGCHLD, SIG_IGN );

	/* 父进程退出, 新进程一定不是所在进程组的头领进程 */
	/* 非头领进程才可执行setsid, 创建新会话 */
	if ( ( pid = fork() ) < 0 )
	{
		perror( "fork" );
		return -1;
	}
	else if ( pid > 0 )
	{
		exit( 0 );
	}

	/* 子进程创建新会话, 成为整个会话的头领进程, 无控制终端 */
	setsid();

	/* 会话头领进程会再次获得终端, 进行二次fork, 生成新进程一定非头领进程 */
	/* 在fork前先屏蔽SIGHUP, 否则当会话头领进程退出, 会话所有子进程会收到该信号 */
	signal( SIGHUP, SIG_IGN );
	if ( ( pid = fork() ) < 0 )
	{
		return -1;
	}
	else if ( pid > 0 )
	{
		exit( 0 );
	}

	/* 屏蔽信号 */
	for ( i = 1; i <= SIGRTMAX; i ++ )
	{
		signal( i, SIG_IGN );
	}

	/* 设置为根目录, 无法卸载 */
	chdir( "/" );

	/* 清文件创建屏蔽字 */
	umask( 0 );

	/* 关闭文件描述符 */
	for ( i = 0; i < 1024; i ++ )
	{
		if ( i != fd )
		{
			close( i );
		}
	}

	/* 重定向输入、输出、错误三个流为日志 */
	open( "/var/log/test.log", O_RDWR | O_CREAT | O_TRUNC | O_CLOEXEC | O_NOCTTY, 0666 );
	dup( 0 );
	dup( 0 );

	return 0;
}

int main()
{
	int i;

	if ( daemonize() == -1 )
	{
		fprintf( stderr, "Hang up!!!!!\n" );
		return -1;
	}


	signal( SIGINT, sig_handler );
	signal( SIGTERM, sig_handler );


	i = 0;
	while ( g_run )
	{
		usleep( 50000 );
		if ( i ++ < 80 )
		{
			printf( "." );
		}

		if ( i == 80 )
		{
			i = 0;
			printf( "\n" );
			fflush( stdout );
		}
		
	}

	printf( "Sytem Quit!\n" );
	return 0;
}
