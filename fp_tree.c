#include<stdio.h>
#include<stdlib.h>
#define MINSUP 0.2
#define MINCONF 0.6
#define TRANS 10
#define ITEMS 7

FILE *fp;
int freq_items;
void mergeSort(int arr[],int ar2[],int low,int mid,int high){
	int i,m,k,l,temp[ITEMS],temp2[ITEMS];
    l=low;
    i=low;
    m=mid+1;
    while((l<=mid)&&(m<=high)){

         if(arr[l]>=arr[m]){
             temp[i]=arr[l];
		temp2[i]=ar2[l];
             l++;
         }
         else{
             temp[i]=arr[m];
		temp2[i]=ar2[m];
             m++;
         }
         i++;
    }

    if(l>mid){
         for(k=m;k<=high;k++){
             temp[i]=arr[k];
		temp2[i]=ar2[k];
             i++;
         }
    }
    else{
         for(k=l;k<=mid;k++){
             temp[i]=arr[k];
		temp2[i]=ar2[k];
             i++;
         }
    }
   
    for(k=low;k<=high;k++){
         arr[k]=temp[k];
	ar2[k]=temp2[k];
    }
}

void partition(int arr[],int arr2[],int low,int high){
    int mid;
    if(low<high){
         mid=(low+high)/2;
         partition(arr,arr2,low,mid);
         partition(arr,arr2,mid+1,high);
         mergeSort(arr,arr2,low,mid,high);
    }
}

struct node
	{
	int value;
	struct node *parent;
	struct node *next;
	struct node *child;
	int count;
	struct node *extra;
	};

struct node* create_node(struct node *parent, struct node *prev, int value)
	{
	struct node *temp = malloc(sizeof(struct node));
	temp->value = value;
	temp->parent = parent;
	if(parent!=NULL)
	parent->child = temp;
	prev->next = temp;
	temp->next = NULL;
	temp->extra = NULL;
	temp->count =1;
	//printf("creTING NODE\n");
	return temp;
	}

struct node * insert_node(struct node *headtable, int value, struct node *parent)
	{
	int k=0;
	struct node *temp,*var;
	while(value!=headtable[k].value)
		{
		k++;
		}
	if(headtable[k].next == NULL)
		{
		temp = create_node(parent, &headtable[k], value); 
		}
	else if(headtable[k].next != NULL)
		{
		var = headtable[k].next;
		while(var->parent != parent && var->next!=NULL)
			{
			var = var->next;
			}
		if(var->parent != parent)
			{
			temp = create_node(parent, var, value);	
			}
		else
			{	
			var->count ++;
			temp = var;
			}
		}
	return temp;	
	}

void print_tree(struct node *headtable)
	{
	int k=0;
	for(k=0;k<freq_items;k++)
		{
		struct node *temp = headtable[k].next;
		while(temp != NULL)
			{
			printf("%d %d\t",temp->value,temp->count);
			temp = temp->next;
			}
		printf("\n");
		}
	}



