
#include <assert.h>

class link_t
{
public:
   link_t(void);                // constructor
   link_t(void *);              // constructor
  ~link_t(void);
   void     set_next(link_t *);
   link_t  *get_next(void);
   void    *get_entity(void);

private:
   link_t  *next;
   void    *entity;

};

typedef class list_t
{
public:
   list_t(void);                 // constructor
   list_t(const list_t &);       // copy constructor
  ~list_t(void);
   void   add(void *entity);     // add entity to end of list
   void   reset(void);           // set current to start of list
   void   *get_entity(void);     // get current entity in list
   int    not_end(void);
   void   next_link(void);

private:
   link_t *first;
   link_t *last;
   link_t *current;
} list_t;

