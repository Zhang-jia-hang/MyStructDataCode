#include <stdio.h>
#include <stdlib.h>
#include "sqstack.h"
#include "csqqueue.h"
#include "adjlist.h"

/*创建运算: 创建一个无向/有向图，结点编号从1开始*/
/*graphtype为UN_DIRECT_GRAPH表示创建无向图的邻接表*/
/*graphtype为DIRECT_GRAPH表示创建有向图的邻接表*/
/*graphtype为UN_DIRECT_GRAPH_REV表示创建有向图的邻接表*/
int CreateGraph(AdjList *G, int graphtype)
{
    int i, s, d;
	EdgeNode *p = NULL, *q = NULL;

	if(graphtype - GRAPH > 2 || graphtype - GRAPH < 0)
		return PARAMETER_ERR;

    printf("Please input graph's vexnum: ");
    scanf("%d", &((*G).vexnum));
	printf("Please input graph's arcnum: ");
    scanf("%d", &((*G).arcnum));

	//初始化顶点向量
    for (i = 1; i <= (*G).vexnum; i++)
	{
		getchar();
		printf("Please input No.%d vertex\'s value: ", i);		
        scanf("%c", &((*G).vexs[i].data));	
		(*G).vexs[i].first = NULL;
	}
	//根据输入的边的信息创建各个顶点对应的邻接链表(采用头部创建法)
	if(UN_DIRECT_GRAPH == graphtype || DIRECT_GRAPH == graphtype)
	{
		for (i = 1; i <= (*G).arcnum; i++)
		{
			printf("Please input NO.%d edge (start,end): ", i);
        	scanf("%d,%d", &s, &d);
        	p = (EdgeNode *)malloc(sizeof(EdgeNode));
			if(NULL == p)
				return STORAGE_ERROR;
			p->adjno = d;
			p->next = (*G).vexs[s].first;
			(*G).vexs[s].first = p;
			if(UN_DIRECT_GRAPH == graphtype)
			{
				q = (EdgeNode *)malloc(sizeof(EdgeNode));
				if(NULL == q)
					return STORAGE_ERROR;
				q->adjno = s;
				q->next = (*G).vexs[d].first;
				(*G).vexs[d].first = q;
			}
		}
	}
	else if(DIRECT_GRAPH_REV == graphtype)
	{
		for (i = 1; i <= (*G).arcnum; i++)
		{
			printf("Please input NO.%d edge (start,end): ", i);
        	scanf("%d,%d", &s, &d);
        	q = (EdgeNode *)malloc(sizeof(EdgeNode));
			if(NULL == q)
				return STORAGE_ERROR;
			q->adjno = s;
			q->next = (*G).vexs[d].first;
			(*G).vexs[d].first = q;
		}
	}

	return SUCCESS;
}

/*判断图类型运算: 判断G的类型*/
/*只能区分是无向图还是有向图, 不能判断是有向图的邻接表结构还是逆邻接表结构*/
int JudgeGraphType(AdjList G)
{
	int count = 0, i;
	EdgeNode *p = NULL;

	//统计表结点的个数	
	for(i = 1; i <= G.vexnum; i++)
	{
		p = G.vexs[i].first;
		while(NULL != p)
		{
			count++;
			p = p->next;
		}
	}
	
	if(count == G.arcnum)
		return DIRECT_GRAPH;       //有向图
	else if(count == 2 * G.arcnum)
		return UN_DIRECT_GRAPH;    //无向图
	
	return FAIL;
}

/*查找运算: 返回顶点vertex的编号*/
int LocateVertex(AdjList G, VertexType vertex)
{
	int i;

	for(i = 1; i <= G.vexnum ; i++)
	{
		if(G.vexs[i].data == vertex)
			return i;
	}
	return NOT_EXIST;
}