void project(struct node head[],int k,int array[])
	{
	if(k<-1)
		return;
	printf("\n\n\nentered in project with k = %d\n",k);
	
	struct node *var = (&head[k])->next;
	struct node *childp=NULL;
	while(var != NULL)
		{
		//if(var->value != -1)
		//{
		struct node *parent = var->parent;
		childp = var;
		while(parent!=NULL)

			{
			if(parent->extra != NULL)
			{

			parent->extra->count = parent->extra->count + 1;	
			parent->extra->parent = parent->parent;
			parent->extra->child = NULL;
		
			}
			else 
			{
			struct node *temp = malloc(sizeof(struct node));
			parent->extra = temp;
                        
			parent->extra->value = parent->value;
			parent->extra->count = childp->count;
			parent->extra->parent = parent->parent;
			
			}
			printf("%d %d  count\n",parent->value,parent->extra->count);
			if(childp != var)
				{
				parent->extra->child = childp;
				}
			childp = parent;
			parent = parent->parent;
			}
		//}
		var = var->next;
		}
	
// til here its creating auxiliary nodes
///////////// done till here (creating extra nodes for nodes) ////////
/*	int j=0;
	var = &head[0];
	struct node *temp;
	while(var!=NULL && j<k+1)
		{
		temp = var->next;
		while(temp!= NULL && j<k+1)
			{
			temp = temp->next;
			}
		j++;
		var++;
		}*/
	// for seperating from the actual tree and removing items with support less than minsup
/////////////////////////////////////////////////////////////////////////////////////////////
	struct node *temp_head = malloc(sizeof(struct node)*k);
	int i;
	struct node *temp;
	for(i=0;i<k;i++)
		{
		temp_head[i].value = head->value;
		var = head->next;
               
		temp = &temp_head[i];
		temp_head[i].child = &temp_head[i+1];
		if(i-1 >= 0)
			temp_head[i].parent = &temp_head[i-1];
		else
			temp_head[i].parent = NULL;
		temp_head[i].count = 0;
		while(var!=NULL)
			{printf("%d is count\n",var->count);
			if(var->extra != NULL)
				{
				printf("%d is count123\n",var->extra->count);
				temp_head[i].count = var->extra->count+temp_head[i].count;
                                
				temp -> next = var->extra;
				temp = var->extra;
				var->extra = NULL;
				}
			var = var->next;
			}
		head++;
	
		}
	printf("after for\n");
// till here removing the auxiliary nodes from the original tree and creating a new copy of modified head table
		i=0;
		printf("printinf the auxiliary tree\n");
		for(i=0;i<k;i++)
		{
		var = &temp_head[i];
		while(var != NULL)
			{
			printf("%d   %d\t",var->value,var->count);
			var = var->next;
			}
		printf("\n");
		}





		temp = NULL;
               struct node * var2 = NULL;
		for(i=0;i<k;i++)
		{
		var = &temp_head[i];
		
		if(temp_head[i].count < MINSUP*TRANS)
			{
			printf("minsup for i = %d, %d\n",i,temp_head[i].count);
			if(i>0)
			{
			temp_head[i-1].child = &temp_head[i+1];
			temp_head[i+1].parent = &temp_head[i-1];
			}
			else
			{
			temp_head[i+1].parent = NULL;
			}
			printf("hi swetha \n");
			var = &temp_head[i];
			var->value = -1;
			//temp = var->next;
			while(var->next!=NULL)
				{
				printf("hi again ;) \n");
				var->parent->child = var->child;
				var->child->parent = var->parent;
				var = var->next;
				
				printf("value is %d\n",var-> value);
				free(var->next);
				
				//temp = var;
				printf("able to free child parent\n");
				}
			}
		}
	printf("after child parent\n");
// till here removing the branch of the item with count < MINSUP
	int m = k-1;
       int k1 = 0;
	var = &temp_head[k-1];
       for(i=0;i<k;i++)
         printf("%d is value of temphead\n",temp_head[i].value);
	while(array[k1]!=0)
		{ 
		k1++;
		}
        var = &temp_head[k-1];
        printf("%d %d is in while\n",var->value,var->count);
	while(var!=NULL)
		{
                   printf("%d %d is in while\n",var->value,var->count);
		if(var->value!=-1)
		{
                 printf("%d %d is in if\n",var->value,var->count);
		array[k1]=var->value;
		int i=0;
		for(i=0;i<=k1;i++)
			{
                        printf("%d ",array[i]);
			}
                printf("\n");project(temp_head,m-1,array);m--;
		}
              var = var->parent;
               printf("%d %d is out while\n",var->value,var->count);
			
		
		}
      }

