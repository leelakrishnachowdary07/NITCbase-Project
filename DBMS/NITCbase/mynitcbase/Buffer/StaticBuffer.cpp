#include "StaticBuffer.h"
// the declarations for this class can be found at "StaticBuffer.h"

unsigned char StaticBuffer::blocks[BUFFER_CAPACITY][BLOCK_SIZE];
struct BufferMetaInfo StaticBuffer::metainfo[BUFFER_CAPACITY];
unsigned char StaticBuffer::blockAllocMap[DISK_BLOCKS];
StaticBuffer::StaticBuffer() {
  // copy blockAllocMap blocks from disk to buffer (using readblock() of disk)
  // blocks 0 to 3
  int k=0;
  for(int i=0;i<4;i++){
    unsigned char buffer[BLOCK_SIZE];
    Disk::readBlock(buffer,i);
    for(int j=0;j<BLOCK_SIZE;j++,k++){
      StaticBuffer::blockAllocMap[k]=buffer[j];
    }
  }
  // initialise all blocks as free
  for (/*bufferIndex = 0 to BUFFER_CAPACITY-1*/int bufferIndex=0;bufferIndex<BUFFER_CAPACITY;bufferIndex++) {
    metainfo[bufferIndex].free = true;
    metainfo[bufferIndex].dirty = false;
    metainfo[bufferIndex].timeStamp = -1;
    metainfo[bufferIndex].blockNum = -1;
  }
}
//INSERT * into relname where condition

/*
At this stage, we are not writing back from the buffer to the disk since we are
not modifying the buffer. So, we will define an empty destructor for now. In
subsequent stages, we will implement the write-back functionality here.
*/
StaticBuffer::~StaticBuffer() {
   // copy blockAllocMap blocks from buffer to disk(using writeblock() of disk)
   int k=0;
   for(int i=0;i<4;i++){
   unsigned char buffer[BLOCK_SIZE];
   for(int j=0;j<BLOCK_SIZE;j++,k++){
    buffer[j]=StaticBuffer::blockAllocMap[k];
   }
   Disk::writeBlock(buffer,i);
   }
  /*iterate through all the buffer blocks,
    write back blocks with metainfo as free=false,dirty=true
    using Disk::writeBlock()
    */
  for(int i=0;i<32;i++){
    if(metainfo[i].free==false && metainfo[i].dirty==true){
      Disk::writeBlock(blocks[i],metainfo[i].blockNum);
    }
  }
}

int StaticBuffer::getFreeBuffer(int blockNum) {
  if (blockNum < 0 || blockNum > DISK_BLOCKS) {
    return E_OUTOFBOUND;
  }
  for(int i=0;i<BUFFER_CAPACITY;i++){
    metainfo[i].timeStamp++;
  }
  int allocatedBuffer=-1;

  // iterate through all the blocks in the StaticBuffer
  // find the first free block in the buffer (check metainfo)
  // assign allocatedBuffer = index of the free block
    for(int i=0;i<BUFFER_CAPACITY;i++){
        if(metainfo[i].free==true){
            allocatedBuffer=i;
            break;
        }
    }
    if(allocatedBuffer==-1){
      int lar=-1;
      int idx=-1;
      for(int i=0;i<32;i++){
        if(metainfo[i].timeStamp>lar){
          idx=i;
          lar=metainfo[i].timeStamp;
        }
      }
      allocatedBuffer=idx;
      if(metainfo[idx].dirty==true){
        Disk::writeBlock(blocks[allocatedBuffer],metainfo[allocatedBuffer].blockNum);
      }
    }
  metainfo[allocatedBuffer].free = false;
  metainfo[allocatedBuffer].dirty = false;
  metainfo[allocatedBuffer].timeStamp = 0;
  metainfo[allocatedBuffer].blockNum = blockNum;

  return allocatedBuffer;
}

/* Get the buffer index where a particular block is stored
   or E_BLOCKNOTINBUFFER otherwise
*/
int StaticBuffer::getBufferNum(int blockNum) {
  // Check if blockNum is valid (between zero and DISK_BLOCKS)
  // and return E_OUTOFBOUND if not valid.
    if(blockNum<0 || blockNum>DISK_BLOCKS){
        return E_OUTOFBOUND;
    }
    for(int i=0;i<BUFFER_CAPACITY;i++){
        if(metainfo[i].blockNum==blockNum){
            return i;
        }
    }

  // find and return the bufferIndex which corresponds to blockNum (check metainfo)

  // if block is not in the buffer
  return E_BLOCKNOTINBUFFER;
}
int StaticBuffer::setDirtyBit(int blockNum){
    // find the buffer index corresponding to the block using getBufferNum().
    int buffnum=getBufferNum(blockNum);

    // if block is not present in the buffer (bufferNum = E_BLOCKNOTINBUFFER)
    //     return E_BLOCKNOTINBUFFER
    if(buffnum==E_BLOCKNOTINBUFFER){
      return E_BLOCKNOTINBUFFER;
    }
    // if blockNum is out of bound (bufferNum = E_OUTOFBOUND)
    //     return E_OUTOFBOUND
    if(buffnum==E_OUTOFBOUND){
      return E_OUTOFBOUND;
    }
    // else
    //     (the bufferNum is valid)
    //     set the dirty bit of that buffer to true in metainfo
    else{
      metainfo[buffnum].dirty=true;
    }

    return SUCCESS;
}