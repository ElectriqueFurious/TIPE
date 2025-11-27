from PIL import Image
import glob
import numpy as np
import shutil
import os
from more_itertools import divide

root_dir = "out/"

def color(n):
    match n:
        case 1:
            return [0,0,255] 
        case 2:
            return [255,0,0]
        case 3:
            return [0,255,0]
        case 4:
            return [0,0,100]
        case 5:
            return [255,0,255]
        case _:
            return [255,255,255]

if os.path.exists(root_dir+"jpg"):
    shutil.rmtree(root_dir+"jpg")
os.mkdir(root_dir+"jpg")
file = open(root_dir+"test.txt","r")

width = int(file.readline())
height = int (file.readline())

img = None
array = None

mid = np.zeros([width,height,3],dtype=np.uint64)
b = np.zeros([width,height])

NUM_THREADS = 100

line = file.read()
line = line.replace("C\nO\n", "#").replace("O\n", "#").replace("C\n", "#").replace("\n", ",")
line = line.split("#")[:-2]
image_count = len(line)
line = [list(c) for c in divide(NUM_THREADS, line)]

import threading 

def task(img_arr, start_index):
    image_counter = start_index
    for img in img_arr:
        array = np.zeros([width,height,3],dtype=np.uint8)
        array[:,:] = [255,255,255]
        b[:,:] = True
        
        img = img.split(",")[:-2]
        for values in img :
            values = values.split(";")
            # print(values)
            v1,v2,v3=int(values[0]),int(values[1]),int(values[2])
            array[v1,v2] = color(v3)
            if b[v1,v2]:
                mid[v1,v2,2] += 1
                b[v1,v2] = False

        img = Image.fromarray(array)
        img.save(root_dir+"jpg/"+str(image_counter)+".bmp","bmp")
        image_counter += 1
        # print(str(image_counter)+"\n")

d = 0
t = []
for i in range(NUM_THREADS):
    thread = threading.Thread(target=task, args=(line[i],d, ))
    d += len(line[i])
    thread.start()
    t.append(thread)
    
for i in range(NUM_THREADS):
    t[i].join()

file.close()

print("Threads finished")

new_mid = np.zeros([width,height,3],dtype=np.uint8)

for i in range(width):
    for j in range(height):
        new_mid[i,j,2] = int(mid[i,j,2]*(255/image_count))

img = Image.fromarray(new_mid)
img.save(root_dir+"mid.bmp")

frames = [Image.open(image) for image in sorted(glob.glob(root_dir+"jpg/*.bmp"), key=lambda x: int(os.path.basename(os.path.splitext(x)[0])))]
frame_one = frames[0]
frame_one.save(root_dir+"test.gif", format="GIF", append_images=frames,save_all=True, duration=80, loop=1)

shutil.rmtree(root_dir+"jpg")