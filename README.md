# activedirectory_command_generator
Generates commands from a given CSV file to create users in Active Directory via the command __New-ADUser__. Users can map CSV headers to given parameters.
## Usage:
1. ### Compile source files

Run this command using __clang++__, __g++__ or any other C++ compiler, with the current directory set to __src__:
```
clang++ *.cpp -c
clang++ *.o
```
This will generate an executable file named <b>a.out</b>, that you can run.
  
2. ### Create a mapping file

Create a file that holds __CSV header__ to __parameter__ mappings.
#### Mapping file formatting:
Two fields separated by a semicolon, first field is the CSV header field, second is the parameter to map.

For example, mapping the field __First name__ to the __-GivenName__ parameter:
```
First name;-GivenName
```
You can not map multiple fields to the same parameter, however you can map the same field to multiple parameters.

There can be unmapped fields and unmapped parameters.

3. ### Run the program

The program takes three mandatory command-line arguments: the input CSV file, a mapping file and an output file:
```
./a.out csvfile mapfile outfile
```
You must not mix the file order up
* #### csvfile

The first argument (after the program name) is the CSV file passed by filename. This file needs to have a valid header, containing header information for all subsequent rows.

This file must exist, or the program will fail to run.

* #### mapfile

The second argument is the mapping file, which contains mappings from CSV header fields to parameters.

This file must exist, or the program will fail to run.

* #### outfile

The third argument is the output file, which all the commands will be printed into.

This file will be overwritten without warning if it exists, if it doesn't exist it will be created.

###### Example program run:
```
./a.out names.csv mappings.txt output.txt
```

#### Optional parameters:
 __[-r | --randomized_passwords]__:
<br>generates random passwords containing lowercase, uppercase and numeric characters, of size 12 for each user, bypassing any mapping for __-AccountPassword__.




## Command and parameter formatting:
Parameters taking __String__'s have their arguments inside two quotation marks:
```
New-ADUser -Name "John Doe"
```
Parameters taking __SecureString__'s have their arguments placed into a __ConvertTo-SecureString__ function:
```
New-ADUser -AccountPassword (ConvertTo-SecureString "Passw0rd" -AsPlainText -Force)
```
Parameters taking __Boolean__'s have a check if the string value contains either __$true__, __$false__, __0__ or __1__, if the check passes, numeric values are converted into the variable form, else it throws an error:
```
New-ADUser -Enabled $true
```
Parameters that take other types of arguments do no formatting and warn the user on the standard output with the affected typename. Other typenames are yet to be implemented:
```
warning: default formatting of parameter taking -AccountExpirationDate
```
