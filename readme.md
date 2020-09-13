## NAL Parser & Interpreter

A parser and interpreter for a text-based gaming language (Neill’s Adventure Language).

Test .nal files are included in the test-nal-files folder with escape211.nal being the most complex.

### Grammar
**The formal grammar for NAL is detailed below:**

  &lt;PROGRAM> := "{" lt;INSTRS>
   &lt;INSTRS> := "}" |  &lt;INSTRUCT>  &lt;INSTRS>
   &lt;INSTRUCT> :=  &lt;FILE> |  &lt;ABORT> |  &lt;INPUT> |  &lt;IFCOND> |  &lt;INC> |  &lt;SET> |
   &lt;JUMP> |  &lt;PRINT> |  &lt;RND>

**The individual commands are covered in more detail below.** </br>

Execute the instructions in file, then return here e.g. : FILE "test1.nal"

&nbsp;&nbsp;&nbsp;&lt;FILE> := "FILE" &lt;STRCON>
  
Halt/abort all execution right now!

&nbsp;&nbsp;&nbsp;&lt;ABORT> := "ABORT"
  
Fill a number−variable with a number, or 2 string−variables with string: IN2STR ( $C, $ZER ) or INNUM ( %NV )

&nbsp;&nbsp;&nbsp;&lt;INPUT> := "IN2STR" "("  &lt;STRVAR> ","  &lt;STRVAR> ")" | "INNUM" "("  &lt;NUMVAR> ")"
  
Jump to the nth word in this file (the first word is number zero!)
Brackets count as one word, "things in quotes" count as one word, e.g. : JUMP 5

&nbsp;&nbsp;&nbsp;&lt;JUMP> := "JUMP"  &lt;NUMCON>
  
Output the value of variable, or constant, to screen with (without a linefeed)

&nbsp;&nbsp;&nbsp;&lt;PRINT> := "PRINT"  &lt;VARCON>
&nbsp;&nbsp;&nbsp;&lt;PRINTN> := "PRINTN"  &lt;VARCON>
  
Set a variable to a random number in the range 0 − 99 e.g. : RND ( %N )
Number should be seeded via the clock to be different on successive executions

&nbsp;&nbsp;&nbsp;&lt;RND> := "RND" "("  &lt;NUMVAR> ")"
  
If condition/test is true, execute INSTRS after brace, else skip braces

&nbsp;&nbsp;&nbsp;&lt;IFCOND> :=  &lt;IFEQUAL> "{"  &lt;INSTRS> |  &lt;IFGREATER> "{"  &lt;INSTRS>
 
&nbsp;&nbsp;&nbsp;&lt;IFEQUAL> := "IFEQUAL" "("  &lt;VARCON> ","  &lt;VARCON> ")"
 
&nbsp;&nbsp;&nbsp;&lt;IFGREATER> := "IFGREATER" "("  &lt;VARCON> ","  &lt;VARCON> ")"
  
Add 1 to a number−variable e.g. : INC ( %ABC )

&nbsp;&nbsp;&nbsp;&lt;INC> := "INC" "("  &lt;NUMVAR> ")"
  
Set a variable. All variables are GLOBAL, and persist across the use of FILE etc. $A = "Hello" or %B = 17.6

&nbsp;&nbsp;&nbsp;&lt;SET> :=  &lt;VAR> "="  &lt;VARCON>
  
Some other helpful variable/constant rules:
(Here ROT18 is ROT13 for letters and rot5 for digits)

&nbsp;&nbsp;&nbsp;&lt;VARCON> :=  &lt;VAR> |  &lt;CON>
 
&nbsp;&nbsp;&nbsp;&lt;VAR> :=  &lt;STRVAR> |  &lt;NUMVAR>
 
&nbsp;&nbsp;&nbsp;&lt;CON> :=  &lt;STRCON> |  &lt;NUMCON>
 
&nbsp;&nbsp;&nbsp;&lt;STRVAR> := $[A−Z]+
 
&nbsp;&nbsp;&nbsp;&lt;NUMVAR> := %[A−Z]+
 
&nbsp;&nbsp;&nbsp;&lt;STRCON> := A plain−text string in double−quotes. <br/>
&nbsp;&nbsp;&nbsp; e.g. "HELLO.TXT", or a ROT18 string in hashes e.g. #URYYB.GKG#

&nbsp;&nbsp;&nbsp;&lt;NUMCON> := A number e.g. 14.301

### Parsers


### Interpreter
