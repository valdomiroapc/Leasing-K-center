#include <bits/stdc++.h>
using namespace std;
int V;
int T;
int L;
int K;
vector< vector<int> > d_i_j;
vector< int> facility_duration;
vector< vector<int> > client;
void read_data_from_file(){
    fstream file("/home/valdomiro/Leasing K center/Instâncias-LKC/inst20.txt");
    int x;
    file>>V>>T>>L>>K;
    d_i_j.resize(V);
    for(int i=0;i<V;i++){
        for(int j=0;j<V;j++){
            int x;file>>x;
            d_i_j[i].push_back(x);
        }
    }
    for(int i=0;i<L;i++){
        int x; file>>x;
        facility_duration.push_back(x);
    }
    client.resize(T);
    for(int i=0;i<T;i++){
        int dt; file>>dt;
        for(int j=0;j<dt;j++){
            int x;file>>x;
            client[i].push_back(x);
        }
    }
}

struct segtree{
    vector<long long int> tree;
    vector<long long int> lazy;
    segtree(int n){
        tree.assign(4*n,0);
        lazy.assign(4*n,0);
    }
    void build(int no,int l,int r,vector<int> &v){
        if(l==r){
            tree[no] = v[l];
            return;
        }
        int m = (l+r)/2;
        build(no*2,l,m,v);build(no*2+1,m+1,r,v);
        tree[no] = tree[no*2]+tree[no*2+1];
    }
    long long int query(int no,int l,int r,int p,int q){
        if(lazy[no]){
            tree[no]+=lazy[no]*(r-l+1);
            tree[no] = max(0LL,tree[no]);
            if(l<r){
                lazy[no*2]+=lazy[no];
                lazy[no*2+1]+=lazy[no];
            }
            lazy[no] = 0;
        }
        if(l>q or r<p)
            return 0;
        if(p<=l and r <= q)
            return tree[no];
        int m = (l+r)/2;
        long long int left = query(no*2,l,m,p,q);
        long long int right = query(no*2+1,m+1,r,p,q);
        tree[no] = tree[no*2] + tree[no*2+1];
        return left+right;
    }
    void update(int no,int l,int r,int p,int q,int val){
        if(lazy[no]){
            tree[no] += lazy[no]*(r-l+1);
            tree[no] = max(0LL,tree[no]);
            if(l<r){
                lazy[no*2]+=lazy[no];
                lazy[no*2+1]+=lazy[no];
            }
            lazy[no]=0;
        }
        if(l>q or r<p)
            return;
        if(p<=l and r<=q){
            lazy[no]+=val;
            tree[no]+=lazy[no]*(r-l+1);
            tree[no] = max(0LL,tree[no]);
            if(l<r){
                lazy[no*2]+=lazy[no];
                lazy[no*2+1]+=lazy[no];
            }
            lazy[no] = 0;
            return;
        }
        int m = (l+r)/2;
        update(no*2+1,m+1,r,p,q,val);
        update(no*2,l,m,p,q,val);
        tree[no] = tree[no*2] + tree[no*2+1];
    }
};

