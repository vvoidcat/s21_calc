# s21_calc

**s21_calc** is a **School 21** educational project - a **C programming language** console application implementing an extended version of the simple **calculator**. It also contains an implementation of a **stack** data structure used to compute the inserted expressions in the **reverse Polish notation**.

The application is able to calculate arbitrary bracketed arithmetic expressions with integers and decimal numbers and plot a simple graph (-10 to 10 on X axis, -10 to 10 on Y axis, size of step 1) of a function if a variable *x* is inserted into the equation or if the program is run with the ```--graph``` option.

The user input is restricted to 255 characters (all exceeding characters will be force-cut). The calculation is performed after the ```[=]``` or ```[enter]``` keys are pressed and the input string is verified as correct.


## Supported arithmetic operations and functions

#### Operators:
| OPERATOR | USAGE |
| --------- | --------- |
| brackets | (a) |
| addition | a + b |
| subtraction | a - b |
| multiplication | a * b |
| division | a / b |
| power | a ^ b |
| modulus | a % b |
| unary plus | +a |
| unary minus | -a |

#### Functions:
| FUNCTION | USAGE |
| ------ | ------ |
| cosine | cos(x) |
| sine | sin(x) |
| tangent | tan(x) |
| arc cosine | acos(x) |
| arc sine | asin(x) |
| arc tangent | atan(x) |
| square root | sqrt(x) |
| natural logarithm | ln(x) |
| common logarithm | log(x) |

## Note
- the program was tested on **Mac OS** (Big Sur) and **Linux** (Ubuntu 20.04.4 LTS (Focal Fossa) and Ubuntu 22.04.1 LTS (Jammy Jellyfish), both Desktop and Server images);
- unit-tests made with the Check library cover **more than 90%** of each module related to string parsing and calculation (**core**, **math** and **graphs** modules);
- unit-tests compare the calculated equation results to the results provided by the Google online calculator;
- memory tests with *valgrind* show **no leaks**

To compile the program, run ```make``` in the terminal. On Linux OS, all the required compilation dependencies (*make*, *gcc*, etc.) can be installed with the ```./linux_package_installer.sh``` script located in the **utils/** folder of the project.

The main executable file will be placed in **build/s21_calc-build-release** directory after successful compilation.
