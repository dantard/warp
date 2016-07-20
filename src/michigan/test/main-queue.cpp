#include "../queue/Compression.h"
#include "../queue/Fragmentation.h"
#include "../queue/Multiqueue.h"
#include "../queue/PriorityQueue.h"

main(){

     PriorityQueue pq;
    Fragmenter f(pq, 25);
    Compressor c(f);
    MultiEnqueue mq(c);

    MultiDequeue<Queue> md(10);
    Uncompressor uc(md);
    Defragmenter d(uc,25);

    char txt[100]="Lorem ipsum dolor sit amet, id iisque deleniti signiferumque eam. Qui cu ullum disputationi";

    std::vector<int> dest;
    for (int i = 0; i< 10; i++){
        mq.push(i,txt,strlen(txt)+1,i,1,dest,0);
    }
    while (pq.size() > 0){
        const char * p;
        int prio, src;
        int size = pq.pop(p,prio, src);
        d.push((char *) p,size,prio,src,dest,0);
        pq.done();
    }
}
