#define CLASS UVloop
#define NO_DESTRUCT

#define UVloop__define \
    VAR(read,uv_loop_t*,loop) \
    METHOD(UVloop,public,void,stop) \
    METHOD(UVloop,public,int,run,(uv_run_mode mode)) \
    METHOD(UVloop,public,uint64_t,now) \
    /**/
