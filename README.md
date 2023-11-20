# Search_Engine-Browser-In-C

Task 1 - The implementation of this problem took approximately 8 hours. 
The program uses a heterogeneous data type to store and retain data about websites: URL, number of views, number of bytes, checksum, title, content, and code. 
Several functions were used to solve the task: the two initialization functions (init, init2), the function for reading the HTML code (cd), and the color conversion function. 
The content of the websites is parsed and stored with the help of two pointers for reading between HTML tags. Additionally, the program uses dynamic memory allocation where necessary.

Task 2 - The implementation of this problem took approximately 3 hours. 
The program uses the principles from Task 1 regarding the heterogeneous data type, functions, and parsing and storing of content. 
Additionally, the program utilizes a function to search the content for a given word and a series of helper functions for sorting the websites (swp - swap, cmp - compare). T
he program also uses dynamic memory allocation where necessary.

Task 3 - The implementation of this problem took approximately 6 hours. 
The program uses the principles from Task 2. 
Additionally, the program uses 3 search functions (cautg, cautpoz, cautneg). "cautg" is used to search the parsed content for a given sequence of words between quotes. "cautpoz" is used to search the parsed content for a given word, and "cautneg" is used to search the parsed content for a given word prefixed with "-". 
The program displays the websites according to the following criteria: if it contains the sequence given in quotes, if it contains the given word, and if it does not contain the word given with "-", in cases where there are multiple results, they will be sorted in increasing lexicographical order.

Task 4 - The implementation of this problem took approximately 6 hours. 
The program uses the principles from Task 1 regarding the heterogeneous data type, functions, and parsing and storing of content. 
Additionally, the program uses 2 functions for bit rotation (rotr and rotl). 
These functions receive a number in decimal base and after converting it to binary, rotate its bits k times to the right or left. 
Thus, the actual checksum is calculated and compared with the given one.





