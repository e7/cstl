noinst_PROGRAMS = mem_test other_rbtree
mem_test_SOURCES = mem_test.c
other_rbtree_SOURCES = other_rbtree.c

if DEBUG
mem_test_CFLAGS = -g -O0
other_rbtree_CFLAGS = -g -O0
else
mem_test_CFLAGS = -NDEBUG -O2
other_rbtree_CFLAGS = -NDEBUG -O2
endif
mem_test_LDFLAGS = -L$(top_srcdir)/src/.libs -lcstl
