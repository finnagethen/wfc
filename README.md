# Wave Function Collapse Algorithm

## Notes
Only works on Posix systems because 'unistd.h' is used for command line argument parsing.

## Example Usage
~~~Terminal
make
./build/a.out -i ./assets/flowers.png -s 1280x800
~~~

## ToDos
- [x] Extract patterns from input image
- [x] Create adjacency rules
- [ ] Actually implement the wfc algorithm
- [ ] Add rotation, reflection 
- [ ] Optimize with frequency count
- [ ] Add edge options (e.g. border, wrap-around)