/*访问邻接顶点运算: 访问顶点vertex的所有邻接顶点*/
int FindAllAdjVertexs(AdjList G, VertexType vertex, int graphtype)
{
	int count_adjvertexs, i;
	int vertex_loc;
	EdgeNode *p = NULL;

	vertex_loc = LocateVertex(G, vertex);  //获得顶点vertex的编号
	if(NOT_EXIST == vertex_loc)
		return PARAMETER_ERR;

	if(UN_DIRECT_GRAPH == graphtype)       //如果G是无向图的邻接表
	{
		count_adjvertexs = 0;
		printf("The adjacent vertexs of the vertex %c(%d) are: ", vertex, vertex_loc);
		p = G.vexs[vertex_loc].first;
		while(NULL != p)
		{
			count_adjvertexs++;
			printf("%c(%d)  ", G.vexs[p->adjno].data, p->adjno);
			p = p->next;
		}
		if(0 == count_adjvertexs)
			printf("no vertex.");
		printf("\nThe vertex %c has %d adjacent vertexs.\n", vertex, count_adjvertexs);

		return SUCCESS;
	}
	else if(DIRECT_GRAPH == graphtype)      //如果G是有向图的邻接表
	{
		count_adjvertexs = 0;
		printf("The vertex %c(%d) adjoins to: ", vertex, vertex_loc);  //输出所有邻接到的顶点
		p = G.vexs[vertex_loc].first;
		while(NULL != p)
		{
			count_adjvertexs++;
			printf("%c(%d)  ", G.vexs[p->adjno].data, p->adjno);
			p = p->next;
		}
		if(0 == count_adjvertexs)
			printf("no vertex.");
		printf("\nThe vertex %c sends %d arcs.\n", vertex, count_adjvertexs);

		count_adjvertexs = 0;
		printf("The vertex %c(%d) is adjoined by: ", vertex, vertex_loc);  //输出所有邻接于的顶点
		for(i = 1; i <= G.vexnum; i++)
		{
			p = G.vexs[i].first;
			while(NULL != p)
			{
				if(p->adjno == vertex_loc)
				{
					printf("%c(%d)  ", G.vexs[i].data, i);
					count_adjvertexs++;
					break;
				}
				p = p->next;
			}
		}
		if(0 == count_adjvertexs)
			printf("no vertex.");
		printf("\nThere are %d arcs which point to the vertex %c.\n", count_adjvertexs, vertex);

		return SUCCESS;
	}
	else if(DIRECT_GRAPH_REV == graphtype)      //如果G是有向图的逆邻接表
	{
		count_adjvertexs = 0;
		printf("The vertex %c(%d) adjoins to: ", vertex, vertex_loc);  //输出所有邻接到的顶点
		for(i = 1; i <= G.vexnum; i++)
		{
			p = G.vexs[i].first;
			while(NULL != p)
			{
				if(p->adjno == vertex_loc)
				{
					printf("%c(%d)  ", G.vexs[i].data, i);
					count_adjvertexs++;
					break;
				}
				p = p->next;
			}
		}
		if(0 == count_adjvertexs)
			printf("no vertex.");
		printf("\nThe vertex %c sends %d arcs.\n", vertex, count_adjvertexs);

		count_adjvertexs = 0;
		printf("The vertex %c(%d) is adjoined by: ", vertex, vertex_loc);  //输出所有邻接于的顶点
		p = G.vexs[vertex_loc].first;
		while(NULL != p)
		{
			count_adjvertexs++;
			printf("%c(%d)  ", G.vexs[p->adjno].data, p->adjno);
			p = p->next;
		}		
		if(0 == count_adjvertexs)
			printf("no vertex.");
		printf("\nThere are %d arcs which point to the vertex %c.\n", count_adjvertexs, vertex);

		return SUCCESS;
	}
	else
		return PARAMETER_ERR;
}

/*求顶点度的运算: 求顶点vertex的度*/
int GetDegreeOfVertex(AdjList G, VertexType vertex, int graphtype, int *degree)
{
	int vertex_loc, indegree = 0, outdegree = 0;
	EdgeNode *p = NULL;

	*degree = 0;
	if(UN_DIRECT_GRAPH == graphtype)       //如果G是无向图的邻接表
	{
		vertex_loc = LocateVertex(G, vertex);  //获得顶点vertex的编号
		if(NOT_EXIST == vertex_loc)
			return PARAMETER_ERR;

		p = G.vexs[vertex_loc].first;
		while(NULL != p)
		{
			(*degree)++;
			p = p->next;
		}

		return SUCCESS;
	}
	else if(DIRECT_GRAPH == graphtype || DIRECT_GRAPH_REV == graphtype)//如果G是有向图
	{
		if(FAIL == GetInDegreeOfVertex(G, vertex, graphtype, &indegree))
			return FAIL;
		if(FAIL == GetOutDegreeOfVertex(G, vertex, graphtype, &outdegree))
			return FAIL;
		*degree = indegree + outdegree;
	}
	return SUCCESS;
}

