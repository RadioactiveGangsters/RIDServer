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
Trie*travp(Trie*const root,char const*const id);
Trie*trav(Trie*const root,char const*const id);
Trie*trieadd(Trie*const,char*const,void*const);
Trie*trielink(Trie*const root,Trie*const e);
Trie*trierm(Trie*const root,char const*const id);
void fortrie(Trie*const,void(*)(Trie*const));
int countTrie(Trie*const trie);
void*findinTrie(Trie const*const table,char const*const name);


#endif
