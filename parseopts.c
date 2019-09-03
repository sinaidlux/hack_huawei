/**********************************************************************
$Id: parseopts.c,v 1.7 2006/12/05 20:47:56 bu Exp $

Copyright 2004, Bulent Yilmaz
for Microbrew Software

Part of Microbrew md5sum/sha1sum/ripemd160sum
GNU textutils md5sum replacement

http://www.microbrew.org/tools/md5sha1sum.html

Microbrew md5sum/sha1sum is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

MicroSieve is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with MicroSieve; if not, write to the Free Software Foundation, Inc.,
59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

**********************************************************************/

#include "parseopts.h"

void showverinfo() {
    printf("Microbrew md5sum/sha1sum/ripemd160sum %s (%s)\n", 
           REL_VERSION, REL_DATE);
    printf("  Compiled %s at %s\n", __DATE__, __TIME__);
    puts("Written by Bulent Yilmaz");
    puts("");
    puts("Copyright (C) 2004,2006 Microbrew Software");
}

void showhelp(char *progname) {
    printf("Usage: %s [<option>] <file> [<file> [...] ]\n", progname);
    printf("       %s [<option>] --check <file>\n", progname);
    puts("");
    puts("Note:  These options are mostly compatible with GNU md5sum");
    puts("       -s, -h, and -V are not available in GNU md5sum");
    puts("");
    puts(" -b, --binary         Read files in binary mode");
    puts(" -c, --check <file>   Check MD5 sums from <file>");
    puts(" -t, --text           Read files in ASCII mode");
    puts("");
    puts(" -s, --status         Silent mode: Use exit code to determine verification");
    puts("");
    puts(" -h, --help           Display this help message and exit");
    puts(" -V, --version        Display program version and exit");
}

char *fixfn_strdup(char *input) {

    int spn;
    char *rstr;

    spn = strspn(input, " \t\n*");
    rstr = strdup(&input[spn]);

    return rstr;
}

int setup_check(struct opts_t *opts, char *cfile) {
    FILE *chk;
    char line[BUFSIZ];
    char *hash, *fn;
    void *holder;

    if(strcmp(cfile, "-")) {
        chk = fopen(cfile, "r");
        if(chk == NULL) {
            perrprintf("setup_check: fopen '%s'", cfile);
            return -1;
        }
    } else chk = stdin;

    while(!feof(chk)) {
#if 0
        printf("Parsing %d\n", opts->filect);
#endif

        fgets(line, BUFSIZ, chk);
        if(feof(chk)) break;

        hash = strtok(line, "\t ");
        if(hash == NULL) {
            printf("Could not parse check file '%s' (1)\n", cfile);
            if(chk != stdin) fclose(chk);
            return -1;
        } 

        fn = strtok(NULL, "\n");
        if(fn == NULL) {
            printf("Could not parse check file '%s' (2)\n", cfile);
            if(chk != stdin) fclose(chk);
            return -1;
        }

        if(strcmp(fn, "-") && strcmp(fn, "*-")) {
            holder = realloc(opts->filenames, 
                             (opts->filect + 1) * sizeof(char *));
            if(holder == NULL) {
                perrputs("setup_check: realloc(opts->filenames)");
                if(chk != stdin) fclose(chk);
                return -1;
            }
            opts->filenames = holder;

            holder = realloc(opts->hashes, (opts->filect + 1) * sizeof(char *));
            if(holder == NULL) {
                perrputs("setup_check: realloc(opts->hashes)");
                if(chk != stdin) fclose(chk);
                return -1;
            }
            opts->hashes = holder;

            opts->filenames[opts->filect] = fixfn_strdup(fn);
            opts->hashes[opts->filect++] = strdup(hash);
        }
    }

    if(chk != stdin) fclose(chk);
    opts->pmode = PMODE_VERIFY;
    return 0;
}

struct opts_t *parseopts(int argc, char **argv) {
    struct opts_t *ropts;
    int i, offs;

    ropts = malloc(sizeof(struct opts_t));
    if(ropts == NULL) {
        perrputs("parseopts: malloc ropts");
        return NULL;
    }

    ropts->fmode = FMODE_ASC; 
    ropts->pmode = PMODE_SUM;
    ropts->vmode = VMODE_NORMAL;
    ropts->filect = 0;
    ropts->filenames = NULL;
    ropts->hashes = NULL;

    for(i = 1; i < argc; i++) {
        if( (!strcmp(argv[i], "-b")) ||
            (!strcmp(argv[i], "--binary")) ) ropts->fmode = FMODE_BIN;
        else if( (!strcmp(argv[i], "-t")) ||
                 (!strcmp(argv[i], "--text")) ) ropts->fmode = FMODE_ASC;
        else if( (!strcmp(argv[i], "-s")) || (!strcmp(argv[i], "--status")) )
           ropts->vmode = VMODE_SILENT;
        else if( (!strcmp(argv[i], "-h")) || (!strcmp(argv[i], "--help")) ) {
            showhelp(argv[0]);
            free(ropts);
            return NULL;
        } else if( (!strcmp(argv[i], "-V")) || 
                   (!strcmp(argv[i], "--version")) ) {
            showverinfo();
            free(ropts);
            return NULL;
        } else if( (!strcmp(argv[i], "-c")) ||
                   (!strcmp(argv[i], "--check")) ) {
            if(argc > (i + 1)) {
                if(setup_check(ropts, argv[++i])) {
                    free(ropts);
                    return NULL;
                }
            } else {
                showhelp(argv[0]);
            }
        } else {
            /* Make sure we're not in verify mode */
            if(ropts->pmode != 0) {
                fprintf(stderr, "Error: --check <filename> cannot be used with additional files\n");
                free(ropts);
                return NULL;
            }

            /* Assume everything else is a filename */
            ropts->filect = argc - i;
            ropts->filenames = malloc(ropts->filect * sizeof(char *));
            if(ropts->filenames == NULL) {
                perrputs("parseopts: malloc ropts->filenames");
                free(ropts);
                return NULL;
            }

            offs = i;
            for(; i < argc; i++) ropts->filenames[(i - offs)] = argv[i];
        }
    }

    return ropts;
}
