#include<bits/stdc++.h>
#define int long long
using namespace std;
const int N=1e5+5;
int low[N], dfn[N], cnt, sum, scc[N], n, m;
bool viss[N];
stack<int> stk;
vector<int> nbr[N], ans[N];
void tarjan(int cur)//找 SCC
{
	stk.push(cur);
	viss[cur]=1;
	low[cur]=dfn[cur]=++cnt;
	for(int nxt:nbr[cur])
	{
		if(!dfn[nxt])
		{
			tarjan(nxt);
			low[cur]=min(low[cur],low[nxt]);
		}
		else if(viss[nxt])
		{
			low[cur]=min(low[cur],dfn[nxt]);
		}
	}
	if(dfn[cur]==low[cur])
	{
		sum++;
		while(stk.top()!=cur)
		{
			int tmp=stk.top();
			stk.pop();
			scc[tmp]=sum;
			ans[sum].push_back(tmp);
			viss[tmp]=0;
		}
		stk.pop();
		ans[sum].push_back(cur);
		scc[cur]=sum;
		viss[cur]=0;
	}
}
signed main()
{
	cin>>n>>m;
	for(int i=1;i<=m;i++)
	{
		int u, v;
		cin>>u>>v;
		nbr[u].push_back(v);
	}
	for(int i=1;i<=n;i++)
	{
		if(!dfn[i])
		{
			tarjan(i);
		}
	}
	cout<<sum<<"\n";
	for(int i=1;i<=n;i++)
	{
		if(dfn[i]==0)
		{
			continue;
		}
		sort(ans[scc[i]].begin(), ans[scc[i]].end());//vector 从小到大排序
		for(int cur:ans[scc[i]])
		{
			cout<<cur<<" ";
			dfn[cur]=0;
		}
		cout<<"\n";
	}
}