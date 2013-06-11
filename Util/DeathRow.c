#include "DeathRow.h"

static void DestroyAutoQe(AutoQ*const e)
{
	free(e->e);
	free(e);
}

void forautoq(AutoQ*const q,void(*cb)(AutoQ*const))
{
	forautoq(q->n,cb);
	cb(q->e);
}

void DestroyAutoQ(AutoQ*const q)
{
	forautoq(q,&DestroyAutoQe);
}

static AutoQ*_AutoQe(void*const e,/*@null@*/AutoQ*const n,unsigned int const space)
{
	AutoQ a={
		.e=e,
		.n=n,
		.space=space-1,
	},*p=malloc(sizeof*p);
	if(p)memcpy(p,&a,sizeof*p);
	return p;
}

AutoQ*AutoQe(void*const e,unsigned int const space)
{
	return _AutoQe(e,NULL,space);
}

static AutoQ*ale(AutoQ*const r)
{
	// return the next-to-last element or r itself.
	return (!r||!r->n||!r->n->n)?r:ale(r->n);
}

AutoQ*AutoQadd(AutoQ*q,void*const e)
{
	// cannot add to nonthing and cannot add nothing.
	if(!q||!e) return NULL;

	// no space left?
	if(q->space==0)
	{
		/*@-mustfreefresh@*/AutoQ*le=ale(q);

		/*

			if the list was  0 elements in length, q was null (hopefully)
			if the list was  1 element  in length, ale returend q, and le should be freed.
			if the list was  2 elements in length, ale returned q, and le->n should be freed.
			if the list was  3 elements in length, ale returned q->n, and le->n. should be freed.
			if the list was >3 elements in length, ale returned q->...->n and le->n should be freed.
		
		*/

		// q :. le
		// is last element?
		if(!le->n)
		{
			// remove the last element
			DestroyAutoQe(le);
			// has no reference other than q;
			q=NULL;

		}
		// q :. le + ! le->n :. le->n->n + ! le->n->n->n
		else
		{
			// remove the last element
			DestroyAutoQe(le->n);
			// remove its reference
			le->n=NULL;
		}
		le=NULL;
	}

	return q=_AutoQe(e,q,(!!q&&(q->space>0))?q->space--:0);
}
