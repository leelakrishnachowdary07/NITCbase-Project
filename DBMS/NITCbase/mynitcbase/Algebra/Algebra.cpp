#include "Algebra.h"
#include<iostream>
#include <cstring>

/* used to select all the records that satisfy a condition.
the arguments of the function are
- srcRel - the source relation we want to select from
- targetRel - the relation we want to select into. (ignore for now)
- attr - the attribute that the condition is checking
- op - the operator of the condition
- strVal - the value that we want to compare against (represented as a string)
*/
// will return if a string can be parsed as a floating point number
bool isNumber(char *str) {
  int len;
  float ignore;
  /*
    sscanf returns the number of elements read, so if there is no float matching
    the first %f, ret will be 0, else it'll be 1

    %n gets the number of characters read. this scanf sequence will read the
    first float ignoring all the whitespace before and after. and the number of
    characters read that far will be stored in len. if len == strlen(str), then
    the string only contains a float with/without whitespace. else, there's other
    characters.
  */
  int ret = std::sscanf(str, "%f %n", &ignore, &len);
  return ret == 1 && len == strlen(str);
}
int Algebra::select(char srcRel[ATTR_SIZE], char targetRel[ATTR_SIZE], char attr[ATTR_SIZE], int op, char strVal[ATTR_SIZE]) {
  int srcRelId = OpenRelTable::getRelId(srcRel);      // we'll implement this later
  if (srcRelId == E_RELNOTOPEN) {
    return E_RELNOTOPEN;
  }

  AttrCatEntry attrCatEntry;
  // get the attribute catalog entry for attr, using AttrCacheTable::getAttrcatEntry()
  //    return E_ATTRNOTEXIST if it returns the error
  int sol=AttrCacheTable::getAttrCatEntry(srcRelId,attr,&attrCatEntry);
  if(sol==E_ATTRNOTEXIST){
    return E_ATTRNOTEXIST;
  }


  /*** Convert strVal (string) to an attribute of data type NUMBER or STRING ***/
  int type = attrCatEntry.attrType;
  Attribute attrVal;
  if (type == NUMBER) {
    if (isNumber(strVal)) {       // the isNumber() function is implemented below
      attrVal.nVal = atof(strVal);
    } else {
      return E_ATTRTYPEMISMATCH;
    }
  } else if (type == STRING) {
    strcpy(attrVal.sVal, strVal);
  }

  /*** Selecting records from the source relation ***/

  // Before calling the search function, reset the search to start from the first hit
  // using RelCacheTable::resetSearchIndex()
  RelCacheTable::resetSearchIndex(srcRelId);

  RelCatEntry relCatEntry;
  // get relCatEntry using RelCacheTable::getRelCatEntry()
  RelCacheTable::getRelCatEntry(srcRelId,&relCatEntry);

  /************************
  The following code prints the contents of a relation directly to the output
  console. Direct console output is not permitted by the actual the NITCbase
  specification and the output can only be inserted into a new relation. We will
  be modifying it in the later stages to match the specification.
  ************************/

  printf("|");
  for (int i = 0; i < relCatEntry.numAttrs; ++i) {
    AttrCatEntry attrCatEntry;
    // get attrCatEntry at offset i using AttrCacheTable::getAttrCatEntry()
    AttrCacheTable::getAttrCatEntry(srcRelId,i,&attrCatEntry);

    printf(" %s |", attrCatEntry.attrName);
  }
  printf("\n");

  while (true) {
    RecId searchRes = BlockAccess::linearSearch(srcRelId, attr, attrVal, op);

    if (searchRes.block != -1 && searchRes.slot != -1) {

      // get the record at searchRes using BlockBuffer.getRecord
      RecBuffer blockbuffer(searchRes.block);

      HeadInfo blockhead;
      blockbuffer.getHeader(&blockhead);

      Attribute recbuffer[blockhead.numAttrs];
      blockbuffer.getRecord(recbuffer,searchRes.slot);

      // print the attribute values in the same format as above
      printf("|");
        for (int i = 0; i < relCatEntry.numAttrs; ++i) {
            AttrCatEntry attrCatEntry;
            // get attrCatEntry at offset i using AttrCacheTable::getAttrCatEntry()
            AttrCacheTable::getAttrCatEntry(srcRelId,i,&attrCatEntry);
            if (attrCatEntry.attrType == NUMBER) printf(" %d |", (int)recbuffer[i].nVal);
                else printf(" %s |", recbuffer[i].sVal);
        }
        printf("\n");

    } else {

      // (all records over)
      break;
    }
  }

  return SUCCESS;
}
int Algebra::insert(char relName[ATTR_SIZE], int nAttrs, char record[][ATTR_SIZE]){
    // if relName is equal to "RELATIONCAT" or "ATTRIBUTECAT"
    // return E_NOTPERMITTED;
    if(!strcmp(relName,"RELATIONCAT") || !strcmp(relName,"ATTRIBUTECAT")){
      return E_NOTPERMITTED;
    }
    // get the relation's rel-id using OpenRelTable::getRelId() method
    int relId = OpenRelTable::getRelId(relName);

    // if relation is not open in open relation table, return E_RELNOTOPEN
    // (check if the value returned from getRelId function call = E_RELNOTOPEN)
    if(relId==E_RELNOTOPEN){
      return E_RELNOTOPEN;
    }
    // get the relation catalog entry from relation cache
    // (use RelCacheTable::getRelCatEntry() of Cache Layer)
    RelCatEntry relcatbuff;
    RelCacheTable::getRelCatEntry(relId,&relcatbuff);

    /* if relCatEntry.numAttrs != numberOfAttributes in relation,
       return E_NATTRMISMATCH */
    if(relcatbuff.numAttrs != nAttrs){
      return E_NATTRMISMATCH;
    }
    // let recordValues[numberOfAttributes] be an array of type union Attribute
    Attribute recordValues[nAttrs];
    /*
        Converting 2D char array of record values to Attribute array recordValues
     */
    // iterate through 0 to nAttrs-1: (let i be the iterator)
    for(int i=0;i<nAttrs;i++)
    {
        // get the attr-cat entry for the i'th attribute from the attr-cache
        // (use AttrCacheTable::getAttrCatEntry())
        AttrCatEntry attrcatbuff;
        AttrCacheTable::getAttrCatEntry(relId,i,&attrcatbuff);

        // let type = attrCatEntry.attrType;
        int type=attrcatbuff.attrType;

        if (type == NUMBER)
        {
            // if the char array record[i] can be converted to a number
            // (check this using isNumber() function)
            if(isNumber(record[i]))
            {
                /* convert the char array to numeral and store it
                   at recordValues[i].nVal using atof() */
                recordValues[i].nVal=atof(record[i]);
                
            }
            else
            {
                return E_ATTRTYPEMISMATCH;
            }
        }
        else if (type == STRING)
        {
            // copy record[i] to recordValues[i].sVal
            strcpy(recordValues[i].sVal,record[i]);
        }
    }

    // insert the record by calling BlockAccess::insert() function
    int retVal=BlockAccess::insert(relId,recordValues);
    // let retVal denote the return value of insert call

    return retVal;
}