//-----------------------------------------------------------------------------
//  WordFrequency.cpp
//  lzsander
//  evaluates word frequency for a given file
//-----------------------------------------------------------------------------

#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include "Dictionary.h"

using namespace std;

std::string to_lower(std::string s){
    int len = s.length();
    string lower = "";
    for(int i=0; i<len; i++){
        lower += tolower(s[i]);
    }
    return lower;
}

int main(int argc, char * argv[]){

   size_t begin, end, len;
   ifstream in;
   ofstream out;
   string line;
   string token;
   string delim = " \t\\\"\',<.>/?;:[{]}|`~!@#$%^&*()-_=+0123456789\n";


   // check command line for correct number of arguments
   if( argc != 3 ){
      cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
      return(EXIT_FAILURE);
   }

   // open files for reading and writing 
   in.open(argv[1]);
   if( !in.is_open() ){
      cerr << "Unable to open file " << argv[1] << " for reading" << endl;
      return(EXIT_FAILURE);
   }

   out.open(argv[2]);
   if( !out.is_open() ){
      cerr << "Unable to open file " << argv[2] << " for writing" << endl;
      return(EXIT_FAILURE);
   }

    Dictionary A;

   // read each line of input file, then count and print tokens 
   while( getline(in, line) )  {
      len = line.length();
      
      // get first token
      begin = min(line.find_first_not_of(delim, 0), len);
      end   = min(line.find_first_of(delim, begin), len);
      token = to_lower(line.substr(begin, end-begin));

      while( token!="" ){  // we have a token
         // insert token to dictionary
         if(A.contains(token)) A.getValue(token)++;
         else A.setValue(token, 1);
         // get next token
         begin = min(line.find_first_not_of(delim, end+1), len);
         end   = min(line.find_first_of(delim, begin), len);
         token = to_lower(line.substr(begin, end-begin));
      }

   }

   out << A << endl;

   // close files 
   in.close();
   out.close();

   return(EXIT_SUCCESS);
}

