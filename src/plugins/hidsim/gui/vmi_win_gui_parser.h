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
 * This file was created by Jan Gruber.                                    *
 * It is distributed as part of DRAKVUF under the same license             *
 ***************************************************************************/

#ifndef VMI_WIN_GUI_PARSER_H_
#define VMI_WIN_GUI_PARSER_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <wchar.h>
#include <getopt.h>
#include <string.h>

#define LIBVMI_EXTRA_JSON
#include <libvmi/libvmi.h>
#include <libvmi/libvmi_extra.h>

#include <json-c/json.h>
#include <json-c/json_util.h>

/* Datastructures */
#include <glib.h>

/* Offset retrieval */
#include "vmi_win_gui_offsets.h"
#include "vmi_win_gui_atoms.h"
#include "vmi_win_gui_utils.h"
#include "vmi_win_gui_filter_definitions.h"

/* Flags indicating window style */
#define WS_MINIMIZE 0x20000000
#define WS_VISIBLE 0x10000000
#define WS_DISABLED 0x08000000

/* Window extended style (exstyle) */
#define WS_EX_DLGMODALFRAME 0x00000001
#define WS_EX_NOPARENTNOTIFY 0x00000004
#define WS_EX_TOPMOST 0x00000008
#define WS_EX_ACCEPTFILES 0x00000010
#define WS_EX_TRANSPARENT 0x00000020

/* Factor of desktop - button, used for simple filtering */
#define BTN_RATIO 4

/* Holds struct-offsets, needed to access relevant fields */
extern struct Offsets symbol_offsets;

/*
 * The following structs encapsulate only the information needed for the purpose
 * of reconstructing the GUI to a level, where dialogs could be identified for
 * clicking
 */
struct winsta
{
    addr_t addr;
    /*
     * For each GUI thread, win32k maps, the associated desktop heap into
     * usermode http://mista.nu/research/mandt-win32k-slides.pdf
     *
     * Therefore do it like volatility: Find a process with matching sessionID
     * and take its VA as _MM_SESSION_SPACE for the WinSta
     * https://github.com/volatilityfoundation/volatility/blob/\
     * a438e768194a9e05eb4d9ee9338b881c0fa25937/volatility/plugins/\
     * gui/sessions.py#L49
     *
     * To accomplish this, it's the most easy way to use vmi_read_xx_va
     */
    vmi_pid_t providing_pid;
    uint32_t session_id;
    addr_t atom_table;
    bool is_interactive;
    size_t len_desktops;
    addr_t* desktops;
    char* name;
};

/* Just the minimal information needed to retrieve windows */
struct desktop
{
    addr_t addr;
    vmi_pid_t providing_pid;
    char* name;
#ifndef DISABLE_ATOMS
    GHashTable* atom_table;
#endif
};

struct rect
{
    int16_t x0;
    int16_t x1;
    int16_t y0;
    int16_t y1;

    /* For convenience */
    uint16_t w;
    uint16_t h;
};

struct wnd
{
    addr_t spwnd_addr;
    uint32_t style;
    uint32_t exstyle;
    int level;
    uint16_t atom;
    struct rect r;
    struct rect rclient;
    wchar_t* text;
};

void clear_wnd_container(struct wnd* w)
{
    if (w)
        free(w->text);
}

void free_wnd_container(struct wnd* w)
{
    clear_wnd_container(w);
    free(w);
}

void clear_winsta_container(struct winsta** w)
{
    if (*w)
    {
        free((*w)->name);
        free((*w)->desktops);
    }
}

void free_winsta_container(struct winsta* w)
{
    clear_winsta_container(&w);
    free(w);
}

void clear_desktop_container(struct desktop* d)
{
    if (d)
    {
        free(d->name);

#ifndef DISABLE_ATOMS
        g_hash_table_destroy(d->atom_table);
#endif
    }
}

