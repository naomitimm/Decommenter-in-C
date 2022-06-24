#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>


enum StateType {
    NORMAL, COMMENTED_STATE, STRING_STATE, CHAR_STATE
};

enum StateType handleNormalState (int userInput, int* lineCount, int* commentCount);
enum StateType handleCommentedState(int userInput, int* lineCount);
enum StateType handleStringState(int userInput);
enum StateType handleCharState(int userInput);
void takeAction (enum StateType state,  int userInput);


int main(int argc, char const *argv[])
{
    int userInput;
    int lineCount = 1;
    int commentCount = 0;
    // directly correlates to the number /*...*/ in input.
    enum StateType state = NORMAL;

    while ((userInput = getchar()) != EOF) {

        if (userInput == '\n') {lineCount += 1;}
        
        switch (state)
        {
        case NORMAL:
            state = handleNormalState(userInput, &lineCount, &commentCount);
            break;

        case COMMENTED_STATE:
            state = handleCommentedState(userInput, &lineCount);
            break;

        case STRING_STATE:
            state = handleStringState(userInput);

        case CHAR_STATE:
            state = handleStringState(userInput);

        default:
            break;
        }

    }
    if (state == COMMENTED_STATE)
    {
        fprintf(stderr, "Error: line %d: unterminated comment\n", commentCount);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
   
}

/**
 * @brief implemets the normal state of the DFA. 
 * reads a word input from stdin, write a word to stdout and change
 * to appropriate states as specified by the DFA
 * 
 * @param userInput 
 * @param lineCount 
 * @param commentCount 
 * @return enum StateType 
 */

enum StateType handleNormalState (int userInput, int* lineCount, int* commentCount) {
    enum StateType state = NORMAL;

    if (userInput == '/')
    {
        int nxtInput = getchar();
        if (nxtInput == '\n')
        {
            (*lineCount) += 1;
        }

        else if (nxtInput == '*')
        {
            *commentCount = *lineCount;
            state = COMMENTED_STATE;
        }
        
        else {
            putchar(userInput);
            userInput = nxtInput;
            state = NORMAL;
            // check this last line
        }   
    }

    else if (userInput == '"')
    {
        state = STRING_STATE;
    }

    else if (userInput == '\'')
    {
        state = CHAR_STATE;
    }

    
    takeAction(state, userInput);
    
    return state;   
}


enum StateType handleCommentedState(int userInput, int* lineCount) {
    enum StateType state = COMMENTED_STATE;
    if (userInput == '*')
    {
        int nxtInput = getchar();
        if (nxtInput == '/')
        {
            state = NORMAL;
        }
        else if (nxtInput == '\n')
        {
            (*lineCount) += 1;
            state = COMMENTED_STATE;
        }
    }

    else if (userInput == '\n')
    {
        putchar(userInput);
        state = COMMENTED_STATE;
    }

    return state;
}

enum StateType handleStringState(int userInput) {
    enum StateType state = STRING_STATE;
    putchar(userInput);

    if (userInput == '"')
    {
        state = NORMAL;
    }
    return state;
}

enum StateType handleCharState(int userInput) {
    enum StateType state = CHAR_STATE; 
    putchar(userInput);
    
    if (userInput == '\'')
    {
        state = NORMAL;
    }
    return state;
}

void takeAction (enum StateType state,  int userInput) {
    if (state != COMMENTED_STATE) {
        putchar(userInput);
    }
    if (state == COMMENTED_STATE) {
        putchar(' ');
    }
}