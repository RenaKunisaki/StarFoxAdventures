object DLLs (maybe all) have the same set of function pointers:
 1. onLoad
 2. update
 3. hitDetect
 4. render
 5. free
 6. ?
 7. getExtraSize
 8. setScale
 9. ?
10. mtx related
11. related to children

not every DLL follows this pattern (eg Camera has get() instead of hitDetect()) but most do