struct rect* get_visible_rect_from_bitmask(
    char* map, size_t n, int scanline, struct rect* r)
{
    struct rect* result = NULL;

    size_t byte;
    int bit_idx;
    unsigned int bit;
    int x0 = -1, x1 = -1, y0 = -1, y1 = -1;
    bool is_y_hole = false;
    bool is_x_hole = false;

    for (int y = r->y0; y < r->y1; y++)
    {
        for (int x = r->x0; x < r->x1; x++)
        {

            byte = (y * scanline + x)/8;

            /* Parts of a wnd can be outside of the desktop's frame */
            if (byte >= n || x < 0)
                continue;

            bit_idx = x % 8;
            bit = 0x80 >> bit_idx;
            if (!(map[byte] & bit))
            {
                if (x0 == -1 && y0 == -1)
                {
                    x0 = x;
                    y0 = y;
                }

                if (!is_x_hole)
                    x1 = x;

                if (!is_y_hole)
                    y1 = y;
            }
            else
            {
                if (x == x0)
                    is_y_hole = true;

                if (y == y1)
                    is_x_hole = true;
            }
        }
    }
    if (x1 != -1 && y1 != -1)
    {
        result = (struct rect*) malloc(sizeof(struct rect));
        result->x0 = x0;
        result->x1 = x1;
        result->y0 = y0;
        result->y1 = y1;
        result->w = x1 - x0;
        result->h = y1 - y0;
    }

    return result;
}

void update_visibility_bitmask(char* map, size_t n, int scanline,
    struct rect* r)
{
    size_t byte;
    int bit_idx;
    unsigned int bit;

    for (int y = r->y0; y < r->y1; y++)
    {

        for (int x = r->x0; x < r->x1; x++)
        {
            byte = (y * scanline + x)/8;

            if (x < 0 || byte >= n )
                continue;

            /* Parts of a wnd can be outside of the desktop's frame */
            bit_idx = x % 8;
            bit = 0x80 >> bit_idx;
            map[byte] |= bit;
        }
    }
}


/*
 * Determine, if windows is visible;
 * important since invisible windows might have visible children
 */
bool is_wnd_visible(vmi_instance_t vmi, vmi_pid_t pid, addr_t wnd)
{
    uint32_t style = 0;

    if (VMI_FAILURE == vmi_read_32_va(vmi, wnd + symbol_offsets.wnd_style, pid, (uint32_t*)&style))
        return false;

    if (style & WS_VISIBLE)
        return true;

    return false;
}

struct wnd* construct_wnd_container(vmi_instance_t vmi, vmi_pid_t pid, addr_t win)
{
    status_t ret = VMI_FAILURE;

    uint32_t x0 = 0;
    uint32_t x1 = 0;
    uint32_t y0 = 0;
    uint32_t y1 = 0;

    uint32_t rx0 = 0;
    uint32_t rx1 = 0;
    uint32_t ry0 = 0;
    uint32_t ry1 = 0;

    uint32_t style = 0;
    uint32_t exstyle = 0;
    wchar_t* wn = NULL;

    ret = vmi_read_32_va(vmi, win + symbol_offsets.rc_wnd_offset + symbol_offsets.rc_left_offset, pid, (uint32_t*)&x0);
    ret = vmi_read_32_va(vmi, win + symbol_offsets.rc_wnd_offset + symbol_offsets.rc_right_offset, pid, (uint32_t*)&x1);
    ret = vmi_read_32_va(vmi, win + symbol_offsets.rc_wnd_offset + symbol_offsets.rc_top_offset, pid, (uint32_t*)&y0);
    ret = vmi_read_32_va(vmi, win + symbol_offsets.rc_wnd_offset + symbol_offsets.rc_bottom_offset, pid, (uint32_t*)&y1);

    /* Determine, if windows is visible */
    ret = vmi_read_32_va(vmi, win + symbol_offsets.wnd_style, pid, (uint32_t*)&style);

    /* Determine extended style attributes */
    ret = vmi_read_32_va(vmi, win + symbol_offsets.wnd_exstyle, pid, (uint32_t*)&exstyle);

    /* Retrieves atom value */
    addr_t pcls = 0;
    ret = vmi_read_addr_va(vmi, win + symbol_offsets.pcls_offset, pid, &pcls);
    uint16_t atom = 0;
    ret = vmi_read_16_va(vmi, pcls + symbol_offsets.cls_atom_offset, pid, &atom);

    ret = vmi_read_32_va(vmi, win + symbol_offsets.rc_client_offset + symbol_offsets.rc_left_offset, pid, (uint32_t*)&rx0);
    ret = vmi_read_32_va(vmi, win + symbol_offsets.rc_client_offset + symbol_offsets.rc_right_offset, pid, (uint32_t*)&rx1);
    ret = vmi_read_32_va(vmi, win + symbol_offsets.rc_client_offset + symbol_offsets.rc_top_offset, pid, (uint32_t*)&ry0);
    ret = vmi_read_32_va(vmi, win + symbol_offsets.rc_client_offset + symbol_offsets.rc_bottom_offset, pid, (uint32_t*)&ry1);

