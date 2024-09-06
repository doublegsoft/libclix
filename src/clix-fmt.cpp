/*
**    ,,    ,,  ,,                 ,,    ,,
**  `7MM    db *MM               `7MM    db
**    MM        MM                 MM
**    MM  `7MM  MM,dMMb.   ,p6"bo  MM  `7MM  `7M'   `MF'
**    MM    MM  MM    `Mb 6M'  OO  MM    MM    `VA ,V'
**    MM    MM  MM     M8 8M       MM    MM      XMX
**    MM    MM  MM.   ,M9 YM.    , MM    MM    ,V' VA.
**  .JMML..JMML.P^YbmdP'   YMbmd'.JMML..JMML..AM.   .MA.
*/
#include <stdlib.h>
#include <string.h>
#include "clix-fmt.hpp"

int static
clix_fmt_count_leading_spaces(const char* line)
{
  int ret = 0;
  long len = strlen(line);
  for (int i = 0; i < len; i++)
  {
    if (line[i] == ' ')
      ret++;
    else if (line[i] == '\t')
      ret += 2;
    else
      break;
  }
  return ret;
}

gfc_string_p
clix::fmt::indent(const char* content)
{
  gfc_string_p ret = gfc_string_new("");
  gfc_list_p lines = gfc_list_new();
  
  ulong len = strlen(content);
  char* cont = (char*)malloc(sizeof(char) * (len + 1));
  strcpy(cont, content);
  cont[len] = '\0';
  
  char* line = strtok(cont, "\n");
  
  int min_spaces = 10000;
  while (line != NULL)
  {
    int count = clix_fmt_count_leading_spaces(line);
    if (min_spaces > count)
      min_spaces = count;
    gfc_list_append(lines, (user_data)line);
    line = strtok(NULL, "\n");
  }
//  content = [content stringByReplacingOccurrencesOfString:@"\t" withString:@"  "];
//  //  content = [content stringByReplacingOccurrencesOfString:@"    " withString:@"  "];
//  NSArray *lines = [content componentsSeparatedByString:@"\n"];
//
//  NSInteger minSpaces = 1000;
//  for (NSString* line in lines)
//  {
//    NSInteger count = countLeadingSpaces(line);
//    if (minSpaces > count && count != 0) {
//      minSpaces = count;
//    }
//  }
//
//  NSString* ret = @"";
//  for (NSString* line in lines)
//  {
//    NSInteger count = countLeadingSpaces(line);
//    if (minSpaces > 2) {
//      count /= 2;
//    }
//    NSString* spaces = [@"" stringByPaddingToLength:count withString:@" " startingAtIndex:0];
//    NSString* trimmedString = [line stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
//    ret = [ret stringByAppendingString: [NSString stringWithFormat:@"${\"\"?left_pad(indent)}%@%@\n", spaces, trimmedString]];
//  }
  gfc_list_free(lines);
  free(cont);
  return ret;
}
