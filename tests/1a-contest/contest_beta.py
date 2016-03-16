import hashlib
from time import sleep
import pickle

import toolspath
from testing.test import *

from check import check_sorted

# tests that use the generate utility and then check the output

def median(vals):
   if len(vals) % 2 == 1:
      return vals[(len(vals) + 1)/ 2 - 1]
   else:
      a = vals[len(vals)/2-1]
      b = vals[len(vals)/2]
      return (a + b) / 2

class Contest(Test):
   name = "contest"
   description = "contest"
   timeout = 10000000
   seeds = [0, 1, 2]
   tests = ["1", "2", "3"]
   n = 100000 # 100 thousand
   key = 1000
   runs = 10

   def run(self):
      generate_path = self.test_path + "/gen"
      infile = self.project_path + "/infile"
      outfile = self.project_path + "/outfile"
      
      res = list()
      score = 0
      for j in self.tests:
         times = list()
         for seed in self.seeds:
            sorted_digest = None

            self.log("Generating input file")
            status = self.run_util([generate_path + j, "-s", str(seed),
                                    "-n", str(self.n), "-o", infile])
            if status != 0:
               raise Exception("generate failed with error " + str(status))

            for i in range(self.runs):
               sleep(0.5)

               self.log("Running fastsort")
               child, outdata = self.runexe(["fastsort", "-" + str(self.key), infile], status = 0, stderr = "", output = False)

               # print type(outdata)

               if status != 0:
                  raise Failure("fastsort exited with status " + str(status))
               self.log("Fastsort completed in " + str(child.wallclock_time) + " seconds")
               times.append(child.wallclock_time);

               self.log("Checking output")
               (is_sorted, msg) = check_sorted(outdata, infile, self.key)
               if not is_sorted:
                  self.fail(msg)
                  raise Failure(msg)

         times = sorted(times)
         # print times
         print "best  ", times[0]
         print "median", median(times)
         print "worst ", times[-1]
         print "\n\n"

         res.append(median(times))

         self.done()

      print "\n\n\n"
      print "**********************************************************************"
      print "Evaluation Summary: "
      print "The performance on three ranges: "
      print res, "\n"
      print "Final result (average): "
      print sum(res) / len(res)

test_list = [Contest]
