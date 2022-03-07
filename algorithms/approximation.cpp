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
    fstream file("/home/valdomiro/Leasing K center/Instâncias-LKC/inst1.txt");
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
    long long int query_maximum(int no,int l,int r,int p,int q){
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
        return max(left,right);
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

struct segtree_max{
    vector<long long int> tree;
    vector<long long int> lazy;
    segtree_max(int n){
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
        tree[no] = max(tree[no*2],tree[no*2+1]);
    }
    long long int query(int no,int l,int r,int p,int q){
        if(lazy[no]){
            tree[no]+=lazy[no];
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
        tree[no] = max(tree[no*2],tree[no*2+1]);
        return max(left,right);
    }
    void update(int no,int l,int r,int p,int q,int val){
        if(lazy[no]){
            tree[no] += lazy[no];
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
            tree[no]+=lazy[no];
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
        tree[no] = max(tree[no*2],tree[no*2+1]);
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
vector<node> neighborhood(int r,vector<node> &solution){
    random_shuffle(solution.begin(),solution.end());
    vector<node> altered_solution;
    r = min(r,(int)solution.size());
    for(int i=0;i<r;i++){
        int l = solution[i].l;
        int new_l = (l + (rand()%L))%L;
        altered_solution.push_back(node(solution[i].v,solution[i].t,new_l));
    }
    for(int i=r;i<solution.size();i++){
        altered_solution.push_back(solution[i]);
    }

    vector< vector<node> > solution_time_based(T);
    for(int i=0;i<altered_solution.size();i++){
        int instant = altered_solution[i].t;
        solution_time_based[instant].push_back( altered_solution[i]);
    }
    segtree_max time_spec(T);
    vector<node> valid_altered_solution;
    for(int t=0;t<T;t++){
        for(int i=0;i<solution_time_based[t].size();i++){
            int l = solution_time_based[t][i].l;
            int duration = facility_duration[l];
            int ts = t; 
            int tf = min(T-1,t+duration-1);
            time_spec.update(1,0,T-1,ts,tf,1);
            while(ts<T and time_spec.query(1,0,T-1,0,T-1) > K){
                time_spec.update(1,0,T-1,ts,tf,-1);
                ts++;
                tf=min(tf+1,T-1);
                if(ts<T)
                    time_spec.update(1,0,T-1,ts,tf,1);
            }
            if(ts<T){
                solution_time_based[t][i].t=ts;
                valid_altered_solution.push_back( solution_time_based[t][i]);
            } 
        }
    }
    int facilities_open_count[T];
    memset(facilities_open_count,0,sizeof(facilities_open_count));
    for(int t=0;t<T;t++){
        facilities_open_count[t] = time_spec.query(1,0,T-1,t,t);
    }
    for(int t=0;t<T;t++){
        int facilities_open = time_spec.query(1,0,T-1,t,t);
        while(facilities_open < K){
            int idx = -1;
            int diff = 10000000;
            for(int i=0;i<valid_altered_solution.size();i++){
                int instant = valid_altered_solution[i].t;
                if(instant > t and t-instant < diff){
                    idx = i;
                    diff = t-instant;
                }
            }
            if(idx != -1){
                int ts = valid_altered_solution[idx].t;
                int l = valid_altered_solution[idx].l;
                int tf= min(T-1,ts+facility_duration[l]-1);
                time_spec.update(1,0,T-1,ts,tf,-1);
                ts = t;
                tf= min(T-1,ts+facility_duration[l]-1);
                time_spec.update(1,0,T-1,ts,tf,1);
                valid_altered_solution[idx].t = ts;
            }
            else{
                break;
            }
            facilities_open = time_spec.query(1,0,T-1,t,t);
        }
    }
    
    memset(facilities_open_count,0,sizeof(facilities_open_count));
    for(int t=0;t<T;t++){
        facilities_open_count[t] = time_spec.query(1,0,T-1,t,t);
    }
    for(int t=0;t<T;t++){
        int facilities_open = time_spec.query(1,0,T-1,t,t);
        while(facilities_open < K){
            int l = rand()%L;
            set<int> used_locations;
            int ts = t;
            int tf = min(T-1,ts+facility_duration[l]-1);
            for(int i=0;i<valid_altered_solution.size();i++){
                int vts = valid_altered_solution[i].t;
                int vl = valid_altered_solution[i].l;
                int vtf = min(T-1,vts+facility_duration[vl]-1);
                int v = valid_altered_solution[i].v;
                if((ts<=vts and vts<=tf) or (ts<=vtf and vtf<=tf)){
                    used_locations.insert(v);
                }
            }
            vector<int> valid_location;
            for(int v=0;v<V;v++){
                if(used_locations.find(v)==used_locations.end()){
                    valid_location.push_back(v);
                }
            }
            if(valid_location.empty())
                break;
            int idx = rand()%valid_location.size();
            int v = valid_location[idx];
            valid_altered_solution.push_back( node(v,t,l));
            time_spec.update(1,0,T-1,ts,tf,1);
            facilities_open = time_spec.query(1,0,T-1,t,t);
        }
    }
    vector<node> *ref_to_solution = &valid_altered_solution;
    return valid_altered_solution;
}
vector<node> neighborhood1(int r,vector<node> &solution){
    vector< pair<int,int> > index_tuples(solution.size());
    for(int i=0;i<solution.size();i++){
        index_tuples[i] = {0,i};
    }
    for(int t=0;t<T;t++){
        vector<pair<int,int> > clients_d(V,{-1,1000000});
        for(auto cl: client[t]){
            for(int i=0;i<index_tuples.size();i++){
                int idx = index_tuples[i].second;
                int l = solution[idx].l;
                int v = solution[idx].v;
                int ts = solution[idx].t;
                int tf = min(T-1,ts+facility_duration[l]-1);
                if(ts<=t and t<=tf){
                    if(d_i_j[cl][v]<clients_d[cl].second){
                        clients_d[cl].second = d_i_j[cl][v];
                        clients_d[cl].first = i;
                    }
                }
            }
            index_tuples[clients_d[cl].first].first++;
        }
    }
    sort(index_tuples.begin(),index_tuples.end());
    reverse(index_tuples.begin(),index_tuples.end());
    vector<node> altered_solution;
    r = max((int)solution.size()-r,1);
    for(int i=0;i<r;i++){
        altered_solution.push_back(solution[index_tuples[i].second]);
    }
    //segtree_max time_spec(T);
    vector<int> time_spec(T,0);
    vector< set<int> > facilities_open(T);
    for(int i=0;i<altered_solution.size();i++){
        int l = altered_solution[i].l;
        int ts = altered_solution[i].t;
        int tf = min(T-1,ts+facility_duration[l]-1);
        int v = altered_solution[i].v;
        for(int t=ts;t<=tf;t++){
            time_spec[t]++;
            facilities_open[t].insert(v);
        }
    }
    bool filled = false;
    while(!filled){
        int t=0;
        int l=rand()%L;
        int tt=t;
        filled = true;
        while(tt<T){
            int ts = tt;
            int tf = min(T-1,ts+facility_duration[l]-1);
            int sz = 0;
            for(int u=ts;u<=tf;u++){
                sz = max(sz,time_spec[u]);
            }
            if(sz+1<=K){
                vector<int> options_facilities;
                for(int v=0;v<V;v++){
                    bool has = false;
                    for(int i=ts;i<=tf;i++){
                        if(facilities_open[i].find(v)!=facilities_open[i].end()){
                            has=true;
                            break;
                        }
                    }
                    if(!has){
                        options_facilities.push_back(v);
                    }
                }
                if(options_facilities.size()==0){
                    tt++;
                    continue;
                }
                int op = rand()%options_facilities.size();
                int v = options_facilities[op];
                for(int u=ts;u<=tf;u++){
                    time_spec[u]++;
                    facilities_open[u].insert(v);
                }
                altered_solution.push_back(node(v,tt,l));
                filled=false;
                break;
            }
            tt++;
        }
    }
    return altered_solution;
}
int local_search(vector<node> &solution){
    bool alteration = true;
    int return_value = 0;
    while(alteration){
        alteration = false;
        vector< vector <int> > server_on_time(T);
        for(int i=0;i<solution.size();i++){
            int start = solution[i].t;
            int end = min(T-1,start+facility_duration[solution[i].l]-1);
            int v = solution[i].v;
            for(int t=start;t<=end;t++){
                server_on_time[t].push_back(v);
            }
        }
        int maximal = 0;
        int distance_at_instant[T];
        for(int t=0;t<T;t++){
            int instant_maximal = 0;
            for(auto cl:client[t]){
                int minimal = 100000000;
                for(auto sv:server_on_time[t]){
                    minimal = min(minimal, d_i_j[cl][sv]);
                }
                instant_maximal = max(instant_maximal,minimal);
                maximal = max(maximal,minimal);
            }
            distance_at_instant[t] = instant_maximal;
        }
        vector<int> scope;
        set<int> conj;
        for(int t=0;t<T;t++){
            if(distance_at_instant[t]==maximal){
                for(int i=0;i<solution.size();i++){
                    int ts = solution[i].t;
                    int l = solution[i].l;
                    int tf = min(T-1,ts+facility_duration[l]-1);
                    if(ts<=t and t<=tf){
                        conj.insert(i);
                    }
                }
            }
        }
        for(auto x:conj){
            scope.push_back(x);
        }
        bool improved = false;
        for(auto idx:scope){
            int ts = solution[idx].t;
            int l = solution[idx].l;
            int tf = min(T-1,ts+facility_duration[l]-1);
            int location = solution[idx].v;
            int upper= 0;
            for(int t=ts;t<=tf;t++){
                upper = max(upper,distance_at_instant[t]);
            }
            set<int> invalid_locations;
            for(int i=0;i<solution.size();i++){
                int vts = solution[i].t;
                int vl = solution[i].l;
                int vtf = min(T-1,vts+facility_duration[vl]-1);
                int vv =solution[i].v;
                if((ts<=vts and vts<=tf) or (ts<=vtf and vtf<=tf)){
                    invalid_locations.insert(vv);
                }
            }
            int range_distance[T];
            for(int v=0;v<V;v++){
                if(invalid_locations.find(v)!=invalid_locations.end())
                    continue;
                for(int t=ts;t<=tf;t++){
                    int range_maximal = 0;
                    for(auto cl:client[t]){
                        int range_minimal = 100000000;
                        for(auto sv:server_on_time[t]){
                            int sv1=sv;
                            if(sv1==location)
                                sv1 = v;
                            range_minimal = min(range_minimal, d_i_j[cl][sv1]);
                        }
                        range_maximal = max(range_maximal,range_minimal);
                    }
                    range_distance[t] = range_maximal;
                }
                int new_upper = 0;
                for(int t=ts;t<=tf;t++){
                    new_upper = max(new_upper,range_distance[t]);
                }
                if(new_upper < upper){
                    upper = new_upper;
                    for(int t=ts;t<=tf;t++){
                        for(int i=0;i<server_on_time[t].size();i++){
                            if(server_on_time[t][i] == location)
                                server_on_time[t][i] = v;
                        }
                    }
                    solution[idx].v = v;
                    for(int t=ts;t<=tf;t++){
                        distance_at_instant[t] = range_distance[t];
                    }
                    int obj_value = 0;
                    for(int t=0;t<T;t++){
                        obj_value = max(distance_at_instant[t],obj_value);
                    }
                    alteration = true;
                }
            }
        }
        int obj_value = 0;
        for(int t=0;t<T;t++){
            obj_value = max(obj_value,distance_at_instant[t]);
        }
        return_value = obj_value;
    }
    return return_value;
}
bool is_valid(vector<node> &solution){
    vector< vector<int> > tab(T, vector<int> (V,0));
    bool wrong = false;
    for(int i=0;i<solution.size();i++){
        int ts = solution[i].t;
        int l = solution[i].l;
        int tf = min(T-1,ts+facility_duration[l]-1);
        int v = solution[i].v;
        for(int t=ts;t<=tf;t++){
            if(tab[t][v]){
                wrong = true;
            }
            tab[t][v]=1;
        }
    }
    for(int t=0;t<T;t++){
        int sum = 0;
        for(int v=0;v<V;v++){
            sum+=tab[t][v];
        }
        if(sum > K){
            wrong = true;
        }
        if(sum==0){
            wrong = true;
        }
    }
    return !wrong;
}
vector<node> VNS(){
    time_t start,end;
    time(&start);
    vector<node> initial_solution = generate_initial_solution();
    vector<node> best_solution = initial_solution;
    int current_objective_value = objective_funtion(best_solution);
    time(&end);
    for(int r=1;r<=best_solution.size()+10;r++){
        vector<node> altered_solution;
        altered_solution=neighborhood1(r,best_solution);
        int new_objective_value = local_search(altered_solution);
        if(new_objective_value < current_objective_value){
            best_solution = altered_solution;
            current_objective_value = new_objective_value;
            r=0;
            time(&end);
        }
    }
    cout<<"result: "<<current_objective_value<<" seconds taken:"<<difftime(end,start)<<endl;
    return best_solution;
}
int main(){
    time_t start,end;
    time(&start);
    srand(time(NULL));
    read_data_from_file();
    vector<node> solution = VNS();
    time(&end);
    cout<<"is valid:"<< is_valid(solution)<<endl;
}