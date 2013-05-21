#ifndef RIDSCONN_H
#define RIDSCONN_H

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <signal.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "../System/Log.h"

int socklisten(void);

#endif
