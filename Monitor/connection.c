#include "connection.h"

int socklisten()
{
    #ifdef _WIN32
    Sleep(100000000);
    #else
    int server_sockfd, client_sockfd;
    socklen_t server_len, client_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;


    server_sockfd = socket( AF_INET, SOCK_STREAM, 0 );
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr( "127.0.0.1" );
    server_address.sin_port = htons( 10000 );

    server_len = (socklen_t)sizeof( server_address );

        if( bind( server_sockfd, ( struct sockaddr *)&server_address, server_len ) != 0 )
        {
                perror("oops: server-tcp-single");
                return EXIT_FAILURE;
        }

    if( listen( server_sockfd, 5 ) ) return EXIT_FAILURE;

    if( signal( SIGCHLD, SIG_IGN ) == SIG_ERR ) return EXIT_FAILURE;

    while( 1 )
    {
        char ch;
        Log(LOGL_SYSTEM_ACTIVITY,"server wait...\n" );

        client_len = (socklen_t)sizeof( client_address );
        client_sockfd = accept( server_sockfd, ( struct sockaddr *)&client_address, &client_len );

        Log(LOGL_CLIENT_ACTIVITY, "Client connected \n" );

        if( fork() == 0 )
        {
            if( read( client_sockfd, &ch, 1 ) ) break; 
            Log(LOGL_DEBUG, "Client send = %c\n", ch );

            ch++;

            if( sleep( 5 ) ) break;

            Log(LOGL_DEBUG, "Server send = %c\n", ch );
            if( write( client_sockfd, &ch, 1 ) ) continue;
            (void)close( client_sockfd );
            return EXIT_SUCCESS;
        }
        else
            (void)close( client_sockfd );

    }
    #endif
    return EXIT_FAILURE;
}
