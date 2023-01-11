class Graph
{
	constructor(v)
	{
		this.v = v;
		this.adj = new Array(v);
		for (let i = 0; i < v; i++) {
			this.adj[i] = [];
		}
	}
	addEdge(v, w)
	{
		//add eleemnt to the graph
		this.adj[v].push(w);
	}
	BFS(s)
	{
		let visited = new Array(this.v)
		for (let i = 0; i < this.v; i++) {
			visited[i] = false;			
		}
		// Create a queue for BFS
		let queue = [];

		//Mark all node as visited
		// add them to the queue
		visited[s] = true;
		queue.push(s);

		while (queue.length > 0) {
			//remove first element and print it
			s = queue[0];
			console.log(s + " ");
			queue.shift();

			// get all neighbour of s and if they aren't visited ,mark them as visited
            // and add them to the queue
			this.adj[s].forEach((elem,i) => {
				if(!visited[elem])
				{
					visited[elem] = true;
					queue.push(elem)
				}
			});
		}

	}
}

g = new Graph(4);
g.addEdge(0, 1);
g.addEdge(0, 2);
g.addEdge(1, 2);
g.addEdge(2, 0);
g.addEdge(2, 3);
g.addEdge(3, 3);

g.BFS(2);