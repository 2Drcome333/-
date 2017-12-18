#include<iostream>
using namespace std;
#include<fstream>
#include<cstring>
#include<cmath>


class courseNode  //将课程信息转换成结点类对象 
{
	public:
		char courseNum[4];  //课程编号 
		char courseName[30];  //课程名 
		int courseTime;  //课程学时
		int courseTerm;  //课程指定开课学期，为0则电脑设定，非0则不用加入电脑设定中
		char prerequisite[20];  //课程的先决条件，即哪些课程需要安排在该课程之前  (Note: 若无先决条件，则为NULL字符串)
}; 




class AdjMatrixGraph
{
	private:
		int vertrixs; //结点数目 
		int edges;    //边数 
		int** matrix;  //邻接矩阵 
		courseNode* allcourse;   //指针指向所有课程构成的结点数组 
		bool* tag;  //标签，设置课程是否已经被 access 过 
		int* preAsk;  //作为邻接矩阵中是先决条件元的储存矩阵 
	public:
		AdjMatrixGraph(char sourcefilename[],int totalCourseNumber);
		courseNode* getAllCourse();
		bool* getTag();
		int getVertrixs();
		int* getPreAsk();
		int** getMatrix();
		int getEdges();
		int first(int v);  //返回与结点v临接的第一个结点 
		int next(int v,int w);   //返回与结点v临接的结点中在w后的第一个结点 
};





AdjMatrixGraph::AdjMatrixGraph(char sourcefilename[],int totalCourseNumber)
{
	fstream sourcefile;
	sourcefile.open(sourcefilename,ios::in);
	
	/////////////////////////////////////////////初始化所有课程构成的结点数组 
	int count=0;
	allcourse=new courseNode[totalCourseNumber];
	
	sourcefile>>allcourse[count].courseNum;
	sourcefile>>allcourse[count].courseName;
	sourcefile>>allcourse[count].courseTime;
	sourcefile>>allcourse[count].courseTerm;
	sourcefile>>allcourse[count].prerequisite;
	while(!sourcefile.eof())
	{
		count++;
		sourcefile>>allcourse[count].courseNum;
		sourcefile>>allcourse[count].courseName;
		sourcefile>>allcourse[count].courseTime;
		sourcefile>>allcourse[count].courseTerm;
		sourcefile>>allcourse[count].prerequisite;
	}
	
	sourcefile.close();
	//**********************************************************************************
	
	////////////////////////////////////////////初始化所有标签为0，表示未 access
	tag=new bool[totalCourseNumber];
	for(int i=0;i<totalCourseNumber;i++)
	{
		tag[i]=0;
	} 
	
	//*********************************************************************************
	
	
	
	///////////////////////////////////////////初始化结点数目
	vertrixs=0;
	for(int i=0;i<totalCourseNumber;i++)
	{
		if(allcourse[i].courseTerm==0)
		{
			vertrixs++;
		}
	} 
	
	//*************************************************************************************
	
	
	//////////////////////////////////////////////初始化preAsk
	preAsk=new int[vertrixs]; 
	for(int i=0;i<vertrixs;i++)
	{
		preAsk[i]=0;
	}
	//**************************************************************************************
	
	
	
	//////////////////////////////////////////初始化边的数目及邻接矩阵 
	edges=0;
	matrix=new int* [totalCourseNumber];
	for(int i=0;i<totalCourseNumber;i++)
	{
		matrix[i]=new int[totalCourseNumber];
	}
	
	for(int i=0;i<vertrixs;i++)
	{
		for(int k=0;k<vertrixs;k++)
		{
			matrix[i][k]=0;
		}
	}
	
	for(int i=0;i<totalCourseNumber;i++)
	{
		if(strcmp(allcourse[i].prerequisite,"NULL"))
		{
			int aimposition=abs((allcourse[i].courseNum[2]-48)+(allcourse[i].courseNum[1]-48)*10-1); //将当前课程的课程编号转换成它在邻接矩阵中对应的列位置号  
			
			
			
			char precourse[(totalCourseNumber-1)*3];
			strcpy(precourse,allcourse[i].prerequisite);
			int num=strlen(precourse);
			
			for(int t=0;t<num;t+=3)
			{
				char temp[3];
				for(int k=0;k<3;k++)
				{
					temp[k]=precourse[t+k];
				}
				
				int sourceposition=abs(temp[2]-'1');  //将当前课程的先决条件课程的课程编号转换成它在邻接矩阵中对应的行位置号  
				matrix[sourceposition][aimposition]=1;
				preAsk[sourceposition]=1;
				preAsk[aimposition]++;
				edges++;
			}
		} 
		
	} 
	
	//****************************************************************************
	
} 



courseNode* AdjMatrixGraph::getAllCourse()
{
	return allcourse;
}

bool* AdjMatrixGraph::getTag()
{
	return tag;
}



int AdjMatrixGraph::getVertrixs()
{
	return vertrixs;
}



int* AdjMatrixGraph::getPreAsk()
{
	return preAsk;
}




int AdjMatrixGraph::getEdges()
{
	return edges;
}


int** AdjMatrixGraph::getMatrix()
{
	return matrix;
}



int AdjMatrixGraph::first(int v)
{
	for(int i=0;i<vertrixs;i++)
	{
		if(matrix[v][i]!=0)
		{
			return i;
		}
			
	}	
	return vertrixs;
} 


