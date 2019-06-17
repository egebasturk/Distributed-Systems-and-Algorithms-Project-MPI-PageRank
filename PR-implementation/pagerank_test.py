import networkx as nx
import collections
import matplotlib.pyplot as plt
import time
from networkx import pagerank_scipy as pagerank_scipy

start = time.time()
G = nx.read_edgelist("Data/web-Google.txt")
#G = nx.read_edgelist("Data/soc-LiveJournal1.txt")
pagerank = pagerank_scipy(G, max_iter=50)
end = time.time()
print("Pagerank time:" + str(end - start))

#print(pagerank)
file = open("test_out.txt", 'w')
pagerank = {int(k) : v for k, v in pagerank.items()}
pagerank = collections.OrderedDict(sorted(pagerank.items()))
for key, value in dict(sorted(pagerank.items())).items():
    file.write(str(key) + " -> " + str(value) + "\n")
file.close()