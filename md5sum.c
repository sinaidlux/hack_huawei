/**********************************************************************
$Id: md5sum.c,v 1.5 2006/12/05 20:47:56 bu Exp $

Copyright 2004, Bulent Yilmaz
for Microbrew Software

Part of Microbrew md5sum/sha1sum
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

Special exemption from the GPL is given solely for the purpose of linking 
with OpenSSL which is distributed under the BSD license.  This is the only
exemption from the GPL which is given and all parts of the GPL are still
in effect including those parts which are similar to the exemption above.

**********************************************************************/

#include <openssl/md5.h>
#include <openssl/sha.h>
#include <openssl/ripemd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "parseopts.h"
#include "macros.h"

void do_md5sum(FILE *mfile, unsigned char *rptr) {
    char rdata[BUFSIZ];
    int rsize;
    MD5_CTX c;
    
    MD5_Init(&c);
    while(!feof(mfile)) {
        rsize = fread(rdata, sizeof(char), BUFSIZ, mfile);
        if(rsize > 0) {
            MD5_Update(&c, rdata, rsize);
        }
    }

    MD5_Final(rptr, &c);
}

void do_ripemd160sum(FILE *mfile, unsigned char *rptr) {
    char rdata[BUFSIZ];
    int rsize;
    RIPEMD160_CTX c;
    
    RIPEMD160_Init(&c);
    while(!feof(mfile)) {
        rsize = fread(rdata, sizeof(char), BUFSIZ, mfile);
        if(rsize > 0) {
            RIPEMD160_Update(&c, rdata, rsize);
        }
    }

    RIPEMD160_Final(rptr, &c);
}

void do_sha1sum(FILE *mfile, unsigned char *rptr) {
    char rdata[BUFSIZ];
    int rsize;
    SHA_CTX c;
    
    SHA1_Init(&c);
    while(!feof(mfile)) {
        rsize = fread(rdata, sizeof(char), BUFSIZ, mfile);
        if(rsize > 0) {
            SHA1_Update(&c, rdata, rsize);
        }
    }

    SHA1_Final(rptr, &c);
}

int main(int argc, char *argv[]) {
    struct opts_t *opts;
    int i,j;
    FILE* tfile;
    unsigned char sumdata[SHA_DIGEST_LENGTH];
    char psum[41];
    int fails = 0;
    int hashsize;
    struct stat stbuf;

    opts = parseopts(argc, argv);
    if(opts == NULL) {
        return -1;
    }

    if(opts->filect <= 0) {
        opts->filenames = malloc(sizeof(char *));
        if(opts->filenames == NULL) {
            perrputs("main: malloc opts->filenames");
            return -1;
        }

        opts->filenames[0] = "-";
        opts->filect = 1;
    }

    for(i = 0; i < opts->filect; i++) {
        if(!strcmp(opts->filenames[i], "-")) {
            tfile = stdin;
        } else {
            /* Stat the file to make sure its a regular file */
            if(stat(opts->filenames[i], &stbuf)) {
                fprintf(stderr, "%s: stat '%s': %s\n", argv[0], 
                    opts->filenames[i], strerror(errno));
                continue;
            }

            if(S_ISREG(stbuf.st_mode)) {
                if(opts->fmode == FMODE_BIN) 
                    tfile = fopen(opts->filenames[i], "rb");
                else
                    tfile = fopen(opts->filenames[i], "r");
            } else {
                fprintf(stderr, "%s: %s: Not a regular file\n", argv[0], 
                    opts->filenames[i]);
                continue;
            }
        }

        if(tfile == NULL) {
            perrprintf("Could not open file '%s'", opts->filenames[i]);
        } else {
            if(strstr(argv[0], "sha1sum")) {
                do_sha1sum(tfile, sumdata);
                for(j = 0; j < SHA_DIGEST_LENGTH; j++)
                    sprintf(&psum[(j*2)], "%02x", (int)sumdata[j]);
                psum[(SHA_DIGEST_LENGTH * 2)] = '\0';
            } else if(strstr(argv[0], "ripemd160sum")) {
                do_ripemd160sum(tfile, sumdata);
                for(j = 0; j < RIPEMD160_DIGEST_LENGTH; j++)
                    sprintf(&psum[(j*2)], "%02x", (int)sumdata[j]);
                psum[(RIPEMD160_DIGEST_LENGTH * 2)] = '\0';
            } else {
                do_md5sum(tfile, sumdata);
                for(j = 0; j < MD5_DIGEST_LENGTH; j++)
                    sprintf(&psum[(j*2)], "%02x", (int)sumdata[j]);
                psum[(MD5_DIGEST_LENGTH * 2)] = '\0';
            }

            if(opts->pmode == PMODE_VERIFY) {
                /* Be nice an offer a warning if hash size appears to be 
                   wrong */

                hashsize = strlen(opts->hashes[i]);
                if(strstr(argv[0], "sha1sum") && (hashsize != (SHA_DIGEST_LENGTH * 2)))
                    fputs("Warning: Hash size is wrong, maybe you want to use 'md5sum'\n", stderr);
                if(strstr(argv[0], "ripemd160sum") && (hashsize != (RIPEMD160_DIGEST_LENGTH * 2)))
                    fputs("Warning: Hash size is wrong, maybe you want to use 'md5sum'\n", stderr);
                if(strstr(argv[0], "md5sum") && hashsize != (MD5_DIGEST_LENGTH
* 2))
                    fputs("Warning: Hash size is wrong, maybe you want to use 'sha1sum' or 'ripemd160sum'\n", stderr);

                if(opts->vmode == VMODE_SILENT) {
                    if(strcmp(opts->hashes[i], psum)) fails++;
                } else {
                    printf("%s: ", opts->filenames[i]);
                    if(!strcmp(opts->hashes[i], psum)) puts("OK");
                    else {
                        puts("FAILED");
                        fails++;
                    }
                }
            } else {
                if(opts->fmode == FMODE_BIN) 
                    printf("%s *%s\n", psum, opts->filenames[i]);
                else
                    printf("%s  %s\n", psum, opts->filenames[i]);
            }
        }

        if(tfile != stdin) fclose(tfile);
    } 

    if(opts->pmode == PMODE_VERIFY) {
        if(fails > 0) {
            if(opts->vmode != VMODE_SILENT) {
                printf("%s: WARNING: %d of %d computed checksums did NOT match\n", argv[0], fails, opts->filect);
            }
            return 1;
        }
    }

    return 0;
}
