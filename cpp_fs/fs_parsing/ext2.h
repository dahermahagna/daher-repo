#ifndef __EXT2_H__
#define __EXT2_H__

#include <stdio.h> /* FILE */

/*
DESCRIPTION:
    findes a file on the device, and prints:
        1. The superblock
        2. group descriptor
        3. File content
Param:
    dev - device name
    file path - path to file
return:
     NULL - on failure.
	 pointer to file handle on success.
*/


FILE *Find(const char *dev, const char *file_path);


#endif /*__EXT2_H__*/

