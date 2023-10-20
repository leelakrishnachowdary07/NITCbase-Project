#include "BlockBuffer.h"

#include <cstdlib>
#include <cstring>
#include <iostream>
// the declarations for these functions can be found in "BlockBuffer.h"

BlockBuffer::BlockBuffer(int blockNum) {
  // initialise this.blockNum with the argument
  this->blockNum=blockNum;
}

// calls the parent class constructor
RecBuffer::RecBuffer(int blockNum) : BlockBuffer::BlockBuffer(blockNum) {}

// load the block header into the argument pointer
int BlockBuffer::getHeader(struct HeadInfo *head) {
  unsigned char *bufferPtr;
  int ret = loadBlockAndGetBufferPtr(&bufferPtr);
  if (ret != SUCCESS) {
    return ret;
  }

  // populate the numEntries, numAttrs and numSlots fields in *head
  memcpy(&head->numSlots, bufferPtr + 24, 4);
  memcpy(&head->numEntries,bufferPtr+16, 4);
  memcpy(&head->numAttrs,bufferPtr+20, 4);
  memcpy(&head->rblock,bufferPtr+12, 4);
  memcpy(&head->lblock,bufferPtr+8, 4);

  return SUCCESS;
}

// load the record at slotNum into the argument pointer
int RecBuffer::getRecord(union Attribute *rec, int slotNum) {
  struct HeadInfo head;

  // get the header using this.getHeader() function
  BlockBuffer::getHeader(&head);

  int attrCount = head.numAttrs;
  int slotCount = head.numSlots;

  // read the block at this.blockNum into a buffer
  unsigned char *bufferPtr;
  int ret = loadBlockAndGetBufferPtr(&bufferPtr);
  if (ret != SUCCESS) {
    return ret;
  }

  /* record at slotNum will be at offset HEADER_SIZE + slotMapSize + (recordSize * slotNum)
     - each record will have size attrCount * ATTR_SIZE
     - slotMap will be of size slotCount
  */
  int recordSize = attrCount * ATTR_SIZE;
  unsigned char *slotPointer = bufferPtr + (HEADER_SIZE + slotCount + (recordSize*slotNum));
  /* calculate buffer + offset */;

  // load the record into the rec data structure
  memcpy(rec, slotPointer, recordSize);

  return SUCCESS;
}


