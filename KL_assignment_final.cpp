#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<cmath>
#include<map>
#include <sstream>
#include <unistd.h>
using namespace std;

class nets
{
    private:
    int netdegree;
    string nodes;
    public:
    void setdegree(int int1)
    {
        netdegree=int1;
    }
    void setnode(string str1)
    {
        nodes=str1;
    }
    int getdegree()
    {
        return netdegree;
    }
    string getnodes()
    {
        return nodes;
    }
};

vector<double> e_values_p1;
vector<double> e_values_p2;

vector<double> i_values_p1;
vector<double> i_values_p2;

vector<double> d_values_p1;
vector<double> d_values_p2;

map<string,int> c_value;
map<string,int>:: iterator c_val;

int num_nodes,num_of_terminals,num_of_pins;
double in_cut = 0.0,initial_cutsize;

vector<double> e_value;
vector<double> i_value;
vector<double> d_value;
vector<int> maxg_values;

vector <string> nodes;
vector <string> partition1;
vector <string> partition2;

int length_p1, length_p2;

vector <nets> net_ind;

map<string,double> d_value_part1;
map<string,double> d_value_part2;
map<string,double>::iterator d;

map<string,int> g_values;
map<string,int>::iterator g_values1;

map<string,int> lockedpairs_gvalues;
vector<string> lockedpairs_1;
vector<string> lockedpairs_2;
map<string,int>::iterator lpg;

vector<string> c_p1;
vector<string> c_p2;

string mystring,filename;

vector<string> d_value_nodes;

vector<string> final_partition1;
vector<string> final_partition2;


vector<string> nodes_tot;
vector<string> temp_nodes;
int total_connections;

void show_e_i_d_values(vector<string> x1,vector<double> x2)
{
    for(int i=0;i<x1.size();i++)
    {
        cout<<x1[i]<<"   "<<x2[i]<<endl;
    }
    cout<<endl;
}

vector<double> compute_d_values(vector<double>x1, vector<double>x2)
{
    d_value.clear();
    for(int i=0;i<x1.size();i++)
    {
        double x=x1[i]-x2[i];
        d_value.push_back(x);
    }
    return d_value;
}
bool find_node_t(string mystring, string current_node)
{
    int len_mystring = mystring.length();
    int len_current_node = current_node.length();
    int result = mystring.find(current_node);
    if(result == -1)
        return false;
    else if (isalpha(mystring[len_current_node + result]))
        return true;
    else if(isdigit(mystring[len_current_node + result]))
        return false;
    else
        return true;

}

vector<double> compute_e_values(vector<string> e1,int e2,int e3,nets net_ind[])
{
    e_value.clear();
    for(int i=0;i<e2;i++)
    {
        string s1,s2;
        double res=0;
        s1=e1[i];
        for(int j=0;j<e3;j++)
        {
            int net_result=1;
            s2=net_ind[j].getnodes();
            bool res1=find_node_t(s2,s1);
            if(res1==true)
            {
                for(int k=0;k<e2;k++)
                {
                    if(e1[k]==s1)
                    {
                        continue;
                    }
                    else
                    {
                        bool res2=find_node_t(s2,e1[k]);
                        if(res2==true)
                            net_result++;
                        else
                        {
                            continue;
                        }
                    }
                    
                }
            }
            else
            {
                continue;
            }
            if(net_result==net_ind[j].getdegree())
            {
                continue;
            }
            else
            {
                res = res + double(1.0/double(net_result));
            }
        }
        e_value.push_back(res);
    }
    return e_value;
}
vector<double> compute_i_values(vector<string> x1,int a1,int a3,nets net_ind[])
{
    i_value.clear();                                                                                                       
    for(int i=0;i<a1;i++)
    {
        string temp_node,temp_node1;
        double net_result=0 ;
        for(int j=0;j<a3;j++)
        {
            temp_node = x1[i];
            string temp_str1= net_ind[j].getnodes();
            bool status = find_node_t(temp_str1,temp_node);
            if(status==true)
            {
                for(int k=0;k<a1;k++)
                {
                    if(x1[k]==temp_node)
                    {
                        continue;
                    }
                    else
                    {
                        temp_node1= x1[k];
                        bool status =find_node_t(temp_str1,temp_node1);
                        if(status==true)
                        {
                            net_result++ ;
                            break;
                        }
                        else
                        continue;
                    }
                }
            }
            else
            continue;
        }
        double x=net_result;
        i_value.push_back(x);
    }
    return i_value;
}
void show_nodes(vector<string> sn)
{
   for(int i=0;i<sn.size();i++)
   {
       cout<<sn[i]<<" ";
   }
   cout<<endl;
} 

