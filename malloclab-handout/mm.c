/* Plan: segregation fits*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include "mm.h"
#include "memlib.h"

team_t team = {};
/* single word (4) or double word (8) alignment */
/* constants */
#define ALIGNMENT 8
#define VERBOSE 0
#define FREE 0
#define BUSY 1
#define MAX_CLASSES_NO 25
#define MIN_BLOCK_SIZE 24
#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))
#define PTR_SIZE (ALIGN(sizeof(void*)))

/*
 * usefull macros 
 */

#define PACK(d,f) ((d) | (f))
/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)
#define INCR_PTR(ptr, size) ((void *) ((char*) ptr + size))

/*
 *
 *  Everything to manipulate block structure
 *
 */

/* macros for reading hdr bits */
#define HELP_BITS(hdr) ( (hdr & 0x7) )
#define SIZE_BITS(hdr) ( (hdr & ~0x7) )

/* direct access to header */
#define SIZED_HDR(bp) (*( (size_t *) INCR_PTR(bp, - (long) SIZE_T_SIZE) ))
#define SIZED_FTR(bp) (*( (size_t *) INCR_PTR(bp, SIZE(bp) \
					      - 2 * SIZE_T_SIZE) ))

/* ronly access to actual size of block */
#define SIZE(bp) ( SIZE_BITS(SIZED_HDR(bp)) )
#define FSIZE(bp) ( SIZE_BITS(SIZED_FTR(bp)) )

/* manipulate size of block with this macro */
#define SET_SIZE(bp, size)  assert(HELP_BITS((size)) == 0);	\
  SIZED_HDR(bp) = PACK((size), HELP_BITS(SIZED_HDR(bp)));	\
  SIZED_FTR(bp) = PACK((size), HELP_BITS(SIZED_FTR(bp)))

/* manipulate with free/busy state of block */
#define CHANGE_STATE(bp, state) SIZED_HDR(bp) = PACK(SIZE(bp), state); SIZED_FTR(bp) = PACK(SIZE(bp), state)
/* set free and set busy */
#define SFREE(bp) CHANGE_STATE(bp, FREE)
#define SBUSY(bp) CHANGE_STATE(bp, BUSY);
#define IS_FREE(bp) ( HELP_BITS(SIZED_HDR(bp)) == FREE )

/*
 * MACROS TO WATCH NEIGHBOURS
 */

/* manipulate with headers and footers */
#define FTR(bp) ( INCR_PTR(bp, SIZE(bp) - SIZE_T_SIZE) )
#define PREV_HDR(bp) ( INCR_PTR(bp, - (long) SIZE(PREV_FTR(bp)) ) )
#define PREV_FTR(bp) ( INCR_PTR(bp, - (long) SIZE_T_SIZE) )
#define NEXT_HDR(bp) ( INCR_PTR(bp, SIZE(bp) ) )
#define NEXT_FTR(bp) ( FTR(NEXT_HDR(bp)))


/* manipulate list entries */
#define PREV_PTR(bp) ( (void **) bp)
#define NEXT_PTR(bp) ( (void **) INCR_PTR(bp, PTR_SIZE))
#define PREV(bp) (*(PREV_PTR(bp)))
#define NEXT(bp) (*(NEXT_PTR(bp)))
#define SET_PREV(bp, prev)  PREV(bp) = prev
#define SET_NEXT(bp, next)  NEXT(bp) = next

typedef struct list_header{
  void* head;
  size_t max_size;
} list_header_t;

/* global variables */
static list_header_t lists[MAX_CLASSES_NO];
static void *global_header;
static void *global_footer;


static size_t get_size_by_idx(int idx)
{
  if(idx < 0) {
    printf("wrong index in function get_size_by_idx: idx = %d", idx);
    exit(1);
  }
  if(idx == 0)
    return 32;
  else
    return 32 << idx;
}

//sizing strategy is defined by this function
static int get_idx_by_size(size_t size)
{
  int idx;
  idx = 0;
  while((size >> idx++) > 32)
    ;
  --idx;
  return idx;
}

void *Mem_sbrk(size_t increment)
{
  if(increment <= 0) {
    printf("bad argument in mem_sbrk, incr = %lu \n", increment);
    exit(1);
  }
  
  void *res;
  if((long)(res = mem_sbrk(ALIGNMENT)) == -1) {
    printf("mem_sbrk finished with error\n");
    exit(1);
  }

  return res;
}

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
  int i;
  //required_allocate = footer of global_header (SIZE_T_SIZE bytes) + header of global_footer (same) |||||
  size_t required_allocate, alignment_overhead;
  
  for(i = 0; i < MAX_CLASSES_NO; ++i) {
    lists[i].head = NULL;
    lists[i].max_size = get_size_by_idx(i);
  }
  
  required_allocate = 2 * SIZE_T_SIZE;
  /*
   * solve alignment problem
   * read below only if you can't understand by yoursefl
   * mem_start<-|alignment|alignment_overhead|...head |...memory...|global_footer
   *      addr % 8 == 0 <-|       global_head         |           
   *            |A  L  I  G  N   M   E   N  T|AL-al_ov| .......... |ALIGNMENT
   */
  
  alignment_overhead = ALIGNMENT - (size_t) ((unsigned long)(Mem_sbrk(ALIGNMENT)) & 0x7) ;
  required_allocate -= alignment_overhead;
  /* set head and foot addr */
  global_header = Mem_sbrk(required_allocate) - alignment_overhead + SIZE_T_SIZE;
  global_footer = INCR_PTR(global_header, SIZE_T_SIZE);
  /* initialize header */
  SIZED_HDR(global_header) = PACK(SIZE_T_SIZE, BUSY);
  /* initialize footer */
  SIZED_HDR(global_footer) = PACK(0, BUSY);
  
  return 0;
}

