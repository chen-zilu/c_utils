## DataStore
- 目的

   缓存最新的多个数据并判断是否超时

- 示例
```
// in init function
// header only
#include "DataStore.hpp"

// init max data number and timeout count
DataStore<int> ds;
ds.init(30, 5); // set buffer size to 30, and report timeout when miss for 5 times
```
```
// in timer function
// read data in a timer
ds.set(data);
```
```
// in main loop
// check and use data
ds.check_timeout();
if (ds.is_stable())
{
    if (ds.is_newdata())
    {
        // do something
    }
}
```