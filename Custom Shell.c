#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <stdbool.h>

#define MAX_LINE 80 
char alias_list[64][64];//Store Alias Codes
char alias_list2[64][64];//Store alias shortcuts

void alias(char *args[])
{
    int len;
    int m;
    int k;  
    int z=0;  
    for(len=0;len<=MAX_LINE;len++)
    {
        if(args[len]==NULL)
        {
            break;
        }
    }//Length of args

    if (!strcmp(args[0],"unalias"))//if input unalias
    {
        for(k=0;k<=64;k++)
        {
            if (strcmp(alias_list2[k],"") && strcmp(alias_list2[k],""))//Check if array is not empty
            {
                if (!strcmp(alias_list2[k],args[1]))//Checks which array the input is in
                {
                    strcpy(alias_list[k],"");//We reset the arrays
                    strcpy(alias_list2[k],"");
                    z=1;
                }

            }
        }
        if (z==0)
        {
            printf("Alias Can Not Found\n");
        }
        z=0;
    }
    else if (!strcmp(args[0],"alias"))//if input is alias 
    {  
        char create_string[MAX_LINE]=" ";//Create empty string
        for(m=1;m<len-1;m++)
        {
            strcat(create_string,args[m]);//Add args to string
            strcat(create_string," ");//Add space
        }
    if(create_string[strlen(create_string)-2]!='"' || create_string[1]!='"' )//If there are no quotation marks, it will give an error
    {
        printf("Wrong İnput\n");
    }
    else
    {
        bool used=0;
        for(k=0;k<=64;k++)
        {
            if (!strcmp(alias_list2[k],args[len-1]))//Check if this shortcut is already assigned
            {
                printf("Already Used Alias\n");
                used=1;
            }
            if (!strcmp(alias_list[k],"") && !strcmp(alias_list2[k],"") && used==0)//Insert alias into first empty array it can find
            {
                int i;
                for(i=2;i<=strlen(create_string)-3;i++)
                {
                    alias_list[k][i-2]=create_string[i];//We create the string char by char
                }
                strcpy(alias_list2[k],args[len-1]);/Adding the last arc to the shortcut array
                break;
            }



        }

    
    }
}

}

bool exists(const char fname[])
{
    return access(fname, F_OK | X_OK) != -1;
}

bool find_in_path(const char name[], char *fullpath, size_t sz) {
    char *paths = strdup(getenv("PATH"));
    char *tmp = paths; // to use in free
    const char *item;
    bool found = false;
    while ((item = strsep(&paths, ":")) != NULL) 
    {
        snprintf(fullpath, sz, "%s/%s", item, name);
        if (exists(fullpath))
        {
            found = true;
            break;
        }
    }
    free(tmp);
    return found;
}

void setup(char inputBuffer[], char *args[],int *background)
{
    int length, /* # of characters in the command line */
        i,      /* loop index for accessing inputBuffer array */
        start,  /* index where beginning of next command parameter is */
        ct;     /* index of where to place the next parameter into args[] */
    ct = 0;
        
    /* read what the user enters on the command line */
    length = read(STDIN_FILENO,inputBuffer,MAX_LINE);  

    /* 0 is the system predefined file descriptor for stdin (standard input),
       which is the user's screen in this case. inputBuffer by itself is the
       same as &inputBuffer[0], i.e. the starting address of where to store
       the command that is read, and length holds the number of characters
       read in. inputBuffer is not a null terminated C-string. */

    start = -1;
    if (length == 0){
        exit(0);            /* ^d was entered, end of user command stream */
    }

/* the signal interrupted the read system call */
/* if the process is in the read() system call, read returns -1
  However, if this occurs, errno is set to EINTR. We can check this  value
  and disregard the -1 value */
    if ( (length < 0) && (errno != EINTR) ) {
        perror("error reading the command");
	exit(-1);           /* terminate with error code of -1 */
    }

	//printf(">>%s<<",inputBuffer);
    for (i=0;i<length;i++){ /* examine every character in the inputBuffer */

        switch (inputBuffer[i]){
	    case ' ':
	    case '\t' :               /* argument separators */
		if(start != -1){
                    args[ct] = &inputBuffer[start];    /* set up pointer */
		    ct++;
		}
                inputBuffer[i] = '\0'; /* add a null char; make a C string */
		start = -1;
		break;

            case '\n':                 /* should be the final char examined */
		if (start != -1){
                    args[ct] = &inputBuffer[start];     
		    ct++;
		}
                inputBuffer[i] = '\0';
                args[ct] = NULL; /* no more arguments to this command */
		break;

	    default :             /* some other character */
		if (start == -1)
		    start = i;
                if (inputBuffer[i] == '&'){
		            *background  = 1;
                    inputBuffer[i-1] = '\0';
		}
	} /* end of switch */
     }    /* end of for */
     args[ct] = NULL; /* just in case the input line was > 80 */

	//for (i = 0; i <= ct; i++)
		//printf("args %d = %s\n",i,args[i]);
} /* end of setup routine */
 









