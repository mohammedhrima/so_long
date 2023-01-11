# Python3 Program to print BFS traversal
# from a given source vertex. BFS(int s)
# traverses vertices reachable from s.
from collections import defaultdict


class Graph:

    def __init__(self):
        self.graph = defaultdict(list)

    #add eleemnt to the graph
    def addEdge(self, u, v):
        self.graph[u].append(v)

    def BFS(self, s):
        # mark all element as not visited
        visited = [False] * (max(self.graph) + 1)

        #queue for BFS
        queue = []

        #Mark the start point as visited
        #Add it to the queue
        queue.append(s)
        visited[s] = True

        while queue:

            # remove first elemnt from the queue and put it in a variable
            s = queue.pop(0)
            print(s, end=" ")

            # get all neighbour of s and if they aren't visited ,mark them as visited
            # and add them to the queue
            for i in self.graph[s]:
                if visited[i] == False:
                    queue.append(i)
                    visited[i] = True


# Driver code

# Create a graph given in
# the above diagram
g = Graph()
g.addEdge(0, 1)
g.addEdge(0, 2)
g.addEdge(1, 2)
g.addEdge(2, 0)
g.addEdge(2, 3)
g.addEdge(3, 3)

g.BFS(2)

# This code is contributed by Neelam Yadav