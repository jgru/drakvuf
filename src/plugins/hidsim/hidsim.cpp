/*********************IMPORTANT DRAKVUF LICENSE TERMS***********************
 *                                                                         *
 * DRAKVUF (C) 2014-2021 Tamas K Lengyel.                                  *
 * Tamas K Lengyel is hereinafter referred to as the author.               *
 * This program is free software; you may redistribute and/or modify it    *
 * under the terms of the GNU General Public License as published by the   *
 * Free Software Foundation; Version 2 ("GPL"), BUT ONLY WITH ALL OF THE   *
 * CLARIFICATIONS AND EXCEPTIONS DESCRIBED HEREIN.  This guarantees your   *
 * right to use, modify, and redistribute this software under certain      *
 * conditions.  If you wish to embed DRAKVUF technology into proprietary   *
 * software, alternative licenses can be acquired from the author.         *
 *                                                                         *
 * Note that the GPL places important restrictions on "derivative works",  *
 * yet it does not provide a detailed definition of that term.  To avoid   *
 * misunderstandings, we interpret that term as broadly as copyright law   *
 * allows.  For example, we consider an application to constitute a        *
 * derivative work for the purpose of this license if it does any of the   *
 * following with any software or content covered by this license          *
 * ("Covered Software"):                                                   *
 *                                                                         *
 * o Integrates source code from Covered Software.                         *
 *                                                                         *
 * o Reads or includes copyrighted data files.                             *
 *                                                                         *
 * o Is designed specifically to execute Covered Software and parse the    *
 * results (as opposed to typical shell or execution-menu apps, which will *
 * execute anything you tell them to).                                     *
 *                                                                         *
 * o Includes Covered Software in a proprietary executable installer.  The *
 * installers produced by InstallShield are an example of this.  Including *
 * DRAKVUF with other software in compressed or archival form does not     *
 * trigger this provision, provided appropriate open source decompression  *
 * or de-archiving software is widely available for no charge.  For the    *
 * purposes of this license, an installer is considered to include Covered *
 * Software even if it actually retrieves a copy of Covered Software from  *
 * another source during runtime (such as by downloading it from the       *
 * Internet).                                                              *
 *                                                                         *
 * o Links (statically or dynamically) to a library which does any of the  *
 * above.                                                                  *
 *                                                                         *
 * o Executes a helper program, module, or script to do any of the above.  *
 *                                                                         *
 * This list is not exclusive, but is meant to clarify our interpretation  *
 * of derived works with some common examples.  Other people may interpret *
 * the plain GPL differently, so we consider this a special exception to   *
 * the GPL that we apply to Covered Software.  Works which meet any of     *
 * these conditions must conform to all of the terms of this license,      *
 * particularly including the GPL Section 3 requirements of providing      *
 * source code and allowing free redistribution of the work as a whole.    *
 *                                                                         *
 * Any redistribution of Covered Software, including any derived works,    *
 * must obey and carry forward all of the terms of this license, including *
 * obeying all GPL rules and restrictions.  For example, source code of    *
 * the whole work must be provided and free redistribution must be         *
 * allowed.  All GPL references to "this License", are to be treated as    *
 * including the terms and conditions of this license text as well.        *
 *                                                                         *
 * Because this license imposes special exceptions to the GPL, Covered     *
 * Work may not be combined (even as part of a larger work) with plain GPL *
 * software.  The terms, conditions, and exceptions of this license must   *
 * be included as well.  This license is incompatible with some other open *
 * source licenses as well.  In some cases we can relicense portions of    *
 * DRAKVUF or grant special permissions to use it in other open source     *
 * software.  Please contact tamas.k.lengyel@gmail.com with any such       *
 * requests.  Similarly, we don't incorporate incompatible open source     *
 * software into Covered Software without special permission from the      *
 * copyright holders.                                                      *
 *                                                                         *
 * If you have any questions about the licensing restrictions on using     *
 * DRAKVUF in other works, are happy to help.  As mentioned above,         *
 * alternative license can be requested from the author to integrate       *
 * DRAKVUF into proprietary applications and appliances.  Please email     *
 * tamas.k.lengyel@gmail.com for further information.                      *
 *                                                                         *
 * If you have received a written license agreement or contract for        *
 * Covered Software stating terms other than these, you may choose to use  *
 * and redistribute Covered Software under those terms instead of these.   *
 *                                                                         *
 * Source is provided to this software because we believe users have a     *
 * right to know exactly what a program is going to do before they run it. *
 * This also allows you to audit the software for security holes.          *
 *                                                                         *
 * Source code also allows you to port DRAKVUF to new platforms, fix bugs, *
 * and add new features.  You are highly encouraged to submit your changes *
 * on https://github.com/tklengyel/drakvuf, or by other methods.           *
 * By sending these changes, it is understood (unless you specify          *
 * otherwise) that you are offering unlimited, non-exclusive right to      *
 * reuse, modify, and relicense the code.  DRAKVUF will always be          *
 * available Open Source, but this is important because the inability to   *
 * relicense code has caused devastating problems for other Free Software  *
 * projects (such as KDE and NASM).                                        *
 * To specify special license conditions of your contributions, just say   *
 * so when you send them.                                                  *
 *                                                                         *
 * This program is distributed in the hope that it will be useful, but     *
 * WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the DRAKVUF   *
 * license file for more details (it's in a COPYING file included with     *
 * DRAKVUF, and also available from                                        *
 * https://github.com/tklengyel/drakvuf/COPYING)                           *
 *                                                                         *
 ***************************************************************************/

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // usleep
#include <linux/input.h> 
#include <inttypes.h>

