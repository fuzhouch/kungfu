## Fuzhou's modified version of Kungfu

### 20230228 - Performance test

* Add KUNGFU_PERFORMANCE_TEST macro. Writer adds three timer counters
  to measure time consumed of ``open_frame``, ``mmap`` and ``close_frame()`` calls.
* Fix a bug in journal.h, that the string version of ``writer::write()``
  resolves to incorrect template method. It causes a bug that it does
  not write complete data there.
