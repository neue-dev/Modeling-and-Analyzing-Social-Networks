/**
 * @ Author: Mo David
 * @ Create Time: 2024-07-19 12:04:06
 * @ Modified time: 2024-07-19 12:24:15
 * @ Description:
 * 
 * A record represents a single entry within our model.
 */

#ifndef RECORD_C
#define RECORD_C

typedef struct Record Record;

struct Record {
  char id[32];    // The record id
  char name[32];  // The name of the record
};

/**
 * Allocates space for a single record entry.
 * 
 * @return  { Record * }  A pointer to the location of the memory.
*/
Record *Record_alloc() {
  Record *pRecord = calloc(1, sizeof(*pRecord));

  return pRecord;
}

/**
 * Initializes a record instance.
 * 
 * @param   { Record * }  this  The record to initialize.
 * @param   { char * }    id    The id of the record.
 * @param   { char * }    name  The name of the record. 
 * @return  { Record * }        The initialized record struct.
*/
Record *Record_init(Record *this, char *id, char *name) {
  
  // Copy the name and id of the record
  strcpy(this->id, id);
  strcpy(this->name, name);

  // Return the initialized record
  return this;
}

/**
 * Creates a new initialized record with the given id and name.
 * 
 * @param   { char * }    id    The id of the record.
 * @param   { char * }    name  The name of the record. 
 * @return  { Record * }        The initialized record struct.
*/
Record *Record_new(char *id, char *name) {
  return Record_init(Record_alloc(), id, name);
}

#endif RECORD_C