/*求顶点入度的运算(仅适用于有向图): 求顶点vertex的入度*/
int GetInDegreeOfVertex(AdjList G, VertexType vertex, int graphtype, int *indegree)
{
	int i, vertex_loc;
	EdgeNode *p = NULL;

	if(DIRECT_GRAPH == graphtype || DIRECT_GRAPH_REV == graphtype)
	{
		vertex_loc = LocateVertex(G, vertex);  //获得顶点vertex的编号
		if(NOT_EXIST == vertex_loc)
			return PARAMETER_ERR;

		*indegree = 0;
		if(DIRECT_GRAPH == graphtype)         //如果G是有向图的邻接表
		{			
			for(i = 1; i <= G.vexnum; i++)
			{
				p = G.vexs[i].first;
				while(NULL != p)
				{
					if(p->adjno == vertex_loc)
					{
						(*indegree)++;
						break;
					}
					p = p->next;
				}
			}

			return SUCCESS;
		}
		else                                  //如果G是有向图的逆邻接表
		{
			p = G.vexs[vertex_loc].first;
			while(NULL != p)
			{
				(*indegree)++;
				p = p->next;
			}

			return SUCCESS;
		}
	}
	else
		return FAIL; 
}

/*求顶点出度的运算(仅适用于有向图): 求顶点vertex的出度*/
int GetOutDegreeOfVertex(AdjList G, VertexType vertex, int graphtype, int *outdegree)
{
	int i, vertex_loc;
	EdgeNode *p = NULL;

	if(DIRECT_GRAPH == graphtype || DIRECT_GRAPH_REV == graphtype)
	{
		vertex_loc = LocateVertex(G, vertex);  //获得顶点vertex的编号
		if(NOT_EXIST == vertex_loc)
			return PARAMETER_ERR;

		*outdegree = 0;
		if(DIRECT_GRAPH_REV == graphtype)      //如果G是有向图的逆邻接表
		{			
			for(i = 1; i <= G.vexnum; i++)
			{
				p = G.vexs[i].first;
				while(NULL != p)
				{
					if(p->adjno == vertex_loc)
					{
						(*outdegree)++;
						break;
					}
					p = p->next;
				}
			}

			return SUCCESS;
		}
		else                                  //如果G是有向图的邻接表
		{
			p = G.vexs[vertex_loc].first;
			while(NULL != p)
			{
				(*outdegree)++;
				p = p->next;
			}

			return SUCCESS;
		}
	}
	else
		return FAIL;
}

/*深度优先搜索运算: 在深度优先搜索遍历的过程中对每个顶点调用一次visit()操作*/
/*只适用于无向图邻接表和有向图邻接表存储结构*/
void DFSTraverse(AdjList G, int visit(VertexType vertex))
{
	int k;
	int *visited_array = NULL;
	EdgeNode *p = NULL;
	
	visited_array = (int *)malloc(sizeof(int) * (G.vexnum + 1));
	if(NULL == visited_array)
		return;

	//初始化标识向量
	for(k = 1; k <= G.vexnum; k++)
		visited_array[k] = 0;        //visited_array[k]为0表示顶点k没有被访问过
	
	for(k = 1; k <= G.vexnum; k++)
	{
		if(0 == visited_array[k])    //若编号为k的顶点未被访问过，则以该顶点为出发顶点
		{
			DFSTraverse_FromVertex(G, k, visited_array, &visit);
		}
	}
	printf("\n");

	free(visited_array);
	visited_array = NULL;
}

