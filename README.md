# activedirectory_command_generator
Generates commands from a given CSV file to create users in Active Directory via the command __New-ADUser__. Users can map CSV headers to given parameters.
## Usage:
1. ### Compile source files

Run this command using __clang++__, __g++__ or any other C++ compiler, with the current directory set to __src__:
```
clang++ *.cpp -c
clang++ *.o -o my_program
```
This will generate an executable file named __my_program__, that you can run.
  
2. ### Create a mapfile

Create a file that holds __CSV header__ to __parameter__ mappings, or __commands__ regarding what values to put into specific parameters.
#### Mapping file formatting:

##### Basic mapping row
Two fields separated by a semicolon, first field is the __CSV header field__, second is the __parameter__ to map.

For example, mapping the field `First name` to the `-GivenName` parameter:
```
First name;-GivenName
```
You can not map multiple fields to the same parameter, however you can map the same field to multiple parameters.\
There can be unmapped fields and unmapped parameters.
##### Command row
First field contains a command and a value, the second field specifies the parameter to forward that value.

`$OVERRIDE$` overrides all mappings to a parameter with a given value.\
`$FALLBACK$` gives the given value to the specified parameter, for which each row has an empty value, or if there is no mapping for the parameter.

Example for giving a description for all generated users:
```
$OVERRIDE$User generated by script;-Description
```
Example for giving a fallback password for users that have empty fields for their passwords, or if the mapfile has no binding for `-AccountPassword`:
```
$FALLBACK$Passw0rd;-AccountPassword
```
It is an error to not give a value after a command.\
An `$OVERRIDE$` is stronger than a `$FALLBACK$`.

##### Comments
Commented-out rows are not processed. The character to start a commentline is `#`.

Example for a commented-out row:
```
# This row won't be processed
#username;-Name this won't either
```

3. ### Run the program

The program takes three mandatory command-line arguments: the input CSV file, a mapping file and an output file:
```
./my_program csvfile mapfile outfile
```
* #### csvfile

The first argument (after the program name) is the CSV file passed by filename. This file needs to have a valid header, containing header information for all subsequent rows.\
This file must exist, or the program will fail to run.

* #### mapfile

The second argument is the mapping file, which contains mappings from CSV header fields to parameters.\
This file must exist, or the program will fail to run.

* #### outfile

The third argument is the output file, which all the commands will be printed into.\
This file will be overwritten without warning if it exists, if it doesn't exist it will be created.

You must not mix the file order up.
###### Example program run:
```
./my_program names.csv mappings.txt output.txt
```

### Optional arguments:

#### `[-r|--randomized_passwords]`
Generates random passwords containing lowercase, uppercase and numeric characters of size 12 for each user, bypassing any mapping and overrides for `-AccountPassword`.

#### `[-o|--no-override]`
Ignores rows in the mapfile having `$OVERRIDE$` in their first field. Output in the outfile won't have any overridden values.

#### `[-f|--no-fallback]`
Ignores rows in the mapfile having `$FALLBACK$` in their first field. Output in the outfile won't have any values for empty fields or unmapped parameters.

#### example usage:
Generating commands with randomized passwords:
```
./my_program list.csv map.txt out.txt --randomized_passwords
```
Equivalent invocation:
```
./my_program list.csv map.txt out.txt -r
```
Mixing multiple arguments:
```
./my_program users.csv rules.txt out.txt -o -f -r
```

## Command and parameter formatting:
Parameters taking `String`s have their arguments inside two quotation marks:
```
New-ADUser -Name "John Doe"
```
Parameters taking `SecureString`s have their arguments placed into a `ConvertTo-SecureString` function:
```
New-ADUser -AccountPassword (ConvertTo-SecureString "Passw0rd" -AsPlainText -Force)
```
Parameters taking `Boolean`s have a check if the string value contains either `$true`, `$false`, `0` or `1`, if the check passes, numeric values are converted into the variable form, else it throws an error:
```
New-ADUser -Enabled $true
```
Parameters that take other types of arguments do no formatting and warn the user on the standard output with the affected typename. Other typenames are yet to be implemented:
```
warning: default formatting of arguments for parameters taking DateTime
```
