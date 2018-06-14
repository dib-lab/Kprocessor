#include <iostream>
#include <string>
#include "ThirdParty/CLI11.hpp"
#include <vector>
#include <stdint.h>
#include <gqf.h>
#include "KmerCounter/KmerCounter.hpp"
#include "KmerCounter/kmer.h"
#include "Utils/utils.hpp"
#include <cmath>

using namespace std;


int KmerCounter_main(int argc, char *argv[]){
  CLI::App app;
  vector<string> input_files;
  string outputMQF;
  string outputKmers="";
  uint64_t nslots=32768;
  uint64_t fixed_size_counter=1;
  double fpr=0;
  int noThreads=1;
  uint64_t maxMemory=0;
  int k;
  string ntcardFile="";

  app.add_option("-i,--input", input_files,
   "Sequence Files to count. can be fasta,fastq or BAM")->required()
  ->check(CLI::ExistingFile)->group("I/O");
  app.add_option("-o,--output", outputMQF,
   "Output MQF filename")->required()->group("I/O");
  app.add_option("-u,--output-kmers", outputKmers,
    "Output in the format of Kmer\tCount. Available only in Exact Counting")->group("I/O");

  app.add_option("-k,--kmer-length",k,"kmer length")->required()->group("MQF Options");
  app.add_option("-s,--nu-slots",nslots,"Number of slots in MQF. Should be of power of two")->group("MQF Options");
  app.add_option("-f,--fixed-size-counter",fixed_size_counter,
  "Number of bits in Fixed-size counter size in MQF. Default 1")->group("MQF Options");
  app.add_option("-r,--fpr",fpr,
  "False Positive Rate of MQF. use 0 for exact counting and less than 1 for probalistic counting. Default 0")->group("MQF Options");


  app.add_option("-n,--ntcard", ntcardFile,"Ntcard file to help estimation of number of slots and fixed size counter")
  ->check(CLI::ExistingFile)->group("MQF Options");


  app.add_option("-t,--threads", noThreads,
   "Number of threads used in kmer counting. Default 1")->group("Misc");
  app.add_option("-m,--max-memory", maxMemory,
   "Max Memory allocated by KmerCounter. Default Unlimited")->group("Misc");



  CLI11_PARSE(app, argc, argv);

  Hasher* hasher;
  uint64_t num_hashbits;
  if(fpr==0){
    hasher=new IntegerHasher(BITMASK(2*k));
    num_hashbits=2*k;
  }
  else if(fpr<1){
    hasher=new MumurHasher(2038074761);
    num_hashbits=-(uint64_t)(log2(fpr))+1;
  }
  else{
    cerr<<"False positive rate should be less than one"<<endl;
    return 1;
  }

  uint64_t requiredMem=0,tagSize=0;
  if(ntcardFile != "")
  {
    estimateMemRequirement(ntcardFile,
        num_hashbits, tagSize,
       &nslots, &fixed_size_counter, &requiredMem);
  // cout<<"Number Slots = "<<nslots<<endl
  //          <<"Fixed size counters= "<<fixed_size_counter<<endl
  //          <<"Memory= "<<requiredMem<<"KB"<<endl;
  }
  uint64_t qbits=(uint64_t)log2((double)nslots);
  if(fpr>0)
    num_hashbits+=qbits;



  QF qf;
  qf_init(&qf, nslots, num_hashbits, 0,fixed_size_counter, true, "", 2038074761);

  for(auto file: input_files)
    loadIntoMQF(file,k,noThreads,hasher,&qf);


  if(outputKmers!=""){
    if(fpr==0){
    dumpMQF(&qf,k,outputKmers);
    }
    else{
      cerr<<"dump mqf in text format is not supported for inexact counting"<<endl;
    }

  }
  qf_serialize(&qf,outputMQF.c_str());

  delete hasher;
  qf_destroy(&qf);

  return 0;
}
