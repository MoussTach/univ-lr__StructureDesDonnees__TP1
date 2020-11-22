

typedef struct _ListNode *List;

struct _ListNode
  {
    List next;
    char data[1];		/*
				 * the pointer data will be consecutive to
				 * the pointer next in memory
				 */
  };

