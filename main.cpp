#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>   //for using setw() and setprecision()
#include <stdio.h>

#define TESTING
//#define ISCLASS_ANS  //If the given dataset have CLASS answer
#define DEBUG
using namespace std;

ifstream dataset;
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

int main()
{   
    vector<vector<float>> datapoints;
    vector<float> each_datapoint;
    vector<string> class_ans;
    int start = 0;
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
    return 0;

}