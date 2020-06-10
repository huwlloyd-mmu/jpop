""" Janko Scraper """

import requests
from bs4 import BeautifulSoup as soup
import re
import numpy as np

for idx in range(1,371):
    base='https://www.janko.at/Raetsel/Futoshiki/'
    url=base+str(idx).zfill(3)+'.a.htm'
    result=requests.get(url)
    result.encoding=result.apparent_encoding
    page=result.text
    doc=soup(page, features='html.parser')
    script=doc.find('script',{"id":"data"})
    m=re.search(r'size\s*(\d*)\s*problem\s*(\S[\S\s]*\S)\s*solution', str(script))
    size=int(m.groups()[0])
    prob=m.groups()[1]
    cells=prob.split()
    grid=np.array(cells).reshape(2*size-1,2*size-1)
    puzzle=np.full((size,size), '-1')
    rels=[]
    for i,j in np.ndindex(grid.shape):
        ii=i//2
        jj=j//2
        if (i%2==0 and j%2==0 and grid[i,j].isdigit()):
            puzzle[ii,jj]=grid[i,j]
        elif (grid[i,j]=='»'):
            rels.append(str(ii*size+jj)+'>'+str(ii*size+jj+1))
        elif (grid[i,j]=='«'):
            rels.append(str(ii*size+jj+1)+'>'+str(ii*size+jj))
        elif (grid[i,j]=='A'):
            rels.append(str(ii*size+jj+size)+'>'+str(ii*size+jj))
        elif (grid[i,j]=='V'):
            rels.append(str(ii*size+jj)+'>'+str(ii*size+jj+size))
    outname='janko'+str(idx).zfill(3)+'_'+str(size)+'.txt'
    #print(outname)
    with open(outname,'w+') as outfile:
        print(size,file=outfile)
        for i,j in np.ndindex(puzzle.shape):
            print(puzzle[i,j],end=' ',file=outfile)
            if (j==size-1):
                print(file=outfile)
        print(len(rels),file=outfile)
        for r in rels:
            print(r,file=outfile)
