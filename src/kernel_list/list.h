#ifndef __list_H  
#define __list_H  
//在linux内核中提供了一个用来创建双向循环链表的结构list_head。   
//list_head只有指针域，没有数据域，不是拿来单独使用，一般被嵌入到其它的结构中  
   
struct list_head {  
         struct list_head *next, *prev;  
};  
  
//需要注意的是，头结点的head是不使用的   
#define LIST_HEAD_INIT(name) { &(name), &(name) }  
//我们知道0地址内容时不允许被访问的，但是0地址我们还是可以访问的  
//这里用一个取址运算符(TYPE *)0表示将0地址强制转换为TYPE *类型  
//((TYPE *)0) -> MEMBER 也就是从0地址找到TYPE的成员MEMBER   
//将main.c中的结构体代入  
//offset(struct list , list);----->展开后((size_t) & (struct list *)0 -> list)   
//写清楚一点时这样:   
//struct  list *p = NULL ;   &p->list ; 即是求p这个结构体指针的成员list的地址,只不过p是0  
//地址，从0地址开始计算list成员的地址，也就是成员list在结构体struct list中的偏移量   
//这个宏的作用就是求解MEMBER成员在TYPE中的偏移量   
//编译器不报错的原因是在编译阶段就已经知道结构体里每个成员的属性的相对偏移量了 ，  
//在代码中对结构体成员的访问其实最终 会被编译器转化为对其相对地址的访问   
//在代码运行期间，其实根本就没有变量名还有属性成员，有的也就只有地址。     
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)  
  
//container_of最终结果返回的是第二个表达式的值,这里所谓的__mptr是一个中间的指针变量，其实就是list_head指针类型，被初始化为ptr   
//而我所说的这个ptr，就是我们要求的list_head中的节点地址，定义这样一个中间的指针变量其实考虑了很多因素  
//如果传参进来的是ptr++，会有副作用，就类似于(p++)+(p++)这样  
//而(char *)__mptr之所以要强制转换为char实质上是因为地址是以字节为单位的，而char的长度是一个字节   
//所以contain_of实质是两个地址相减的结果  
//__mptr是结构体中list_head节点的地址，offset宏求的是list_head节点在MEMBER在结构体中TYPE中的偏移量，那么  
//__mptr减去它所在结构体中的偏移量，就是结构体的地址了。   
#define container_of(ptr, type, member) ({                      \  
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \  
        (type *)( (char *)__mptr - offsetof(type,member) );})  
      
      
#define LIST_HEAD_INIT(name) { &(name), &(name) }  
  
//生成名字为name的双向链表的头节点   
#define LIST_HEAD(name) \  
    struct list_head name = LIST_HEAD_INIT(name)  
  
//初始化链表头   
static inline void INIT_LIST_HEAD(struct list_head *list) 
{
//链表的前一个节点和后一个节点都指向它自己  
        list->next = list;  
        list->prev = list;  
}   

static inline void __list_add(struct list_head *new, struct list_head *prev,struct list_head *next)
{
        next->prev = new;
        new->next = next;
        new->prev = prev;
        prev->next = new;
}
//双向链表插入节点的操作---在头节点head的下一个节点,即是头插 ,即使将new这个节点插入  
static inline void list_add(struct list_head *new, struct list_head *head)
{
        __list_add(new, head, head->next);
}

//双向链表插入节点的操作---向尾部插入new节点 
static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
    __list_add(new, head->prev, head);
}
 
 
static inline void __list_del(struct list_head * prev, struct list_head * next)
{
        next->prev = prev;
        prev->next = next;
}
 
//双向链表中节点删除 ,并把被删除的位置清0 
static inline void list_del(struct list_head *entry)
{
        __list_del(entry->prev, entry->next);
        entry->next = NULL;
        entry->prev = NULL;
}
//将上面entry被删除的节点重新初始化 
static inline void list_del_init(struct list_head *entry)
{
    __list_del_entry(entry);
    INIT_LIST_HEAD(entry);
}

//将老的节点old换成新的节点new 
static inline void list_replace(struct list_head *old,
                struct list_head *new)
{
    new->next = old->next;
    new->next->prev = new;
    new->prev = old->prev;
    new->prev->next = new;
}
//将上面的过程进行重新初始化 
static inline void list_replace_init(struct list_head *old,
                    struct list_head *new)
{
    list_replace(old, new);
    INIT_LIST_HEAD(old);
}

//将链表中的list节点移动到链表的头部 
static inline void list_move(struct list_head *list, struct list_head *head)
{
    __list_del_entry(list);
    list_add(list, head);
}