    addr_t str_name_off;

    /* Retrieves window name */
    if (VMI_FAILURE != vmi_read_addr_va(vmi, win + symbol_offsets.wnd_strname_offset +
            symbol_offsets.large_unicode_buf_offset, pid, &str_name_off))
    {
        /* Length is always 0, therefore always read 255 chars */
        wn = read_wchar_str_pid(vmi, str_name_off, (size_t)255, pid);
    }

    if (ret == VMI_FAILURE)
        return NULL;

    /* Populate struct, if no failure occured */
    struct wnd* wc = (struct wnd*)malloc(sizeof(struct wnd));
    memset(wc, 0, sizeof(struct wnd));

    wc->r.x0 = x0;
    wc->r.x1 = x1;
    wc->r.y0 = y0;
    wc->r.y1 = y1;
    wc->r.w = x1 - x0;
    wc->r.h = y1 - y0;

    wc->rclient.x0 = rx0;
    wc->rclient.x1 = rx1;
    wc->rclient.y0 = ry0;
    wc->rclient.y1 = ry1;
    wc->rclient.w = rx1 - rx0;
    wc->rclient.h = ry1 - ry0;

    wc->style = style;
    wc->exstyle = exstyle;
    wc->spwnd_addr = win;
    wc->text = wn;
    wc->atom = atom;

    return wc;
}

bool filter_wnd_text(wchar_t* c)
{
    /* Checks, if a target text is present */
    for (size_t j = 0; j < ARRAY_SIZE(BTN_TEXTS); j++)
    {
        if (c)
        {
            const wchar_t* m = wcswcs(c, BTN_TEXTS[j]);
            if (!m)
                continue;

            if (m - c < BTN_THRESHOLD)
            {
                return true;
            }
        }
    }

    return false;
}

int find_button_to_click(vmi_instance_t vmi, struct desktop* desk,
    GArray* wins, struct wnd* btn)
{
    int res = 0;
    /* Current window address */
    addr_t wa = 0;
    /* Current window parsed container */
    struct wnd* wnd = NULL;
    /* Root window used for size calculations */
    struct wnd* root = NULL;
    /* Candidate window */
    struct wnd* cand = NULL;
    /* Visibile part of candidate */
    struct rect* r = NULL;
    /* Maximum button size to consider */
    uint16_t mw, mh;
    /* Signifies a matching wnd text */
    bool match = false;

    if (!wins)
        return -1;

    /* Gets the desktop pane  */
    wa = g_array_index(wins, addr_t, 0);

    root = construct_wnd_container(vmi, desk->providing_pid, wa);
    /*
     * Naive assumption, that buttons will be at least BTN_RATIO times smaller
     * than the respective desktop dimension
     */
    mw = root->r.x1 / BTN_RATIO;
    mh = root->r.y1 / BTN_RATIO;

    /* Frame of desktop */
    size_t w = root->r.x1;
    size_t h =root->r.y1;
    free_wnd_container(root);

    /* Keeping track of occupied screen locations with a bitmap */
    size_t n = ((w + w % 8) / 8) * h;
    char* map = (char*) malloc(sizeof(char) * n);
    memset(map, 0, sizeof(char) * n);

    size_t l = wins->len;

    for (size_t i = 0; i < l; i++)
    {
        wa = g_array_index(wins, addr_t, l - (i+1));
        wnd = construct_wnd_container(vmi, desk->providing_pid, wa);

        /* Performs filtering based on size */
        if (wnd->r.w > mw || wnd->r.h > mh)
        {
            update_visibility_bitmask(map, n, w, &wnd->r);
            free_wnd_container(wnd);
            continue;
        }
        if (wnd->text)
        {
            match = filter_wnd_text(wnd->text);
            if (match)
                cand = wnd;
        }
#ifndef DISABLE_ATOMS
        /* Refines selection by filtering based on window class */
        if (cand && cand->atom )
        {
            struct atom_entry* a = (struct atom_entry*) g_hash_table_lookup(
                    desk->atom_table, GUINT_TO_POINTER(wnd->atom));

            if (a && a->name)
                for (size_t j=0; j< ARRAY_SIZE(IRRELEVANT_ATOM_CLASSES); j++)
                {
                    if (wcscmp(a->name, IRRELEVANT_ATOM_CLASSES[j]) == 0)
                        cand = NULL;
                }
        }
#endif
        if (!cand)
        {
            /* Update visibility */
            update_visibility_bitmask(map, n, w, &wnd->r);

            /* Clean up current wnd struct */
            free_wnd_container(wnd);
            wnd = NULL;
            continue;
        }

        /* Checks visibility of candidate btn */
        r = get_visible_rect_from_bitmask(map, n, w, &cand->r);

        if (r)
        {
            break;
        }

        free_wnd_container(wnd);

        /* Not visible at all, reset candidate */
        cand = NULL;
        wnd = NULL;
    }

    if (cand)
    {
        *btn = *wnd;
        btn->text = wcsdup(wnd->text);
        btn->r = *r;
        res = 1;

        /* Clean up */
        free_wnd_container(cand);
        free(r);
    }

    free(map);

    return res;
}