int AdjMatrixGraph::next(int v,int w)
{
	for(int i=w+1;i<vertrixs;i++)
	{
		if(matrix[v][i]!=0)
		{
			return i;
		}
	}
	return vertrixs;
}








//////////////////////////////////////////////////////////construct queue

class Queue
{
	private:
		int* queue;
		int size;
	public:
		Queue(int maxSize);
		void enqueue(int num);
		int dequeue();
		int Length();
};



Queue::Queue(int maxSize)
{
	queue=new int[maxSize];
	size=0;
}


void Queue::enqueue(int num)
{
	queue[size]=num;
	size++;
}


int Queue::dequeue()
{
	int temp=queue[0];
	for(int i=0;i<size-1;i++)
	{
		queue[i]=queue[i+1];
	}
	size--;
	return temp;
}


int Queue::Length()
{
	return size;
}





//////////////////////////////////////////////////////////construct topsort


Queue** topsort(AdjMatrixGraph gragh,Queue** queue)
{
	int flag=0;
	Queue tempqueue(gragh.getVertrixs());
	int* count=new int[gragh.getVertrixs()];
	for(int i=0;i<gragh.getVertrixs();i++)
	{
		count[i]=0;
		if(gragh.getPreAsk()[i]==0)
		{
			count[i]=gragh.getVertrixs();   //用于排除不作为先决条件的课程 
		}
	}
	for(int i=0;i<gragh.getVertrixs();i++)
	{
		for(int k=gragh.first(i);k<gragh.getVertrixs();k=gragh.next(i,k))
		{
			count[k]++;
		}
	}
	for(int i=0;i<gragh.getVertrixs();i++)
	{
		if(count[i]==0)
		{
			queue[flag]->enqueue(i);
			tempqueue.enqueue(i);
		}
	}
	int Num=gragh.getVertrixs();
	int judge=0;
	while(Num!=0)
	{
		if(judge==queue[flag]->Length())
		{
			flag++;
			judge=0;
		}
		if(tempqueue.Length()==0)
		{
			break;
		} 
		int temp=tempqueue.dequeue();
		Num--;
		judge++;
		for(int k=gragh.first(temp);k<gragh.getVertrixs();k=gragh.next(temp,k))
		{
			count[k]--;
			if(count[k]==0)
			{
				queue[flag+1]->enqueue(k);
				tempqueue.enqueue(k);
			}
		}
	}
	return queue;
}





int main(void)
{
	char name[15];
	int num;
	cout<<"请输入文件名称:";
	cin>>name;
	cout<<"请输入课程数:";
	cin>>num;
	AdjMatrixGraph graph(name,num);
	
	
	Queue** queue=new Queue*[graph.getVertrixs()];
	for(int i=0;i<graph.getVertrixs();i++)
	{
		queue[i]=new Queue(graph.getVertrixs());
	}
	queue=topsort(graph,queue);
	int count[8]={0,0,0,0,0,0,0,0};
	int askNumber[8]={6,7,3,6,5,5,5,1};
	int* reflash=new int[graph.getVertrixs()];  //用于更新先决条件 
	for(int i=0;i<graph.getVertrixs();i++)
	{
		reflash[i]=0;
	} 
	
	for(int flag=0,temp=1;temp<=8;flag++,temp++)
	{
		cout<<endl<<"************************第"<<temp<<"学期************************"<<endl;
		
		/////////////////////////////////////////////////从系统指定学期的课程中安排 
		for(int i=graph.getVertrixs();i<num;i++)
		{
			if(graph.getAllCourse()[i].courseTerm==temp)
			{
				cout<<graph.getAllCourse()[i].courseName<<endl;
				

				////////////////////////先决条件的更新 
				for(int p=0;p<graph.getVertrixs();p++)
				{
					if(graph.getPreAsk()[p])
					{
						if(graph.getMatrix()[i][p])
						{
							reflash[p]++;
						}
					}
				}
				////////////////////////////////////////////////// 
				
				graph.getTag()[i]=1;
				count[temp-1]++;
			}
		}
		
		
		////////////////////////////////////////////////从先决条件的课程中安排 
		while(queue[flag]->Length()!=0&&count[temp-1]<askNumber[temp-1])
		{
			int course=queue[flag]->dequeue();
			cout<<graph.getAllCourse()[course].courseName<<endl;
			graph.getTag()[course]=1;
			
			////////////////////////先决条件的更新 
			for(int p=0;p<graph.getVertrixs();p++)
			{
				if(graph.getPreAsk()[p])
				{
					if(graph.getMatrix()[course][p])
					{
						reflash[p]++;
					}
				}
			}
			////////////////////////////////////////////////// 
			
			count[temp-1]++;
		}
		
		////////////////////////////////////////////////从无先决条件及非系统指定的课程中安排 
		for(int i=0;count[temp-1]<askNumber[temp-1]&&i<graph.getVertrixs();i++)
		{
			if(!graph.getTag()[i]&&!graph.getPreAsk()[i])
			{
				cout<<graph.getAllCourse()[i].courseName<<endl;
				graph.getTag()[i]=1;
				count[temp-1]++;
			}
		}
		
		///////////////////////////////////////////正式更新先决条件 
		for(int i=0;i<graph.getVertrixs();i++)
		{
			if(graph.getPreAsk()[i])
			{
				graph.getPreAsk()[i]-=reflash[i];
			}
		}
		//////////////////////////////////////////////////////////// 
		
	}
	return 0;
}












