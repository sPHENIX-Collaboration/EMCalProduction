import numpy as np
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
#from skimage import io
#from sklearn import cluster
#from scipy import signal
#import cv2
import os
import sys


def main(targetpic):
    # resize the target picture to filter out light leaking. denoising too slow and ineffective
    target0 = mpimg.imread(targetpic)

    (height, width, thickness) = target0.shape
    #start = int(round(height * 0.42))
    start = 431
    end = int(round(height * 0.48))
    horizontal = int(round(width * 0.5))

    R = target0[start, horizontal, 0] * 255
    G = target0[start, horizontal, 1] * 255
    B = target0[start, horizontal, 2] * 255
    #print(R)
    #print(G)
    #print(B)
    length = int(round(0.06 * height))
    denied = False
    found = False
    approved = False
    for i in range(length):
        if found:
            break
        for j in range(length):
            if found:
                break
            if R > 229 and R < 249:
                if G > 11 and G < 31:
                    if B > 17 and B < 37:
                       denied = True
                       found = True
            if R < 10:
                if G > 138 and G < 158:
                    if B > 31 and B < 51:
                        approved = True
                        found = True

    if not found:
        print("color spot not found")
    if denied:
        print("Denied")
    if approved:
        print("Approved")
    # os.rename(targetpic[:-13]+"-block.JPG","DBN_{0}-{1}".format(dbn,ed)+'.JPG')
    # exit("Size: {0}-{1}".format(height, width))


if __name__ == "__main__":
    main(sys.argv[1])