string Nodes_list(string main_string)
{
    string temp_str1,temp_str2;
    temp_str1=main_string;
    stringstream sstr2(temp_str1);
    sstr2>>temp_str2;
    return temp_str2;
}

int find_int(string main_string)
{
    string temp_str1,temp_str2;
    temp_str1= main_string;
    int temp_int,final_int;
    stringstream sstr1(temp_str1);
    while(!sstr1.eof())
    {
        sstr1>>temp_str2;
        if(stringstream(temp_str2)>>temp_int)
            final_int=temp_int;
        temp_str2.clear();
    }
    return final_int;
}

void openinputfile(ifstream& inFile)
{
    cout<<"Enter the file name: ";
    cin>>filename;
    inFile.open(filename);
    while(!inFile)
    {
        cout<<"File doesnt exist or unable to open file"<<endl;
        cout<<"Enter the file name: ";
        cin>>filename;
        inFile.clear();
        inFile.open(filename);
    }
}

int main()
{   
    ifstream inFile;
    cout<<"For Information Regarding Nodes : "<<endl<<endl;
    openinputfile(inFile);
    string str;
    string temp_node;
    while (!inFile.eof())
    {
       getline(inFile,str);
       if(str.find("NumNodes")==0)
       {
           num_nodes= find_int(str);
       }
       else if(str.find("NumTerminals")==0)
       {
           num_of_terminals = find_int(str);
       }
       else if(str.find("a")==0 || str.find("p")==0)
       {
           temp_node=Nodes_list(str);
           nodes_tot.push_back(temp_node);
       }
    } 
    inFile.close();
    cout<<endl<<"Total number of nodes are "<<num_nodes<<endl;
    cout<<endl<<"Total number of terminals are "<<num_of_terminals<<endl;
    cout<<endl<<"Nodes list "<<endl;
    for(int i=0;i<nodes_tot.size();i++)
    {
        cout<<nodes_tot[i]<<" ";
    }
    cout<<endl<<endl<<endl<<"------Processing of nodes file has completed--------"<<endl;
    cout<<endl<<endl<<endl<<"For Information Regarding Nets : "<<endl<<endl;
    openinputfile(inFile);
    int temp=0;
    while(!inFile.eof())
    {
        getline(inFile,str);
        if(str.find("NetDegree")==0)
        {
            temp++;
        }
    }
    const int total_edgeconnections = temp;
    inFile.clear();
    inFile.seekg(0,ios::beg);
    int total_connections = total_edgeconnections;
    cout<<endl<<"Total connections are "<<total_connections<<endl;
    nets net_ind[total_edgeconnections];
    temp=0;
    int temp_netdegree;
    while(!inFile.eof())
    {
        getline(inFile,str);
        if(str.find("Numpins")==0)
        {
            num_of_pins=find_int(str);
        }
        else if(str.find("NetDegree")==0)
        {
            temp_node.clear();
            temp_netdegree = find_int(str);
            for(int i=0;i<temp_netdegree;i++)
            {
                getline(inFile,str);
                temp_node = temp_node + Nodes_list(str);
                temp_nodes.push_back( Nodes_list(str) );
            }
            for(int n1 = 0; n1<temp_netdegree;n1++)
            {
                for(int n2 =n1+1; n2<temp_netdegree;n2++)
                {
                    c_value.insert(make_pair(temp_nodes[n1]+temp_nodes[n2],1));
                    c_value.insert(make_pair(temp_nodes[n2]+temp_nodes[n1],1));
                }
            }
        net_ind[temp].setdegree(temp_netdegree);
        net_ind[temp].setnode(temp_node);
        temp++;
        temp_nodes.clear();
        }
    }
    inFile.close();
    //cout<<endl<<"c_values for pairs "<<endl;
    //for(c_val=c_value.begin();c_val != c_value.end();c_val++)
    //{
    //    cout<<c_val->first<<" "<<c_val->second<<endl;
    //}
    cout<<endl<<"Node connections and respective netdegrees : "<<endl;
    for(int i=0;i<total_connections;i++)
    {
        cout<<net_ind[i].getnodes()<<" "<<net_ind[i].getdegree()<<endl;
    }
    cout<<endl<<"--------Processing of nets file has completed---------"<<endl;
    length_p1= num_nodes/2;
    cout<<endl<<"size of partition 1 is : "<<length_p1<<endl;

    length_p2= num_nodes - length_p1;
    cout<<"size of partition 2 is : "<<length_p2<<endl;

    for (int i=0;i<length_p1;i++)
    {
        partition1.push_back(nodes_tot[i]);
    }

    for(int i=length_p1;i<num_nodes;i++)
    {
        partition2.push_back(nodes_tot[i]);
    }
    cout<<endl<<"Nodes in partition 1 :"<<endl;
    show_nodes(partition1);
    cout<<endl<<"Nodes in partition 2 :"<<endl;
    show_nodes(partition2);
    //e_values,i_values and d_values computation starts
    
    e_values_p1=compute_e_values(partition1,length_p1,total_connections,net_ind);
    cout<<endl<<"e values of partition 1"<<endl;
    show_e_i_d_values(partition1,e_values_p1);
    
    e_values_p2= compute_e_values(partition2,length_p2,total_connections,net_ind);
    cout<<endl<<"e values of partition 2"<<endl;
    show_e_i_d_values(partition2,e_values_p2);

    i_values_p1=compute_i_values(partition1,length_p1,total_connections,net_ind);
    cout<<endl<<"i values of partition 1"<<endl;
    show_e_i_d_values(partition1,i_values_p1);

    i_values_p2=compute_i_values(partition2,length_p2,total_connections,net_ind);
    cout<<endl<<"i values of partition 2"<<endl;
    show_e_i_d_values(partition2,i_values_p2);
    d_values_p1=compute_d_values(e_values_p1,i_values_p1);
    cout<<endl<<"d values of partition 1"<<endl;
    show_e_i_d_values(partition1,d_values_p1);

    d_values_p2=compute_d_values(e_values_p2,i_values_p2);
    cout<<endl<<"d values of partition 2"<<endl;
    show_e_i_d_values(partition2,d_values_p2);

    for(int i=0;i<d_values_p1.size();i++)
    {
        d_value_part1.insert(make_pair(partition1[i],d_values_p1[i]));
    }
    
    for(int i=0;i<d_values_p2.size();i++)
    {
        d_value_part2.insert(make_pair(partition2[i],d_values_p2[i]));
    }
    //end of computation and storage of e,i,d values
    cout<<endl<<"The nodes in temporary partition1 iterator "<<endl;
    c_p1=partition1;
    c_p2=partition2;
    show_nodes(c_p1);
    cout<<endl<<"The nodes in temporary partition2 iterator "<<endl;
    show_nodes(c_p2);
    cout<<endl;

    //Initial cutsize evaluation starts
    for(int i=0;i<e_values_p1.size();i++)
    {
        in_cut=in_cut+e_values_p1[i];
    }
    initial_cutsize=in_cut;
    cout<<"The initial cutsize is "<<initial_cutsize<<endl;
    //end of initial cutsize evaluation
    //system("pause");
    //return 0;+

    //KL algorithm iteration starts
    for(int i=0;i<length_p1;i++)
    {
        //computation of g_values
        string temp_node,temp_node1;
        int len1= c_p1.size();
        int len2= c_p2.size();
        string node_pair;
        for (int j1=0;j1<len1;j1++) //computation of g_values for each pair of nodes from two partitions starts
        {
            int g_value=0;
            temp_node=c_p1[j1];
            for(int j=0;j<len2;j++)
            {
                double connection_val=0;
                temp_node1=c_p2[j];
                node_pair=temp_node+temp_node1;
                for( c_val=c_value.begin();c_val != c_value.end();c_val++)
                {
                    string check_n= c_val->first;
                    if(check_n == node_pair)
                    {
                        connection_val = 1.0;
                        break;
                    }   
                    else
                    {
                        continue;
                    }
                }
            g_value=d_values_p1[j1]+ d_values_p2[j]-(connection_val+connection_val) ;
            g_values.insert(make_pair(node_pair,g_value));
            }
        }
        //cout<<endl<<"The nodepairs and g values "<<endl;
        //for(g_values1=g_values.begin(); g_values1 != g_values.end() ;g_values1++)
        //{
        //cout<<g_values1->first<<" "<<g_values1->second<<endl;
        //}
       
        
        //end of extraction of g_pairs and respective values 

        //start of finding the maxg pair
        int maxg;
        g_values1=g_values.begin();
        maxg=g_values1->second;
        string maxg_pair=g_values1->first;
        for(g_values1=g_values.begin();g_values1 != g_values.end();g_values1++)
        {
            if((g_values1->second) > maxg)
            {
                maxg=g_values1->second;
                maxg_pair=g_values1->first;
            }
            else
                continue;
        }
        cout<<endl<<"Maximum gain value is "<<maxg<<" and the node pair is "<<maxg_pair<<endl;
        lockedpairs_gvalues.insert(make_pair(maxg_pair,maxg));//stores all the maxg pairs and its value
        maxg_values.push_back(maxg);

        for(int k=1;k<maxg_pair.length();k++)//for finding out each node present in the maxg nodepair
        {
            if(maxg_pair[k]=='a'|| maxg_pair[k]=='p')
                {
                    temp_node= maxg_pair.substr(0,k);
                    temp_node1= maxg_pair.substr(k,maxg_pair.length()-k);
                    break;
                }
        }
        cout<<endl<<"The nodes of nodepair are "<<endl<<temp_node<<"  "<<temp_node1<<endl;//
        lockedpairs_1.push_back(temp_node);
        lockedpairs_2.push_back(temp_node1);
        g_values.clear();
        //end of finding maxgpair and individual nodes
        //deleting the current individual maxg pair nodes from the temporary partitions
        string del_node;
        del_node= lockedpairs_1[i];
        for(int j1=0;j1<c_p1.size();j1++)
        {
            if(c_p1[j1]==del_node)
            {
                c_p1.erase(c_p1.begin()+j1);
                d_values_p1.erase(d_values_p1.begin()+j1);
                break;
            }        
        }
        cout<<endl<<"The nodes in partition1 iterator "<<endl;
        show_nodes(c_p1);
        if(i==length_p1-1)
        {
            cout<<"Empty/Null"<<endl;
        }
        del_node= lockedpairs_2[i];
        for(int j2=0;j2<c_p2.size();j2++)
        {
            if(c_p2[j2]==del_node)
            {
                c_p2.erase(c_p2.begin()+j2);
                d_values_p2.erase(d_values_p2.begin()+j2);
                break;
            }
        }
        cout<<endl<<"The nodes in partition2 iterator "<<endl;
        show_nodes(c_p2);
        if(i==length_p1-1)
        {
            cout<<"Empty/Null"<<endl;
        }
        //end of process of deleting nodes
    
        if(i==length_p1-1)
        {
            break;
        }
        //Updation of  d_values start
        string nd1= lockedpairs_1[i];
        string nd2;
        int l1= nd1.length();
        for(c_val=c_value.begin();c_val != c_value.end();c_val++)
        {
            int p=0,q=0;
            string mystring = c_val->first;
            if(mystring.substr(0,l1)==nd1)
            {
                nd2= mystring.substr(l1,mystring.length()-l1);
                for(int x=0;x<c_p1.size();x++)
                {
                    if(nd2 == c_p1[x])
                    {
                        p=1;
                        break;
                    }
                }
                if(p != 1)
                {
                    for(int x=0;x<c_p2.size();x++)
                    {
                        if(nd2 == c_p2[x])
                        {
                            q=1;
                            break;
                        }
                    }
                }
                if(p==1 || q==1)
                {
                    d_value_nodes.push_back(nd2);
                }
                else
                continue;
            }
            else
            continue;
        }
        
        nd1= lockedpairs_2[i];
        l1= nd1.length();
        for(c_val=c_value.begin();c_val != c_value.end();c_val++)
        {
            int p=0,q=0,r=0;
            string mystring = c_val->first;
            if(mystring.substr(0,l1)==nd1)
            {
                nd2= mystring.substr(l1,mystring.length()-l1);
                for(int x=0;x<d_value_nodes.size();x++)
                {
                    if(nd2==d_value_nodes[x])
                    {
                        r=1;
                        break;
                    }
                }
                if(r != 1)
                {
                    for(int x=0;x<c_p1.size();x++)
                    {
                        if(nd2 == c_p1[x])
                        {
                            p=1;
                            break;                    
                        }
                    }
                    if(p != 1)
                    {
                        for(int x=0;x<c_p2.size();x++)
                        {
                            if(nd2 == c_p2[x])
                            {
                                q=1;
                                break;
                            }
                        }
                    }
                    if(p==1 || q==1  )
                    {
                        d_value_nodes.push_back(nd2);
                    }
                }
            }
        }
        cout<<endl<<"The nodes connected to locked nodes are "<<endl;
        show_nodes(d_value_nodes);
        for(int s=0;s<d_value_nodes.size();s++)
        {
            int index=0,c1=0,c2=0;
            temp_node= d_value_nodes[s];
            for(int d=0;d<c_p1.size();d++)
            {
                if(c_p1[d]==temp_node)
                {
                    index=d;
                    c1=1;
                    break;
                }
            }
            if(c1 !=1)
            {
               for(int d=0;d<c_p2.size();d++)
                {
                    if(c_p2[d]==temp_node)
                    {
                    index=d;
                    c2=1;
                    break;
                    }
                }
            }
            if(c1==1)
            {
                double cD=0.0,D,cv1=0.0,cv2=0.0;
                D = d_values_p1[index];
                string fnode,fnode1,fnode2,pairnode1,pairnode2;
                fnode=c_p1[index];
                fnode1= lockedpairs_1[i];
                fnode2=lockedpairs_2[i];
                pairnode1= fnode+fnode1;
                pairnode2= fnode+fnode2;
                for(c_val=c_value.begin();c_val != c_value.end();c_val++)
                {
                    if(c_val->first == pairnode1)
                    {
                        cv1= 2;
                    }
                    else if(c_val->first == pairnode2)
                    {
                        cv2= 2;
                    }
                    else if(cv1 != 0 && cv2 !=0)
                    break;
                    else
                    continue;
                }
                cD=D+cv1-cv2;
                d_values_p1[index]=cD;
            }
            else
            {
                double cD=0,D,cv1=0,cv2=0;
                D = d_values_p2[index];
                string fnode,fnode1,fnode2,pairnode1,pairnode2;
                fnode=c_p2[index];
                fnode1= lockedpairs_1[i];
                fnode2=lockedpairs_2[i];
                pairnode1= fnode+fnode1;
                pairnode2= fnode+fnode2;
                for(c_val=c_value.begin();c_val != c_value.end();c_val++)
                {
                    if(c_val->first == pairnode1)
                    {
                        cv1= 2;
                    }
                    else if(c_val->first == pairnode2)
                    {
                        cv2= 2;
                    }
                    else if(cv1 != 0 && cv2 !=0)
                    break;
                    else
                    continue;
                }
                cD=D-cv1+cv2;
                d_values_p2[index]=cD;
            } 
        }
        d_value_nodes.clear();
        /*cout<<endl<<endl<<"The updated d_values of partition 1 iterator "<<endl;
        show_e_i_d_values(c_p1,d_values_p1);//
        cout<<endl;//
        cout<<endl<<"The updated d_values of partition 2 iterator "<<endl;
        show_e_i_d_values(c_p2,d_values_p2);//*/
        //end of updating D values
    }
    cout<<endl<<endl<<"---------------End of maxg Iterations--------------"<<endl<<endl;
    cout<<endl<<"The locked node pairs and respective maxg_values during each iteration"<<endl<<endl;
    for( lpg=lockedpairs_gvalues.begin();lpg != lockedpairs_gvalues.end(); lpg++)
    {
        cout<<lpg->first<<" "<<lpg->second<<endl;
    }
    cout<<endl<<endl<<"The maxg values in order of iterations are"<<endl<<endl;;
    for(int i=0;i<maxg_values.size();i++)
    {
        cout<<maxg_values[i]<<endl;
    }
    //loop completed
    //final evaluation starts
    double init_g=0.0;
    double g=0.0,g1;
    int i,j=(-1);
    for(i=0;i<maxg_values.size();i++)
    {
        g= init_g+maxg_values[i];
        if(g>init_g)
        {
            j=i;
            init_g=g;
            g1=g;
        }
        else if(g<=init_g)
        {
            break;
        }
        else
        {
            continue;
        }
        
    }
    final_partition1=partition1;
    final_partition2=partition2;
    string n1,n2;
    int x,y;
    if(j>-1)
    {
        for(int k=0;k<=j;k++)
        {
            n1=lockedpairs_1[k];
            for(x=0;x<final_partition1.size();x++)
            {
                if(n1==final_partition1[x])
                    break;
                else
                    continue;
            }
            n2=lockedpairs_2[k];
            for( y=0;y<final_partition2.size();y++)
            {
                if(n2==final_partition2[y])
                    break;
                else 
                    continue;
            }
            final_partition2[y]=n1;
            final_partition1[x]=n2;
        }
    }
    //end of final iterations
    double final_cutsize= initial_cutsize-g1;
    cout<<endl<<endl<<"The Initial Cutsize is "<<initial_cutsize<<endl;
    cout<<endl<<endl<<"The final_cutsize is "<<final_cutsize<<endl;
    cout<<endl<<endl<<"The final partition 1 "<<endl;
    show_nodes(final_partition1);
    cout<<endl<<endl<<"The final partition 2 "<<endl;
    show_nodes(final_partition2);
    cout<<endl<<endl<<endl;
    system("pause");
    return 0;
}