#include <iostream>
#include <fstream>

#include <libdrakvuf/libdrakvuf.h>

#include "../output_format.h"
#include "../private.h" //  PRINT_DEBUG
#include "hidsim.h"

#define CMD_BUF_LEN 0x400

/* Stop condition */
static volatile sig_atomic_t sis_stopping = 0;

int dump_screen(qmp_connection* qc, const char* path)
{
    char cmd[0x200];
    snprintf(cmd, 0x200,
        "{ \"execute\":\"screendump\", \"arguments\": { \"filename\": \"%s\" } }",
        path);
    char* out = NULL;

    qmp_communicate(qc, (const char*) cmd, &out);

    if (strcmp(out, QMP_SUCCESS) != 0)
    {
        fprintf(stderr, "[HIDSIM]: Error dumping screen - %s\n", out);
    }
    return 0;
}

/* For absolute pointing devices 2^15 is used to specify maximum */
float calculate_pixel_unit(int u)
{
    return (float)(1<<15)/u;
}

int get_display_dimensions(qmp_connection* qc, struct dimensions* dims)
{
    const char* tmp_path = "/tmp/tmp.ppm";

    /* Dumps screen via QMP-command and stores result at tmp_path */
    if (dump_screen(qc, tmp_path) != 0)
        return -1;

    /* Extracts display size from .ppm-file created by screendump */
    FILE* fr = fopen(tmp_path, "r");
    char ppm_hdr[3];
    int matches = 0;
    int ret = 0;

    /* Checks PPM-header */
    matches = fscanf(fr, "%s", ppm_hdr);

    if (matches != 1 || strcmp(ppm_hdr, "P6") != 0)
        ret = -1;

    /* Reads actual dimensions */
    matches = fscanf(fr, "%d%d", &(dims->width), &(dims->height));

    /* Calculates pixel mapping */
    dims->dx = calculate_pixel_unit(dims->width);
    dims->dy = calculate_pixel_unit(dims->height);

    PRINT_DEBUG("[HIDSIM] Display dimensions %d x %d (dx=%f, dy=%f)\n", dims->width, dims->height, dims->dx, dims->dy);

    if (matches != 2 || dims->width < 0 || dims->height < 0)
        ret = -1;

    /* Clean up */
    if (remove(tmp_path) != 0)
        fprintf(stderr, "[HIDSIM]: Error deleting screen dummp %s\n", tmp_path);

    return ret;
}

/* Constructs JSON string */
void construct_button_command(char* buf, int len, const char* btn , int is_down)
{
    const char* type = is_down ? "true" : "false";
    snprintf(buf, len, "{ \"execute\": \"input-send-event\" , \"arguments\": { \"events\": [ { \"type\": \"btn\" , \"data\" : { \"down\": %s, \"button\": \"%s\" } } ] } }", type, btn);
}

/* Constructs JSON string */
void construct_move_mouse_command(char* buf, int len, int x, int y, bool is_rel)
{
    const char* type = is_rel ? "rel" : "abs";
    snprintf(buf, len, "{\"execute\": \"input-send-event\" , \"arguments\":    \                                                                                                                                                                             
    {\"events\": [{\"type\": \"%s\", \"data\" : {\"axis\": \"x\", \"value\": %d \                                                                                                                                                                            
    }}, {\"type\": \"%s\", \"data\": { \"axis\": \"y\", \"value\": %d }}]}}",
        type, x, type, y);
}

