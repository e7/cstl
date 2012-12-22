lib_LTLIBRARIES = libcstl.la
libcstl_la_SOURCES = adv_string.c list.c mem.c map.c search.c sort.c main.c

if DEBUG
libcstl_la_CFLAGS = -g -O0
else
libcstl_la_CFLAGS = -DNDEBUG -O2
endif
