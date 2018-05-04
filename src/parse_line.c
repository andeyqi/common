#include <stdio.h>
#include <stdlib.h>

#define CFG_MAXARGS  (10)

int parse_line (char *line, char *argv[])
{
    int nargs = 0;

#ifdef DEBUG_PARSER
    printf ("parse_line: \"%s\"\n", line);
#endif
    while (nargs < CFG_MAXARGS) {

        /* skip any white space */
        while ((*line == ' ') || (*line == '\t')) {
            ++line;
        }

        if (*line == '\0') {    /* end of line, no more args    */
            argv[nargs] = NULL;
#ifdef DEBUG_PARSER
        printf ("parse_line: nargs=%d\n", nargs);
#endif
            return (nargs);
        }

        argv[nargs++] = line;   /* begin of argument string */

        /* find end of string */
        while (*line && (*line != ' ') && (*line != '\t')) {
            ++line;
        }

        if (*line == '\0') {    /* end of line, no more args    */
            argv[nargs] = NULL;
#ifdef DEBUG_PARSER
        printf ("parse_line: nargs=%d\n", nargs);
#endif
            return (nargs);
        }

        *line++ = '\0';     /* terminate current arg     */
    }

    printf ("** Too many args (max. %d) **\n", CFG_MAXARGS);

#ifdef DEBUG_PARSER
    printf ("parse_line: nargs=%d\n", nargs);
#endif
    return (nargs);
}

int main(int argc,char** argv)
{
	char* p_argv[CFG_MAXARGS] = {NULL};
	char data[100] = "ls -l -d -c -a -d -5";
	parse_line(data,p_argv);
	return 0;
} 