status_t traverse_windows_pid(vmi_instance_t vmi, addr_t win,
    vmi_pid_t pid, GHashTable* seen_windows, GArray* result_windows, int level)
{
    addr_t* cur = (addr_t*) malloc(sizeof(addr_t));
    *cur = win;

    /* Needed for ordered traversal */
    GArray* wins = g_array_new(true, true, sizeof(addr_t));

    while (*cur)
    {
        if (g_hash_table_contains(seen_windows, (gconstpointer)cur))
        {
            printf("Cycle after %d siblings\n", g_hash_table_size(seen_windows));
            break;
        }

        /* Keeps track of current window in order to detect cycles later */
        g_hash_table_add(seen_windows, (gpointer)cur);

        /* Stores current window for ordered traversal */
        g_array_append_val(wins, *cur);

        /* Advances to next window */
        addr_t* next = (addr_t*) malloc(sizeof(addr_t));
        *next = 0;

        if (VMI_FAILURE == vmi_read_addr_va(vmi, *cur + symbol_offsets.spwnd_next, pid, next))
        {
            free(next);
            g_array_free(wins, true);
            return VMI_FAILURE;
        }
        cur = next;
    }

    if (cur)
        free(cur);

    size_t len = wins->len;
    /*
     * Traverses the windows in the reverse order.
     * This is important to ensure correct Z ordering, since the last window
     * in the linked list is the bottom one.
     */
    for (size_t i = 0; i < len; i++)
    {
        addr_t val = g_array_index(wins, addr_t, len - (i + 1));

        if (!is_wnd_visible(vmi, pid, val))
            continue;

        g_array_append_val(result_windows, val);

        addr_t* child = (addr_t*) malloc(sizeof(uint64_t));

        /* Reads the window's child */
        if (VMI_FAILURE == vmi_read_addr_va(vmi, val + symbol_offsets.spwnd_child, pid, child))
        {
            free(child);
            g_array_free(wins, true);
            return VMI_FAILURE;
        }

        if (child)
        {
            /* Exits the loop, if a window was already processed before */
            if (g_hash_table_contains(seen_windows, (gconstpointer)child))
            {
                free(child);
                break;
            }

            /*
             * Recursive call to process the windows children, its siblings and
             * grandchildren and their respective siblings, grandgrandchildren
             * and so on.
             */
            traverse_windows_pid(vmi, *child, pid, seen_windows, result_windows,
                level + 1);

            free(child);
        }

    }

    g_array_free(wins, true);

    return VMI_SUCCESS;
}

void free_data (gpointer data)
{
    free(data);
}

