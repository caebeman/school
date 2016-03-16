import os, inspect
import subprocess

import toolspath
from testing.test import *

curdir = os.path.dirname(inspect.getfile(inspect.currentframe()))

class CmpTest(Test):
   cmd = []
   ref = "no/such/file"

   def run(self):
      # print curdir
      # print self.project_path
      
      self.log("Copying files")
      subprocess.Popen(["cp", "-r", curdir + "/input", curdir + "/output", 
                        self.project_path])

      self.cmd[0] = self.project_path + "/" + self.cmd[0]
      if (len(self.cmd) != 1):
         self.cmd[-1] = self.project_path + "/" + self.cmd[-1]
      self.ref = self.project_path + "/" + self.ref

      self.log("Running fastsort")
      p1 = subprocess.Popen(self.cmd, stdout=subprocess.PIPE)
      out1, err1 = p1.communicate()      

      self.log("Checking output")
      p2 = subprocess.Popen(["cat", self.ref], stdout=subprocess.PIPE)
      out2, err2 = p2.communicate()

      if not (out1 == out2):
         print "expected output is: \n\n", out2
         print "but got:\n\n", out1
         raise Failure("Output mismatched")

      self.done()

class Empty(CmpTest):
   name = "empty"
   description = "empty input file"
   cmd = ["fastsort", "input/empty"]
   ref = "output/empty"
   timeout = 15
   point_value = 10

class One(CmpTest):
   name = "one"
   description = "a single record"
   cmd = ["fastsort", "input/one"]
   ref = "output/one"
   timeout = 15
   point_value = 10

class Five1(CmpTest):
   name = "five1"
   description = "five records"
   cmd = ["fastsort", "input/five1"]
   ref = "output/five1"
   timeout = 15
   point_value = 10

class Five2(CmpTest):
   name = "five2"
   description = "five records"
   cmd = ["fastsort", "-2", "input/five2"];
   ref = "output/five2"
   timeout = 15
   seed = 2
   point_value = 10

class Ten(CmpTest):
   name = "ten"
   description = "10 records"
   cmd = ["fastsort", "-2", "input/ten"]
   ref = "output/ten"
   timeout = 20
   point_value = 10

class MissKey(CmpTest):
   name = "misskeys"
   description = "missing keys in some lines"
   cmd = ["fastsort", "-5", "input/missing_keys"]
   ref = "output/missing_keys"
   timeout = 20
   point_value = 10
   
test_list = [Empty, One, Five1, Five2, MissKey]
