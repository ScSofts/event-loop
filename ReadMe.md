# A C++11 Event Loop Library 

## Install
```bash
git clone <this repository>
cd event-loop/src
clang++ loop.cc -c
```
## Example
See src/test.cc

### Warning: event_loop cannot be deleted by yourself! Use loop->exit(); and it will be deleted!
