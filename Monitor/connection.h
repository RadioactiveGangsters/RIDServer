#ifndef RIDSCONN_H
#define RIDSCONN_H

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "../System/Log.h"

int socklisten(void);

#endif
