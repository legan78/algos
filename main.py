
from pgmpy.models import BayesianModel
import xml.etree.ElementTree as ET

import inspect
from xml.dom import minidom


from pgmpy.factors import TabularCPD
import numpy as np

from pgmpy.inference import BeliefPropagation
from pgmpy.inference import VariableElimination


if __name__ == "__main__":

    doc = minidom.parse("/home/angel/Documents/RECENT_RESULTS/bayes_network.xml")

    # doc.getElementsByTagName returns NodeList
    ns = doc.getElementsByTagName("network_structure")
    n_nodes = doc.getElementsByTagName("n_nodes")[0]
    n_nodes = int(n_nodes.firstChild.data)

    adjList = []

    for i in range(n_nodes):
        var = 'x_'+str(i)
        x = doc.getElementsByTagName(var)[0]
        parents = []
        if hasattr(x.firstChild, 'data'):
            par = x.firstChild.data
            par = par.split()
            for p in par:
                parents.append(int(p))
#            print(parents)
        adjList.append(parents)
            

    nf = doc.getElementsByTagName('network_factors')

    factList = []

    for i in range(n_nodes):
        var = 'x_'+str(i)
        x = doc.getElementsByTagName(var)[1]

        rows = x.getAttribute("rows")
        cols = x.getAttribute("cols")
        
        data = x.firstChild.data.split()
        m = ''

        array = []

        for j in range(len(data)):
            if (j+1)%int(cols) == 0:
                m = m + ' ' + data[j]+';'
            else:
                m = m + ' ' + data[j]

        a = np.matrix(m[:-1])
        a = a.transpose()

        for j in range(int(cols)):
            r = []

            for k in range(int(rows)):
                r.append(a[j,k])

            array.append(r)

        factList.append(array)

    G = BayesianModel();
    count = 0

    for p in adjList:
        for e in p:
            print('x_'+str(e),'->', 'x_'+str(count))
            G.add_edge('x_'+str(e), 'x_'+str(count))
        count = count + 1

    count = 0
    for m in factList:
        if 'x_'+str(count) in G:
            if not adjList[count]:
                print('x_'+str(count))
                cpd = TabularCPD('x_'+str(count), 2, m)
                cpd.normalize()
                G.add_cpds(cpd)
            else:
                n = len(adjList[count])
                car = [2]*n
                evi = ['x_'+str(i) for i in adjList[count]]
                print(car, evi)
                cpd = TabularCPD('x_'+str(count), 2, m, evidence=evi, evidence_card=car)
                print('x_'+str(count))
                cpd.normalize()
                G.add_cpds(cpd)
        count = count + 1

    inference = VariableElimination(G)

    print(inference.max_marginal(['x_0']))


