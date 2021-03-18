#include "colored_kDataFrame.hpp"

using namespace std;


colored_kDataFrame::colored_kDataFrame()
{
  frame=new kDataFrameMQF();
  colors=new intVectorsTable();
  nextAvailableColor=1;
}

void colored_kDataFrame::addNewColor(uint32_t color, vector<uint32_t> & samplesIds)
{
  colors->setColor(color,samplesIds);
}
void colored_kDataFrame::setColor(string kmer,uint32_t color)
{
  frame->setCount(kmer,color);
}
void colored_kDataFrame::setColor(uint64_t kmer,uint32_t color)
{
    frame->setCount(kmer,color);
}
uint32_t colored_kDataFrame::getColor(string kmer)
{
  return frame->getCount(kmer);
}

uint32_t colored_kDataFrame::getColor(uint64_t kmer)
{
    return frame->getCount(kmer);
}

void colored_kDataFrame::getKmerSource(string kmer,vector<uint32_t>& result)
{
  uint32_t color=getColor(kmer);
  result.clear();
  if(color==0)
  {
    return;
  }
  colors->getSamples(color,result);
}

void colored_kDataFrame::getKmerSource(uint64_t kmer,vector<uint32_t>& result)
{
    uint32_t color=getColor(kmer);
    result.clear();
    if(color==0)
    {
        return;
    }
    colors->getSamples(color,result);
}

vector<uint32_t> colored_kDataFrame::getKmerSource(string kmer)
{
    vector<uint32_t> result;
    uint32_t color=getColor(kmer);
    if(color==0)
    {
        return {};
    }
    colors->getSamples(color,result);
}

void colored_kDataFrame::getKmerSourceFromColor(uint32_t color,vector<uint32_t>& result)
{
  colors->getSamples(color,result);
}

vector<uint32_t> colored_kDataFrame::getKmerSourceFromColor(uint32_t color)
{
    vector<uint32_t> result;
    colors->getSamples(color,result);
    return result;
}

void colored_kDataFrame::colorKmer(string kmer,vector<uint32_t> & samplesIds){
  uint64_t color=colorsInv->getColorId(samplesIds);
  if(color==0)
  {
    color=nextAvailableColor++;
    colorsInv->setColorId(color,samplesIds);
    colors->setColor(color,samplesIds);
  }
  setColor(kmer,color);
}
void colored_kDataFrame::setColorTable(colorTable* table){
  colors=table;
}
void colored_kDataFrame::setkDataFrame(kDataFrame* f)
{
    delete frame;
  frame=f;
}
uint64_t colored_kDataFrame::getkSize(){
  return frame->getkSize();
}
void colored_kDataFrame::save(string prefix)
{
    frame->serialize(prefix);
  colors->save(prefix);
  ofstream namesMapOut(prefix+".namesMap");
  namesMapOut<<namesMap.size()<<endl;
  for(auto it:namesMap)
  {
    namesMapOut<<it.first<<" "<<it.second<<endl;
  }
  namesMapOut.close();
}
colored_kDataFrame* colored_kDataFrame::load(string prefix)
{
  colored_kDataFrame* res=new colored_kDataFrame();
  res->frame=kDataFrame::load(prefix);
  res->colors=colorTable::load(prefix);
  ifstream namesMapIn(prefix+".namesMap");
  uint64_t size;
  namesMapIn>>size;
  for(int i=0;i<size;i++)
  {
    uint32_t color;
    string name;
    namesMapIn>>color>>name;
    res->namesMap[color]=name;
    res->namesMapInv[name]=color;
  }
  return res;
}

unordered_map<int, string> colored_kDataFrame::names_map(){
    unordered_map<int, string> names_map;
    for(const auto &item : this->namesMap){
        names_map[item.first] = item.second;
    }
    return names_map;
}

unordered_map<string, int> colored_kDataFrame::inverse_names_map(){
    unordered_map<string, int> inv_names_map;
    for(const auto &item : this->namesMapInv){
        inv_names_map[item.first] = item.second;
    }
    return inv_names_map;
}

kDataFrame * colored_kDataFrame::getkDataFrame() {
    return this->frame;
}