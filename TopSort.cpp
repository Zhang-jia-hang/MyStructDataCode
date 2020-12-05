#include <stdio.h>
#include <stdlib.h>
#include "linkstack.h"
#include "adjlist.h"


/*******基于栈实现的拓扑排序算法*******/
int TopSort(AdjList AOV) 
{
	int i, count = 0;
	EdgeNode *p = NULL;
	int indegree[MAXSIZE];  //用来存放各顶点的入度，indegree[i]的值为编号为i的顶点的入度
	LinkStack S;

	InitStack(&S);
	for(i = 1; i <= AOV.vexnum; i++)  //初始化indegree数组
		indegree[i] = 0;
 
	for(i = 1; i <= AOV.vexnum; i++)
		GetInDegreeOfVertex(AOV, AOV.vexs[i].data, DIRECT_GRAPH, &(indegree[i]));  //求各顶点的入度
    for(i = 1; i <= AOV.vexnum; i++)  //将入度为0的顶点编号入栈
    {
		if(0 == indegree[i])
        	Push(S, i);
	}
    while(!StackEmpty(S))
    {
		Pop(S, &i);
		printf("%c(%d) ", AOV.vexs[i].data, i);  //输出编号为i的顶点
		count++;
		p = AOV.vexs[i].first;  //删除顶点i发出的所有有向边
		while(NULL != p)
		{
			indegree[p->adjno]--;
			if(0 == indegree[p->adjno])
				Push(S, p->adjno);  //新入度为0的顶点入栈
			p = p->next;
		}
	}
	if(count < AOV.vexnum)
	{
		printf("\n There is a cycle in the AOV!\n");
			return FAIL;  //拓扑排序失败
	}
	printf("\n There is not a cycle in the AOV!\n");

	return SUCCESS;  //拓扑排序成功
}

void main()
{
	AdjList G;

	CreateGraph(&G, DIRECT_GRAPH);
	ShowGraph(G, DIRECT_GRAPH);

	printf("=====test TopSort=====\n");
	TopSort(G);
}
