if DEBUG
    lib_LTLIBRARIES = libcstl_dbg.la
    libcstl_dbg_la_SOURCES = adv_string.c list.c mem.c map.c search.c sort.c main.c
    libcstl_dbg_la_CFLAGS = -g -O0
else
    lib_LTLIBRARIES = libcstl.la
    libcstl_la_SOURCES = adv_string.c list.c mem.c map.c search.c sort.c main.c
    libcstl_la_CFLAGS = -DNDEBUG -O2
endif
