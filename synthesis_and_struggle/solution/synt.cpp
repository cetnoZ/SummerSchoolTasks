#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cstring>
#include <cmath>
#include <queue>
 
const int64_t maxn = 1e3 + 7;
const int64_t maxp = 1e5 + 7;

int64_t N, K;
int64_t d[maxn];
int64_t dis[maxn][maxn];
int64_t f[maxn][maxn];
int64_t p[maxn];
int64_t ans[maxn];

int64_t head[maxn], next[maxp], to[maxp];
int64_t top;
 
void to__link(int64_t u, int64_t v)
{
	next[++top] = head[u];   
    head[u] = top;   
    to[top] = v; 
}

void dfs(int64_t node, int64_t far, int64_t diss, int64_t remote)
{
	dis[remote][node] = diss;
	for (int64_t i = head[node], u; u = to[i], i; i = next[i])
		if (u != far)    
            dfs(u, node, diss + 1, remote);
}

void dfs(int64_t node, int64_t far)
{
	for (int64_t i = head[node], u; u = to[i], i; i = next[i])
		if (u != far)  
          dfs(u, node);
	for (int64_t i = 1; i <= N; i++)    
        f[node][i] = K + d[dis[node][i]];
	for (int i = head[node], u; u = to[i], i; i = next[i])
		if (u != far)
			{
				for (int j = 1; j <= N; j++)
					f[node][j] = f[node][j] + std::min(f[u][j] - K,f[u][p[u]]);
			}
	p[node] = 1;
	for (int64_t i = 2; i <= N; i++)
		if (f[node][i] < f[node][p[node]])    
            p[node] = i;
}

void dfs(int64_t node, int64_t far, int64_t w)
{
	ans[node] = w;
	for (int64_t i = head[node], u; u = to[i], i; i = next[i])
		if (u != far)
			{
				if (f[u][w] - K < f[u][p[u]])
                    dfs(u, node, w);
				else 
                    dfs(u, node, p[u]);
			}
}
//TODO: write full solution
int main()
{
	std::cin >> N >> K;   
    
	for (int64_t i = 1; i < N; i++)
        std::cin >> d[i];

	for (int64_t i = 1; i < N; i++)
    {
        int64_t u, v;
        std::cin >> u >> v;
        to__link(u, v);
        to__link(v, u);
    }
	

	for (int64_t i = 1; i <= N; i++)
		dfs(i,-1, 0, i);
	dfs(1, 0);
	std::cout << f[1][p[1]] << std::endl;

    return 0;
}