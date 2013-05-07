#ifndef TRIE_H
#define TRIE_H

#include<string.h>

typedef struct _Trie
{
	char id[128];
	void*e;
	struct _Trie*l;
	struct _Trie*g;
} Trie;

Trie triee(char id[128],void const*const e);

Trie*travp(Trie const*const root,char const*const id);

Trie*trav(Trie const*const root,char const*const id);


Trie*trieadd(Trie*const root,char const*const id,void const*const e);


Trie*trielink(Trie*const root,Trie const*const e);

Trie*trierm(Trie*const root,char const*const id);
#endif
