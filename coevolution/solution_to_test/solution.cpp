#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cstring>
#include <cmath>
#include <queue>
#include <unordered_set>
#include <set>
#include <fstream>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
const int64_t maxn = 250'000 + 7;

using namespace __gnu_pbds;

typedef long long int ll;

typedef tree<
ll,
null_type,
std::less<ll>,
rb_tree_tag,
tree_order_statistics_node_update>
ordered_set;

typedef tree<ll, null_type, std::less_equal<ll>, rb_tree_tag, tree_order_statistics_node_update> indexed_multiset;

#define CONSOLE



int main(){
	std::ifstream in;
	std::ofstream out;
#ifndef CONSOLE
	in.open("input.txt");
	out.open("output.txt");
#endif
	
	ll N, M;
	ll res = 0;
	bool flag = false;
	indexed_multiset X1;
	indexed_multiset X2;

#ifdef CONSOLE
	#define STREAM_OUT std::cout
	#define STREAM_IN std::cin
#else
	#define STREAM_OUT out
	#define STREAM_IN in
#endif
	STREAM_IN >> N >> M;
	
    for(ll i = 0; i < N; i++)
	{
		ll num;
		STREAM_IN >> num;
		X1.insert(num);
	}
	for(ll i = 0; i < M; i++)
	{
		ll num;
		STREAM_IN >> num;
		X2.insert(num);
	}
	while(!X1.empty() && !X2.empty()){
		ll first = *X1.begin();
		ll second = *X2.begin();
		X1.erase(X1.begin());
		X2.erase(X2.begin());
		if(first == second)
			flag ^= true;
		if(first > second || flag){
			first += second;
			X1.insert(first);
			res += X1.order_of_key(first + 1);
			
			continue;
		}
		if(first < second || !flag){
			second += first;
			X2.insert(second);
			res += X2.order_of_key(second + 1);
			
			continue;
		}
	}
	STREAM_OUT << res << std::endl;
	return 0;
}