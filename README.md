# activedirectory_command_generator
Generates commands from a given CSV file to create users in Active Directory via the command <b>New-ADUser</b>. Users can map CSV headers to given parameters.
## Usage:
<ol>
  <li><h3>Compile source files</h3></li>
  <p>Run this command using <b>clang++</b>, <b>g++</b> or any other C++ compiler, with the current directory set to <b>src</b>:</p>
  <pre>clang++ *.cpp -c<br>clang++ *.o</pre>
  <p>This will generate an executable file named <b>a.out</b>, that you can run.</p>
  
  <li><h3>Create a mapping file</h3></li>
  <p>Create a file that holds <b>CSV header</b> to <b>parameter</b> mappings.</p>
  <h4>Mapping file formatting:</h4>
  <p>Two fields separated by a semicolon, first field is the CSV header field, second is the parameter to map.</p>
  <p>For example, mapping the field 'First name' to the '-GivenName' parameter:</p>
  <pre>First name;-GivenName</pre>
  <p>You can not map multiple fields to the same parameter, however you can map the same field to multiple parameters.</p>
  <p>There can be unmapped fields and unmapped parameters.</p>
  <li><h3>Run the program</h3></li>
  <p>The program takes three command-line arguments: the input CSV file, a mapping file and an output file:</p>
  <pre>./a.out csvfile mapfile outfile</pre>
  <p>You must not mix the file order up.</p>
  <ol>
    <li><h4>csvfile</h4></li>
    <p>The first argument (after the program name) is the CSV file passed by filename. This file needs to have a valid header, containing header information for all subsequent rows.</p>
    <p>This file must exist, or the program will fail to run.</p>
    <li><h4>mapfile</h4></li>
    <p>The second argument is the mapping file, which contains mappings from CSV header fields to parameters.</p>
    <p>This file must exist, or the program will fail to run.</p>
    <li><h4>outfile</h4></li>
    <p>The third argument is the output file, where all the commands will be printed into.</p>
    <p>This file will be overwritten without warning if it exists, if it doesn't exist it will be created.</p>
  </ol>
  <p>Example program run:</p>
  <pre>./a.out names.csv mappings.txt output.txt</pre>
</ol>

## Command and parameter formatting:
<p>Parameters taking <b>String</b>s have their arguments inside two quotation marks.</p>
<pre>New-ADUser -Name "John Doe"</pre>
<p>Parameters taking <b>SecureString</b>s have their arguments placed into a <b>ConvertTo-SecureString</b> function.</p>
<pre>New-ADUser -AccountPassword (ConvertTo-SecureString "Passw0rd" -AsPlainText -Force)</pre>
<p>Parameters taking <b>Boolean</b>s have a check if the string value contains either <b>$true</b>, <b>$false</b>, <b>0</b> or <b>1</b>, if yes, has numeric values are converted into the variable form, else it throws an error.</p>
<pre>New-ADUser -Enabled $true</pre>
<p>Parameters that take other types of arguments do no formatting and warns the user on the standard output with the typename. Other typenames are yet to be implemented.</p>
<pre>warning: default formatting of parameter taking -AccountExpirationDate</pre>
