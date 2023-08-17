#include "Buffer/StaticBuffer.h"
#include "Cache/OpenRelTable.h"
#include "Disk_Class/Disk.h"
#include "FrontendInterface/FrontendInterface.h"
#include <iostream>	
//  cd DBMS/NITCbase/mynitcbase
int main(int argc, char *argv[]) {
  /* Initialize the Run Copy of Disk */
  // StaticBuffer buffer;
  // OpenRelTable cache;
  
  Disk disk_run;
// Stage 1
// vs code
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
//Stage 1 Exercise
    unsigned char buffer[BLOCK_SIZE];
    Disk::readBlock(buffer,0);
    for (int i = 0; i < 6; i++)
	{
		std::cout << (int)buffer[i] << " ";
  }
  
  //return FrontendInterface::handleFrontend(argc, argv);
  return 0;
}
