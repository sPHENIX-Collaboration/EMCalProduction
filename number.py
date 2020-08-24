# Author: Xiaoning Wang
# Number Recognition code based on error map

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
from skimage import io
from sklearn import cluster
from scipy import signal
import cv2
import os
import sys


def main(targetpic, templates):
    clst = 5
    # resize the target picture to filter out light leaking. denoising too slow and ineffective
    target0 = np.array(io.imread(targetpic, 0))
    target1 = cv2.resize(target0, (int(len(target0[0])/10), int(len(target0)/10)))
    evals, target=cv2.threshold(target1, 100, 255, cv2.THRESH_BINARY)

    template={}
    numbers={}
    coords1={}
    coords={}
    # initialize the templates
    for j in range(10):
        test=np.array(io.imread("{0}/{1}.JPG".format(templates, j)))
        test2=cv2.resize(test, (int(len(test[0])/10), int(len(test)/10)))
        evals, template[str(j)]=cv2.threshold(
            test2, 100, 255, cv2.THRESH_BINARY)
        coords1[str(j)]=np.where(template[str(j)] == 255)
        itera=len(coords1[str(j)][0])
        coords[str(j)]=np.zeros((itera, 2))
        for m in range(itera):
            coords[str(j)][m, 0]=coords1[str(j)][0][m]
            coords[str(j)][m, 1]=coords1[str(j)][1][m]
        # cut off the edges on template pictures so it only contains the region of numbers
        template[str(j)]=template[str(j)][int(np.amin(coords[str(j)][:, 0])):int(np.amax(
            coords[str(j)][:, 0])), int(np.amin(coords[str(j)][:, 1])):int(np.amax(coords[str(j)][:, 1]))]

    # find the target pictures region of numbers. Firstly find all light pixels
    targetcoords1=np.where(target == 255)
    targetcoords=np.zeros((len(targetcoords1[0]), 2))
    for k in range(len(targetcoords1[0])):
        targetcoords[k, 0]=targetcoords1[0][k]
        targetcoords[k, 1]=targetcoords1[1][k]

    # find the clusters
    nclusters=cluster.KMeans(n_clusters=clst, random_state=0).fit(targetcoords)
    labels=nclusters.labels_
    centroids = np.array(nclusters.cluster_centers_)
    num={}
    ordering = np.argsort(centroids[:,1])


    for a in range(1, clst+1):
        numbers[str(a)]=0
        num[str(a)]=np.zeros((len(np.where(labels == ordering[a-1])[0]), 2))
        # group the elements of same cluster together
        for n1 in range(len(np.where(labels == ordering[a-1])[0])):
            num[str(a)][n1, 0]=targetcoords[np.where(
                labels == ordering[a-1])[0][n1], 0]
            num[str(a)][n1, 1]=targetcoords[np.where(
                labels == ordering[a-1])[0][n1], 1]
        # find boundary of each cluster
        xstart=int(np.amin(num[str(a)][:, 0]))
        xend=int(np.amax(num[str(a)][:, 0]))
        ystart=int(np.amin(num[str(a)][:, 1]))
        yend=int(np.amax(num[str(a)][:, 1]))
        targetn=target[xstart:xend, ystart:yend]
        im = Image.fromarray(targetn.astype('uint8'))
        im.save(str(a)+"th.jpg")

        # use response_t to store the response of each template
        response_t={'0': 0, '1': 0, '2': 0, '3': 0, '4': 0, 
                    '5': 0, '6': 0, '7': 0, '8': 0, '9': 0}
        for b in range(10):
            # resize the template so that they're the same size as target cluster
            responset=cv2.resize(
                template[str(b)], (yend - ystart, xend - xstart))
            evals, responset=cv2.threshold(
                responset, 100, 255, cv2.THRESH_BINARY)
            response_t[str(b)]=np.sum(np.equal(targetn, responset))
            # find maximum response
            if b!=0 and response_t[str(b)] > response_t[str(numbers[str(a)])]:
                    numbers[str(a)]=b

    #print("1: ")
    #print(numbers['1'])
    #print("2: ")
    #print(numbers['2'])
    #print("3: ")
    #print(numbers['3'])
    if numbers['1']==0:
        ed = "W"
    if numbers['1']==1:
        ed = "N"

    dbn = ''
    for p in range(2,clst+1):
        dbn = dbn + str(numbers[str(p)])
    #os.rename(targetpic[:-13]+"-block.JPG","DBN_{0}-{1}".format(dbn,ed)+'.JPG')
    exit("DBN_{0}-{1}".format(dbn,ed)+".JPG")

if __name__ == "__main__":
    main(sys.argv[1],sys.argv[2])
