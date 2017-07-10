#ifndef _APP_TYPES_H_
#define _APP_TYPES_H_

#include "shared_types.h"

#define BUFFER_SIZE     4096
#define MAX_MSG_SIZE    4096

#define VERBOSE         1

#ifndef SAFE_FREE
#define SAFE_FREE(ptr)     {if (NULL != (ptr)) {free(ptr); (ptr)=NULL;}}
#endif

typedef struct {
    char *bgp_serv_addr;
    int bgp_serv_port;
    char *pctrlr_serv_addr;
    int pctrlr_serv_port;
} net_conf_t;

typedef struct {
    char ip_num;
    char **ips;
} as_ips_t;

typedef struct {
    uint32_t as_size;
    uint32_t *as_id_2_n;
    as_ips_t *as_ips;
    as_policy_t *as_policies;
    char *rib_file_dir;
} as_cfg_t;

// message processing related states
typedef struct {
    uint32_t as_size;
    int *pctrlr_sfds;
    as_ips_t *as_ips;
} msg_state_t;

#endif