//TODO correct usages of get_head to a new contract;
static list_header_t *get_head(list_header_t *lists, size_t list_size, size_t block_size)
{
  int idx = get_idx_by_size(block_size);
  assert(idx >= 0);
  if((unsigned long)idx >= (unsigned long)list_size) {
    printf("index is too big\n");
    return NULL;
  }
  
  return &lists[idx];
}


/* gets head of list */
static list_header_t *Get_head(size_t size)
{
  return get_head(lists, MAX_CLASSES_NO, size);
}

static void Set_head(size_t size, void *bp)
{
  Get_head(size)->head = bp;
}

static size_t align_size(size_t size)
{
  if(size < MIN_BLOCK_SIZE)
    return MIN_BLOCK_SIZE;
  else
    return ALIGN(size);
}

/* returns link to free block if given list class 
 * has a member that satisfies "size" requirement
 */
static void *get_approp(void *list_head, size_t size)
{
  void *cur_block = list_head;
  while(cur_block != NULL && SIZE(cur_block) < size)
    cur_block = NEXT(cur_block);
  return cur_block;
}

//******************************************************************************
/* finds free block in one of free lists. 
 *if it exists returns it, else returns NULL 
 */
static void *find_block_if_exists(size_t size) {
  int i;
  void *fbp;
  list_header_t header_i;
  
  /* find appropriate non-empty size-class then check if it 
   * has appropriate member else continue search */
  fbp = NULL;
  i = 0;
  
  header_i = lists[0];
  while(i < MAX_CLASSES_NO
	&& (header_i.head == NULL
	    || header_i.max_size < size
	    || (fbp = get_approp(header_i.head, size)) != NULL)) {
    ++i;
    header_i = lists[i];
  }
  return fbp;
}

static void* allocate_new_block(size_t size)
{
  //TODO: move calls to init function
  size_t page, apage;
  void *fbp;
  
  page = mem_pagesize();
  apage = size % page == 0 ? size : ((size / page) + 1) * page;

  if(VERBOSE) {
    printf("allocated block of size %d\n", (int)apage);
    printf("mem page size: %d\n", (int)page);
  }

  fbp = mem_sbrk(apage);
  printf("initial fbp: %p\n", fbp);

  /* footer is now before allocated block */
  assert(SIZE(fbp) == 0);
  SET_SIZE(fbp, apage);
  SFREE(fbp);
  SIZED_HDR(fbp + SIZE(fbp)) = PACK(0,BUSY);

  global_footer = INCR_PTR(fbp, SIZE(fbp));
  return fbp;
}


/* deletes free block from it's list */
static void delete_from_list(size_t list_size, void *fbp)
{
  if(fbp == NULL)
    return;
  
  void *prev, *next;
  /* if prev == NULL then bp is head */
  prev = PREV(fbp);
  next = NEXT(fbp);

  /* not head not last */
  if(prev != NULL && next != NULL) {
    SET_NEXT(prev, next);
    SET_PREV(next, prev);
  }

  /* case of head that is last entry */
  if(prev == NULL && next == NULL) {
    Set_head(list_size ,NULL);
    return;
  } 

  //head which is not last
  if(prev == NULL && next != NULL) {
    Set_head(list_size ,next);
    SET_PREV(next, NULL);
  }

  //not head and last
  if(prev != NULL && next == NULL) {
    SET_NEXT(prev, NULL);
  }
}

static void insert_into_list(void *bp)
{
  void *prev_head;
  list_header_t *head_t;
  size_t size;

  //we need size without free bits
  size = SIZE(bp) & ~0x7;

  head_t = Get_head(size);
  prev_head = head_t->head;
  
  //assert it is head
  assert(prev_head == NULL ? 1 : PREV(prev_head) == NULL);

  head_t->head = bp;
  if(prev_head != NULL) {
    SET_PREV(prev_head, bp);
    SET_NEXT(head_t->head, prev_head);
  }
  return;
}

/* Plan is given a free block:
 * first: split it in two parts, first of needed size, second of remainder size
 * second: refresh lists entries(put two newly created blocks into appropriate list entires
 */
