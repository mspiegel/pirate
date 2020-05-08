#ifndef _PIRATE_PAL_H
#define _PIRATE_PAL_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct pirate_resource_param {
    char *prp_name;
    char *prp_value;
};

struct pirate_resource {
    char *pr_name;
    void *pr_obj;
    struct pirate_resource_param *pr_params;
    unsigned char padding[8];
} __attribute__((packed));
// ^ FIXME: Do we want to include something else for these?

typedef char   *pal_string  __attribute__((pirate_resource_type("string")));
typedef int64_t pal_integer __attribute__((pirate_resource_type("integer")));
typedef bool    pal_boolean __attribute__((pirate_resource_type("boolean")));
typedef int     pal_file    __attribute__((pirate_resource_type("file")));

struct pirate_resource _dummy_pirate_res_string[0]
    __attribute__((used, section(".pirate.res.string")));
struct pirate_resource _dummy_pirate_res_integer[0]
    __attribute__((used, section(".pirate.res.integer")));
struct pirate_resource _dummy_pirate_res_boolean[0]
    __attribute__((used, section(".pirate.res.boolean")));
struct pirate_resource _dummy_pirate_res_file[0]
    __attribute__((used, section(".pirate.res.file")));
// ^ FIXME: Remove this once it's taken care of in clang

/* Search environment for "PAL_FD=XXXX", where "XXXX" is a valid file
 * descriptor number.
 *
 * Return "XXXX" if found. Otherwise, return -1;
 */
int get_pal_fd();

/* Get a resource of type "boolean" from the application launcher.
 *
 * Return 0 on success. Return 1 if the format of the received resource
 * is incorrect. Otherwise, return a negative errno value.
 */
int get_boolean_res(int fd, const char *name, bool *outp);

/* Get a resource of type "integer" from the application launcher.
 *
 * Return 0 on success. Return 1 if the format of the received resource
 * is incorrect. Otherwise, return a negative errno value.
 */
int get_integer_res(int fd, const char *name, int64_t *outp);

/* Get a resource of type "string" from the application launcher. The
 * contents of `*outp` must be freed on successful return.
 *
 * Return 0 on success. Return 1 if the format of the received resource
 * is incorrect. Otherwise, return a negative errno value.
 */
int get_string_res(int fd, const char *name, char **outp);

/* Get a resource of type "file" from the application launcher.
 *
 * Return 0 on success. Return 1 if the format of the received resource
 * is incorrect. Otherwise, return a negative errno value.
 */
int get_file_res(int fd, const char *name, int *outp);

#ifdef __cplusplus
}
#endif

#endif // _PIRATE_PAL_H