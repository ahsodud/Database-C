# Database-C
The basic behaviour of the program is:
- The program starts without any data and runs in an endless loop
- The user can enter every command he would like to (but in the beginning, the only meaningful command would be ADD to add a BST from a file)
- After having at least one BST saved in the memory, the user can choose from several commands which are further described in the COMMANDS-section.
- The progam ends when the user types in QUIT
## Commands:
1) `QUIT`
Just terminates the program.
2) `ADD filename.txt tablename sorting`
- `filename.txt`: The filename of the data for the BST
- `tablename`: The name of the BST inside the program
- `sorting`: The key after which the data should be sorted/inserted into the BST *= sortingkey*.
3) `PRINT`
This command just prints the (specified) BST.
4) `SELECT key FROM tablename WHERE = value`
- `SELECT key`: The key which should be looked for. For example "Name", "Job"...
- `FROM tablename`: The table in which the looking up should be performed.
- `WHERE =`: The condition which should be used for the search operation. The operation is always `=`.
- `value`: The value which should be looked for (must match with the chosen key!).
5) `INSERT INTO tablename value1 value2 value3....`
A new node with certain data should be added to the BST in consideration of the current sorting of the BST. 
- `INTO tablename`: The name of the table where the new entry should be inserted to
- `valueX`: An inorder listing of the new values of the new node.
6) `DELETE FROM tablename WHERE key = value`
- `FROM tablename`: The name of the table in which the deletion should be performed.
- `WHERE key`: The key the user wants to look for.
- `= value`: The value which should be looked for (must match with the chosen key!). 

