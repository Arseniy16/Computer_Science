/* ------------------------------------------------ */
/* This is the program which emulates _id_ in Linux */
/* ------------------------------------------------ */ 

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <stdlib.h>
#include <grp.h>
#include <limits.h>

#define SG_SIZE (NGROUPS_MAX + 1) // the maximum size for group

int Display (uid_t UID, gid_t GID, char * name, gid_t * groups, int num_GROUPS, int argc);

int main(int argc, char *argv[])
{
    /**
     * Initialize all variables
     */
    struct passwd *pw = NULL;
    gid_t *groups = NULL;
    
    uid_t UID = 0;
    gid_t GID = 0;
    
    gid_t supp_groups[SG_SIZE];
    int num_GROUPS = 0;

    /**
     * When there aren't parametrs in console: <myid __> 
     */
    if (argc == 1)
    {
        /**
         * Get _real_ user id and _real_ group id
         */
        UID = getuid();
        GID = getgid();

        /**
         * Get number of groups and supplimentary groups
         */
        num_GROUPS = getgroups(SG_SIZE, supp_groups);

        if (num_GROUPS == -1)
        {
            printf("ERROR <getgroups()>\n");
            return -1;
        }

        /**
         * Get password file entry using uid
         */
        pw = getpwuid(UID);
        
        if (pw == NULL)
        {
            printf("ERROR <getpwuid()> \n");
            return -1;
        }

        if (Display(UID, GID, pw->pw_name, supp_groups, num_GROUPS, argc)) 
            printf("Error Display");
    }
    else if (argc == 2)
    {
        /**
         * Get password file entry using name
         */
        pw = getpwnam(argv[1]);

        if (pw == NULL) 
        {
            printf("myid: <%s>: такого пользователя нет\n", argv[1]);
            return -1;
        }

        UID = (uid_t)pw->pw_uid;
        GID = (gid_t)pw->pw_gid;

        num_GROUPS = getgroups(SG_SIZE, supp_groups);

        if (num_GROUPS == -1)
        {
            printf("ERROR <getgroups()>\n");
            return -1;
        }

        groups = calloc(num_GROUPS, sizeof(gid_t));

        if (groups == NULL)
        {
            printf("ERROR calloc groups\n");
            free(groups);
            return -1;
        }    

        if (getgrouplist(argv[1], pw->pw_gid, groups, &num_GROUPS) == -1)
        {
            printf("getgrouplist() returned -1\n");
            free(groups);
            return -1;
        }

        if (Display(UID, GID, pw->pw_name, groups, num_GROUPS, argc))
            printf("Error Display");
    }
    else
    {
        printf("myid: лишний операнд <%s>\n", argv[2]);
    }

    free(groups);

    return 0;
}

/**
 * Display all information about id and belonging to groups
 */
int Display (uid_t UID, gid_t GID, char * name, gid_t * groups, int num_GROUPS, int argc)
{
    gid_t GROUPS = 0;
    int i = 0;

    if (groups == NULL || name == NULL) 
    {
        printf("ERROR, NULL was detected in parameters of Display() \n");
        return -1;
    }

    /**
     * To correctly print groups()
     */
    if (argc == 1)
    {
        GROUPS = groups[--num_GROUPS];
    }
    else 
    {
        GROUPS = groups[0];
        i = 1;
    }

    /**
     * Print all information
     */
    printf("uid = %d(%s) gid = %d(%s) groups = %d(%s) ", UID, name, GID, name, GROUPS, name);

    for (i; i < num_GROUPS; i++)
    {
        printf("%d", groups[i]);
        
        struct group *gr = getgrgid(groups[i]);
        
        if (gr != NULL) printf("(%s) ", gr->gr_name);  
    }
    printf("\n");

    return 0;
}
