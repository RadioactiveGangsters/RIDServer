#include"Trie.h"

Trie triee(char id[static 1],void const*const e)
{
	Trie x;
	strcpy(x.id,id);
	x.e=e;
	x.l=NULL;
	x.g=NULL;
	return x;
}

Trie*travp(Trie const*const root,char const*const id)
{
	//can't traverse rootless trie
	if(root==NULL)
		return NULL;
	const int diff=strcmp(root->id,id);
	// Can't traverse to parent of root.
	if(!diff)
		return NULL;
	Trie**hook;
	// Find the one closest to the id
	if(diff<0)
	{
		hook=&root->l;
	}
	else
	{
		hook=&root->g;
	}
	// Check if we're on the parent right now
	if(!strcmp((*hook)->id,id))
		// This is the parent
		return root;
	else
		// Look for the parent some more.
		return travp(*hook,id);
}

Trie*trav(Trie const*const root,char const*const id)
{
	if(root==NULL)
		return NULL;
	const int diff=strcmp(root->id,id);
	if(!diff)
		return NULL;
	if(diff<0)
	{
		if(root->l==NULL)
			return root;
		else
			return trav(root->l,id);
	}
	else
	{
		if(root->g==NULL)
			return root;
		else
			return trav(root->g,id);
	}
}



Trie*trieadd(Trie*const root,char const*const id,void const*const e)
{
	Trie x=triee(id,e);
	return trielink(root,&x);
	// TODO: make sure x doesn't get deallocated after scope
}

Trie*trielink(Trie*const root,Trie const*const e)
{
	if(root==NULL)
	{
		if(e==NULL)
			return NULL;
		return*root=*e,root;
	}
	Trie*const hook=trav(root,e->id);
	if(hook==NULL)
		return NULL;
	const int diff=strcmp(hook->id,e->id);
	if(!diff)
		return NULL;
	if(diff<0)
		return hook->l=e;
	else
		return hook->g=e;
	
}

Trie*trierm(Trie*const root,char const*const id)
{
	// Can't remove from a NULL trie
	if(root==NULL)
		return NULL;
	
	Trie *const phook=travp(root,id);
	// No suitable parent found
	if(phook==NULL)
		return NULL;
	
	Trie** target;
	//seek the item itself
	if(!strcmp(phook->l->id,id))
		target=&phook->l;
	else if(!strcmp(phook->g->id,id))
		target=&phook->g;
	else
		return NULL;
	
	// Relink the tree so we don't lose nodes
	if(!trielink((*target)->g,(*target)->l)) return NULL;

	// Remove the item from the trie
	return *target=(*target)->g;
}
