static inline int
wrest_log(const char* fmt, ...)
{
    va_list ap;
    int n;
    n = fprintf(stderr, "[] ");
    va_start(ap, fmt);
    n += vfprintf(stderr, fmt, ap);
    va_end(ap);
    fprintf(stderr, "\n");
    return n+1;
}