/* Helper to center cursor */
void center_cursor(qmp_connection* qc)
{
    /* Command buffer */
    char buf[CMD_BUF_LEN];

    /* 2^15 == max, 2^14 == max/2 */
    construct_move_mouse_command(buf, CMD_BUF_LEN, 1<<14, 1<<14, false);
    qmp_communicate(qc, buf, NULL);
}

/* Infers socket path from drakvuf's actual domID */
char* construct_sock_path(drakvuf_t drakvuf){
    
    /* Retrieves domid as string */
    char domid[0x20];
    sprintf(domid, "%d", drakvuf_get_dom_id(drakvuf));    
    
    /* Constructs filepath to QMP Unix domain socket */
    char *sock_path = (char*) malloc(sizeof(char) * (strlen(SOCK_STUB) + strlen(domid)));
    strcpy(sock_path, SOCK_STUB);
    strcpy(sock_path + strlen(SOCK_STUB), domid);
    return sock_path;
}

int read_header(FILE* f){
    int bytes_read;
    uint32_t magic = 0x00000000;
    uint32_t drak = 0x00000000;
    uint32_t version = 0x00000000;
    bytes_read = fread(&magic, sizeof(magic), 1, f);
    bytes_read = fread(&drak, sizeof(drak), 1, f);
    bytes_read = fread(&version, sizeof(version), 1, f);
    
    PRINT_DEBUG("[HIDSIM] Read header bytes %x %x %x\n", magic, drak, version);
    
    return 0;
}

int reset_hid_injection(qmp_connection* qc, FILE* f, timeval* tv, int* nx, int* ny){
    /* Jumps to the beginning of the HID data */
    int ret = fseek(f, HEADER_LEN, SEEK_SET);

    /* Center coords */
    *nx = 1 << 14;
    *ny = 1 << 14;
    center_cursor(qc);

    timerclear(tv);

    return ret;
}