status_t retrieve_window_addresses(vmi_instance_t vmi, struct desktop* d,
    GArray** result_windows)
{
    uint32_t desk_id = 0;

    addr_t desktop = d->addr;
    vmi_pid_t pid = d->providing_pid;

    addr_t addr = desktop + symbol_offsets.desk_desktopid_off;

    /* Reads desktop ID */
    if (VMI_FAILURE == vmi_read_32_va(vmi, addr, pid, &desk_id))
    {
        printf("\t\tFailed to read desktop ID at %" PRIx64 "\n", desktop +
            symbol_offsets.desk_desktopid_off);
        return VMI_FAILURE;
    }

    addr_t desktop_info;
    addr = desktop + symbol_offsets.desk_pdeskinfo_off;
    /* Retrieves pointer desktop info struct */
    if (VMI_FAILURE == vmi_read_addr_va(vmi, addr, pid, &desktop_info))
    {
        printf("\t\tFailed to read pointer to _DESKTOPINFO at %" PRIx64 "\n",
            desktop + symbol_offsets.desk_pdeskinfo_off);
        return VMI_FAILURE;
    }

    addr_t spwnd = 0;

    addr = desktop_info + symbol_offsets.deskinfo_spwnd_offset;

    /* Retrieves pointer to struct pointer window */
    if (VMI_FAILURE == vmi_read_addr_va(vmi, addr, pid, &spwnd))
    {
        printf("\t\tFailed to read pointer to _WINDOW at %" PRIx64 "\n",
            desktop_info + symbol_offsets.deskinfo_spwnd_offset);
        return VMI_FAILURE;
    }

    if (!spwnd)
    {
        printf("\t\tNo valid windows for _DESKTOPINFO %" PRIx64 "\n", desktop_info);
        return VMI_FAILURE;
    }


    *result_windows = g_array_new(true, true, sizeof(addr_t));

    /* No value destroy function, since the hash table is used as a set  */
    GHashTable* seen_windows = g_hash_table_new_full(g_int64_hash, g_int64_equal,
            free_data, NULL);

    status_t ret = traverse_windows_pid(vmi, spwnd, pid, seen_windows, *result_windows, 0);

    g_hash_table_destroy(seen_windows);

    return ret;
}

/* Traverses this singly-linked list of desktops belonging to one WinSta */
status_t traverse_desktops(vmi_instance_t vmi, addr_t* desktops,
    size_t* max_len, addr_t list_head)
{
    addr_t cur = list_head;
    addr_t next = 0;
    size_t i = 0;

    while (cur)
    {
        if (i < *max_len - 1)
        {
            desktops[i] = cur;
            i++;
        }
        if (VMI_FAILURE == vmi_read_addr_va(vmi, cur + symbol_offsets.desk_rpdesk_next_off, 0, &next))
        {
            printf("Failed to read pointer to next desktop at %" PRIx64 "\n",
                cur + symbol_offsets.desk_rpdesk_next_off);
            *max_len = i;
            return VMI_FAILURE;
        }

        if (next == list_head)
            break;
        cur = next;
    }
    *max_len = i;

    return VMI_SUCCESS;
}

/*
 * Reads relevant data from tagWINDOWSTATION-structs and the children of
 * type tagDESKTOP
 */
status_t populate_winsta(vmi_instance_t vmi, struct winsta* winsta, addr_t addr,
    vmi_pid_t providing_pid)
{
    winsta->addr = addr;

    /*
     * Do it like volatility: Find a process with matching sessionID and take its
     * VA as _MM_SESSION_SPACE
     */
    winsta->providing_pid = providing_pid;

    /* Reads pointer to global atom table */
    if (VMI_FAILURE == vmi_read_addr_va(vmi, addr + symbol_offsets.winsta_pglobal_atom_table_offset, 0, &winsta->atom_table))
    {
        printf("Failed to read pointer to atom table at %" PRIx64 "\n", addr +
            symbol_offsets.winsta_pglobal_atom_table_offset);
        return VMI_FAILURE;
    }

    if (VMI_FAILURE == vmi_read_32_va(vmi, addr + symbol_offsets.winsta_session_id_offset, 0, &winsta->session_id))
    {
        printf("Failed to read session ID at %" PRIx64 "\n", addr + symbol_offsets.winsta_session_id_offset);
        return VMI_FAILURE;
    }

    uint32_t wsf_flags = 0;

    if (VMI_FAILURE == vmi_read_32_va(vmi, addr + symbol_offsets.winsta_wsf_flags, 0, &wsf_flags))
    {
        printf("Failed to read wsfFlags at %" PRIx64 "\n", addr + symbol_offsets.winsta_wsf_flags);
        return VMI_FAILURE;
    }

    winsta->is_interactive = !(wsf_flags & 4);

    addr_t desk = 0;

    if (VMI_FAILURE == vmi_read_addr_va(vmi, addr + symbol_offsets.winsta_rpdesk_list_offset, 0, &desk))
    {
        printf("Failed to read pointer to rpdesklist at %" PRIx64 "\n", addr +
            symbol_offsets.winsta_rpdesk_list_offset);
        return VMI_FAILURE;
    }

    size_t len = 0x10;
    winsta->desktops = (addr_t*)malloc(len * sizeof(addr_t));
    memset(winsta->desktops, 0, sizeof(addr_t) * len);

    if (VMI_FAILURE == traverse_desktops(vmi, winsta->desktops, &len, desk))
    {
        printf("Failed to traverse desktops of winsta at %" PRIx64 "\n", winsta->addr);
        winsta->len_desktops = len;
        return VMI_FAILURE;
    }
    winsta->len_desktops = len;

    winsta->name = retrieve_objhdr_name(vmi, addr);

    return VMI_SUCCESS;
}

