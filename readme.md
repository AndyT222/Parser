### NAL Parser & Interpreter

A parser and interpreter for a text-based gamging language (Neil's Action Language).

## Grammar
The formal grammar for NAL is detailed below:

  &lt;PROGRAM> := "{" lt;INSTRS>
   &lt;INSTRS> := "}" |  &lt;INSTRUCT>  &lt;INSTRS>
   &lt;INSTRUCT> :=  &lt;FILE> |  &lt;ABORT> |  &lt;INPUT> |  &lt;IFCOND> |  &lt;INC> |  &lt;SET> |
   &lt;JUMP> |  &lt;PRINT> |  &lt;RND>

  
Execute the instructions in file, then return here e.g. : FILE "test1.nal"

	&lt;FILE> := "FILE" &lt;STRCON>
  
Halt/abort all execution right now!

	&lt;ABORT> := "ABORT
  
% Fill a number−variable with a number, or 2 string−variables with string :
% IN2STR ( $C, $ZER ) or INNUM ( %NV )
 &lt;INPUT> := "IN2STR" "("  &lt;STRVAR> ","  &lt;STRVAR> ")" | "INNUM" "("  &lt;NUMVAR> ")"
  
% Jump to the nth word in this file (the first word is number zero!)
% Brackets count as one word, "things in quotes" count as one word, e.g. :
% JUMP 5
 &lt;JUMP> := "JUMP"  &lt;NUMCON>
  
% Output the value of variable, or constant, to screen with (without a linefeed)
 &lt;PRINT> := "PRINT"  &lt;VARCON>
 &lt;PRINTN> := "PRINTN"  &lt;VARCON>
  
% Set a variable to a random number in the range 0 − 99 e.g. :
% RND ( %N )
% Number should be seeded via the clock to be different on successive executions
 &lt;RND> := "RND" "("  &lt;NUMVAR> ")"
  
% If condition/test is true, execute INSTRS after brace, else skip braces
 &lt;IFCOND> :=  &lt;IFEQUAL> "{"  &lt;INSTRS> |  &lt;IFGREATER> "{"  &lt;INSTRS>
 &lt;IFEQUAL> := "IFEQUAL" "("  &lt;VARCON> ","  &lt;VARCON> ")"
 &lt;IFGREATER> := "IFGREATER" "("  &lt;VARCON> ","  &lt;VARCON> ")"
  
% Add 1 to a number−variable e.g. :
% INC ( %ABC )
 &lt;INC> := "INC" "("  &lt;NUMVAR> ")"
  
% Set a variable. All variables are GLOBAL, and persist across the use of FILE etc.
% $A = "Hello" or %B = 17.6
 &lt;SET> :=  &lt;VAR> "="  &lt;VARCON>
  
% Some helpful variable/constant rules
% (Here ROT18 is ROT13 for letters and rot5 for digits)
 &lt;VARCON> :=  &lt;VAR> |  &lt;CON>
 &lt;VAR> :=  &lt;STRVAR> |  &lt;NUMVAR>
 &lt;CON> :=  &lt;STRCON> |  &lt;NUMCON>
 &lt;STRVAR> := $[A−Z]+
 &lt;NUMVAR> := %[A−Z]+
 &lt;STRCON> := A plain−text string in double−quotes, e.g. "HELLO.TXT",
or a ROT18 string in hashes e.g. #URYYB.GKG#
 &lt;NUMCON> := A number e.g. 14.301
