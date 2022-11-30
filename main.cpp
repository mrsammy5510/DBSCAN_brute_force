#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>   //for using setw() and setprecision()
#include <stdio.h>
#include <math.h>
#define TESTING
//#define ISCLASS_ANS  //If the given dataset have CLASS answer
//#define DEBUG
#define element float
using namespace std;

ifstream dataset;


class queue_node
{
    public:
        element data = 0;
        queue_node* next = NULL;
};
queue_node* head = NULL;
queue_node* tail = NULL;

void enqueue(element data)
{
    if(head)
    {
        tail->next = new queue_node;
        tail = tail->next;
        tail->data = data;
    }
    else
    {
        head = new queue_node;
        tail = head;
        head->data = data;
    }
    return;
}

element dequeue(void)
{
    element data;
    queue_node* temp;
    if(head)
    {
        data = head->data;
        temp = head;
        head = head->next;
        delete temp;
        return data;
    }
    else
    {
        return -1;
    }
}

void print_2d_vector(vector<vector<float>> vector)
{
    for(int i = 0;i<vector.size();i++)
    {
        cout<<"For point "<<setw(5)<<left<<i<<"the attributes is: ";    
        for(int j = 0;j<vector[0].size();j++)
        {
            cout<<fixed<<setprecision(1)<<vector[i][j]<<" ";
        }
        cout<<endl;
    }
    return;
}

int get_dim(string input_filename)
{
    string temp = "";
    int i = 0,dim = 0;
    getline(dataset,temp);
    //######用來數dim, 會扣掉class的那個dim######
    while(temp[i]!='\0')
    {
        if(temp[i]==',')
        {
            dim++;
        }
        i++;
    }
    //#########################################
    dataset.close();
    dataset.open(input_filename);   //Read the dataset once more since the first element is used for count dim
    return dim;
}

float dist(vector<float> point1, vector<float> point2)
{
    float sum = 0;
    for(int i = 0;i<point1.size();i++)
    {
        sum+= pow(point1[i]-point2[i],2);
    }
    return sqrt(sum);
}

bool all_visited(vector<bool> visited)
{
    for(int i = 0;i<visited.size();i++)
    {
        if(visited[i]==0)
        {
            return false;
        }
    }
    return true;
}

void DBSCAN_bf(vector<vector<float>> distance, int* clustering_result, float dc, int epsilon)
{
    vector<bool> visited(distance[0].size(),false);
    enqueue(0);
    int point_now, neighbor = 0;
    int c = 0;
    while(!all_visited(visited))
    {
        point_now = dequeue();
        if(point_now == -1)     //-1 means Queue empty
        {
            c++;
            for(int i = 0;i<visited.size();i++)
            {
                if(visited[i]==false)
                {
                    point_now = i;
                    break;
                }
            }
            
        }
        if(visited[point_now]!=true)
        {
            for(int i=0;i<distance[0].size();i++)
            {
                if(distance[point_now][i]<dc)
                {
                    neighbor++;
                    enqueue(i);
                }
            }
            neighbor = 0;
            clustering_result[point_now] = c;
            visited[point_now] = true;
        }
        

    }
}

int main()
{   

    vector<vector<float>> datapoints;
    vector<float> each_datapoint;
    vector<string> class_ans;
    int i = 0,dim = 0;
    string input_filename  = "iris.data";
    string temp = "temp"; 
    #ifndef TESTING
                cout<<"Insert the dataset name:"<<endl;
                getline(cin,input_filename);
    #endif
    
    dataset.open(input_filename);
    if(dataset.fail())
    {
        cout<<"File "<<input_filename<<" doesn't exist."<<endl;
    }
    else
    {
        dim = get_dim(input_filename);

        while(!dataset.eof())
        {
            for(int k = 0;k<dim;k++)
            {
                getline(dataset,temp,',');      //抓直到下一個逗號或是換行之前的文字
                each_datapoint.push_back(atof(temp.c_str()));
            }
            getline(dataset,temp,'\n');  //最後的這個是class

            #ifdef ISCLASS_ANS
                class_ans.push_back(temp);
            #endif

            datapoints.push_back(each_datapoint);
            each_datapoint.clear();
            
        }
        int number_of_points = datapoints.size();       //The number is 3 if from 0~2
        vector<vector<float>> distance_between_all_points;
        vector<float> distance_for_a_point;
        int cluster_result[number_of_points] = {-1};    //-1 refers to noise
        for(int row = 0;row<number_of_points;row++)
        {
            for(int col = 0;col<number_of_points;col++)
            {
                distance_for_a_point.push_back(dist(datapoints[row],datapoints[col]));
            }
            distance_between_all_points.push_back(distance_for_a_point);
            distance_for_a_point.clear();
        }
        print_2d_vector(distance_between_all_points);
        DBSCAN_bf(distance_between_all_points,cluster_result,0.6,3);

        for(int i =0 ;i<number_of_points;i++)
        {
            cout<<"For point "<<i<<" is belong to cluster "<<cluster_result[i]<<endl;
        }

        #ifdef DEBUG
            #ifdef ISCLASS_ANS
                for(int k = 0;k<class_ans.size();k++)
                {
                    cout<<"Point"<<k<<"'s class is:"<< class_ans[k]<<endl;
                }
            #endif
            print_2d_vector(datapoints);
        #endif
            
            
        }


    system("pause");
}