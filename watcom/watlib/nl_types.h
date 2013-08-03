
#ifndef __nl_types_h
#define __nl_types_h

#ifdef __cplusplus
    extern "C" {
#endif

#define NL_SETD         1
#define NL_CAT_LOCALE   1

#define CAT_MAGIC       505
#define CATD_ERR        ((nl_catd) -1)
#define NL_MAXOPEN      10


struct _catalog_descriptor {
    char            *_mem;
    char            *_name;
    struct __file   *_fd;
    struct _header  *_hd;
    struct _catset  *_set;
    int             _setmax;
    int             _count;
    int             _pid;
    int             _oflag;
};

typedef int nl_item;
typedef struct _catalog_descriptor *nl_catd;

int  catclose(nl_catd);
char catgets(nl_catd,int,int,const char*);
nl_catd catopen(const char*,int);

#ifdef __cplusplus
   }
#endif

#endif

