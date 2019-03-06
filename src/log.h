static inline int
wrest_log(const char* fmt, ...)
{
    va_list ap;
    int n;
    n = fprintf(stdout, "[] ");
    va_start(ap, fmt);
    n += vfprintf(stdout, fmt, ap);
    va_end(ap);
    fprintf(stdout, "\n");
    return n+1;
}

