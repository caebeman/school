#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>

#include <unistd.h>
#include <string.h>

using namespace std;

void
usage(char *prog) 
{
    fprintf(stderr, "usage: %s <-s random seed> <-n number of records> <-o output file>\n", prog);
    exit(1);
}

void 
genstr(string& str, const unordered_set<string>& dup) {
  static const char alphanum[] =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";

  while (dup.find(str) != dup.end()) {
    for (unsigned i = 0; i < str.size(); i++) {
      str[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }
  }

  return;
}

int
main(int argc, char* argv[]) {
  // arguments
  int randomSeed  = 0;
  int recordsLeft = 0;
  const char *outFile   = "/no/such/file";

  // input params
  int c;
  opterr = 0;
  while ((c = getopt(argc, argv, "n:s:o:")) != -1) {
    switch (c) {
    case 'n':
      recordsLeft = atoi(optarg);
      break;
    case 's':
      randomSeed  = atoi(optarg);
      break;
    case 'o':
      outFile     = strdup(optarg);
      break;
    default:
      usage(argv[0]);
    }
  }

  // seed random number generator
  srand(randomSeed);

  const unsigned line_len = 126;
  string word(line_len, '0');

  unordered_set<string> dup_word;

  ofstream output(outFile);
  for (int i = 0; i < recordsLeft; i++) {
    genstr(word, dup_word);
    output << word << endl;
    dup_word.insert(word);
  }
  output.close();

  return 0;
}