//判断当前链表list节点是不是链表的最后一个节点 
static inline int list_is_last(const struct list_head *list,
                const struct list_head *head)
{
    return list->next == head;
}
//判断链表是不是空的，如果空返回真，否则返回假 
//也就是判断表头的节点的下一个节点是不是它本身 
static inline int list_empty(const struct list_head *head)
{
    return head->next == head;
}
//判断链表是不是空的，如果空返回真，否则返回假 ，稍微和上面有区别的是 
//判断表头的前一个节点和后一个节点是否为本身，是返回真，不是，返回假 
//也就是prev和next 
static inline int list_empty_careful(const struct list_head *head)
{
    struct list_head *next = head->next;
    return (next == head) && (next == head->prev);
}

//将链表进行翻转，也就是将head的next和head的本身做交换 
static inline void list_rotate_left(struct list_head *head)
{
    struct list_head *first;

    if (!list_empty(head)) {
        first = head->next;
        list_move_tail(first, head);
    }
}

//判断链表中是否只有一个节点 
static inline int list_is_singular(const struct list_head *head)
{
    return !list_empty(head) && (head->next == head->prev);
}

static inline void __list_cut_position(struct list_head *list,
        struct list_head *head, struct list_head *entry)
{
    struct list_head *new_first = entry->next;
    list->next = head->next;
    list->next->prev = list;
    list->prev = entry;
    entry->next = list;
    head->next = new_first;
    new_first->prev = head;
}

//分割链表，以entry为界限进行分割，分割完后的另一半的那条链表被list所指 
static inline void list_cut_position(struct list_head *list,
        struct list_head *head, struct list_head *entry)
{
    if (list_empty(head))
        return;
    if (list_is_singular(head) &&
        (head->next != entry && head != entry))
        return;
    if (entry == head)
        INIT_LIST_HEAD(list);
    else
        __list_cut_position(list, head, entry);
}

static inline void __list_splice(const struct list_head *list,
                 struct list_head *prev,
                 struct list_head *next)
{
    struct list_head *first = list->next;
    struct list_head *last = list->prev;

    first->prev = prev;
    prev->next = first;

    last->next = next;
    next->prev = last;
}

//将list和head这两条链表合并，注意，链表不能为空 
static inline void list_splice(const struct list_head *list,
                struct list_head *head)
{
    if (!list_empty(list))
        __list_splice(list, head, head->next);
}

//将list和head这两条链表合并后并初始化 
static inline void list_splice_init(struct list_head *list,
                    struct list_head *head)
{
    if (!list_empty(list)) {
        __list_splice(list, head, head->next);
        INIT_LIST_HEAD(list);
    }
}



#define prefetch(x) __builtin_prefetch(x)

//双向链表的正向遍历，从前往后 
#define list_for_each(pos, head) \
         for (pos = (head)->next; prefetch(pos->next), pos != (head); \
                 pos = pos->next)

//双向链表的遍历,跟上面那个有所不同的是，下面这个添加了通过list_entry，方便很多  
#define list_for_each_entry(pos, head, member)              \
    for (pos = list_entry((head)->next, typeof(*pos), member);  \
         &pos->member != (head);    \
         pos = list_entry(pos->member.next, typeof(*pos), member))

//如果遍历链表不是从头开始，而是从某个已知的节点pos开始， 从当前的位置往后遍历，注意和头的情况区分 
#define list_for_each_entry_continue(pos, head, member)         \
    for (pos = list_entry(pos->member.next, typeof(*pos), member);  \
         &pos->member != (head);    \
         pos = list_entry(pos->member.next, typeof(*pos), member))

                 
//双向链表的反向遍历，从后往前 
#define list_for_each_prev(pos, head) \
    for (pos = (head)->prev; pos != (head); pos = pos->prev)
    
//双向链表遍历的时候可以同时删除pos节点 
#define list_for_each_safe(pos, n, head) \
    for (pos = (head)->next, n = pos->next; pos != (head); \
        pos = n, n = pos->next)

        
//提供一个和pos一样的指针n，在for循环中暂时保留pos的下一个节点的地址,避免因pos节点被释放而造成断链的结果 
#define list_for_each_entry_safe(pos, n, head, member)          \
    for (pos = list_entry((head)->next, typeof(*pos), member),  \
        n = list_entry(pos->member.next, typeof(*pos), member); \
         &pos->member != (head);                    \
         pos = n, n = list_entry(n->member.next, typeof(*n), member))

//当我们知道list_head的地址时，可以通过list_entry这个宏获取它的父结构的地址。 
//所以泽火革宏的功能就是由结构体成员的地址求结构体的地址。其中ptr是所求结构体中
//list_head成员的指针，type是所求结构体的类型，member是结构体list_head成员名 
#define list_entry(ptr, type, member) \
         container_of(ptr, type, member)
 
 
//哈希链表相关的数据结构
//单指针双向循环链表 
struct hlist_head {
    struct hlist_node *first;
};

struct hlist_node {
    struct hlist_node *next, **pprev;
};

