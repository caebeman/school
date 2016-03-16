import os, inspect
import subprocess

import toolspath
from testing.test import *

curdir = os.path.dirname(inspect.getfile(inspect.currentframe()))

class BetaTest(Test):
   cmd = []
   ref = "no/such/file"

   def run(self):
      generate_path = self.test_path + "/generate"
      infile = self.project_path + "/infile"
      outfile = self.project_path + "/outfile"

      self.cmd[0] = self.project_path + "/" + self.cmd[0]
      if (len(self.cmd) != 1):
         self.cmd[-1] = curdir + "/" + self.cmd[-1]
      self.ref = curdir + "/" + self.ref

      self.log("Running fastsort")
      (status, out1) = self.runexe(self.cmd, status=0, stderr="", output=False)

      self.log("Checking output")
      with open(self.ref, 'r') as refout:
          out2=refout.read()

      if not (out1 == out2):
         print "expected output is: \n\n", out2
         print "but got:\n\n", out1
         raise Failure("Output mismatched")

      self.done()

class Empty(BetaTest):
   name = "empty"
   description = "empty input file"
   cmd = ["fastsort", "input/empty"]
   ref = "output/empty"
   timeout = 15
   point_value = 10

class One(BetaTest):
   name = "one"
   description = "a single record"
   cmd = ["fastsort", "input/one"]
   ref = "output/one"
   timeout = 15
   point_value = 10

class Five1(BetaTest):
   name = "five1"
   description = "five records"
   cmd = ["fastsort", "input/five1"]
   ref = "output/five1"
   timeout = 15
   point_value = 10

class Five2(BetaTest):
   name = "five2"
   description = "five records"
   cmd = ["fastsort", "-2", "input/five2"];
   ref = "output/five2"
   timeout = 15
   seed = 2
   point_value = 10

class Ten(BetaTest):
   name = "ten"
   description = "10 records"
   cmd = ["fastsort", "-2", "input/ten"]
   ref = "output/ten"
   timeout = 20
   point_value = 10

class MissKey(BetaTest):
   name = "misskeys"
   description = "missing keys in some lines"
   cmd = ["fastsort", "-5", "input/missing_keys"]
   ref = "output/missing_keys"
   timeout = 20
   point_value = 10
   
test_list = [Empty, One, Five1, Five2, MissKey]
