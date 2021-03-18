class colored_kDataFrame{
    public:
        flat_hash_map<uint32_t,string> namesMap;
        flat_hash_map<string,uint32_t> namesMapInv;
        colored_kDataFrame();
        void addNewColor(uint32_t color, vector<uint32_t> & samplesIds);
        void setColor(string kmer,uint32_t color);
        void setColor(uint64_t kmer,uint32_t color);
        uint32_t getColor(string kmer);
        uint32_t getColor(uint64_t kmer);
        vector<uint32_t> getKmerSource(string kmer);
        void getKmerSource(string kmer,vector<uint32_t> & result);
        void getKmerSource(uint64_t kmer,vector<uint32_t> & result);
        vector<uint32_t> getKmerSourceFromColor(uint32_t color);
        void getKmerSourceFromColor(uint32_t color,vector<uint32_t> & result);

        void colorKmer(string kmer,vector<uint32_t> & samplesIds);

        void setColorTable(colorTable* table);
        void setkDataFrame(kDataFrame* f);
        void save(string prefix);
        static colored_kDataFrame* load(string prefix);
        uint64_t getkSize();

        // Converting phmap to unordered_map, mainly for the python interface
        unordered_map<int, string> names_map();
        unordered_map<string, int> inverse_names_map();

        // Get the kDataFrame of the colored_kDataFrame
        kDataFrame * getkDataFrame();

};