/*从指定顶点开始进行深度优先搜索运算: 以编号为vertex_loc的顶点为出发顶点对G进行深度优先搜索遍历*/
void DFSTraverse_FromVertex(AdjList G, int vertex_loc, int *visited_array, int visit(VertexType vertex))
{
	SqStack S;
	EdgeNode *p = NULL;

	InitStack(&S);
	visit(G.vexs[vertex_loc].data); //访问出发顶点
	visited_array[vertex_loc] = 1;  //visited_array[vertex_loc]为1表示顶点vertex_loc已被访问过
	p = G.vexs[vertex_loc].first;
	Push(&S, p);
	while(!StackEmpty(S))
	{
		Pop(&S, &p);
		while(NULL != p && 1 == visited_array[p->adjno])
			p = p->next;
		if(NULL != p)
		{
			visit(G.vexs[p->adjno].data);
			visited_array[p->adjno] = 1;
			Push(&S, p->next);
			p = G.vexs[p->adjno].first;
			Push(&S, p);
		}
	}
}

/*广度优先搜索运算: 在广度优先搜索遍历的过程中对每个顶点调用一次visit()操作*/
/*只适用于无向图邻接表和有向图邻接表存储结构*/
void BFSTraverse(AdjList G, int visit(VertexType vertex))
{
	int k;
	int *visited_array = NULL;
	EdgeNode *p = NULL;
	
	visited_array = (int *)malloc(sizeof(int) * (G.vexnum + 1));
	if(NULL == visited_array)
		return;

	//初始化标识向量
	for(k = 1; k <= G.vexnum; k++)
		visited_array[k] = 0;       //visited_array[k]为0表示顶点k没有被访问过

	for(k = 1; k <= G.vexnum; k++)
	{
		if(0 == visited_array[k])    //若编号为k的顶点未被访问过，则以该顶点为出发顶点
		{
			BFSTraverse_FromVertex(G, k, visited_array, &visit);
		}
	}
	printf("\n");

	free(visited_array);
	visited_array = NULL;
}

/*从指定顶点开始进行广度优先搜索运算: 以编号为vertex_loc的顶点为出发顶点对G进行广度优先搜索遍历*/
void BFSTraverse_FromVertex(AdjList G, int vertex_loc, int *visited_array, int visit(VertexType vertex))
{
	CSqQueue Q;
	EdgeNode *p = NULL, *q = NULL;
	
	InitQueue(&Q);
	visit(G.vexs[vertex_loc].data);  //访问编号为vertex_loc的顶点
	visited_array[vertex_loc] = 1;

	p = G.vexs[vertex_loc].first;
	while(NULL != p)
	{
		if(0 == visited_array[p->adjno])
		{
			visit(G.vexs[p->adjno].data); 
			visited_array[p->adjno] = 1;
			EnQueue(&Q, p);
		}
		p = p->next;
	}

	while(!QueueEmpty(Q))
	{
		DeQueue(&Q, &q);
		p = G.vexs[q->adjno].first;
		while(NULL != p)
		{
			if(0 == visited_array[p->adjno])
			{
				visit(G.vexs[p->adjno].data); 
				visited_array[p->adjno] = 1;
				EnQueue(&Q, p);
			}
			p = p->next;
		}
	}
}

/*输出/打印运算: 输出图G*/
void ShowGraph(AdjList G, int graphtype)
{
	int i;

	if(UN_DIRECT_GRAPH == graphtype)
		printf("This is an undirected graph, it has %d vertexs and %d edeges.\n", G.vexnum, G.arcnum);
	else if(DIRECT_GRAPH == graphtype || DIRECT_GRAPH_REV == graphtype)
		printf("This is a directed graph, it has %d vertexs and %d arcs.\n", G.vexnum, G.arcnum);
	else
	{
		printf("The parameter \"graphtype\" is wrong!\n");
		return;
	}

	for(i = 1; i <= G.vexnum; i++)
	{
		FindAllAdjVertexs(G, G.vexs[i].data, graphtype);
	}
}

/*撤销运算: 撤销图G*/
void DestroyGraph(AdjList *G)
{
	int i, count = 0;
	EdgeNode *p = NULL, *q = NULL;

	for(i = 1; i <= (*G).vexnum; i++)
	{
		p = (*G).vexs[i].first;
		(*G).vexs[i].first = NULL;
		while(NULL != p)
		{
			q = p;
			p = p->next;
			free(q);
			q = NULL;
			count++;
		}		
	}
	(*G).arcnum = 0;
	(*G).vexnum = 0;
	printf("%d list nodes have been deleted!\n", count);
}