int run_injection(qmp_connection* qc, dimensions* dim, FILE* f){
    /* Command buffer */
    char buf[CMD_BUF_LEN];
    input_event ie;

    size_t bytes_read = 0;
    int new_x, new_y = 0;

    struct timeval tv_old;
    struct timeval tv_diff;
    struct timeval tv_curr;
    struct timeval tv_last_qmp;
    struct timeval tv_last_qmp_diff;
    bool is_wait = false;

    while (!this->is_stopping)
    {
        if (bytes_read == 0)
        {
            /* Prepares injection by initializing all variables */
            if(reset_hid_injection(qc, f, &tv_old, &new_x, &new_y) != 0)
                fprintf(stderr, "[HIDSIM] Error resetting HID injection");

        }

        bytes_read = fread(&ie, sizeof(ie), 1, f);

        /* Handles move events */
        if (ie.type == EV_REL){
            //PRINT_DEBUG("[HIDSIM] EV_REL\n");                                                                                                                                                                                                              
            if (ie.code == REL_X)
            {
                new_x += ie.value;
                //PRINT_DEBUG("[HIDSIM] X %d\n", new_x);                                                                                                                                                                                                     
            }
            if (ie.code == REL_Y)
            {
                new_y += ie.value;
                //PRINT_DEBUG("[HIDSIM] Y %d \n", new_y);                                                                                                                                                                                                    
                //construct_move_mouse_command(buf, CMD_BUF_LEN, old_x,                                                                                                                                                                                      
                //new_y, false);                                                                                                                                                                                                                             
            }
        }

        /* Handles click events */
        if (ie.type == EV_KEY){
            if(ie.code == BTN_LEFT){
                construct_button_command(buf, CMD_BUF_LEN, "left", ie.value);
            }
            if(ie.code == BTN_MIDDLE){
                construct_button_command(buf, CMD_BUF_LEN, "middle", ie.value);
            }
            if(ie.code == BTN_RIGHT){
                construct_button_command(buf, CMD_BUF_LEN, "right", ie.value);

            }
            //PRINT_DEBUG("[HIDSIM] %s \n", buf);                                                                                                                                                                                                            
            qmp_communicate(qc, buf, NULL);
            gettimeofday(&tv_last_qmp, NULL);
        }

        timersub(&ie.time, &tv_old, &tv_diff);

        //PRINT_DEBUG("[HIDSIM] Sleeping %ld.%ld secs \n", tv_diff.tv_sec, tv_diff.tv_usec);                                                                                                                                                                 

        if (tv_diff.tv_sec > 0)
        {
            //PRINT_DEBUG("[HIDSIM] Sleeping %ld secs \n", tv_diff.tv_sec);                                                                                                                                                                                  
            sleep(tv_diff.tv_sec);
            is_wait = true;
        }

        if (tv_diff.tv_usec > 0)
        {
            //PRINT_DEBUG("[HIDSIM] Sleeping %ld usecs\n", tv_diff.tv_usec);                                                                                                                                                                                 
            usleep(tv_diff.tv_usec);
            is_wait = true;
        }

        gettimeofday(&tv_curr, NULL);
        timersub(&tv_curr, &tv_last_qmp, &tv_last_qmp_diff);

        /* Throttles the sending of QMP commands */
        if (is_wait && tv_last_qmp_diff.tv_usec > 5000){
            //PRINT_DEBUG("[HIDSIM] %s \n", buf);                                                                                                                                                                                                            
            construct_move_mouse_command(buf, CMD_BUF_LEN, new_x, new_y, false);
            qmp_communicate(qc, buf, NULL);
            gettimeofday(&tv_last_qmp, NULL);
            is_wait = false;
        }

        tv_old.tv_sec = ie.time.tv_sec;
        tv_old.tv_usec = ie.time.tv_usec;
    }

    return 0; 
}
/* this function is run by the second thread */
void *inject_from_file(void* p)
{   
    PRINT_DEBUG("[HIDSIM] Starting thread\n");

    /* Passed parameters */
    char ** paths = (char**) p; 
    char * sock_path = paths[0];
    char * template_path = paths[1];

    /* Initializes qmp connection */
    qmp_connection qc; 

    int ret = qmp_init_conn(&qc, sock_path);

    PRINT_DEBUG("[HIDSIM] Connecting to %s\n", sock_path);

    if (ret < 0)
    {
        fprintf(stderr, "[HIDSIM] Could not connect to Unix Domain Socket %s.\n", sock_path);
        return NULL;
    }
    dimensions dim; 

    if (get_display_dimensions(&qc, &dim) != 0)
    {
        fprintf(stderr, "[HIDSIM] Error getting display dimensions. Using default values\n");
    }
  
    FILE *f = fopen(template_path,"rb");

    if (!f) {
        fprintf(stderr, "Error reading from %s\n", template_path);
        return NULL;
    }

    if(read_header(f) != 0){
        fprintf(stderr, "Not a valid HID template file. Stopping");
        return NULL;
    }

    run_injection(&qc, &dim, f);

    /* Stop and clean up */
    qmp_close_conn(&qc);
    fclose(f);

    return NULL; 
}

hidsim::hidsim(drakvuf_t drakvuf, const hidsim_config* config, output_format_t output)
{
    PRINT_DEBUG("[HIDSIM] Starting plugin\n");

    /* Copies template path */
    if (config->template_fp)
    {
        this->template_path = (char *)malloc(sizeof(char) * strlen(config->template_fp));
        strcpy(this->template_path, config->template_fp);
        PRINT_DEBUG("[HIDSIM] Using template file: %s\n", this->template_path);
    }
    else
    {
        PRINT_DEBUG("[HIDSIM] Random HID injection\n");
    }

    /* Constructs path to Unix domain socket of Xen guest under investigation */
    this->sock_path = construct_sock_path(drakvuf);
    
    /* Retrieves display dimensions for coordination of mouse movements */
    //

    
    char **paths = (char**)malloc(sizeof(char*) * 2); 
    paths[0] = this->sock_path;
    paths[1] = this->template_path; 

    /* Starts worker thread */
    this->t = (pthread_t*) malloc(sizeof(pthread_t));
    if (pthread_create(this->t, NULL, inject_from_file, (void *) paths))
    {
        fprintf(stderr, "Error creating thread\n");
        return;
    }
}

hidsim::~hidsim()
{
    if (this->t != NULL || !sis_stopping)
    {
        this->stop();
    }

    if (this->template_path)
        free(this->template_path);
    
    if (this->sock_path)
        free(this->sock_path);
};

bool hidsim::stop()
{
    PRINT_DEBUG("[HIDSIM] Stopping thread\n");
    
    this->m_is_stopping = true; 
    sis_stopping = 1; 

    if (this->t != NULL)
    {
        if (pthread_join(*(this->t), NULL))
        {

            fprintf(stderr, "Error joining thread\n");
            return false;
        }
        //free(this->t); 
        
        PRINT_DEBUG("[HIDSIM] Successfully joined thread \n");
    }
    return true;
}