/* Iterates over process list an retrieves all tagWINDOWSTATION-structs */
status_t retrieve_winstas_from_procs(vmi_instance_t vmi, GArray** resulting_winstas)
{
    /* Workbench array */
    GArray* winstas = g_array_new(true, true, sizeof(struct winsta*));
    /* Clean up function to free all dynamically allocated member fields */
    g_array_set_clear_func(winstas, (GDestroyNotify) clear_winsta_container);
    /* Sets result to point to workbench */
    *resulting_winstas = winstas;

    addr_t cur_list_entry = symbol_offsets.ps_active_process_head;
    addr_t next_list_entry = 0;

    if (VMI_FAILURE == vmi_read_addr_va(vmi, cur_list_entry, 0, &next_list_entry))
    {
        printf("Failed to read next pointer at %" PRIx64 "\n", cur_list_entry);
        return VMI_FAILURE;
    }

    addr_t current_process = 0;
    vmi_pid_t pid;

    /* Walks the process list */
    while (1)
    {
        /* Calculate offset to the start of _EPROCESS-struct */
        current_process = cur_list_entry - symbol_offsets.active_proc_links_offset;

        vmi_read_32_va(vmi, current_process + symbol_offsets.pid_offset, 0, (uint32_t*)&pid);

        addr_t thrd_list_head = 0;

        /* Retrieves pointer of ThreadListHead-member == associated thread */
        if (VMI_FAILURE == vmi_read_addr_va(vmi, current_process +
                symbol_offsets.thread_list_head_offset, 0, &thrd_list_head))
        {
            printf("Failed to read ThreadListHead-pointer at %" PRIx64 "\n",
                current_process + symbol_offsets.thread_list_head_offset);
            return VMI_FAILURE;
        }

        addr_t cur_thrd_list_entry = thrd_list_head;
        addr_t cur_ethread = 0;
        addr_t next_thread_entry = 0;
        bool is_first = true;

        /* Walks the list of threads belonging to the current process */
        while (1)
        {
            if (!is_first) /* Consecutive calls */
            {
                /* Retrieves pointer of ThreadListHead-member == associated thread */
                if (VMI_FAILURE == vmi_read_addr_va(vmi, cur_thrd_list_entry, 0,
                        &next_thread_entry))
                {
                    printf("Failed to read ThreadListHead-pointer at %" PRIx64 "\n",
                        current_process + symbol_offsets.thread_list_head_offset);
                    return VMI_FAILURE;
                }

                /* All threads processed, exit loop */
                if (next_thread_entry == thrd_list_head)
                {
                    break;
                }

                cur_thrd_list_entry = next_thread_entry;

            }
            else
                is_first = false;

            /* Calculates offset to the start of the _ETHREAD-struct */
            cur_ethread = cur_thrd_list_entry - symbol_offsets.thread_list_entry_offset;


            /* _ETHREAD contains a  _KTHREAD structure (of size 0x200 for Win7) in the beginning */
            addr_t cur_kthread = cur_ethread;
            addr_t teb = 0;

            /* Retrieves pointer to TEB  */
            if (VMI_FAILURE == vmi_read_addr_va(vmi, cur_kthread + symbol_offsets.teb_offset, 0, &teb))
            {
                printf("Failed to read Teb-pointer at %" PRIx64 "\n",
                    cur_kthread + symbol_offsets.teb_offset);
                return VMI_FAILURE;
            }

            addr_t w32thrd_info = 0;

            if (teb < 0x8000000)
            {
                continue;
            }

            /* Retrieves pointer to Win32ThreadInfo-struct */
            if (VMI_FAILURE == vmi_read_addr_va(vmi, teb +
                    symbol_offsets.teb_win32threadinfo_offset, pid, &w32thrd_info))
            {
                continue;
            }

            /* Since not every thread has a THREADINFO-struct, skip thread in this case */
            if (!w32thrd_info) //|| w32thrd_info < 0x8000000
            {
                continue;
            }

            addr_t desktop_info = 0;
            /*
             * Retrieves pointer desktop info struct. This used for determining,
             * whether winsta is present
             */
            if (VMI_FAILURE == vmi_read_addr_va(vmi, w32thrd_info +
                    symbol_offsets.w32t_deskinfo_offset, pid, &desktop_info))
            {
                continue;
            }

            addr_t cur_pwinsta = 0;

            /* Retrieves pointer to winsta struct */
            if (VMI_FAILURE == vmi_read_addr_va(vmi, w32thrd_info +
                    symbol_offsets.w32t_pwinsta_offset, pid, &cur_pwinsta))
            {
                printf("\t\tFailed to read pointer to tagWINDOWSTATION at %" PRIx64
                    "\n", w32thrd_info + symbol_offsets.w32t_pwinsta_offset);
                continue;
            }

            if (cur_pwinsta && cur_pwinsta > 0x1000)
            {
                bool is_known = false;
                size_t i = 0;

                for (; i < winstas->len; i++)
                {
                    struct winsta* cand = g_array_index(winstas, struct winsta*, i);

                    if (!cand)
                        break;
                    if (cand->addr == cur_pwinsta)
                        is_known = true;
                }
                if (!is_known)
                {
                    struct winsta* w = (struct winsta*) malloc(sizeof(struct winsta));
                    memset(w, 0, sizeof(struct winsta));
                    populate_winsta(vmi, w, cur_pwinsta, pid);
                    g_array_append_val(winstas, w);
                }
            }
        }

        cur_list_entry = next_list_entry;

        if (VMI_FAILURE == vmi_read_addr_va(vmi, cur_list_entry, 0, &next_list_entry))
        {
            printf("Failed to read next pointer in loop at %" PRIx64 "\n", cur_list_entry);
            return VMI_FAILURE;
        }

        /*
         * In Windows, the next pointer points to the head of list, this pointer
         * is actually the address of PsActiveProcessHead symbol, not the
         * address of an ActiveProcessLink in EPROCESS struct. It means in
         * Windows, we should stop the loop at the last element in the list
         */
        if (next_list_entry == symbol_offsets.ps_active_process_head)
        {
            break;
        }
    }

    return VMI_SUCCESS;
}

