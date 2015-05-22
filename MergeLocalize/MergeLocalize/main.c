//
//  main.c
//  MergeLocalize
//
//  Created by Koichi MATSUMOTO on 15/05/01.
//  Copyright (c) 2015 Leafia Tech. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structures.h"

LocaleTSV getLocaleData(char * buf)
{
    LocaleTSV l;

    l.translation = l.value = l.index = l.ref_key = l.key = l.name = l.ref_name = "";

    char *p = strchr(buf, TAB);
    *p = '\0';
    l.name = buf;

    p++;
    buf = p;
    p = strchr(buf, TAB);
    if (! p)
        return l;
    *p = '\0';
    l.key = buf;

    p++;
    buf = p;
    p = strchr(buf, TAB);
    if (! p)
        return l;
    *p = '\0';
    l.ref_name = buf;

    p++;
    buf = p;
    p = strchr(buf, TAB);
    if (! p)
        return l;
    *p = '\0';
    l.ref_key = buf;

    p++;
    buf = p;
    p = strchr(buf, TAB);
    if (! p)
        return l;
    *p = '\0';
    l.index = buf;

    p++;
    buf = p;
    p = strchr(buf, TAB);
    if (! p)
        return l;
    *p = '\0';
    l.value = buf;

    return l;
}


TranslatedTSV getTranslationData(char * buf)
{
    TranslatedTSV t;

    t.index = t.value = t.id = t.key = t.translation = "";

    char *p = strchr(buf, TAB);
    if (! p)
        return t;
    *p = '\0';
    t.id = buf;

    p++;
    buf = p;
    p = strchr(buf, TAB);
    if (! p)
        return t;
    *p = '\0';
    t.index = buf;
    p++;
    buf = p;
    p = strchr(buf, TAB);
    if (! p)
        return t;
    *p = '\0';
    t.key = buf;

    p++;
    buf = p;
    p = strchr(buf, TAB);
    if (! p)
        return t;
    *p = '\0';
    t.value = buf;

    p++;
    buf = p;
    p = strchr(buf, TAB);
    if (! p)
        return t;
    *p = '\0';
    t.translation = buf;

    return t;
}

int main(int argc, const char * argv[])
{
    // check arguments
    if (argc < 4)
    {
        fprintf(stderr, "Missing arguments...\n");
        exit (1);
    }

    // check read in file
    FILE * fp_t = fopen(argv[1], "r");
    if (! fp_t)
    {
        fprintf(stderr, "File not found: %s\n", argv[1]);
        exit (2);
    }
    FILE * fp_s = fopen(argv[2], "r");
    if (! fp_s)
    {
        fprintf(stderr, "File not found: %s\n", argv[2]);
    }
    FILE * fp_w = fopen(argv[3], "w");
    if (! fp_w)
    {
        fprintf(stderr, "File not open: %s\n", argv[3]);
    }


    // Read Localization TSV
    char buf[BUFFER_SIZE];
    char org[BUFFER_SIZE];
    char out[BUFFER_SIZE];
    while (fgets(buf, sizeof(buf) - 1, fp_t))
    {
        strcpy(org, buf);

        if (buf[0] == '#')
        {
            strcpy(out, buf);
        }
        else
        {
            LocaleTSV l = getLocaleData(buf);

            char guf[BUFFER_SIZE];
            while (fgets(guf, sizeof(guf) - 1, fp_s))
            {

                TranslatedTSV t = getTranslationData(guf);

                if (! strcmp(l.ref_name, t.id) && ! strcmp(l.ref_key, t.index) && ! strcmp(l.index, t.key) && l.value[0] != '[')
                {
                    sprintf(out, "%s\t%s\t%s\t%s\t%s\t%s\t%s\r\n", l.name, l.key, l.ref_name, l.ref_key, l.index, l.value, t.translation);
                    break;
                }
            }
            if (feof(fp_s))
            {
                strcpy(out, org);
            }
            rewind(fp_s);
        }
        fputs(out, fp_w);
    }

    fclose(fp_w);
    fclose(fp_s);
    fclose(fp_t);

    return 0;
}
