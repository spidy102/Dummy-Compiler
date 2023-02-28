# Lexer

The lexer is the first part of the front-end of the compiler. It converts a string of characters into a string of tokens.

## Lexer implementation

The lexer that we have have implemented is a representation of the DFA of the tokens. It takes in the ERPLAG file as input and ouputs the tokens. The tokens are returned one at a time. The getnexttoken() function from the parser returns the subsequent token. 
It is returned in the form of a data structure that contains the token, the lexeme and the line number. 

These tokens are then further passed in to the parser for parse tree construction

to run the lexer file separately - 
create an ERPLAG file with the token to tokenize and run - gcc lexer.c ./data_structures/hashmap.c ./data_structures/twinbuffer.c

## removeComments
The removeComments function is to ignore all the comment part of the file

## make_token
The make_token function is to copy the TOKEN, LEXEME and line number to the data structure for TOKEN and return the data structure

## getSize
The getSize function is to get the size of the twin buffer passed through the DFA

## copyLexeme
The copyLexeme function is to copy the string of the token/ the lexeme

## error
The error function displays the line number of the error

## getNextToken
The getNextToken function implements all the states of the DFA and its transitions and finally returns the TOKEN data structure that coresponds to the file


