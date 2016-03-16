import os, inspect
from tempfile import mktemp

import toolspath
from testing import Test

# tests for checking correct error behavior

cmd_error = "Error: Bad command line parameters\n"
line_error = "Line too long\n"
curdir = os.path.dirname(inspect.getfile(inspect.currentframe()))

class Arg(Test):
   name = "arg"
   description = "Bad arguments"
   point_value = 10
   timeout = 5
   def run(self):
      self.runexe(["fastsort"], stderr = cmd_error, status = 1)
      self.done()

class Arg2(Test):
   name = "arg2"
   description = "Bad arguments"
   point_value = 10
   timeout = 5
   def run(self):
      self.runexe(["fastsort", "a", "b", "c", "d"],
            stderr = cmd_error, status = 1)
      self.done()

class Arg3(Test):
   name = "arg3"
   description = "Invalid key value"
   point_value = 10
   timeout = 5
   def run(self):
      self.runexe(["fastsort", "-i", "input/empty"],
            stderr = cmd_error, status = 1)
      self.done()

class Badin(Test):
   name = "badin"
   description = "Bad input file"
   timeout = 5
   point_value = 10
   def run(self):
      "mktemp --> mkstemp"
      invalid = mktemp(prefix='/invalid/path/')
      self.runexe(["fastsort", invalid], 
          stderr = "Error: Cannot open file {0}\n".format(invalid), status = 1)
      self.done()

class TooLong(Test):
   name = "toolong"
   description = "some line has more than 128 characters"
   cmd = ["fastsort", "input/toolong"]
   ref = "output/toolong"
   timeout = 20
   point_value = 10
   def run(self):
      self.runexe(["fastsort", curdir + "/input/toolong"], 
                  stderr = line_error , status = 1)
      self.done()

test_list = [Arg, Arg2, Arg3, Badin, TooLong]
