void *handle = dlopen("/user/lib/python3.12/config-3.12-x86-linux-gnu/libpython3.12.so", RTLD_LAZY | RTLD_GLOBAL);
PyInterpreterConfig config = {
    .use_main_obmalloc = 0,
    .allow_fork = 1,
    .allow_exec = 1,
    .allow_threads = 1,
    .allow_daemon_threads = 1,
    .check_multi_interp_extensions = 1,
    .gil = PyInterpreterConfig_OWN_GIL,
};


