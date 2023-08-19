#include "Buffer/StaticBuffer.h"
#include "Cache/OpenRelTable.h"
#include "Disk_Class/Disk.h"
#include "FrontendInterface/FrontendInterface.h"
#include <iostream>
int main(int argc, char *argv[]) {
  //stage 2
  Disk disk_run;

<<<<<<< Updated upstream
  // create objects for the relation catalog and attribute catalog
  RecBuffer relCatBuffer(RELCAT_BLOCK);
  RecBuffer attrCatBuffer(ATTRCAT_BLOCK);

  HeadInfo relCatHeader;
  HeadInfo attrCatHeader;

  // load the headers of both the blocks into relCatHeader and attrCatHeader.
  // (we will implement these functions later)
  relCatBuffer.getHeader(&relCatHeader);
  attrCatBuffer.getHeader(&attrCatHeader);
  // std::cout <<relCatHeader.numEntries <<" ";
  for (int i=0,k=0;i<relCatHeader.numEntries;i++) {

    Attribute relCatRecord[RELCAT_NO_ATTRS]; // will store the record from the relation catalog

    relCatBuffer.getRecord(relCatRecord, i);

    printf("Relation: %s\n", relCatRecord[RELCAT_REL_NAME_INDEX].sVal);
    int j=0;
    // std::cout <<relCatRecord[RELCAT_NO_ATTRIBUTES_INDEX].nVal <<" ";
    for (;j<relCatRecord[RELCAT_NO_ATTRIBUTES_INDEX].nVal;j++,k++) {

      // declare attrCatRecord and load the attribute catalog entry into it
      Attribute attrCatRecord[ATTRCAT_NO_ATTRS];

      attrCatBuffer.getRecord(attrCatRecord,k);

    // printf("%d\n",(int)relCatRecord[RELCAT_NO_ATTRIBUTES_INDEX].nVal);
      if (!strcmp(attrCatRecord[ATTRCAT_REL_NAME_INDEX].sVal,relCatRecord[RELCAT_REL_NAME_INDEX].sVal)) {
        const char *attrType = attrCatRecord[ATTRCAT_ATTR_TYPE_INDEX].nVal == NUMBER ? "NUM" : "STR";
        printf("  %s: %s\n", attrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal, attrType);
      }
      if(k == attrCatHeader.numSlots-1)
      {
        k=-1;
        attrCatBuffer=RecBuffer(attrCatHeader.rblock);
        attrCatBuffer.getHeader(&attrCatHeader);
        hello;
        hi;
      }
    }
    printf("\n");
=======
// Stage 1
// via vs code
  // unsigned char buffer[BLOCK_SIZE];
  // Disk::readBlock(buffer,7000);
  // char message[]="hello";
  // memcpy(buffer+20,message,6);
  // Disk::writeBlock(buffer,7000);
  
  // unsigned char buffer2[BLOCK_SIZE];
  // char message2[6];
  // Disk::readBlock(buffer2,7000);
  // memcpy(message2,buffer2+20,6);
  // std::cout << message2;
    unsigned char buffer[BLOCK_SIZE];
    Disk::readBlock(buffer,0);
    for (int i = 0; i < 6; i++)
	{
		std::cout << (int)buffer[i] << " ";
>>>>>>> Stashed changes
  }

  return 0;
}