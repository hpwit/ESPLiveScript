# DONE :
## pointers
               * Local functions:
                              * test pointer loval variable (Done)

## write functions directly  in assembly (Done)
solution:
               add and option in Function isAssembly
               si just assembly then display only the string         

## Execution:
### second core
*modify the driver to handle the stop add a flag __isDriverEnable  in the DriverinsterruptDriver and test if it solves the issues
void kill()
{
               driver.__isDriverEnable=false;
               while(driver.isDisplaying){}
               driver.__isDriverEnable=true;
               destroy the task
}
 
* add run to the parser command
* add ctrl-R                                    

# To be done:
* pointers
               * Local functions:
                              
                              * create pointer global variable 
                              * create pointer call ext variable
 
               * external functions:
                              * create pointer loval variable
                              * create  pointer global variable
                              * create pointer call ext variable
                             
               * quid du déférencement *df=23
 
* float
               * Implementation float and calculation
 
* return management
              
* objects with more than 4 bytes long
               * seee how to do it when longer than 3 bytes
              
*create structures
  add "xxx" to variable types
  remove the Dot possible in the identifier
  add the "dot" token
 
* modify the tokenizer to adapt
               variables struct types to vectors
               have a variable lis a appart
              
 

 
* constant in arrays
              
* import functions
               * asm
               * c
               * basic library ?
              
* assembly optimization
               * copyingstack in a couple of move instead of the one by one.
              
* test boolean expressions ...
 
* format testing for call functions
 
* string management (beurk)
 
 
EXECUTION:
faire que si une modification a eu lieu il faille recompiler
 
 editor:
* Highlighting
               * block Comment
               * curlybraket
               * existing functions