struct node{
    int v,t,l;
    node(int vv,int tt,int ll){
        v = vv;
        t = tt;
        l = ll;
    }
    node(){}
};
vector<int> gon_algorithm(int t){
    // cover time scope with random servers.
    vector<int> server;
    server.push_back(rand()%V);
    int q = 1;
    while(q<K){
        int next_server=-1;
        int distance=0;
        for(int c=0;c<V;c++){
            int minimal_distance = 100000000;
            bool is_in_the_set = false;
            for(auto x:server){
                if(c==x){
                    is_in_the_set = true;
                    break;
                }
                minimal_distance = min(minimal_distance,d_i_j[c][x]);
            }
            if(minimal_distance > distance and not is_in_the_set){
                next_server = c;
                distance = minimal_distance;
            }
        }
        q++;
        server.push_back(next_server);
    }
    return server;
}
int best_time_cover(int t,vector< int> &dp, vector<segtree> &server_along_time, vector< pair<int,int> > &recover, vector< vector<int> > &rep){
    if(t>=T){
        return 0;
    }
    int &ans = dp[t];
    if(ans!=-1)
        return ans;
    ans = -10000000;
    for(int l=0;l<L;l++){
        int end_time = min(T,t + facility_duration[l]);
        for(int v=0;v<V;v++){
            if(rep[t][v])
                continue;
            int quantity = server_along_time[v].query(1,0,T-1,t,end_time-1);
            int future_value = best_time_cover(end_time,dp,server_along_time,recover,rep)+quantity;
            //cout<<"t: "<<t<<" v: "<<v<<" l: "<<l<<" future: "<<future_value<<endl;
            if(future_value > ans){
                recover[t] = {l,v};
                ans = future_value;
            }
        }
    }
    return ans;
}
vector< node> generate_initial_solution(){
    vector< vector<int> > individual_solution;
    for(int t=0;t<T;t++){
        individual_solution.push_back(gon_algorithm(t));
    }
    vector< segtree > server_along_time;
    for(int i=0;i<V;i++){
        server_along_time.push_back( segtree(T) );
    }
    for(int t=0;t<T;t++){
        for(auto x:individual_solution[t]){
            server_along_time[x].update(1,0,T-1,t,t,1);
        }
    }
    vector< pair<int,int> > recover(T);
    vector<node> initial_sol;
    vector< vector<int> > rep(T, vector<int> (V,0));
    for(int k=0;k<K;k++){
        vector<int> dp(T,-1);
        best_time_cover(0,dp,server_along_time,recover,rep);
        int current = 0;
        while(current<T){
            int l_type = recover[current].first;
            int v_open = recover[current].second;
            initial_sol.push_back( node(v_open,current,l_type));
            int next = min(T,current + facility_duration[l_type]);
            server_along_time[v_open].update(1,0,T-1,current,next-1,-1);
            for(int t=current;t<next;t++){
                rep[t][v_open]=1;
            }
            current = min(T,current + facility_duration[l_type]);
        }
    }
    return initial_sol;
}
int objective_funtion( vector< node > &C){
    vector< vector <int> > server_on_time(T);
    for(int i=0;i<C.size();i++){
        int start = C[i].t;
        int end = min(T,start+facility_duration[C[i].l]);
        int v = C[i].v;
        for(int t=start;t<end;t++){
            server_on_time[t].push_back(v);
        }
    }
    int maximal = 0;
    for(int t=0;t<T;t++){
        for(auto cl:client[t]){
            int minimal = 100000000;
            for(auto sv:server_on_time[t]){
                minimal = min(minimal,d_i_j[cl][sv]);
            }
            maximal = max(maximal,minimal);
        }
    }
    return maximal;
}
vector<node>* neighborhood(int r,vector<node> &solution){
    random_shuffle(solution.begin(),solution.end());
    vector<node> altered_solution;
    r = min(r,(int)solution.size());
    for(int i=0;i<r;i++){
        int l = solution[i].l;
        int new_l = l + rand()%L;
        altered_solution.push_back(node(solution[i].v,solution[i].t,new_l));
    }
    for(int i=r;i<solution.size();i++){
        altered_solution.push_back(solution[i]);
    }
    
}
vector<node> VNS(int R){
    vector<node> initial_solution = generate_initial_solution();
    for(int r=0;r<R;r++){

    }
}
int main(){
    srand(time(NULL));
    read_data_from_file();
    vector< node > initial_sol = generate_initial_solution();
    cout<<"initial solution:"<<endl;
    for(int i=0;i<initial_sol.size();i++){
        cout<<"server: "<<initial_sol[i].v<<" start:"<<initial_sol[i].t<<" type:"<<initial_sol[i].l<<endl;
    }
    cout<<objective_funtion(initial_sol)<<endl;
}