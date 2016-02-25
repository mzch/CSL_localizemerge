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
        fprintf(stderr, "%s En.tsv Ja.tsv Output.tsv\n\n", argv[0]);
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
        exit (3);
    }
    FILE * fp_w = fopen(argv[3], "w");
    if (! fp_w)
    {
        fprintf(stderr, "File not open: %s\n", argv[3]);
        exit (4);
    }


    // Read Localization TSV
    char buf[BUFFER_SIZE];
    char org[BUFFER_SIZE];
    char out[BUFFER_SIZE];
    int line_num = 1;
    
    fgets(buf, sizeof(buf) - 1, fp_t);          // 1行目は無条件で出力する
    fputs(buf, fp_s);

    while (fgets(buf, sizeof(buf) - 1, fp_t))
    {
        line_num++;
        strcpy(org, buf);

        if (buf[0] == '#')
        {
            strcpy(out, buf);
        }
        else
        {
            LocaleTSV l = getLocaleData(buf);
            
            if (l.value[0] == '[' && l.value[1] == '[')
            {
                strcpy(out, org);
            }
            else
            {
                char guf[BUFFER_SIZE];
            
                fgets(guf, sizeof(guf) - 1, fp_s);      // 先頭行を読み飛ばす
            
                while (fgets(guf, sizeof(guf) - 1, fp_s))
                {

                    TranslatedTSV t = getTranslationData(guf);

                    if (! strcmp(l.ref_name, t.id) && ! strcmp(l.ref_key, t.index) && ! strcmp(l.index, t.key))
                    {
                        if (! strlen(t.translation))
                        {
                            printf("Missing translation: [%d] %s, %s, %s\n", line_num, l.ref_name, l.ref_key, l.index);
                            exit (7);
                        }
                        char * crlf = strchr(t.translation, CRLF);
                        if (crlf)
                        {
                            printf("Illegal translation with CRLF: [%d] %s, %s, %s\n", line_num, l.ref_name, l.ref_key, l.index);
                            exit (6);
                        }

                        sprintf(out, "%s\t%s\t%s\t%s\t%s\t%s\t%s\n", l.name, l.key, l.ref_name, l.ref_key, l.index, l.value, t.translation);
                        break;
                    }
            
                }
            }
            if (feof(fp_s))
            {
                printf("Missing translation: [%d] %s, %s, %s\n", line_num, l.ref_name, l.ref_key, l.index);
                exit (7);
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
