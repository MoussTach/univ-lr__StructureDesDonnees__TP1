#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "list.h"

/*
 * creation of a node whose data size equal to size
 */
List
_list_create (const void *data, size_t size)
{
  List list;

  list = (List) malloc (sizeof (List) + size);
  if (list)
    {
      memcpy (list->data, data, size);
      list->next = NULL;
      return list;
    }
  return list;
}

List
list_new (void)
{
  return NULL;
}

size_t
list_length (List list)
{
  size_t length = 0;

  while (list)
    {
      length++;
      list = list->next;
    }
  return length;
}

int
list_is_empty (List list)
{
  return list == NULL;
}

int
list_remove_first (List * plist, void (*delete) (void *))
{
  List new_list;

  if (*plist)
    {
      new_list = (*plist)->next;
      if (delete)
	delete ((*plist)->data);
      free (*plist);
      *plist = new_list;
      return true;
    }
  else
    return false;
}

int
list_remove_last (List * plist, void (*delete) (void *))
{
  if (*plist)
    {
      while ((*plist)->next)
	plist = &(*plist)->next;
      if (delete)
	delete ((*plist)->data);
      free (*plist);
      *plist = NULL;
      return true;
    }
  else
    return false;
}

int
list_remove_nth (List * plist, size_t nth, void (*delete) (void *))
{
  while (*plist && nth)
    {
      plist = &(*plist)->next;
      nth--;
    }
  if (nth)
    return false;
  else
    return list_remove_first (plist, delete);
}

void
list_delete (List * plist, void (*delete) (void *))
{
  while (list_remove_first (plist, delete));
}

int
list_prepend (List * plist, const void *data, size_t size)
{
  List new_list, last;

  new_list = _list_create (data, size);

  if (new_list)
    {
      new_list->next = *plist;
      *plist = new_list;
      return true;
    }
  else
    return false;
}

int
list_append (List * plist, const void *data, size_t size)
{
  List new_list, last;

  new_list = _list_create (data, size);

  if (new_list)
    {
      if (*plist)
	list_last (*plist)->next = new_list;
      else
	*plist = new_list;
      return true;
    }
  else
    return false;
}

int
list_insert (List * plist,
	     const void *data,
	     size_t size,
	     size_t nth)
{
  while (*plist && nth)
    {
      nth--;
      plist = &(*plist)->next;
    }
  if (nth)
    return false;
  else
    return list_prepend (plist, data, size);
}

int
list_insert_sorted (List * plist,
		    const void *data,
		    size_t size,
		    int (*compare) (const void *, const void *))
{
  while (*plist && compare (data, (*plist)->data) > 0)
    plist = &(*plist)->next;
  return list_prepend (plist, data, size);
}

void *
list_first_data (List list)
{
  if (list)
    return list->data;
  else
    return NULL;
}


void *
list_last_data (List list)
{
  return list_first_data (list_last (list));
}

void *
list_nth_data (List list, size_t nth)
{
  return list_first_data (list_nth (list, nth));
}

/*
 * returns the pointer to the last node or NULL if there is no
 * nodes
 */
List
list_last (List list)
{
  if (list)
    {
      while (list->next)
	list = list->next;
    }

  return list;
}

List
list_nth (List list, size_t nth)
{
  while (list && nth)
    {
      list = list->next;
      nth--;
    }
  return list;
}

void *
list_find_data (List list,
		const void *data,
		int (*compare) (const void *, const void *))
{
  while (list && compare (data, list->data))
    list = list->next;
  return list_first_data (list);
}

int
list_index_data (
		  List list,
		  const void *data,
		  int (*compare) (const void *, const void *))
{
  int index = 0;
  while (list && compare (data, list->data))
    {
      list = list->next;
      index++;
    }
  if (list)
    return index;
  else
    return -1;
}

void
list_foreach (List list, void (*func) (void *, void *), void *extra_data)
{
  while (list)
    {
      func (list->data, extra_data);
      list = list->next;
    }
}

void
list_concat (List * plist, List add)
{
  List last = list_last (*plist);
  if (last)
    last->next = add;
  else
    *plist = add;
}

void
list_split (List * plist,
	    List * pbefore,
	    List * pafter,
	    const void *data,
	    int (*compare) (const void *, const void *))
{
  List tmp;

  *pbefore = list_new ();
  *pafter = list_new ();

  while (*plist)
    {
      if (compare ((*plist)->data, data) <= 0)
	{
	  tmp = *pbefore;
	  *pbefore = *plist;
	  *plist = (*plist)->next;
	  (*pbefore)->next = tmp;
	}
      else
	{
	  tmp = *pafter;
	  *pafter = *plist;
	  *plist = (*plist)->next;
	  (*pafter)->next = tmp;
	}
    }
}
void
list_sort (List * plist, int (*compare) (const void *, const void *))
{
  List before, after, middle;
  if (*plist)
    {
      middle = *plist;
      (*plist) = (*plist)->next;
      middle->next = NULL;
      list_split (plist, &before, &after, middle->data, compare);
      list_sort (&before, compare);
      list_sort (&after, compare);
      list_concat (&middle, after);
      list_concat (&before, middle);
      *plist = before;
    }
}

void
list_merge (List * plist,
	    List add,
	    int (*compare) (const void *, const void *))
{
  list_concat (plist, add);
  list_sort (plist, compare);
}
