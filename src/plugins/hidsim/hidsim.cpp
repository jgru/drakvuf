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

int hidsim::dump_screen(const char* path)
{
    char cmd[0x200];
    snprintf(cmd, 0x200,
        "{ \"execute\":\"screendump\", \"arguments\": { \"filename\": \"%s\" } }",
        path);
    char* out = NULL;

    this->qc.communicate((const char*) cmd, &out);

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

int hidsim::get_display_dimensions(struct dimensions* dims)
{
    const char* tmp_path = "/tmp/tmp.ppm";

    /* Dumps screen via QMP-command and stores result at tmp_path */
    if (dump_screen(tmp_path) != 0)
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
void construct_move_mouse_command(char* buf, int len, int x, int y, bool is_rel)
{
    const char* type = is_rel ? "rel" : "abs";
    snprintf(buf, len, "{\"execute\": \"input-send-event\" , \"arguments\":    \
    {\"events\": [{\"type\": \"%s\", \"data\" : {\"axis\": \"x\", \"value\": %d \
    }}, {\"type\": \"%s\", \"data\": { \"axis\": \"y\", \"value\": %d }}]}}",
        type, x, type, y);
}

/* Helper to center cursor */
void hidsim::center_cursor()
{
    /* Command buffer */
    char buf[CMD_BUF_LEN];

    /* 2^15 == max, 2^14 == max/2 */
    construct_move_mouse_command(buf, CMD_BUF_LEN, 1<<14, 1<<14, false);
    this->qc.communicate(buf, NULL);
}

void hidsim::hid_injector_random(drakvuf_t){
    /* Converts interval to usecs */
    int uinterval = this->interval * 1000;
    /* Declares points in time */
    gint64 t1, t2;
    /* Command buffer */
    char buf[CMD_BUF_LEN];

    this->center_cursor();
    t1 = g_get_monotonic_time();

    /* Used for inverting mouse movement */
    int sign = 0x1;
    
    /* Loops, until stopped */
    while (!this->is_stopping)
    {
        t2 = g_get_monotonic_time();

        int elapsed = t2 - t1;

        if (elapsed > uinterval)
        {
            construct_move_mouse_command(buf, CMD_BUF_LEN, (int)((1<<14) + (128 * this->dim.dx * sign)),
                (int)((1<<14) + (128 * this->dim.dx * sign)), false);
            this->qc.communicate(buf, NULL);
            sign = ~sign + 1;
            t1 = g_get_monotonic_time();
        }
        else
        {
            // Sleeps waiting interval is over
            usleep((uinterval - elapsed));
        }
    }

}

int hidsim::reset_injection(FILE* f, timeval* tv, int* ox, int* oy, int* nx, int* ny){
    int ret = fseek(f, 0, SEEK_SET);
    timerclear(tv);
    this->center_cursor();
    *ox = 1 << 14;
    *oy = 1 << 14;
    *nx  = 0;
    *ny = 0;

    return ret; 
}
void hidsim::hid_injector_from_file(drakvuf_t drakvuf)
{
    PRINT_DEBUG("[HIDSIM] Starting HID simulation thread\n");

    //struct timeval tv;
    //uint16_t type;
    //uint16_t code;
    //int32_t value;
    //Input to read from
    FILE *f;
    f = fopen("/usr/local/src/output.bin","rb");
    
    if (!f) {
        fprintf(stderr, "Error reading from %s\n", "/usr/local/src/output.bin"); 
        return;
    }


    /* Command buffer */
    char buf[CMD_BUF_LEN];
    input_event ie; 

    size_t bytes_read = 0;
    int old_x, old_y, new_x, new_y = 0; 

    struct timeval tv_old;
    struct timeval tv_diff;
    
    
    //reset_injection(f, &tv_old, &old_x, &old_y, &new_x, &new_y);

    while (!this->is_stopping)
    {
        if (bytes_read == 0)
        {   
            /* Prepares injection by initializing all variables */
            if(reset_injection(f, &tv_old, &old_x, &old_y, &new_x, &new_y) != 0)
                fprintf(stderr, "[HIDSIM] Error reading template file");
        }

        bytes_read = fread(&ie, sizeof(ie), 1, f);
        timersub(&ie.time, &tv_old, &tv_diff);

        //PRINT_DEBUG("[HIDSIM] Sleeping %ld.%ld secs \n", tv_diff.tv_sec, tv_diff.tv_usec);

        if (tv_diff.tv_sec > 0)
        {
            PRINT_DEBUG("[HIDSIM] Sleeping %ld secs \n", tv_diff.tv_sec);
            sleep(tv_diff.tv_sec);
        }
        
        if (tv_diff.tv_usec > 0)
        {
            PRINT_DEBUG("[HIDSIM] Sleeping %ld usecs\n", tv_diff.tv_usec);
            usleep(tv_diff.tv_usec);
        }

        if (ie.type == EV_REL){
            //PRINT_DEBUG("[HIDSIM] EV_REL\n");
            if (ie.code == REL_X)
            {   
                new_x = old_x + ie.value;
                //PRINT_DEBUG("[HIDSIM] X %d\n", ie.value);
                construct_move_mouse_command(buf, CMD_BUF_LEN, new_x,
                old_y , false);
                old_x = new_x; 

            }
            if (ie.code == REL_Y)
            {
                new_y = old_y + ie.value; 
                //PRINT_DEBUG("[HIDSIM] Y %d \n", ie.value);
                construct_move_mouse_command(buf, CMD_BUF_LEN, old_x,
                new_y, false);
                old_y = new_y; 
            }
            //PRINT_DEBUG("[HIDSIM] %s \n", buf);
            this->qc.communicate(buf, NULL);
        }
            tv_old.tv_sec = ie.time.tv_sec;
            tv_old.tv_usec = ie.time.tv_usec;
    }
}

void construct_sock_path(char *str, drakvuf_t drakvuf){
    /* Infers socket path from drakvuf's actual domID */
    strcpy(str, "/run/xen/qmp-libxl-");
    sprintf(str + strlen(str), "%d", drakvuf_get_dom_id(drakvuf));
}
hidsim::hidsim(drakvuf_t drakvuf, const hidsim_config* config, output_format_t output) //: sock_path("/run/xen/qmp-libxl-")
{
    PRINT_DEBUG("[HIDSIM] Starting plugin\n");
    
    /* Constructs path to Unix domain socket of Xen guest under investigation */
    construct_sock_path(this->sock_path, drakvuf);

    /* Initializes connection */
    int ret = this->qc.init_conn_unix_sock(this->sock_path);
    PRINT_DEBUG("[HIDSIM] Connecting to %s\n", this->sock_path);

    if (ret < 0)
    {
        fprintf(stderr, "[HIDSIM] Could not connect to Unix Domain Socket %s.\n", this->sock_path);
        return;
    }

    /* Retrieves display dimensions for coordination of mouse movements */
    if (this->get_display_dimensions(&(this->dim)) != 0)
    {
        fprintf(stderr, "[HIDSIM] Error getting display dimensions. Using default values\n");
    }

    /* Starts worker thread */
    this->t = new std::thread(&hidsim::hid_injector_from_file, this, drakvuf);
}

hidsim::~hidsim()
{
    if (this->t != NULL || !this->is_stopping)
    {
        this->stop();
    }
};

bool hidsim::stop()
{
    PRINT_DEBUG("[HIDSIM] Stopping thread\n");
    this->is_stopping = true;

    if (this->t != NULL)
    {
        this->t->join();
        this->t = NULL;
        PRINT_DEBUG("[HIDSIM] Successfully joined thread \n");
    }
    return true;
}