int BlockBuffer::loadBlockAndGetBufferPtr(unsigned char **buffPtr) {
  // check whether the block is already present in the buffer using StaticBuffer.getBufferNum()
  int bufferNum = StaticBuffer::getBufferNum(this->blockNum);
  if (bufferNum == E_OUTOFBOUND) {
      return E_OUTOFBOUND;
    }
  // if present (!=E_BLOCKNOTINBUFFER),
  // set the timestamp of the corresponding buffer to 0 and increment the
  // timestamps of all other occupied buffers in BufferMetaInfo.
  if(bufferNum!=E_BLOCKNOTINBUFFER){
    for(int i=0;i<32;i++){
      StaticBuffer::metainfo[i].timeStamp++;
    }
    StaticBuffer::metainfo[bufferNum].timeStamp=0;
  }
  // else
        // get a free buffer using StaticBuffer.getFreeBuffer()

        // if the call returns E_OUTOFBOUND, return E_OUTOFBOUND here as
        // the blockNum is invalid

        // Read the block into the free buffer using readBlock()

    // store the pointer to this buffer (blocks[bufferNum]) in *buffPtr
  else{
    bufferNum = StaticBuffer::getFreeBuffer(this->blockNum);
    if (bufferNum == E_OUTOFBOUND) {
      return E_OUTOFBOUND;
    }

    Disk::readBlock(StaticBuffer::blocks[bufferNum], this->blockNum);
  }

  // store the pointer to this buffer (blocks[bufferNum]) in *buffPtr
  *buffPtr = StaticBuffer::blocks[bufferNum];

  return SUCCESS;
}
/* used to get the slotmap from a record block
NOTE: this function expects the caller to allocate memory for `*slotMap`
*/
int RecBuffer::getSlotMap(unsigned char *slotMap) {
  unsigned char *bufferPtr;

  // get the starting address of the buffer containing the block using loadBlockAndGetBufferPtr().
  int ret = loadBlockAndGetBufferPtr(&bufferPtr);
  if (ret != SUCCESS) {
    return ret;
  }
  RecBuffer recordBlock(this->blockNum);
  struct HeadInfo head;
  // get the header of the block using getHeader() function
  recordBlock.getHeader(&head);

  int slotCount = /* number of slots in block from header */head.numSlots;

  // get a pointer to the beginning of the slotmap in memory by offsetting HEADER_SIZE
  unsigned char *slotMapInBuffer = bufferPtr + HEADER_SIZE;

  // copy the values from `slotMapInBuffer` to `slotMap` (size is `slotCount`)
  memcpy(slotMap,slotMapInBuffer,slotCount);

  return SUCCESS;
}
int compareAttrs(union Attribute attr1, union Attribute attr2, int attrType) {

    double diff;
    if (attrType == STRING){
        diff = strcmp(attr1.sVal, attr2.sVal);
    }

    else
        diff = attr1.nVal - attr2.nVal;

    if (diff > 0) return 1;
    if (diff < 0) return -1;
    else return 0;
  
}
int RecBuffer::setRecord(union Attribute *rec, int slotNum) {
    unsigned char *bufferPtr;
    /* get the starting address of the buffer containing the block
       using loadBlockAndGetBufferPtr(&bufferPtr). */
    int ret=loadBlockAndGetBufferPtr(&bufferPtr);
    if(ret!=SUCCESS){
      return ret;
    }
    // if loadBlockAndGetBufferPtr(&bufferPtr) != SUCCESS
        // return the value returned by the call.

    /* get the header of the block using the getHeader() function */
    HeadInfo head;
    getHeader(&head);

    // get number of attributes in the block.
    int attrcount=head.numAttrs;

    // get the number of slots in the block.
    int slotcount=head.numSlots;

    // if input slotNum is not in the permitted range return E_OUTOFBOUND.
    if(slotNum>=slotcount){
      return E_OUTOFBOUND;
    }

    /* offset bufferPtr to point to the beginning of the record at required
       slot. the block contains the header, the slotmap, followed by all
       the records. so, for example,
       record at slot x will be at bufferPtr + HEADER_SIZE + (x*recordSize)
       copy the record from `rec` to buffer using memcpy
       (hint: a record will be of size ATTR_SIZE * numAttrs)
    */
      int recsize=attrcount*ATTR_SIZE;
      unsigned char *slotpointer=bufferPtr+HEADER_SIZE+slotcount+(recsize*slotNum);
      memcpy(slotpointer,rec,recsize);
    // update dirty bit using setDirtyBit()
    ret=StaticBuffer::setDirtyBit(this->blockNum);
    if(ret!=SUCCESS){
      std::cout<<"ERROR!";
    }

    /* (the above function call should not fail since the block is already
       in buffer and the blockNum is valid. If the call does fail, there
       exists some other issue in the code) */

    return SUCCESS;
}
int BlockBuffer::setHeader(struct HeadInfo *head){

    unsigned char *bufferPtr;
    // get the starting address of the buffer containing the block using
    // loadBlockAndGetBufferPtr(&bufferPtr).
    int ret=loadBlockAndGetBufferPtr(&bufferPtr);
    // if loadBlockAndGetBufferPtr(&bufferPtr) != SUCCESS
        // return the value returned by the call.
    if(ret!=SUCCESS){
      return ret;
    }
    // cast bufferPtr to type HeadInfo*
    struct HeadInfo *bufferHeader = (struct HeadInfo *)bufferPtr;

    // copy the fields of the HeadInfo pointed to by head (except reserved) to
    // the header of the block (pointed to by bufferHeader)
    //(hint: bufferHeader->numSlots = head->numSlots )
    bufferHeader->blockType=head->blockType;
    bufferHeader->lblock=head->lblock;
    bufferHeader->numAttrs=head->numAttrs;
    bufferHeader->numEntries=head->numEntries;
    bufferHeader->numSlots=head->numSlots;
    bufferHeader->pblock=head->pblock;
    bufferHeader->rblock=head->rblock;
    // update dirty bit by calling StaticBuffer::setDirtyBit()
    ret=StaticBuffer::setDirtyBit(this->blockNum);
    // if setDirtyBit() failed, return the error code
    if(ret != SUCCESS){
      return ret;
    }
    return SUCCESS;
}
int BlockBuffer::setBlockType(int blockType){

    unsigned char *bufferPtr;
    /* get the starting address of the buffer containing the block
       using loadBlockAndGetBufferPtr(&bufferPtr). */
    int ret=loadBlockAndGetBufferPtr(&bufferPtr);
    // if loadBlockAndGetBufferPtr(&bufferPtr) != SUCCESS
        // return the value returned by the call.
    if(ret!=SUCCESS){
      return ret;
    }
    // store the input block type in the first 4 bytes of the buffer.
    // (hint: cast bufferPtr to int32_t* and then assign it)
    // *((int32_t *)bufferPtr) = blockType;
    *((int32_t *)bufferPtr) = blockType;
    // update the StaticBuffer::blockAllocMap entry corresponding to the
    // object's block number to `blockType`.
    StaticBuffer::blockAllocMap[this->blockNum]=blockType;
    // update dirty bit by calling StaticBuffer::setDirtyBit()
    ret=StaticBuffer::setDirtyBit(this->blockNum);
    // if setDirtyBit() failed
        // return the returned value from the call
    if(ret!=SUCCESS){
      return ret;
    }
    return SUCCESS;
}
int BlockBuffer::getFreeBlock(int blockType){

    // iterate through the StaticBuffer::blockAllocMap and find the block number
    // of a free block in the disk.
    int free;
    for(int i=0;i<8192;i++){
      if(StaticBuffer::blockAllocMap[i]==UNUSED_BLK){
        free=i;
        break;
      }
    }
    // if no block is free, return E_DISKFULL.
    if(free>=8192){
      return E_DISKFULL;
    }

    // set the object's blockNum to the block number of the free block.
    this->blockNum=free;
    // find a free buffer using StaticBuffer::getFreeBuffer() .
    int buff=StaticBuffer::getFreeBuffer(free);
    if (buff < 0 && buff >= BUFFER_CAPACITY) {
		printf ("Error: Buffer is full\n");
		return buff;
	}
    // initialize the header of the block passing a struct HeadInfo with values
    // pblock: -1, lblock: -1, rblock: -1, numEntries: 0, numAttrs: 0, numSlots: 0
    // to the setHeader() function.
    HeadInfo blockhead;
    blockhead.pblock=blockhead.lblock=blockhead.rblock=-1;
    blockhead.numAttrs=blockhead.numEntries=blockhead.numSlots=0;
    setHeader(&blockhead);
    // update the block type of the block to the input block type using setBlockType().
    setBlockType(blockType);

    // return block number of the free block.
    return free;
}
BlockBuffer::BlockBuffer(char blockType){
    // allocate a block on the disk and a buffer in memory to hold the new block of
    // given type using getFreeBlock function and get the return error codes if any.
    int blk;
    if(blockType=='R'){
      blk=REC;
    }
    else{
      blk=UNUSED_BLK;
    }
    int blocknum=getFreeBlock(blk);
    if(blocknum < 0 || blockNum>=DISK_BLOCKS){
      std::cout<<"ERROR BLOCK NOT AVAILABLE";
      this->blockNum=blockNum;    
      return;
    }
    // set the blockNum field of the object to that of the allocated block
    // number if the method returned a valid block number,
    // otherwise set the error code returned as the block number.
    this->blockNum=blockNum;
    // (The caller must check if the constructor allocatted block successfully
    // by checking the value of block number field.)
}
RecBuffer::RecBuffer() : BlockBuffer('R'){}
int RecBuffer::setSlotMap(unsigned char *slotMap) {
    unsigned char *bufferPtr;
    /* get the starting address of the buffer containing the block using
       loadBlockAndGetBufferPtr(&bufferPtr). */
    int ret=loadBlockAndGetBufferPtr(&bufferPtr);
    // if loadBlockAndGetBufferPtr(&bufferPtr) != SUCCESS
        // return the value returned by the call.
    if(ret!=SUCCESS){
      return ret;
    }
    // get the header of the block using the getHeader() function
    HeadInfo head;
    getHeader(&head);

    int numSlots = /* the number of slots in the block */head.numSlots;

    // the slotmap starts at bufferPtr + HEADER_SIZE. Copy the contents of the
    // argument `slotMap` to the buffer replacing the existing slotmap.
    // Note that size of slotmap is `numSlots`
    unsigned char *slotPointer = bufferPtr + HEADER_SIZE;
	  memcpy(slotPointer, slotMap, numSlots);
    // update dirty bit using StaticBuffer::setDirtyBit
    ret=StaticBuffer::setDirtyBit(this->blockNum);
    // if setDirtyBit failed, return the value returned by the call
    if(ret != SUCCESS){
      return ret;
    }

    return SUCCESS;
}
int BlockBuffer::getBlockNum(){
    return this->blockNum;
    //return corresponding block number.
}
void BlockBuffer::releaseBlock(){

    // if blockNum is INVALID_BLOCK (-1), or it is invalidated already, do nothing
    if(blockNum==INVALID_BLOCKNUM || StaticBuffer::blockAllocMap[blockNum]==UNUSED_BLK){
      return;
    }

    // else
        /* get the buffer number of the buffer assigned to the block
           using StaticBuffer::getBufferNum().
           (this function return E_BLOCKNOTINBUFFER if the block is not
           currently loaded in the buffer)
            */
          int buffnum=StaticBuffer::getBufferNum(blockNum);
        // if the block is present in the buffer, free the buffer
        // by setting the free flag of its StaticBuffer::tableMetaInfo entry
        // to true.
        if(buffnum>=0 && buffnum<BUFFER_CAPACITY){
          StaticBuffer::metainfo[buffnum].free=true;
        }
        // free the block in disk by setting the data type of the entry
        // corresponding to the block number in StaticBuffer::blockAllocMap
        // to UNUSED_BLK.
        StaticBuffer::blockAllocMap[blockNum]=UNUSED_BLK;

        // set the object's blockNum to INVALID_BLOCK (-1)
        this->blockNum=-1;
}
// call the corresponding parent constructor
IndBuffer::IndBuffer(char blockType) : BlockBuffer(blockType){}

