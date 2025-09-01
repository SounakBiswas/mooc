void graph::shortest_path_dijkstra(int src, double dist[] ){
    double newdist;
    //closed array keeps track of vertices that have already been removed form the open set
    bool *closed = new bool[n_verts];
    int current_node;
    for(int vert=0; vert<n_verts; vert++){
        dist[vert]=numeric_limits<double>::infinity();
        closed[vert]=false;
    }

    //initialise priority queue with the source node
    priority_queue open_set(n_verts);
    dist[src]=0;
    open_set.push(src,dist[src]);
    while(open_set.get_size()!=0){
        current_node=open_set.pop();
        closed[current_node]=true;
        for(int edge=0; edge<n_edges[current_node]; edge++){
            int nbr=edge_list[current_node][edge];
            newdist=dist[current_node]+cost[current_node][nbr];
            if(not closed[nbr]){
                if(! open_set.contains_element(nbr) ){
                    open_set.push(nbr,newdist);
                    dist[nbr]=newdist;
                    //cout<<"pushed "<<nbr<<endl;
                }
                else{
                    if(newdist<dist[nbr]){
                        dist[nbr]=newdist;
                        open_set.change_priority(nbr,newdist);
                    }
                }
            }
        }
    }
}
