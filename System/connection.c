#include <connection.h>

int main()
{
    int server_sockfd, client_sockfd;
    int server_len, client_len;

    struct sockaddr_in server_address;
    struct sockaddr_in client_address;


    server_sockfd = socket( AF_INET, SOCK_STREAM, 0 );
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr( "127.0.0.1" );
    server_address.sin_port = htons(32000);

    server_len = sizeof( server_address );

     if( bind( server_sockfd, ( struct sockaddr *)&server_address, server_len ) 	!= 0 )
        {
                perror("oops: server-tcp-single");
                exit( 1 );
        }

    listen( server_sockfd, 5 );

    signal( SIGCHLD, SIG_IGN );

    while( 1 )
    {
        char ch;
        printf( "server wait...\n" );

        client_len = sizeof( client_address );
        client_sockfd = accept( server_sockfd, ( struct sockaddr *)&client_addre	ss, &client_len );

        printf( "Client connected \n" );

        if( fork() == 0 )
        {
            read( client_sockfd, &ch, 1 );
            printf( "Client send = %c\n", ch );

            ch++;

            sleep( 5 );

            printf( "Server send = %c\n", ch );
            write( client_sockfd, &ch, 1 );
            close( client_sockfd );
            exit (0 );
        }
        else
	{	
            close( client_sockfd );
    	}	
    }
