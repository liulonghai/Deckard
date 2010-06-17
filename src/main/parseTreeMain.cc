#include <map>
#include <string>
#include <algorithm>
#include <cctype>
#include <ptree.h>

using namespace std;

map<string,int> name2id;
map<int,string> id2name;
#ifdef JAVA
string identifierTypeName = "ID_TK";
#else
string identifierTypeName = "IDENTIFIER";
#endif

template <class T> bool from_string(T& t, 
                 const std::string& s, 
                 std::ios_base& (*f)(std::ios_base&))
{
  std::istringstream iss(s);
  return !(iss >> f >> t).fail();
}

int main( int argc, char **argv )
{
  if ( argc!=4 && argc!=5 ) {
    cerr << "Usage: " << argv[0] << " <filename> <start token id> <end token id> [force dump]" << endl;
    return 1;
  }

  FILE * fin = NULL;
  if ( (fin=fopen(argv[1], "r"))==NULL ) {
    cerr << "Can't open file: " << argv[1] << endl;
    return 2;
  }

  long tbid, teid;
  if(!from_string(tbid, string(argv[2]), dec) || tbid<0 ) {
    cerr << "Error: start token id incorrect: " << argv[2] << "-->" << tbid << endl;
    return 1;
  }
  if(!from_string(teid, string(argv[3]), dec) || teid<=0 ) {
    cerr << "Error: end token id incorrect: " << argv[3] << "-->" << teid << endl;
    return 1;
  }
  
  id_init();

  ParseTree* pt = parseFile(argv[1]);
  if ( pt==NULL ) {
    cerr << "Error: no parse tree created for file: " << argv[1] << endl;
    return 1;
  }

  if(argc>4) {
    pt->dumpParseTree(true);
  } else {
    pt->dumpParseTree(false);
  }

  Tree* node = pt->tokenRange2Tree(tbid, teid);
  long i = pt->tree2sn(node);
  if(i<=0) {
    cerr << "Warining: incorrect tree node order number: " << endl;
  }
  cout << pt->filename << " " << tbid << " " << teid << " " << i << endl;

  return 0;
}
