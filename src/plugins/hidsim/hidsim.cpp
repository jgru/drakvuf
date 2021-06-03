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

#include <libdrakvuf/libdrakvuf.h>

#include "../output_format.h"
#include "../private.h" //  PRINT_DEBUG
#include "hidsim.h"

char buf[0x100];
const char* move = "mouse_move";

void move_mouse(drakvuf_t drakvuf, int x, int y)
{
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s %d %d 0", move, x, y);
    char* out = NULL;

    drakvuf_send_qemu_monitor_command(drakvuf, (const char*) buf, &out);

    PRINT_DEBUG("[HIDSIM] Sent HMP command: \"%s\"\n", buf);
}

int dump_screen(drakvuf_t drakvuf, const char* path)
{
    sprintf(buf, "%s %s", "screendump", path);
    char* out = NULL;

    drakvuf_send_qemu_monitor_command(drakvuf, (const char*) buf, &out);

    if (strlen(out) != 0)
    {
        fprintf(stderr, "[HIDSIM]: Error dumping screen - %s\n", out);
    }
    return 0;
}

int get_display_dimensions(drakvuf_t drakvuf, struct dimensions* dims)
{
    const char* tmp_path = "/tmp/tmp.ppm";

    // Dumps screen via HMP-command and stores result at tmp_path
    if (dump_screen(drakvuf, tmp_path) != 0)
        return -1;

    // Extracts display size from screendump
    FILE* fr = fopen(tmp_path, "r");
    char ppm_hdr[3];
    int matches = 0;
    int ret = 0;

    // Checks header
    matches = fscanf(fr, "%s", ppm_hdr);

    if (matches != 1 || strcmp(ppm_hdr, "P6") != 0)
        ret = -1;

    // Reads actual dimensions
    matches = fscanf(fr, "%d%d", &(dims->width), &(dims->height));

    if (matches != 2 || dims->width < 0 || dims->height < 0)
        ret = -1;

    //Clean up
    if (remove(tmp_path) != 0)
        fprintf(stderr, "[HIDSIM]: Error deleting screen dummp %s\n", tmp_path);

    return ret;
}

void hidsim::hid_injector(drakvuf_t drakvuf)
{
    PRINT_DEBUG("[HIDSIM] Starting thread\n");

    // Converts intervall to usecs
    int uinterval = this->interval * 1000;
    // Declares points in time
    gint64 t1, t2;

    // Position cursor at center of the screen by moving to zero
    move_mouse(drakvuf, this->dim.width * -1, this->dim.height * -1);
    usleep(10000);

    // TODO: Investigate this behaviour!
    // Positioning the cursor at the middle of the screen is not working,
    // since the HMP-command induces some strange mapping
    move_mouse(drakvuf, this->dim.width / 2, this->dim.height / 2);

    t1 = g_get_monotonic_time();
    int sign = 0x1;

    // Loops infinitely, until stopped
    while (!this->is_stopping)
    {
        t2 = g_get_monotonic_time();

        int elapsed = t2 - t1;

        if (elapsed > uinterval)
        {
            move_mouse(drakvuf, this->dim.width / 16 * sign, this->dim.height / 16 * sign);
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

hidsim::hidsim(drakvuf_t drakvuf, const hidsim_config* config, output_format_t output)
{
    PRINT_DEBUG("[HIDSIM] Startup\n");

    // Retrieves display dimension for coordinating of relative mouse movements
    if (get_display_dimensions(drakvuf, &(this->dim)) != 0)
    {
        fprintf(stderr, "[HIDSIM] Error getting display dimensions. Using default values\n");
    }

    // Starts worker thread
    this->t = new std::thread(&hidsim::hid_injector, this, drakvuf);
}

hidsim::~hidsim()
{
    if (this->t != NULL)
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
