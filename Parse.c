/*
 * Parse.c
 *
 *  Created on: Sep 13, 2013
 *      Author: David Hunt & Thenmozhi Elayaperumal
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXARGS 32
/*structure to hold input data */
typedef struct PARAM
{
    char *inputRedirect;   //file name or NULL
    char *outputRedirect;  //file name or NULL
    int background;        //either 0 (false) or 1 (true)
    int argumentCount;     //same as argc in main()
    char *argumentVector[MAXARGS]; //array of strings
} Param_t;

//Functions - functionality of the functions are described on top of each function where they are defined
//printPrompt - prints a prompt to indicate input from the user
void printPrompt();

//firstChar - evaluates the first character of a word in order to categorize the arguments
int firstChar(char *, Param_t *);

//parseString - takes a sentence of arguments and breaks them up into manageable words
int parseString(char *, Param_t *);

//commandLine - like the main function, it controls the time of the program and calls the 
//the functions accordingly. The return values from the other functions lets the commandLine
//know if the program is to continue or exit.
void commandLine();

//printParams - prints out the struct PARAM
void printParams(Param_t * );

int main(int argc, char** argv)
{
    commandLine();
    return (EXIT_SUCCESS);
}

/*
firstChar - Parameters char pointer, and pointer to struct Param_t
checks the first character in the word passed to it, then passes the word to
the appropriate action or function
returns 1 if the word is exit
returns 3 if there is an error
returns 0 to continue
*/
int firstChar(char * ch, Param_t * pm)
{
    int cont = 0;
    if(pm->background == 1)
    {
            if(ch[0] != '\0')
            {
            	fprintf(stderr, "There is an ampersand before the end\n");
            	return 3;
            }
    }


    if(ch[0] == '<')
    {
    	if((ch[1] == '\n') || (ch[1] == '\0'))
    	{
    		fprintf(stderr, "There is no file specified for the input redirect\n");
    		cont = 3;
    	}
        else
        {
         	if(pm->inputRedirect == NULL)
                pm->inputRedirect = &ch[1];
        	else
        	{
                fprintf(stderr, "There is too many input redirects\n");
                cont = 3;
        	}
        }
    }
    else if(ch[0] == '>')
    {
    	if((ch[1] == '\n') || (ch[1] == '\0'))
    	{
    		fprintf(stderr, "There is no file specified for the output redirect\n");
    		cont = 3;
    	}
        else
        {
           if(pm->outputRedirect == NULL)
                pm->outputRedirect = &ch[1];
        	else
        	{
                fprintf(stderr, "There is too many output redirects\n");
                cont = 3;
        	}
        }
    }
    else if(ch[0] == '&')
            {
                if(pm->background == 0)
                        pm->background = 1;
                else
                {
                        fprintf(stderr, "Error, too many ampersands\n");
                        cont = 3;
                }
        }
    else if((strcmp(ch, "exit\n") == 0) || (strcmp(ch, "exit") == 0))
    {
         fprintf(stdout, "Exiting...\n Exit Successful!\n");
         cont = 1;
    }
    else
    {
    	if(pm->argumentCount < 32)
    	{
         pm->argumentVector[pm->argumentCount] = ch;
         pm->argumentCount = pm->argumentCount + 1;
         }
         else
         {
         	fprintf(stderr, "Too many arguments have been passed, exiting\n");
         	return 4;
         }
    }


    return cont;
}


/*
parseString - Parameters char pointer, and pointer to struct Param_t
Takes a string of words and breaks it up into individual words that will later
be passed to the firstChar function
returns 1 to end
returns 0 to continue
*/
int parseString(char * ch, Param_t * pm)
{
   char * wd;
   int cont = 0;

   wd = strtok(ch, " ,.-\n");
   while((wd != NULL) && (cont == 0))
   {
      
      cont = firstChar(wd, pm);
      wd = strtok(NULL, " ,.-\n");
   }
   
   
    return cont;
}


/*
printParams - Parameters char pointer, and pointer to struct Param_t
Prints the struct Param_t to the screen
*/

/** Prints the '$' to the screen**/
void printPrompt()
{
     printf( "$$$ ");
}

/*
commandLine - Parameters char pointer, and pointer to struct Param_t
Fetches a string of characters in from the command line to pass on to the
parseString function
returns 1 to end
returns 0 to continue
returns 3 if error and end
returns 4 if error and end
*/
/**Reads the string and disperses to a struct. Returns 1 to exit and 0 to continue**/
void commandLine()
{
    int i=0;
    char str[100];
    while (i ==0)
    {
    	Param_t parse_S;
    	parse_S.background = 0;
    	parse_S.argumentCount = 0;
    	parse_S.inputRedirect= NULL;
    	parse_S.outputRedirect = NULL;
    	Param_t *parseS = &parse_S;
    	printPrompt();
    	
    	fgets(str, 100, stdin); //gets a string from standard input
    	i=parseString(str, parseS);
    	if(i==0)
    		printParams(parseS);
	}

}


/** prints the content of the struct distinguishing the input redirect and output redirect for the user.
Background and argument count is also done. Also, it lists all the arguments in the order they appear**/
void printParams(Param_t* param)
{
     int i;
     printf("InputRedirect: [%s]\n",
     (param->inputRedirect != NULL) ? param->inputRedirect:"NULL");
     printf("OutputRedirect:[%s]\n",
     (param->outputRedirect != NULL) ? param->outputRedirect:"NULL");
     printf("Background:[%d]\n",param->background);
     printf("ArgumentCount:[%d]\n",param->argumentCount);
     for(i=0;i < param->argumentCount;i++)
          printf("ArgumentVector[%2d]: [%s]\n",i,param->argumentVector[i]);


     }
