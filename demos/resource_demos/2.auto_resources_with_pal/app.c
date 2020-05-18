#include <errno.h>
#include <limits.h>
#include <pal/pal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#pragma pirate enclave declare(my_app)

pal_boolean my_boolean __attribute__((pirate_resource("my_boolean", "my_app")));
pal_integer my_integer __attribute__((pirate_resource("my_integer", "my_app")));
pal_string  my_string  __attribute__((pirate_resource("my_string",  "my_app")));
pal_file    my_file    __attribute__((pirate_resource("my_file",    "my_app")));

int __attribute__((pirate_enclave_main("my_app"))) main(void)
{
    printf("my_boolean = %s\n", my_boolean ? "true" : "false");

    printf("my_integer = %ld\n", my_integer);

    printf("my_string = %s\n", my_string);

    {
        char buf[16] = {0};
        if(read(my_file, buf, sizeof(buf) - 1) < 0)
            printf("Failed to read from \"my_file\": %s\n", strerror(errno));
        else
            printf("my_file = %s\n", buf);
        close(my_file);
    }

    return EXIT_SUCCESS;
}
