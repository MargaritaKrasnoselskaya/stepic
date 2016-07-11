#include <iostream>
#include <unistd.h>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <omp.h>
#include <algorithm>
#include <cstdio>
#include <ctime>
using namespace std;
static int n;
static const long chunk=8;
void mymerge(long lbeg, long len1, long rbeg, long len2, long dstbeg, vector<int>* src, vector<int>*dst)
{
    long l=lbeg; int r=rbeg; long d=dstbeg;
    long m=dst->size(); long dsize=min(m, dstbeg+len1+len2);
    long l_end = (long)len1+lbeg; long r_end = (long)len2+rbeg;
   //  cerr<<"I merge Left: "<<l<<" with len "<<len1<<", right: "<<r<<" with rlen: "<<len2<<" in dest: "<<d<<"( "<<dsize<<")\n";
    while(d<dsize)
    {
         if (l>=l_end)
        {
             for (; r<r_end; r++)
            {
                dst->at(d) = src->at(r); d++;
            }
            break;
        }
        if (r>=r_end)
        {
             for (; l<l_end; l++)
            {
                dst->at(d) = src->at(l); d++;
            }
            break;
        }
        if (src->at(l)<src->at(r))
        {
            dst->at(d) = src->at(l); d++; l++;
        }
        else
        {
            dst->at(d) = src->at(r); d++; r++;
        }
    }
}
using std::time;
int main()
{
    n = sysconf(_SC_NPROCESSORS_ONLN);    // set num of processes
    vector<int>* src =new vector<int>; vector<int>* dst=new vector<int>;
    vector<int> *tmp=new vector<int>; time_t time1 = clock();
    int val;
    while (scanf("%d", &val) == 1)
    {
        src->push_back(val);
        dst->push_back(-1);
    }
    long vsize = src->size();
    time_t time2 = clock();

    for (long i=0; i<vsize; i=i+chunk)
    {
        sort(src->begin()+i, src->begin()+min(i+chunk, vsize));
    }
    long csize = chunk;
    long lastel;
    time_t time3 = clock();
     while (csize*2<=vsize)
    {
        lastel = vsize-vsize%(2*csize);
         #pragma omp parallel for
       for (long i=0; i<lastel; i=i+2*csize)
       {    mymerge(i, csize, i+csize, csize, i, src, dst);  }
      if (lastel+csize < vsize )
                mymerge(lastel, csize, lastel+csize, vsize-lastel-csize, lastel, src, dst);
        else
        {
            if (lastel+csize/2 < vsize )
                mymerge(lastel, csize/2, lastel+csize/2, vsize-lastel-csize/2, lastel, src, dst);
        }
       tmp = dst; dst = src; src= tmp;
       csize = csize*2;
    }
    //last call
    if (csize<vsize)
        mymerge(0, csize, csize, vsize-csize, 0, src, dst);
    else {tmp = dst; dst = src; src= tmp;}
    time_t time4 = clock();
    int tnp=-1;
    cerr<<"ex. time "<<time1<<"\n"<<time2<<"\n"<<time3<<"\n"<<time4<<"\n";
    for (long i=0; i<vsize; i++)
    {
        if (dst->at(i)<tnp) cerr<<"\n---! "<<i<<" !---";
        cout<<dst->at(i)<<" ";
        tnp = dst->at(i);
    }
return 0;
}
