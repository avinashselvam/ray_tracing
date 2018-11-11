import numpy as np

objfile = open("./models/cow.obj", "r")

contents = objfile.read()
contents = contents.split("\n")

v, vt, f = [], [], []

for each in contents:
    each = each.split(" ")
    if each[0] == "v": v.append(each[1:])
    elif each[0] == "vt": vt.append(each[1:])
    elif each[0] == "f": f.append(each[1:4])


for i in range(len(f)):
    f[i] = [int(j.split("/")[0]) for j in f[i]]


facefile = open("cow.txt", "w+")

print(len(f), max(f))

for face in f:
   v1, v2, v3 = v[face[0]-1], v[face[1]-1], v[face[2]-1]
   line = "{} {} {} {} {} {} {} {} {}\n".format(v1[0], v1[1], v1[2], v2[0], v2[1], v2[2], v3[0], v3[1], v3[2])
   facefile.write(line)

objfile.close()
facefile.close()
