class kDataFrameMQF: public kDataFrame{

public:
  kDataFrameMQF();
  kDataFrameMQF(uint64_t kSize);
  kDataFrameMQF(uint64_t kSize, int mode);
  kDataFrameMQF(uint64_t ksize, uint8_t q, hashingModes HM);
  kDataFrameMQF(readingModes RM, hashingModes HM, map<string, int> params);
  kDataFrameMQF(uint64_t ksize,uint8_t q,uint8_t fixedCounterSize,uint8_t tagSize,double falsePositiveRate);

  ~kDataFrameMQF(){
    qf_destroy(mqf);
    delete mqf;
  }

  void reserve (uint64_t n);

  kDataFrame* getTwin();

  bool setCount(string kmer,uint64_t count);
  bool setCount(uint64_t kmer, uint64_t count);
  bool insert(string kmer,uint64_t count);
  bool insert(string kmer);
  bool insert(uint64_t kmer, uint64_t count);
  bool insert(uint64_t kmer);
  uint64_t getCount(string kmer);
  uint64_t getCount(uint64_t kmer);

  bool erase(string kmer);
  bool erase(uint64_t kmer);

  uint64_t size();
  uint64_t max_size();
  float load_factor();
  float max_load_factor();

  QF* getMQF(){
    return mqf;
  }

  void save(string filePath);
  static kDataFrame* load(string filePath);

  kDataFrameIterator begin();
  kDataFrameIterator end();
};