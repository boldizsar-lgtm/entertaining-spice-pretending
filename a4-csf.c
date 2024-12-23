//-----------------------------------------------------------------------------
// Entertaining Spice Pretending
//
// Tutor: Michael Neuhold
//
// Author: 12417350
//-----------------------------------------------------------------------------
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAGIC_NUM_SIZE 4

#define COMMAND_LINE_PARAMETERS_ERROR 1
#define CONFIG_FILE_ERROR 2
#define MAGIC_NUM_ERROR 3
#define MEMORY_ERROR 4

typedef struct _Card_
{
  int value_;
  char* spice_;
  struct _Card_* next_;
} Card;


//-----------------------------------------------------------------------------
/// check if the correct number of command line parameters were entered
///
/// @param argc the number of command line parameters entered when calling
///             this program
///
/// @return true if there are not two command line parameters
/// @return flase if there are exactly two command line parameters
/// 
bool commandLineParametersError(int argc)
{
  if (argc != 2)
  {
    printf("Usage: ./a4-csf <config file>\n");
    return true;
  }
  return false;
}

//-----------------------------------------------------------------------------
/// check if the file that was entered through command line parameters
/// can be opened
///
/// @param file
/// @param filename the second command line paramenter
///
/// @return true
/// @return flase 
/// 
bool configFileError(FILE* file, char *filename)
{
  if (file == NULL)
  {
    printf("Error: Cannot open file: %s\n", filename);
    return true;
  }
  return false;
}

//-----------------------------------------------------------------------------
/// check if the file that was entered through command line parameters
/// has the correct magic number at the beginning of the file
///
/// @param file
/// @param filename the second command line paramenter
///
/// @return true
/// @return flase 
/// 
bool magicNumberError(FILE* file, char *filename)
{
  char magic_number[MAGIC_NUM_SIZE + 1];
  int i = 0;
  int ch;
  while ((ch = fgetc(file)) != EOF && i < MAGIC_NUM_SIZE)
  {
    magic_number[i] = ch;
    i++;
  }
  magic_number[i] = '\0';
  if (strcmp(magic_number, "ESP\n") != 0)
  {
    printf("Error: Invalid file: %s\n", filename);
    return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
/// check if the allocation of memory failed
///
/// @param ptr a pointer to allocated memory
///
/// @return true if memory allocation failed
/// @return flase if no error ocurred
/// 
bool AllocationError(void *ptr)
{
  if (ptr == NULL)
    {
      printf("[ERROR] Memory allocation failed!\n\n");
      return true;
    }
  return false;
}

//-----------------------------------------------------------------------------
/// The actual main function of the program!
/// 
/// @return int
/// 
int main(int argc, char *argv[])
{
  if (commandLineParametersError(argc))
    return COMMAND_LINE_PARAMETERS_ERROR;
  
  FILE *file = fopen(argv[1], "r");
  if (configFileError(file, argv[1]))
    return CONFIG_FILE_ERROR;
  
  if (magicNumberError(file, argv[1]))
  {
    fclose(file);
    return MAGIC_NUM_ERROR;
  }

  fclose(file);
  return 0;
}
