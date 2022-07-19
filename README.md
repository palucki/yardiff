yardiff - Yet another rdiff implementation.

Rolling hash based file diffing algorithm. When comparing original and an updated version of an input, it returns a description ("delta") which can be used to upgrade an original version of the file into the new file. The description contains the chunks which:
- Can be reused from the original file
- have been added or modified and thus would need to be synchronized

-----
Usage:
```
    yardiff BASE_FILE UPDATED_FILE BLOCK_SIZE
```
