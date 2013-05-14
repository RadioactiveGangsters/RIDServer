/* 
 * File:   toFile.c
 * Author: Bas
 *
 */

#include "toFile.h"

int toFile(char const*const path, char const*const data)
{
    if(path == NULL)
    {
        return 0;
    }
    else
    {
        FILE *printFile;
    
        if((printFile = fopen(path,"a")) == NULL )
        {
            return 0;
        }
        fprintf(printFile, "%s\n", data);

        fclose(printFile);
        return 1;
    }
}
