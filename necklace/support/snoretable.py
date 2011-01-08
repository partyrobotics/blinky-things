#!/usr/bin/env python

steps = 256

t0 = 0.0
t1 = 1.0
amax = 99.0
aoffset = 1

def func(x):
    return 2*x*x*x - 3*x*x + 1

timeslice = (t1 - t0) / (steps - 2)
v0 = func(t0)
scaler = amax / func(t0)

table = []
for i in xrange(0, int(steps-1)):
    v = (func((timeslice * float(i)) + t0) * scaler) + aoffset
    #print "%f: %f" % (timeslice * float(i) + t0, v)
    table.append("%d" % int(v))

print "#define SNORE_STEPS %d" % (steps - 1)
print "unsigned char snore_table[] PROGMEM = {",
print ",".join(table),
table.reverse()
print ",",
print ",".join(table),
print "};"