status_t find_first_active_desktop(vmi_instance_t vmi, struct desktop* d)
{
    status_t ret = VMI_FAILURE;
    char* desk_name = NULL;
    GArray* winstas = NULL;
    /* Gathers windows stations with all desktops by iterating over all procs */
    ret = retrieve_winstas_from_procs(vmi, &winstas);

    if (!winstas || ret != VMI_SUCCESS)
    {
        return ret;
    }


    for (size_t i = 0; i < winstas->len; i++)
    {
        struct winsta* w = g_array_index(winstas, struct winsta*, i);

        /* Ignore session 0 */
        if (w->session_id == 0)
            continue;

        /*
         * Discard non-interactive window stations as well as window stations
         * with a name other than "WinSta0"
         */
        if (!w->is_interactive)
            continue;

        /* Only take WinSta0 into account */
        if (w->name && strcmp(w->name, "WinSta0") != 0)
            continue;

        for (size_t j = 0; j < w->len_desktops; j++)
        {
            desk_name = retrieve_objhdr_name(vmi, w->desktops[j]);

            if (desk_name && strncmp(desk_name, "Default\0", 8) != 0)
            {
                free(desk_name);
                continue;
            }

            d->name = desk_name;
            d->providing_pid = w->providing_pid;
            d->addr = w->desktops[j];

#ifndef DISABLE_ATOMS
            d->atom_table = build_atom_table(vmi, w->atom_table);
            g_hash_table_foreach(d->atom_table, print_atom, NULL);
#endif
            ret = VMI_SUCCESS;
        }
    }
    g_array_free(winstas, true);

    return ret;
}

int scan_for_clickable_button(vmi_instance_t vmi, struct desktop* d,
    struct wnd* btn)
{
    int res;

    GArray* win_addresses = NULL;

    if (VMI_FAILURE == retrieve_window_addresses(vmi, d, &win_addresses))
    {
        g_array_free(win_addresses, true);
        return -1;
    }

    res = find_button_to_click(vmi, d, win_addresses, btn);
    g_array_free(win_addresses, true);

    return res;
}

#endif // VMI_WIN_GUI_PARSER_H