int main(void)
{
    char inputBuffer[MAX_LINE]; /*buffer to hold command entered */
    int background; /* equals 1 if a command is followed by '&' */
    char *args[MAX_LINE/2 + 1]; /*command line arguments */
    while (1)
    {
        background = 0;
        printf("myshell: ");
        fflush(stdout);
        setup(inputBuffer, args, &background); 
        int len;
        for(len = 0; len< sizeof(args);len++ )
        {
          if(args[len] == NULL)
          {
            break;
          }
        }
        char fullpath[512];
        bool found = find_in_path(args[0], fullpath, sizeof(fullpath));
        if(!strcmp(args[0],"alias") && strcmp(args[1],"-l"))// If the input alias is equal and not wanted to be listed
        {
            if(len<=2)//If the arguments are insufficient to enter the Alias
            {
                printf("Too Few Arguments\n");
            }
            else
            {
                alias(args);
            }
        }
        else if(!strcmp(args[0],"alias") &&!strcmp(args[1],"-l"))// If we want to list aliases
        {
            int k,y=0;
            for(k=0;k<=64;k++)
            {
                if ((strcmp(alias_list[k],""))&&(strcmp(alias_list2[k],"")))//Show filled arrays in alias arrays
                {
                    printf("\t %s %c%s%c \n",alias_list2[k],'"',alias_list[k],'"');
                    y++;
                }

            }
            if(y==0)//If there are no Alias to display, give an error
            {
                printf("No Alias Founded\n");
            }
            
        }
        else if(!strcmp(args[0],"unalias"))//If unalias is requested as input
        {
            if(len<=1)//Give an error if only one argument is given
            {
                printf("Too Few Arguments\n");
            }
            else
            {
                alias(args);
            }
        }
        else if(background==1)//If there is a background input, enter it
        {
            
            if (!strcmp(args[len-1],"&"))
            {
                args[len-1]=NULL;//Remove ampersand from arguments
            }

        }

        if (found && strcmp(args[0],"alias") &&strcmp(args[0],"unalias")  )//returns 1 if the requested command is found in the path
        {
            pid_t childpid;
            childpid = fork();//Creating Fork
            if (childpid == -1) 
            {
                perror("Failed to fork");
                return 1; 
            }

            if (childpid == 0)//Child Node
            { 
                execv(fullpath,&args[0]);
                perror("Child failed");
                return 1; 
            }
            int a,b;
            if (background==0)
            {
                while ((a = wait(&b)) != -1 && a != childpid)
                {/* parent code */
                    perror("Parent failed to wait due to signal or error");
                    EXIT_FAILURE;
                    return 1;
                }

            }
            
            
        }
        if(found==0)
        {
            int k,y=0;
            for(k=0;k<=64;k++)
            {
                if (!strcmp(alias_list2[k],args[0]))//We check if the desired shortcut inputs exist in the alias.
                {
                    y++;
                    system(alias_list[k]);//the code corresponding to the shortcuts is executed with the system() function
                }
            }
            if(y==0)
            {
                printf("No Command Founded\n");//If there is no such command in Alias, it gives an error.
            }


        }
    }
}