// call the corresponding parent constructor
IndBuffer::IndBuffer(int blockNum) : BlockBuffer(blockNum){}

IndInternal::IndInternal() : IndBuffer('I'){}
// call the corresponding parent constructor
// 'I' used to denote IndInternal.

IndInternal::IndInternal(int blockNum) : IndBuffer(blockNum){}
// call the corresponding parent constructor

IndLeaf::IndLeaf() : IndBuffer('L'){} // this is the way to call parent non-default constructor.
                      // 'L' used to denote IndLeaf.

//this is the way to call parent non-default constructor.
IndLeaf::IndLeaf(int blockNum) : IndBuffer(blockNum){}

int IndInternal::getEntry(void *ptr, int indexNum) {
    // if the indexNum is not in the valid range of [0, MAX_KEYS_INTERNAL-1]
    //     return E_OUTOFBOUND.
    if(indexNum<0 || indexNum>=MAX_KEYS_INTERNAL){
      return E_OUTOFBOUND;
    }

    unsigned char *bufferPtr;
    /* get the starting address of the buffer containing the block
       using loadBlockAndGetBufferPtr(&bufferPtr). */
    int ret=loadBlockAndGetBufferPtr(&bufferPtr);
    // if loadBlockAndGetBufferPtr(&bufferPtr) != SUCCESS
    //     return the value returned by the call.
    if(ret != SUCCESS){
      return ret;
    }
    // typecast the void pointer to an internal entry pointer
    struct InternalEntry *internalEntry = (struct InternalEntry *)ptr;

    /*
    - copy the entries from the indexNum`th entry to *internalEntry
    - make sure that each field is copied individually as in the following code
    - the lChild and rChild fields of InternalEntry are of type int32_t
    - int32_t is a type of int that is guaranteed to be 4 bytes across every
      C++ implementation. sizeof(int32_t) = 4
    */

    /* the indexNum'th entry will begin at an offset of
       HEADER_SIZE + (indexNum * (sizeof(int) + ATTR_SIZE) )         [why?]
       from bufferPtr */
    unsigned char *entryPtr = bufferPtr + HEADER_SIZE + (indexNum * 20);

    memcpy(&(internalEntry->lChild), entryPtr, 4);
    memcpy(&(internalEntry->attrVal), entryPtr + 4, sizeof(Attribute));
    memcpy(&(internalEntry->rChild), entryPtr + 20, 4);

    return SUCCESS;
}
int IndLeaf::getEntry(void *ptr, int indexNum) {

    // if the indexNum is not in the valid range of [0, MAX_KEYS_LEAF-1]
    //     return E_OUTOFBOUND.
   if(indexNum<0 || indexNum>=MAX_KEYS_LEAF){
      return E_OUTOFBOUND;
    }

    unsigned char *bufferPtr;
    /* get the starting address of the buffer containing the block
       using loadBlockAndGetBufferPtr(&bufferPtr). */
  int ret=loadBlockAndGetBufferPtr(&bufferPtr);
    // if loadBlockAndGetBufferPtr(&bufferPtr) != SUCCESS
    //     return the value returned by the call.
    if(ret != SUCCESS){
      return ret;
    }
    // copy the indexNum'th Index entry in buffer to memory ptr using memcpy

    /* the indexNum'th entry will begin at an offset of
       HEADER_SIZE + (indexNum * LEAF_ENTRY_SIZE)  from bufferPtr */
    unsigned char *entryPtr = bufferPtr + HEADER_SIZE + (indexNum * LEAF_ENTRY_SIZE);
    memcpy((struct Index *)ptr, entryPtr, LEAF_ENTRY_SIZE);

    return SUCCESS;
}
int IndInternal::setEntry(void *ptr, int indexNum) {
  return 0;
}

int IndLeaf::setEntry(void *ptr, int indexNum) {
  return 0;
}