#ifdef CONFIG_ILLEGAL_POINTER_VALUE
# define POISON_POINTER_DELTA _AC(CONFIG_ILLEGAL_POINTER_VALUE, UL)
#else
# define POISON_POINTER_DELTA 0
#endif
#define LIST_POISON1  ((void *) 0x00100100 + POISON_POINTER_DELTA)
#define LIST_POISON2  ((void *) 0x00200200 + POISON_POINTER_DELTA)        
//对定义的struct hlist_head 哈希链表变量进行初始化为空 
#define HLIST_HEAD_INIT { .first = NULL }
//初始化哈希链表头 
#define HLIST_HEAD(name) struct hlist_head name = {  .first = NULL }
//这个宏用在删除节点之后对节点的操作当中
#define INIT_HLIST_HEAD(ptr) ((ptr)->first = NULL)

static inline void INIT_HLIST_NODE(struct hlist_node *h)
{
    h->next = NULL;
    h->pprev = NULL;
}

//判断h->prev是不是为空，如果pprev的指向是空的话，表示这个节点没有添加到这个链表当中来，如果是空，返回true，否则返回false*/
static inline int hlist_unhashed(const struct hlist_node *h)
{
    return !h->pprev;
}


//判断哈希链表是否为空,空返回true，否则返回false
static inline int hlist_empty(const struct hlist_head *h)
{
    return !h->first;
}

static inline void __hlist_del(struct hlist_node *n)
{
    struct hlist_node *next = n->next;
    struct hlist_node **pprev = n->pprev;
    *pprev = next;
    if (next)
        next->pprev = pprev;
}

//对哈希链表的节点进行删除 
//对于删除操作的话，要注意n是不是末尾节点，如果是末尾节点的话，
//next就是NULL，所以就没有指向的pprev，就更不能进行相应的修改了，否则进行修改。
static inline void hlist_del(struct hlist_node *n)
{
    __hlist_del(n);
    n->next = LIST_POISON1;
    n->pprev = LIST_POISON2;
}

//删除哈希链表的节点并对其进行初始化 
static inline void hlist_del_init(struct hlist_node *n)
{
    if (!hlist_unhashed(n)) {
        __hlist_del(n);
        INIT_HLIST_NODE(n);
    }
}

//为哈希链表添加一个新的节点
//n：要添加的新的节点。
//h：hlist链表的表头节点。
//这个函数是给h的下一个和first节点中添加一个新的hlist_node节点，类似于上面的双向链表的头插 
static inline void hlist_add_head(struct hlist_node *n, struct hlist_head *h)
{
    struct hlist_node *first = h->first;
    n->next = first;
    if (first)
        first->pprev = &n->next;
    h->first = n;
    n->pprev = &h->first;
}
//在next的前面添加一个新节点n 
static inline void hlist_add_before(struct hlist_node *n,
                    struct hlist_node *next)
{
    n->pprev = next->pprev;
    n->next = next;
    next->pprev = &n->next;
    *(n->pprev) = n;
}
//在next的后面添加一个新节点n 
static inline void hlist_add_after(struct hlist_node *n,
                    struct hlist_node *next)
{
    next->next = n->next;
    n->next = next;
    next->pprev = &n->next;

    if(next->next)
        next->next->pprev  = &next->next;
}
//将新的节点new替换老的节点old 
static inline void hlist_move_list(struct hlist_head *old,
                   struct hlist_head *new)
{
    new->first = old->first;
    if (new->first)
        new->first->pprev = &new->first;
    old->first = NULL;
}

//哈希链表得到ptr所指地址的这个结构体的首地址 
#define hlist_entry(ptr, type, member) container_of(ptr,type,member)

//哈希链表的遍历操作 
//pos：struct hlist_node类型的一个指针；
//head：struct hlist_head类型的一个指针，表示hlist链表的头结点。
//这个实际上就是一个for循环，从头到尾遍历链表。
#define hlist_for_each(pos, head) \
    for (pos = (head)->first; pos ; pos = pos->next)
    
//以下的遍历于双向循环链表遍历操作大同小异，在此不做注释 

#define hlist_for_each_safe(pos, n, head) \
    for (pos = (head)->first; pos && ({ n = pos->next; 1; }); \
         pos = n)


#define hlist_for_each_entry(tpos, pos, head, member)            \
    for (pos = (head)->first;                    \
         pos &&                          \
        ({ tpos = hlist_entry(pos, typeof(*tpos), member); 1;}); \
         pos = pos->next)


#define hlist_for_each_entry_continue(tpos, pos, member)         \
    for (pos = (pos)->next;                      \
         pos &&                          \
        ({ tpos = hlist_entry(pos, typeof(*tpos), member); 1;}); \
         pos = pos->next)

#define hlist_for_each_entry_from(tpos, pos, member)             \
    for (; pos &&                            \
        ({ tpos = hlist_entry(pos, typeof(*tpos), member); 1;}); \
         pos = pos->next)

#define hlist_for_each_entry_safe(tpos, pos, n, head, member)        \
    for (pos = (head)->first;                    \
         pos && ({ n = pos->next; 1; }) &&               \
        ({ tpos = hlist_entry(pos, typeof(*tpos), member); 1;}); \
         pos = n)
#endif