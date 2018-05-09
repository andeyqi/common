#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>
  
#include "list.h"
 
#define  SIZE  100  
#define  hname  "yangyuanxin"  
#define true  1   
#define false 0  
  
//创建一个双向链表结构   
struct list  
{  
    char name[11] ;  
    char sex[5] ;  
    char company_name[10];  
    char seq_number[32];  
    //嵌套在struct list这个自己创建的结构体里面   
    //list_head就是它的父结构的一个成员,   
    struct list_head list;  
};  
  
//创建一个哈希链表结构   
struct hlist  
{  
    int id ;  
    char name[20] ;  
    struct hlist_node hlist ;     
}; 


  
//内核链表中哈希链表的使用   
int hlist_fuction(void) ;  
//内核链表中基本的双向链表的使用    
int double_list(void) ;  
  
int main(int argc, char *argv[])   
{  
    hlist_fuction();  
    double_list();  
    return 0;  
}  
  
//定义一个和哈希链表操作的相关函数   
int hlist_fuction(void)  
{  
    printf("欢迎进入哈希链表模块！\n");  
    //初始化哈希链表头   
    HLIST_HEAD(hhead);  
    //定义一个哈希链表成员   
    struct hlist people ;  
    //定义一个哈希链表节点   
    struct hlist *entry ;   
    //定义一个已知的某个哈希链表的节点   
    struct hlist_node *p ;  
    printf("请输入成员id:\n");   
    scanf("%d",&people.id);  
    printf("请输入成员name:\n");  
    scanf("%s",&people.name);  
    //将成员添加到哈希链表的头部   
    hlist_add_head(&people.hlist , &hhead);  
    //遍历哈希链表   
    hlist_for_each(p,&hhead)  
    {  
        //获取哈希链表的地址   
        entry = hlist_entry(p , struct hlist , hlist);  
        printf("成员id为:\n");  
        printf("%d\n",entry->id);  
        printf("成员name:\n");  
        printf("%s\n",entry->name);  
    }  
    int errcount = 0 ;  
    repest :  
    printf("需要删除链表吗?需要y，否则n\n");  
    //刷新输入缓冲区   
    fflush(stdin);  
    char ch ;  
    scanf("%c",&ch) ;  
    switch(ch)  
    {  
        case 'y' :  printf("正在准备删除链表节点!\n");  
                    hlist_del(&people.hlist);  
                    printf("链表节点已删除!\n");  
                    break ;   
        case 'n' :  printf("链表节点没有被删除!\n");  break ;  
        default:  
                    printf("你的输入有误请重新输入!\n");  
                    errcount ++ ;   
                    if(errcount == 3){  
                        printf("输入错误次数太多，已退出程序!\n");  
                        exit(1);  
                    }  
                    system("cls");  
                    goto repest ;  
    }     
   
    switch(hlist_empty(&hhead))  
    {  
        case true : printf("哈希链表为空\n"); break ;  
        case false :printf("哈希链表不为空\n");    break ;  
    }  
    sleep(2);  
    system("cls");  
    return 0 ;   
}  
  
int double_list(void)  
{  
    printf("欢迎进入双向链表模块!\n");  
    //链表头初始化的两种方式   
    //1.  
    //  struct list_head head;  
    //    INIT_LIST_HEAD(&head);  
    //2.  
    LIST_HEAD(head);  
    //创建一个结构体成员变量   
    struct list people;  
    //创建一个链表节点   
    struct list *entry;  
    //已知的某个链表的节点   
    struct list_head *p;  
  
    printf("请输入员工姓名:\n");  
    scanf("%s",&people.name);  
    printf("请输入公司名称:\n");  
    scanf("%s",&people.company_name);  
    printf("请输入工号:\n");  
    scanf("%s",&people.seq_number);  
    list_add(&people.list,&head);  
      
    #if 0  
    printf("\n------------正向遍历-------------------\n\n");  
    printf("遍历的第一种方式:\n");  
    list_for_each(p,&head)  
    {  
        entry=list_entry(p,struct list,list);  
        printf("姓名: %s\n",entry->name);  
        printf("公司名: %s\n",entry->company_name);  
        printf("工号: %s\n",entry->seq_number);  
    }  
    #endif  
    printf("遍历的第二种方式:\n");  
    list_for_each_entry(entry,&head,list)  
    {  
        printf("name:%s\n",entry->name);  
        printf("company_name:%s\n",entry->company_name);  
        printf("seq_number:%s\n",entry->seq_number);  
    }  
      
    #if 1  
    printf("\n------------反向遍历------------------\n\n");  
    list_for_each_prev(p,&head)  
    {  
        entry=list_entry(p,struct list , list);  
        printf("name:%s\n",entry->name);  
        printf("company_name:%s\n",entry->company_name);  
        printf("seq_number:%s\n",entry->seq_number);  
    }  
    #endif  
    static int errcount ;  
    fflush(stdin);  
    char ch ;  
    respest:  
    printf("要删除双向链表节点吗?要输入y，否则输入n\n");  
    scanf("%c",&ch);  
    switch(ch)  
    {  
        case 'y' : printf("准备删除双向链表所有节点!\n");  
                   sleep(2);  
                   list_del(&people.list);  
                   printf("删除成功!\n");  
                   switch(list_empty(&head))  
                   {  
                        case true : printf("双向链表为空!\n"); break ;   
                        case false : printf("双向链表不为空!\n"); break ;  
                   }    
                   break ;  
        case 'n' : printf("双向链表节点没有被删除!\n");  
                   switch(list_empty(&head))  
                   {  
                        case true : printf("双向链表为空!\n"); break ;   
                        case false : printf("双向链表不为空!\n"); break ;  
                   }    
                   break ;  
        default :  
                    if(errcount == 3){  
                        printf("你的输入错误次数太多，程序退出!");  
                        exit(1);  
                    }  
                    printf("你的输入有误，请重新输入!\n");  
                    errcount ++ ;  
                    system("cls");  
                    goto respest ;   
    }  
    return 0 ;  
}  
