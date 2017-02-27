#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h> 
#include "wilddog.h"

#define DEMO_AUGEND "augend"
#define DEMO_ADDEND "addend"
#define DEMO_SUM "sum"
/* modify to you url */
#define DEMO_YOUR_URL "coap://c_test.wilddogio.com"

/* sum set call back */
STATIC void sumSet_callback(void *arg,Wilddog_Return_T err)
{
    if(err < WILDDOG_HTTP_OK || err >= WILDDOG_HTTP_NOT_MODIFIED)
    {
        wilddog_debug("set failed, err = %d!",err);
        return;
    }
	
	return;
}
/* get node pointer according the key value */
STATIC Wilddog_Node_T *node_get(Wilddog_Node_T *p_head,const char* key_name)
{
	Wilddog_Node_T *node = p_head;
	u8 len = 0;
	while(node )
	{
		if( node->p_wn_key )
		{
			/* get the short len*/
			len = strlen((const char*)key_name)>strlen((const char*)node->p_wn_key)\
					?strlen((const char*)node->p_wn_key):strlen((const char*)key_name);
			if( !memcmp(key_name,node->p_wn_key,len))
				return node;
		}
		node=node->p_wn_next;
		if(node == p_head)
			return NULL;
	}
	return NULL;
}
/* count sum and set to server */
STATIC Wilddog_Return_T sum_count(const Wilddog_Node_T *p_sn,Wilddog_T wilddog)
{
	static s32 old_augend = 0;
	static s32 old_addend = 0;
	
    Wilddog_Node_T *p_node,*p_augend,*p_addend,*p_sum;
	int len ;
	if( !p_sn || !p_sn->p_wn_child)
		return WILDDOG_ERR_NULL;
	/*find sum/augend/addend node */
	p_node = p_sn->p_wn_child;
	p_augend = node_get(p_node,DEMO_AUGEND);
	p_addend = node_get(p_node,DEMO_ADDEND);
	p_sum = node_get(p_node,DEMO_SUM);
	if( !p_augend || !p_addend || !p_sum )
	{
		wilddog_debug("deficiency of data !");
		return WILDDOG_ERR_NULL;
	}
	/* add */
	s32 augend = *(s32*)wilddog_node_getValue(p_augend,&len);	
	s32 addend = *(s32*)wilddog_node_getValue(p_addend,&len);
	
	if(old_augend == augend  && old_addend == addend)
		return WILDDOG_ERR_NOERR;
	
	old_augend = augend;
	old_addend = addend;
	
	s32 sum = augend + addend;
	wilddog_node_setValue(p_sum,(u8*)&sum,sizeof(sum));
	printf("sum count : %ld + %ld = %ld\n",augend,addend,sum);
	wilddog_setValue(wilddog,(Wilddog_Node_T*)p_sn,sumSet_callback,NULL);

	return WILDDOG_ERR_NOERR;
}
/* notify call back  */
STATIC void addObserver_callback
    (
    const Wilddog_Node_T* p_snapshot, 
    void* arg,
    Wilddog_Return_T err
    )
{
	Wilddog_T wilddog = (Wilddog_T )arg;
     if((err < WILDDOG_HTTP_OK || err >= WILDDOG_HTTP_NOT_MODIFIED ) &&
        err != WILDDOG_ERR_RECONNECT)
    {
        wilddog_debug("addObserver failed! error code = %d",err);
        return;
    }
    printf("Observe new data! \n");
	if(p_snapshot && wilddog)
    	sum_count(p_snapshot,wilddog);

    return;
}

STATIC void set_callback(void *arg,Wilddog_Return_T err)
{
    if(err < WILDDOG_HTTP_OK || err >= WILDDOG_HTTP_NOT_MODIFIED)
    {
        wilddog_debug("tree creat fault !");
        return;
    }
    printf("tree creat success! \n");
    *(BOOL*)arg = TRUE;
    return;
}
int main(void){
	
    BOOL set_finish = FALSE;
    Wilddog_T wilddog = 0;
    Wilddog_Node_T *p_head,*p_augend,*p_addend,*p_sum;

    /*creat wilddog*/
    wilddog = wilddog_initWithUrl((Wilddog_Str_T*)DEMO_YOUR_URL);
	/* creat head node */ 
    p_head = wilddog_node_createObject(NULL);
	/* creat augend node */
    p_augend = wilddog_node_createNum((Wilddog_Str_T*)DEMO_AUGEND,0);
	/* creat addend node */    
    p_addend = wilddog_node_createNum((Wilddog_Str_T*)DEMO_ADDEND,0);
	/* creat sum node */
    p_sum = wilddog_node_createNum((Wilddog_Str_T*)DEMO_SUM,0);
    /*creat tree */
	wilddog_node_addChild(p_head,p_augend);
	wilddog_node_addChild(p_head,p_addend);
	wilddog_node_addChild(p_head,p_sum);

	/*set data tree */
	wilddog_setValue(wilddog,p_head,set_callback,&set_finish);
	/*abserver node */

	/* release tree memory */	
    wilddog_node_delete(p_head);
	while(1){
		if(set_finish)
		{			
			wilddog_addObserver(wilddog,WD_ET_VALUECHANGE,addObserver_callback,(void*)wilddog);
			set_finish = FALSE;
		}
        wilddog_trySync();
    }
    wilddog_destroy(&wilddog);
}
