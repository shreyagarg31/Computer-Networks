#include<bits/stdc++.h>
using namespace std;

int main()
{
	vector<int> msg;
	int r=0,n;
	cout<<"No of message bits:";
	cin>>n;
	while(n+r+1 > pow(2,r)) r++;
	cout<<"No of redundant bits: "<<r<<endl;
	cout<<"Enter the message:";
	for(int i=0;i<n;i++)
	{
		int val; cin>>val;
		msg.push_back(val);
	}
	int final_msg[n+r+1],p=0,j=0;
	for(int i=1;i<=n+r;i++)
	{
		if((i) == pow(2,p)){final_msg[i]=0; p++;}
		else
		{
			final_msg[i]=msg[j++];
		}
	}
    
    int cnt=0;
	for(int i=0;i<r;i++)
	{
		int pos = pow(2,i);
		cnt=0;
		for(int j=1;j<=(n+r);j++)
		{
			int num = j>>i;
			if(num&1)
			{
				if(final_msg[j]){cnt++;}
			}
		}
		if(cnt%2)final_msg[pos]=1;
	}
	cout<<"Message that will be sent by sender:";
	for(int i=1;i<=n+r;i++)cout<<final_msg[i]<<" ";
    cout<<"\n";
    
    int er_pos,real_pos=0; cin>>er_pos;
    final_msg[er_pos]=!final_msg[er_pos];

    cout<<"Message received: ";
    for(int i=1;i<=n+r;i++)cout<<final_msg[i]<<" ";
    cout<<"\n";

    for(int i=0;i<r;i++)
    {
    	int pos = pow(2,i);
    	cnt=0;
    	for(int j=1;j<=n+r;j++)
    	{
    		int num = j>>i;
			if(num&1)
			{
				if(final_msg[j] && j!=pos){cnt++;}
			}
    	}
    	if(cnt%2)
    	{
    		if(final_msg[pos]==0){real_pos+=pos;}
    	}
    	else
    	{
    		if(final_msg[pos]==1){real_pos+=pos;}
    	}
    }
    cout<<"Error at position: "<<real_pos<<endl;
}