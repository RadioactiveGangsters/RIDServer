#include"Trie.h"

Trie*triee(char*const id,void*const e)
{
	if(!id||!e)return NULL;
	{
		Trie*x=malloc(sizeof(Trie));
		if(!x)return x;
		strncpy(x->id,id,sizeof(char)*RTRIE_HNAMELEN);
		x->e=e;
		// has no braches yet
		x->l=NULL;
		x->g=NULL;
		return x;
	}
}

Trie*travp(Trie*const root,char const*const id)
{
	//can't traverse rootless trie or find no id
	if(!root||!id)
		return NULL;
	{
		Trie*const*hook;
		const int diff = strcmp(root->id,id);
		// Can't traverse to parent of root.
		if(!diff)
			return NULL;
		// Find the one closest to the id
		if(diff<0)
		{
			// lesser branch is closer
			hook=&(root->l);
		}
		else
		{
			// greater branch is closor
			hook=&(root->g);
		}
		// Check if we're on the parent right now
		if(!strcmp((*hook)->id,id))
			// This is the parent
			return root;
		else
			// Look for the parent some more.
			return travp(*hook,id);
	}
}

Trie*trav(Trie*const root,char const*const id)
{
	if(root==NULL)
		return NULL;

	{
		const int diff=strcmp(root->id,id);
		// no difference, found it.
		if(!diff)
			return root;
		// which branch brings us closer?
		if(diff<0)
		{
			// left branch?
			if(root->l==NULL)
				// this is the closest we can get
				return root;
			else
				return trav(root->l,id);
		}
		else
		{
			// right branch
			if(root->g==NULL)
				// this is the closest we can get
				return root;
			else
				return trav(root->g,id);
		}
	}
}


Trie*trieadd(Trie*const root,char*const id,void*const e)
{
	return trielink(root,triee(id,e));
}

Trie*trielink(Trie*const root,Trie*const e)
{
	if(root==NULL)
	{
		// no point in linking null to null
		if(e==NULL)
			return NULL;
		// make e the root and return it.
		return*root=*e,root;
	}
	{
		// find the Trie with the id the closest to our new link
		Trie*const hook=trav(root,e->id);
		// There is none?
		if(hook==NULL)
			// can't link
			return NULL;
		// see where to link to;
		{
			const int diff=strcmp(hook->id,e->id);
			if(!diff)
				// element already present
				return NULL;
			// link left or right.
			if(diff<0)
				return hook->l=e;
			else
				return hook->g=e;
		}
	}
}

Trie*trierm(Trie*const root,char const*const id)
{
	// Can't remove from a NULL trie
	if(root==NULL)
		return NULL;
	{
		Trie *const phook=travp(root,id);
		Trie** target;
		// No suitable parent found
		if(phook==NULL)
			return NULL;
		
		
		//seek the item itself
		if(!strcmp(phook->l->id,id))
			target=&phook->l;
		else if(!strcmp(phook->g->id,id))
			target=&phook->g;
		else
			// not present?
			return NULL;
		
		// Relink the tree so we don't lose nodes
		if(!trielink((*target)->g,(*target)->l)) return NULL;

		// Remove the item from the trie
		return *target=(*target)->g;
	}
}

void fortrie(Trie*const trie,void(*cb)(Trie*const))
{
	if(!trie)return;
	fortrie(trie->l,cb);
	fortrie(trie->g,cb);
	cb(trie);
}

int countTrie(Trie*const trie)
{	
	if(!trie)return 0;
	return 1 + countTrie(trie->l) + countTrie(trie->g);
}

void*findinTrie(Trie const*const table,char const*const name)
{
	if(!table)return NULL;
	if(!name)return NULL;
	{
	const int diff = strcmp(table->id,name);

	if(!diff)
		return table->e;
	
	if(diff<0)
		return findinTrie(table->l,name);
	else
		return findinTrie(table->g,name);
	}
}
