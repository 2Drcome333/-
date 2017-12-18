#include<iostream>
using namespace std;
#include<fstream>
#include<cstring>
#include<cmath>


class courseNode  //���γ���Ϣת���ɽ������� 
{
	public:
		char courseNum[4];  //�γ̱�� 
		char courseName[30];  //�γ��� 
		int courseTime;  //�γ�ѧʱ
		int courseTerm;  //�γ�ָ������ѧ�ڣ�Ϊ0������趨����0���ü�������趨��
		char prerequisite[20];  //�γ̵��Ⱦ�����������Щ�γ���Ҫ�����ڸÿγ�֮ǰ  (Note: �����Ⱦ���������ΪNULL�ַ���)
}; 




class AdjMatrixGraph
{
	private:
		int vertrixs; //�����Ŀ 
		int edges;    //���� 
		int** matrix;  //�ڽӾ��� 
		courseNode* allcourse;   //ָ��ָ�����пγ̹��ɵĽ������ 
		bool* tag;  //��ǩ�����ÿγ��Ƿ��Ѿ��� access �� 
		int* preAsk;  //��Ϊ�ڽӾ��������Ⱦ�����Ԫ�Ĵ������ 
	public:
		AdjMatrixGraph(char sourcefilename[],int totalCourseNumber);
		courseNode* getAllCourse();
		bool* getTag();
		int getVertrixs();
		int* getPreAsk();
		int** getMatrix();
		int getEdges();
		int first(int v);  //��������v�ٽӵĵ�һ����� 
		int next(int v,int w);   //��������v�ٽӵĽ������w��ĵ�һ����� 
};





AdjMatrixGraph::AdjMatrixGraph(char sourcefilename[],int totalCourseNumber)
{
	fstream sourcefile;
	sourcefile.open(sourcefilename,ios::in);
	
	/////////////////////////////////////////////��ʼ�����пγ̹��ɵĽ������ 
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
	
	////////////////////////////////////////////��ʼ�����б�ǩΪ0����ʾδ access
	tag=new bool[totalCourseNumber];
	for(int i=0;i<totalCourseNumber;i++)
	{
		tag[i]=0;
	} 
	
	//*********************************************************************************
	
	
	
	///////////////////////////////////////////��ʼ�������Ŀ
	vertrixs=0;
	for(int i=0;i<totalCourseNumber;i++)
	{
		if(allcourse[i].courseTerm==0)
		{
			vertrixs++;
		}
	} 
	
	//*************************************************************************************
	
	
	//////////////////////////////////////////////��ʼ��preAsk
	preAsk=new int[vertrixs]; 
	for(int i=0;i<vertrixs;i++)
	{
		preAsk[i]=0;
	}
	//**************************************************************************************
	
	
	
	//////////////////////////////////////////��ʼ���ߵ���Ŀ���ڽӾ��� 
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
			int aimposition=abs((allcourse[i].courseNum[2]-48)+(allcourse[i].courseNum[1]-48)*10-1); //����ǰ�γ̵Ŀγ̱��ת���������ڽӾ����ж�Ӧ����λ�ú�  
			
			
			
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
				
				int sourceposition=abs(temp[2]-'1');  //����ǰ�γ̵��Ⱦ������γ̵Ŀγ̱��ת���������ڽӾ����ж�Ӧ����λ�ú�  
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
			count[i]=gragh.getVertrixs();   //�����ų�����Ϊ�Ⱦ������Ŀγ� 
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
	cout<<"�������ļ�����:";
	cin>>name;
	cout<<"������γ���:";
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
	int* reflash=new int[graph.getVertrixs()];  //���ڸ����Ⱦ����� 
	for(int i=0;i<graph.getVertrixs();i++)
	{
		reflash[i]=0;
	} 
	
	for(int flag=0,temp=1;temp<=8;flag++,temp++)
	{
		cout<<endl<<"************************��"<<temp<<"ѧ��************************"<<endl;
		
		/////////////////////////////////////////////////��ϵͳָ��ѧ�ڵĿγ��а��� 
		for(int i=graph.getVertrixs();i<num;i++)
		{
			if(graph.getAllCourse()[i].courseTerm==temp)
			{
				cout<<graph.getAllCourse()[i].courseName<<endl;
				

				////////////////////////�Ⱦ������ĸ��� 
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
		
		
		////////////////////////////////////////////////���Ⱦ������Ŀγ��а��� 
		while(queue[flag]->Length()!=0&&count[temp-1]<askNumber[temp-1])
		{
			int course=queue[flag]->dequeue();
			cout<<graph.getAllCourse()[course].courseName<<endl;
			graph.getTag()[course]=1;
			
			////////////////////////�Ⱦ������ĸ��� 
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
		
		////////////////////////////////////////////////�����Ⱦ���������ϵͳָ���Ŀγ��а��� 
		for(int i=0;count[temp-1]<askNumber[temp-1]&&i<graph.getVertrixs();i++)
		{
			if(!graph.getTag()[i]&&!graph.getPreAsk()[i])
			{
				cout<<graph.getAllCourse()[i].courseName<<endl;
				graph.getTag()[i]=1;
				count[temp-1]++;
			}
		}
		
		///////////////////////////////////////////��ʽ�����Ⱦ����� 
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












