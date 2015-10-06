# to run:
#   python makevalues.py
# redirect output to a file named outfile
#   python makevalues.py > outfile
# change the number of values generated (and redirect output to a file)
#   python makevalues.py 10 > file_10
#   python makevalues.py 100 > file_100
#   python makevalues.py 1000 > file_1000
import numpy as np
import sys

def main():
 
  mu = 80
  sigma = 15 
  numvals = 300

  if(sys.argv >= 2):
      numvals = int(sys.argv[1])

  vals = np.random.normal(mu, sigma, numvals)

  for i in vals: 
      if(i <= 100 and i > 0): 
          whole = int(i)
          left = i - whole
          whole = whole*1.0
          if(left >= 0.5): 
              whole = whole + 0.5 
          print whole 

main()
