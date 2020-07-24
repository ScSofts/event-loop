# A C++11 Event Loop Library 

## Install
```bash
git clone <this repository>
cd event-loop/src
g++ loop.cc -c
```
## example
See src/test.cc

### Warning: event_loop cannot be deleted! Use loop->exit(); and it will be deleted!