main()
{
fp = fopen("frequent_item","r");
int item[TRANS][ITEMS]={{1,4,6},{1,3,4,5},{2,4},{2,3,4},{2,3},{1,2,4},{2,4,5},{2,3,5,7},{3,4,6},{1,2,4}};
int count[ITEMS]={0};
int attribute[ITEMS]={1,2,3,4,5,6,7};
int i;
// for counting 
for(i=0;i<TRANS;i++)
	{
	int j=0;
	for(j=0;item[i][j]!='\0' && j<ITEMS;j++)
		{
		count[item[i][j]-1]++;
		}
	}

// sorting the single item sert
partition(count,attribute,0,ITEMS-1);
// removing infrequent single item sets frpom transactions
for(i=0;i<TRANS;i++)
	{
	int j=0;
	for(j=0;item[i][j]!='\0' && j<ITEMS;j++)
		{
		if(count[item[i][j]-1]<MINSUP*TRANS)
			item[i][j]='\0';
		}
	}
// modifying the item set and count array
/*for(i=0;i<TRANS;i++)
	{
	int j=0;
	for(j=0;item[i][j]!='\0' && j<ITEMS;j++)
		{
		printf("%d   ",item[i][j]);
		}
	printf("\n");
	}*/
int non_f=0;
for(i=0;i<ITEMS;i++)
	{
	//printf("%d %d\n",count[i],attribute[i]);
	if(count[i]<MINSUP*TRANS)
		{
		attribute[i]=0;  //better if we free the space
		count[i]=0;
		non_f++;
		}
	//printf("%d %d\n",count[i],attribute[i]);
	}
freq_items = ITEMS - non_f;


//////////////////////////////////////////////////////////////
//sorting each transaction

int flag[ITEMS]={0};int k;
int j;
for(i=0;i<TRANS;i++){
	k=0;
	for(j=0;j<ITEMS;j++)
		flag[j]=0;
	
	for(j=0;j<ITEMS&& item[i][j]!='\0';j++){
		flag[item[i][j]-1]++;
	
	}
	for(j=0;j<ITEMS;j++){
		if(flag[attribute[j]-1]==1)
		{
			item[i][k]=attribute[j];k++;
		}
		/*else
		{
			item[i][k]='\0';k++;
		}*/		
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////*************preprocessing done*/

struct node head[freq_items];
for(i=0;i<freq_items;i++)
	{
	head[i].value = attribute[i];
	head[i].parent = NULL;
	head[i].next = NULL;
	head[i].count = count[i];
	head[i].extra = NULL;
	}
struct node *null_pointer = malloc(sizeof(struct node));
null_pointer->value = -1;
null_pointer->parent = NULL;

struct node *var=NULL, *temp2=NULL, *temp=NULL;

for(i=0;i<TRANS;i++)
	{
	temp = null_pointer;
	for(j=0;item[i][j]!='\0';j++)
		{
			temp = insert_node(head,item[i][j],temp);
		}	
	}

print_tree(head); 	
/*for(i=0;i<TRANS;i++)
	{
	k=0;
	temp2 = NULL;
	var = NULL;
	temp = NULL;

	while(item[i][0]!=head[k].value)
		{
		k++;
		}
	if(head[k].next == NULL)
		{
		temp = create_node(NULL, &head[k], head[k].value); 
		}
	else if(head[k].next != NULL)
		{
		var = head[k].next;
		while(var->parent != NULL && var->next!=NULL)
			{
			var = var->next;
			}
		if(var->parent != NULL)
			{
			temp = create_node(NULL, var, head[k].value);	
			new_node_made =1;
			}
		else
			{	
			var->count ++;
			temp = var;
			}
		}
		temp2 = temp;
	for(j=1;j<ITEMS && item[i][j]!='\0';j++)
		{
		while(head[k].value != item[i][j])
			{
			k++;
			}
		
		if(head[k].next==NULL)
			{
	
			temp = create_node(temp2, &head[k], head[k].value);
			new_node_made = 1;
			
			}

		else
			{
			//recursive function
			var = head[k].next;
			while(var->next != NULL && var->parent != temp2)
				{
				var = var->next;
				}
				
			
			if(var->parent != temp2)
				{
				temp = create_node(temp2, var, head[k].value);
				new_node_made = 1;
				}
			else
				{
				temp = var;
				var->count++;
				}
			}
		
		temp2 = temp;
		}
	}
	k=0;
	for(k=0;k<freq_items;k++)
		{
		var = &head[k];
		while(var != NULL)
			{
			printf("%d   %d\t",var->value,var->count);
			var = var->next;
			}
		printf("\n");
		}
	printf("\n\n\n\n");
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int c = 1;	
	k=0;
	while(c!=head[k].value && k<freq_items)
		{
		k++;
		}
	
	int array[10]={0};
	array[0]=1;
	printf("%d value of k \n\n\n",k);
	//project (head,k,array);*/
}
