#ifndef _LIST_H_
#define _LIST_H_

typedef struct _ListNode *List;

struct _ListNode
  {
    List next;
    char data[1];		/*
				 * the pointer data will be consecutive to
				 * the pointer next in memory
				 */
  };



/*
 * function creating a list
 */
List list_new (void);

/*
 * function removing the first element of a list.
 * TRUE is returned in the case of a success, FALSE otherwise
 */
int list_remove_first (List * plist, void (*delete) (void *));

/*
 * function removing the last element of a list.
 * TRUE is returned in the case of a success, FALSE otherwise
 */
int list_remove_last (List * plist, void (*delete) (void *));

/*
 * function removing the nth element of a list.
 * TRUE is returned in the case of a success, FALSE otherwise
 */
int list_remove_nth (List * plist, size_t nth, void (*delete) (void *));

/*
 * function deleting a list entirely
 */
void list_delete (List * plist, void (*delete) (void *));

/*
 * function computing the length of a list
 */
size_t
list_length (List list);

/*
 * function returning the test: is list empty?
 */
int list_is_empty (List list);

/*
 * function prepending an element to a list.
 * TRUE is returned in the case of a success, FALSE otherwise
 */
int list_prepend (List * plist, const void *data, size_t size);

/*
 * function appending an element to a list
 * TRUE is returned in the case of a success, FALSE otherwise
 */
int list_append (List * plist, const void *data, size_t size);

/*
 * function inserting an element in the nth position.
 * TRUE is returned in the case of a success, FALSE otherwise
 */
int list_insert (List * plist,
		 const void *data,
		 size_t size,
		 size_t nth);

/*
 * function inserting an element in a sorted list
 * the compare function has the same meaning in the standard qsort function
 */
int list_insert_sorted (List * plist,
			const void *data,
			size_t size,
			int (*compare) (const void *, const void *));

/*
 * function to give the pointer to the first data
 * NULL is returned if the list is empty
 */
void *list_first_data (List list);

/*
 * function giving the pointer to the last data
 * NULL is returned if the list is empty
 */
void *list_last_data (List list);

/*
 * function giving the pointer to the nth data
 * NULL is returned if the case of an error
 */
void *list_nth_data (List list, size_t nth);


/*
 * function giving the last sublist of a list.
 * NULL is returned if the list is empty
 */
List
list_last (List list);

/*
 * function giving the nth sublist of a list.
 * NULL is returned if the case of an error
 */
List
list_nth (List list, size_t nth);

/*
 * function finding the pointer to a data.
 * NULL is returned if the case of an error: the data does not exist
 * in the list
 */
void *list_find_data (List list,
		      const void *data,
		      int (*compare) (const void *, const void *));

/*
 * function giving the index of a data. -1 is returned if the data
 * is not in the list
 */
int list_index_data (List list,
		     const void *data,
		     int (*compare) (const void *, const void *));

/*
 * function applicating an operation to the entire list
 * extra_data is passed as the second argument of the func function for each
 * element of the list
 */
void list_foreach (List list, void (*func) (void *, void *), void *extra_data);


/*
 * function concatening the list add to the list pointed by plist
 */
void list_concat (List * plist, List add);

/*
 * function splitting a list into two list
 * *pbefore will contain all elements "inferior or equal" to data
 * *pafter will contain all elements "superior" to data
 */
void list_split (List * plist,
		 List * pbefore,
		 List * pafter,
		 const void *data,
		 int (*compare) (const void *, const void *));

/*
 * function sorting a list
 */
void list_sort (List * plist, int (*compare) (const void *, const void *));


/*
 * function merging two list. The list *plist will be sorted according
 * to the function compare
 */
void list_merge (List * plist,
		 List add,
		 int (*compare) (const void *, const void *));

#endif
