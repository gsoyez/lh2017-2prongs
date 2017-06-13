#!/usr/bin/python
#
# given a histogram, find the smallest-width window that contains a
# fraction f of the events

import sys,argparse

# get the command-line arguments
parser = argparse.ArgumentParser(description='given a histogram, find the smallest-width window that contains a fraction f of the events.')
parser.add_argument('-f', type=float, help='fraction of events in the window')
args = parser.parse_args()
f=args.f

# read the data
a=[]
for line in sys.stdin:
    if ((len(line)>1) and (line[0]!="#")):
        cols=line.split()
        if (len(a)==0):
            for col in cols:
                a.append([])
        for i in range(len(cols)):
            a[i].append(float(cols[i]))

# initialise the window
winsum=0.0
winmin=0.0
winmax=0.0
minwidth=1.0e100
iminlo=-1
iminhi=-1

ilo=0
ihi=0
n=len(a[0])

# loop
while True:
    # move the upper end of the window until we reach f
    while ihi<n-1 and (winsum+(a[2][ihi]-a[0][ihi])*a[3][ihi])<f:
        ihi+=1
        winsum+=(a[2][ihi]-a[0][ihi])*a[3][ihi]
    if (ihi==n-1): break

    # deal with the floating upper end
    next_add = (a[2][ihi]-a[0][ihi])*a[3][ihi]
    frac = (f-winsum)/next_add
    
    lower_end = a[0][ilo]
    upper_end = a[0][ihi]+frac*(a[2][ihi]-a[0][ihi])
    #print "  ",lower_end,upper_end,upper_end-lower_end
    if upper_end-lower_end < minwidth:
        winmin = lower_end
        winmax = upper_end
        minwidth = upper_end-lower_end
        iminlo=ilo
        iminhi=-1
        #print "  new min"

    ihi+=1
    winsum+=next_add

    # move the lower end until we're below the fraction
    while (winsum-(a[2][ilo]-a[0][ilo])*a[3][ilo])>f:
        ilo+=1
        winsum-=(a[2][ilo]-a[0][ilo])*a[3][ilo]

    # deal with the flaoting lower end
    next_sub = (a[2][ilo]-a[0][ilo])*a[3][ilo]
    frac = (winsum-f)/next_sub
    
    lower_end = a[0][ilo]+frac*(a[2][ilo]-a[0][ilo])
    upper_end = a[0][ihi]
    #print "  ",lower_end,upper_end,upper_end-lower_end
    if upper_end-lower_end < minwidth:
        winmin = lower_end
        winmax = upper_end
        minwidth = upper_end-lower_end
        iminlo=-1
        iminhi=ihi
        #print "  new min"

    ilo+=1
    winsum-=next_sub

# Now, within that window, find the median
winsum=0.0
winmid=0.0
if iminhi<0:
    ilo=iminlo
    ihi=ilo
    while (winsum+(a[2][ihi]-a[0][ihi])*a[3][ihi])<0.5*f:
        ihi+=1
        winsum+=(a[2][ihi]-a[0][ihi])*a[3][ihi]
    next_add = (a[2][ihi]-a[0][ihi])*a[3][ihi]
    frac = (0.5*f-winsum)/next_add
    winmid=a[0][ihi]+frac*(a[2][ihi]-a[0][ihi])

if iminlo<0:
    ihi=iminhi
    ilo=ihi-1
    while (winsum+(a[2][ilo]-a[0][ilo])*a[3][ilo])<0.5*f:
        ilo-=1
        winsum+=(a[2][ilo]-a[0][ilo])*a[3][ilo]
    next_add = (a[2][ilo]-a[0][ilo])*a[3][ilo]
    frac = (0.5*f-winsum)/next_add
    winmid=a[2][ilo]-frac*(a[2][ilo]-a[0][ilo])

print winmin,winmid,winmax
        
