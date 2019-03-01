#include <iostream>
#include <string>

using namespace std;

#define KPROCESSOR_VERSION "0.1"

int KmerCounter_main(int argc, char *argv[]);
int estimateMemory_main(int argc, char *argv[]);
int dump_main(int argc, char *argv[]);
int index_main(int argc, char *argv[]);
int playground_main(int argc, char *argv[]);


string KprocessorVersion()
{
    return KPROCESSOR_VERSION;
}

static void usage()
{
    /* Please improve the grouping */

    cout<<
"\n"
"This is Kprocessor version "<<KprocessorVersion()
<<" developed by Mostafa Shokrof <mostafa.shokrof@gmail.com> and Tamer Mansour(UC Davis)<drtamermansour@gmail.com>\n"
<<"Usage:   kprocessor <command> [options]\n"<<
"\n"<<
"Commands:\n"<<
"  -- Counter\n"<<
"     count          count kmers in seqeunces file \n"<<
"     estimate       estimate the memory requirements for kmer counting\n"<<
"     dump           dump kmers in the MQF in format of (kmer count) \n"<<
"     index          create a colored debruijn graph of a fasta file\n";
}

int main(int argc, char *argv[])
{
    if (argc < 2) { usage(); return 1; }

    if (string(argv[1])== "help"  || string(argv[1])== "--help" ) {
        if (argc == 2) { usage(); return 0; }

        // display help of the specific tool
        argv++;
        argc = 2;
    }

    int ret = 0;
    if (string(argv[1])== "count")  ret =KmerCounter_main(argc-1,argv+1);
    else if (string(argv[1])== "estimate")  ret =estimateMemory_main(argc-1,argv+1);
    else if (string(argv[1])== "dump")  ret =dump_main(argc-1,argv+1);
    else if (string(argv[1])== "index")  ret =index_main(argc-1,argv+1);
    else if (string(argv[1])== "play")  ret =playground_main(argc-1,argv+1);
    else if (string(argv[1])== "--version" ) {
      cout<<"This is Kprocessor version "<<KprocessorVersion()<<" developed by Mostafa Shokrof <mostafa.shokrof@gmail.com>\n";
    }
    else {
      cout<<"[main] unrecognized command "<< argv[1]<<endl;
      usage();
        return 1;
    }
    return ret;
}
