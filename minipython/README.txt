Minipython is based on http://www.adintr.com/article/blog/281(Python 2.7.2). I updated it to 2.7.15.

Based on the description from the author:
1. 去掉 python 解释器对 Python 源代码的标准库的依赖. 
2. 去掉了一下的一些模块: cPickle, _io, datetime,  _random, cStringIO, _lsprof, _collections, _struct, _sre, time, _locale, _headq, _functools, _weakref, itertools
3. 去掉了这两个内建的对象: bytearray,  memoryobject

English:
1. Removed the dependency between Python interpreter and Python source code standard library.
2. Removed internal modules: cPickle, _io, datetime,  _random, cStringIO, _lsprof, _collections, _struct, _sre, time, _locale, _headq, _functools, _weakref and itertools
3. Removed internal objects: bytearray,  memoryobject