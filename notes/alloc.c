if (bOnlyUseHeaps1and2 == 1) {
    buf = heapAlloc(1,size,tag,name);
    if (buf == NULL) buf = heapAlloc(2,size,tag,name);
    if (buf == NULL) return NULL;
}
else if (bOnlyUseHeap3 != 0) {
    buf = heapAlloc(3,size,tag,name);
    if (buf == NULL)
}
else {
    if (size < 0x3000) {
      if (size < 0x400) {
        //try heaps 2, 1, 0
        buf = heapAlloc(2,size,tag,name);
        if (buf == NULL) buf = heapAlloc(1,size,tag,name);
        if (buf == NULL) buf = heapAlloc(0,size,tag,name);
      }
      else { //size >= 0x400 - try heaps 1, 2, 0
        buf = heapAlloc(1,size,tag,name);
        if (buf == NULL) buf = heapAlloc(2,size,tag,name);
        if (buf == NULL) buf = heapAlloc(0,size,tag,name);
      }
    }
    else { //size >= 0x3000 - try heaps 0, 1
      buf = heapAlloc(0,size,tag,name);
      if (buf == NULL) buf = heapAlloc(1,size,tag,name);
    }
}
return buf;

/*
Heap 0: for large  allocations (default for >= 0x3000)
Heap 1: for medium allocations (default for >= 0x400)
Heap 2: for small  allocations (default for < 0x400)
Heap 3: for some special cases (certain audio, textures, ?)

There's also a flag to force only using heaps 1 and 2 (in that order)

It will try 100 times to allocate before giving up, but does nothing in between
each attempt to try to relieve the pressure (not even waiting for next frame)
so this is probably not helpful.
- actually no it won't because `keepGoing = false` at the end of the loop.
*/