//******************************************************************************
static int prepare_block(void *fbp, size_t needed_size)
{
  void *rbp; 			/* remainder block pointer, r -- stands for remainder */
  size_t init_size, r_size;

  if(fbp == NULL)
    return -1;

  //we need size without free bits
  init_size = SIZE(fbp);
  delete_from_list(init_size, fbp);
  
  r_size = init_size - needed_size;
  
  /* what to do depending on remainder size */
  if(r_size < MIN_BLOCK_SIZE) {
    needed_size = init_size;
  }else{
    /* prepare rbp*/
    rbp = INCR_PTR(fbp, needed_size);
    SET_SIZE(rbp, r_size);
    SFREE(rbp);
    insert_into_list(rbp);
  }
  
  /* prepare fbp */
  SET_SIZE(fbp, needed_size);
  SBUSY(fbp);
  return 0;
}

void print_memory() {
  void *start = global_header;
  int i = 0;
  while(start != global_footer) {
    if(i == 0){
      printf("Global start: %lu\n",SIZED_HDR(start));
      start = INCR_PTR(start, SIZE_T_SIZE);
    }else {
      printf("Free: %d Header: %lu, Memory..., Footer: %lu\n",IS_FREE(start), SIZE(start),FSIZE(start));
      start = NEXT_HDR(start);
    }
    i++;
  }
}

void print_lists();
/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
  //0. подобрать размер блока
  //1. найти подходящий блок
  //1.5 провести подготовительные работы с свободным блоком
  //2. пометить его как используемый
  //3. вернуть пользователю 

  size_t asize;
  void* fbp;
  printf("malloc called, size: %lu \n", size);
  /* overhead for size's */
  size+=8;
  asize = align_size(size);
  fbp = find_block_if_exists(asize);
  /* if not found then allocate, if can't allocate then return NULL */
  if(fbp == NULL && (fbp = allocate_new_block(asize)) == NULL)
    return NULL;
  
  int res = prepare_block(fbp, size);
  print_memory();
  return res < 0 ? NULL : fbp;
}

static void coalise(void *fbp)
{
   void *prev_hdr, *next_hdr;
   int prev_free, next_free;

   next_hdr = NEXT_HDR(fbp);

   prev_free = IS_FREE(PREV_FTR(fbp));
   next_free = IS_FREE(next_hdr);
      
   if(prev_free && next_free) {
     prev_hdr = PREV_HDR(fbp);

     delete_from_list(SIZE(prev_hdr), prev_hdr);
     delete_from_list(SIZE(next_hdr), next_hdr);
     delete_from_list(SIZE(fbp), fbp);
     SET_SIZE(prev_hdr, (SIZE(prev_hdr) + SIZE(fbp) + SIZE(next_hdr)));
     insert_into_list(prev_hdr);
     return;
   }
   
   if(!prev_free && next_free) {
     delete_from_list(SIZE(next_hdr), next_hdr);
     delete_from_list(SIZE(fbp), fbp);
     SET_SIZE(fbp, SIZE(fbp) + SIZE(next_hdr));
     insert_into_list(fbp);
     return;
   }
   
   if(prev_free && !next_free) {
     prev_hdr = PREV_HDR(fbp);

     delete_from_list(SIZE(prev_hdr), prev_hdr);
     delete_from_list(SIZE(fbp), fbp);
     SET_SIZE(prev_hdr, SIZE(prev_hdr) + SIZE(fbp));
     insert_into_list(prev_hdr);
     return;
   }
   
   if(!prev_free && !next_free) {
     insert_into_list(fbp);
     return;
   }  
 }
 
/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
  /* mark fbp as free */
  SFREE(ptr);
  coalise(ptr);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
  void *oldptr = ptr;
  void *newptr;
  size_t copySize;
    
  newptr = mm_malloc(size);
  if (newptr == NULL)
    return NULL;
  copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
 if (size < copySize)
    copySize = size;
  memcpy(newptr, oldptr, copySize);
  mm_free(oldptr);
  return newptr;
}

void print_block(void *fbp)
{
  if(fbp == NULL)
    printf("block is null");
  else
    printf("Size: %lu, Prev: %p, Next: %p", SIZE(fbp), PREV(fbp), NEXT(fbp));
  
}
void print_lists()
{
  int i;
  void *cur_block;
  for(i = 0; i < MAX_CLASSES_NO; ++i) {
    printf("List max_size: %lu:\n", lists[i].max_size);

    cur_block = lists[i].head;
    while(cur_block != NULL) {
      print_block(cur_block);
      printf(" -> ");
      cur_block = NEXT(cur_block);
    }
    
    printf("end of list\n");
  }
}

void malloc_n(size_t size, int times)
{
  for(int i = 0; i < times; ++i) {
    mm_malloc(size);
    print_lists();
  }
}

static void process();
/* int main() { */
/*   mem_init(); */
/*   mm_init(); */
/*   process(); */
/*   mem_deinit(); */
/*   return 0; */
/* } */

static void process()
{
  void *p1, *p2, *p3;
  
  /* p1 = mm_malloc(4000); */
  /* print_lists(); */
  /* p2 = mm_malloc(80); */
  /* print_lists(); */
  p3 = mm_malloc(2040);
  print_memory();
  p2 = mm_malloc(2040);
  print_memory();
  mm_free(p2);
  print_memory();
  /* mm_free(p1); */
  /* print_lists(); */
  /* mm_free(p3); */
  /* print_lists(); */
  /* mm_free(p2); */
  /* print_lists(); */
}
