
from pgmpy.models import BayesianModel
import xml.etree.ElementTree as ET

import inspect
from xml.dom import minidom


from pgmpy.factors import TabularCPD
import numpy as np

if __name__ == "__main__":

    doc = minidom.parse("/home/angel/Downloads/bayes_network.xml")

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
            G.add_edge('x_'+str(count), 'x_'+str(e))
        count = count + 1

    count = 0
    for m in factList:
        if not adjList[count]:
            cpd = TabularCPD('x_'+str(count), 2, m)
        else:
            n = len(adjList[count])
            car = [2]*n
            evi = [str(i) for i in adjList[count]]
            cpd = TabularCPD('x_'+str(count), 2, m, evidence=evi, evidence_card=car)

        count = count + 1

    

