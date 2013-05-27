#ifndef TRIE_H
#define TRIE_H

#include <string.h>
#include <stdlib.h>

#define RTRIE_HNAMELEN 128

typedef struct _Trie
{
	char id[RTRIE_HNAMELEN];
	void*e;
	struct _Trie*l;
	struct _Trie*g;
} Trie;

Trie*triee(char*const id,void*const);

Trie*const travp(Trie*const root,char const*const id);

Trie*const trav(Trie*const root,char const*const id);


Trie*trieadd(Trie*const,char*const,void*const);


Trie*trielink(Trie*const root,Trie*const e);

Trie*trierm(Trie*const root,char const*const id);
#endif
