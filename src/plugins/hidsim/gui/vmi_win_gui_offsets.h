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

#ifndef VMI_WIN_GUI_OFFSETS_H
#define VMI_WIN_GUI_OFFSETS_H

#define LIBVMI_EXTRA_JSON
#include <libvmi/libvmi.h>
#include <libvmi/libvmi_extra.h>

/*
 * Flag in _OBJECT_HEADER InfoMask field, indicating the presence of the
 * specified struct in fornt of the _OBJECT_HEADER
 */
#define OBJ_HDR_INFOMASK_CREATOR_INFO 0x01
#define OBJ_HDR_INFOMASK_NAME 0x02

struct Offsets
{
    addr_t ps_active_process_head;

    /* _OBJECT_HEADER_NAME_INFO  */
    addr_t objhdr_name_info_length;
    addr_t objhdr_name_info_name_offset;  /* _UNICODE_STRING */

    /* _OBJECT_HEADER_CREATOR_INFO  */
    addr_t objhdr_creator_info_length;

    /* _OBJECT_HEADER */
    addr_t objhdr_length; /* Nominal length */

    /*
     * Offset to the body, which resembles the effective length,
     * as it is seen from the executive object
     */
    addr_t objhdr_body_offset;
    addr_t objhdr_infomask_offset;

    /* For _EPROCESS */
    addr_t active_proc_links_offset;
    addr_t pid_offset;
    addr_t name_offset;
    addr_t thread_list_head_offset;

    /* For _ETRHEAD */
    addr_t thread_list_entry_offset;
    addr_t tcb_offset;

    /* For _KTHREAD */
    addr_t teb_offset;

    /* For TEB */
    addr_t teb_win32threadinfo_offset;
    addr_t w32t_deskinfo_offset;
    addr_t w32t_pwinsta_offset;

    /* For tagWINDOWSTATION */
    addr_t winsta_session_id_offset;
    addr_t winsta_pglobal_atom_table_offset;
    addr_t winsta_rpdesk_list_offset;
    addr_t winsta_wsf_flags;

    /* For _RTL_ATOM_TABLE */
    addr_t atom_table_buckets_off;
    addr_t atom_table_num_buckets_off;

    /* For _RTL_ATOM_TABLE_ENTRY */
    addr_t atom_entry_atom_offset;
    addr_t atom_entry_name_offset;
    addr_t atom_entry_name_len_offset;
    addr_t atom_entry_ref_count_offset;
    addr_t atom_entry_hashlink_offset;

    /* For tagDESKTOP */
    addr_t desk_pdeskinfo_off;
    addr_t desk_rpdesk_next_off;
    addr_t desk_pwinsta_parent;
    addr_t desk_desktopid_off;

    /* For tagDESKTOPINFO */
    addr_t deskinfo_spwnd_offset;

    /* For tagWND */
    addr_t spwnd_next;
    addr_t spwnd_child;
    addr_t wnd_style;
    addr_t wnd_exstyle;
    addr_t pcls_offset;
    /* tagRECT of window */
    addr_t rc_wnd_offset;
    /* tagRECT of clients within the window */
    addr_t rc_client_offset;
    /* Offset to _LARGE_UNICODE_STRING */
    addr_t wnd_strname_offset;

    addr_t large_unicode_buf_offset;

    /* For tagRECT */
    addr_t rc_left_offset;
    addr_t rc_top_offset;
    addr_t rc_right_offset;
    addr_t rc_bottom_offset;

    /* For tagCLS */
    addr_t cls_atom_offset;
} symbol_offsets;

status_t find_offsets_from_ntkr_json(vmi_instance_t vmi, json_object* profile, bool is_x86)
{
    /*
     * Parse IST-file containing debugging information to retrieve offsets
     * Might have just used `vmi_get_kernel_json(vmi);`, but using
     * the supplied IST-file is more explicit
     */

    if (!profile)
    {
        fprintf(stderr, "Kernel-JSON-profile was not provided\n");
        return VMI_FAILURE;
    }

    /* Find size of _OBJECT_HEADER */
    if (VMI_FAILURE == vmi_get_struct_size_from_json(
            vmi, profile, "_OBJECT_HEADER",
            &symbol_offsets.objhdr_length))
    {
        printf("Error retrieving size of _OBJECT_HEADER-struct : %ld\n",
            symbol_offsets.objhdr_length);
        return VMI_FAILURE;
    }

    /* Find offset to the body of _OBJECT_HEADER, which is the effective length */
    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, profile, "_OBJECT_HEADER", "Body",
            &symbol_offsets.objhdr_body_offset))
    {
        printf("Error retrieving offset to Body-field of _OBJECT_HEADER: %ld\n",
            symbol_offsets.objhdr_body_offset);
        return VMI_FAILURE;
    }

    /* Find size of _OBJECT_HEADER_CREATOR_INFO */
    if (VMI_FAILURE == vmi_get_struct_size_from_json(
            vmi, profile, "_OBJECT_HEADER_CREATOR_INFO",
            &symbol_offsets.objhdr_creator_info_length))
    {
        printf("Error retrieving size of -struct : %ld\n",
            symbol_offsets.objhdr_creator_info_length);
        return VMI_FAILURE;
    }

    /* Retrieve offset to InfoMask-field of _OBJECT_HEADER */
    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, profile, "_OBJECT_HEADER", "InfoMask",
            &symbol_offsets.objhdr_infomask_offset))
    {
        printf("Error retrieving offset to InfoMask-field of _OBJECT_HEADER: %ld\n",
            symbol_offsets.objhdr_infomask_offset);
        return VMI_FAILURE;
    }

    /* Find size of _OBJECT_HEADER_NAME_INFO */
    if (VMI_FAILURE == vmi_get_struct_size_from_json(
            vmi, profile, "_OBJECT_HEADER_NAME_INFO",
            &symbol_offsets.objhdr_name_info_length))
    {
        printf("Error retrieving size of _OBJECT_HEADER_NAME_INFO-struct : %ld\n",
            symbol_offsets.objhdr_name_info_length);
        return VMI_FAILURE;
    }

    /* Find offset to Name-field of _OBJECT_HEADER_NAME_INFO */
    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, profile, "_OBJECT_HEADER_NAME_INFO", "Name",
            &symbol_offsets.objhdr_name_info_name_offset))
    {
        printf("Error retrieving name-offset in _OBJECT_HEADER_NAME_INFO: %ld\n",
            symbol_offsets.objhdr_name_info_name_offset);
        return VMI_FAILURE;
    }

    /* Find offsets within _EPROCESS */
    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, profile, "_EPROCESS", "ActiveProcessLinks",
            &symbol_offsets.active_proc_links_offset))
    {
        printf("Error retrieving ActiveProcessLinks-offset : %ld\n",
            symbol_offsets.active_proc_links_offset);
        return VMI_FAILURE;
    }

    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, profile, "_EPROCESS", "UniqueProcessId",
            &symbol_offsets.pid_offset))
    {
        printf("Error retrieving UniqueProcessId-offset : %ld\n", symbol_offsets.pid_offset);
        return VMI_FAILURE;
    }

    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, profile, "_EPROCESS", "ImageFileName",
            &symbol_offsets.name_offset))
    {
        printf("Error retrieving ImageFileName-offset : %ld\n", symbol_offsets.name_offset);
        return VMI_FAILURE;
    }

    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, profile, "_EPROCESS", "ThreadListHead",
            &symbol_offsets.thread_list_head_offset))
    {
        printf("Error retrieving ThreadListHead-offset : %ld\n",
            symbol_offsets.thread_list_head_offset);
        return VMI_FAILURE;
    }

    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, profile, "_ETHREAD", "ThreadListEntry",
            &symbol_offsets.thread_list_entry_offset))
    {
        printf("Error retrieving ThreadListEntry-offset : %ld\n",
            symbol_offsets.thread_list_entry_offset);
        return VMI_FAILURE;
    }

    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, profile, "_ETHREAD", "Tcb", &symbol_offsets.tcb_offset))
    {
        printf("Error retrieving Tcb-offset : %ld\n", symbol_offsets.tcb_offset);
        return VMI_FAILURE;
    }

    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, profile, "_ETHREAD", "Teb", &symbol_offsets.teb_offset))
    {
        printf("Error retrieving TEB at offset %ld\n", symbol_offsets.tcb_offset);
        return VMI_FAILURE;
    }

    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, profile, "_RTL_ATOM_TABLE", "Buckets",
            &symbol_offsets.atom_table_buckets_off))
    {
        printf("Error retrieving Buckets-offset of _RTL_ATOM_TABLE\n");
        return VMI_FAILURE;
    }

    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, profile, "_RTL_ATOM_TABLE", "NumberOfBuckets",
            &symbol_offsets.atom_table_num_buckets_off))
    {
        printf("Error retrieving Number of Buckets of _RTL_ATOM_TABLE\n");
        return VMI_FAILURE;
    }

    /*
     * Kernel-PDB-file supplies *wrong* offset, its not 0x3c! buf either 0xC,
     * 0x18 or 0x58 (on patched versions)
     *
     * This is a preprocessor thing as stated here:
     * https://code.google.com/archive/p/volatility/issues/131
     * https://github.com/volatilityfoundation/volatility/blob/\
     * a438e768194a9e05eb4d9ee9338b881c0fa25937/volatility/plugins/\
     * gui/win32k_core.py#L659
     */
    if (is_x86)
    {
        symbol_offsets.atom_table_num_buckets_off = 0xC;
        symbol_offsets.atom_table_buckets_off = 0x10;
    }
    else
    {
        symbol_offsets.atom_table_num_buckets_off = 0x18;
        symbol_offsets.atom_table_buckets_off = 0x20;
    }

    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, profile, "_RTL_ATOM_TABLE_ENTRY", "HashLink",
            &symbol_offsets.atom_entry_hashlink_offset))
    {
        printf("Error retrieving offset toHashLink of _RTL_ATOM_TABLE_ENTRY\n");
        return VMI_FAILURE;
    }

    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, profile, "_RTL_ATOM_TABLE_ENTRY", "Name",
            &symbol_offsets.atom_entry_name_offset))
    {
        printf("Error retrieving offset to Name of _RTL_ATOM_TABLE_ENTRY\n");
        return VMI_FAILURE;
    }

    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, profile, "_RTL_ATOM_TABLE_ENTRY", "NameLength",
            &symbol_offsets.atom_entry_name_len_offset))
    {
        printf("Error retrieving offset to NameLength of _RTL_ATOM_TABLE_ENTRY\n");
        return VMI_FAILURE;
    }

    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, profile, "_RTL_ATOM_TABLE_ENTRY", "Atom",
            &symbol_offsets.atom_entry_atom_offset))
    {
        printf("Error retrieving offset to Atom of _RTL_ATOM_TABLE_ENTRY\n");
        return VMI_FAILURE;
    }

    return VMI_SUCCESS;
}

status_t find_offsets_from_win32k_json(vmi_instance_t vmi, json_object* w32k_json)
{

    if (!w32k_json)
    {
        fprintf(stderr, "Win32k-JSON was not provided\n");
        return VMI_FAILURE;
    }

    /* Reads offset to Win32ThreadInfo-struct from beginning of _TEB */
    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, w32k_json, "_TEB", "Win32ThreadInfo",
            &symbol_offsets.teb_win32threadinfo_offset))
    {
        fprintf(stderr, "Error reading offset to Win32ThreadInfo from _TEB\n");
        return VMI_FAILURE;
    }

    /*
     * Reads offset to pDeskInfo from beginning of tagTHREADINFO, which is a
     * Win32ThreadInfo-struct retrieved in the previous call
     */
    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, w32k_json, "tagTHREADINFO", "pDeskInfo",
            &symbol_offsets.w32t_deskinfo_offset))
    {
        fprintf(stderr, "Error reading offset to pDeskInfo from tagTHREADINFO\n");
        return VMI_FAILURE;
    }

    /* Reads offset to pwinsta from beginning of tagTHREADINFO */
    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, w32k_json, "tagTHREADINFO", "pwinsta",
            &symbol_offsets.w32t_pwinsta_offset ))
    {
        fprintf(stderr, "Error reading offset to pDeskInfo from tagTHREADINFO\n");
        return VMI_FAILURE;
    }

    /* Reads offset to dwSessionId from beginning of tagWINDOWSTATION */
    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, w32k_json, "tagWINDOWSTATION", "dwSessionId",
            &symbol_offsets.winsta_session_id_offset))
    {
        fprintf(stderr, "Error reading offset to session ID from tagWINDOWSTATION\n");
        return VMI_FAILURE;
    }

    /* Reads offset to pGlobalAtomTable from beginning of tagWINDOWSTATION */
    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, w32k_json, "tagWINDOWSTATION", "pGlobalAtomTable",
            &symbol_offsets.winsta_pglobal_atom_table_offset))
    {
        fprintf(stderr, "Error reading offset to pGlobalAtomTable from tagWINDOWSTATION\n");
        return VMI_FAILURE;
    }

    /* Reads offset to rpdeskList from beginning of tagWINDOWSTATION */
    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, w32k_json, "tagWINDOWSTATION", "rpdeskList",
            &symbol_offsets.winsta_rpdesk_list_offset))
    {
        fprintf(stderr, "Error reading offset to rpdeskList from tagWINDOWSTATION\n");
        return VMI_FAILURE;
    }

    /* Reads offset to dwWSF_Flags from beginning of tagWINDOWSTATION */
    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, w32k_json, "tagWINDOWSTATION", "dwWSF_Flags",
            &symbol_offsets.winsta_wsf_flags))
    {
        fprintf(stderr, "Error reading offset to dwWSF_Flags from tagWINDOWSTATION\n");
        return VMI_FAILURE;
    }

    /* Reads offset to pDeskInfo from beginning of tagDESKTOP */
    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, w32k_json, "tagDESKTOP", "pDeskInfo",
            &symbol_offsets.desk_pdeskinfo_off))
    {
        fprintf(stderr, "Error reading offset to pDeskInfo from tagDESKTOP\n");
        return VMI_FAILURE;
    }

    /* Reads offset to rpdeskNext from beginning of tagDESKTOP */
    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, w32k_json, "tagDESKTOP", "rpdeskNext",
            &symbol_offsets.desk_rpdesk_next_off))
    {
        fprintf(stderr, "Error reading offset to desk_rpdesk_next_off from tagDESKTOP\n");
        return VMI_FAILURE;
    }

    /* Reads offset to rpwinstaParent from beginning of tagDESKTOP */
    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, w32k_json, "tagDESKTOP", "rpwinstaParent",
            &symbol_offsets.desk_pwinsta_parent))
    {
        fprintf(stderr, "Error reading offset to rpwinstaParent from tagDESKTOP\n");
        return VMI_FAILURE;
    }

    /* Reads offset to rpdeskNext from beginning of tagDESKTOP */
    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, w32k_json, "tagDESKTOP", "dwDesktopId",
            &symbol_offsets.desk_desktopid_off))
    {
        fprintf(stderr, "Error reading offset to dwDesktopId from tagDESKTOP\n");
        return VMI_FAILURE;
    }

    /* Reads offset to spwnd from beginning of tagDESKTOPINFO */
    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, w32k_json, "tagDESKTOPINFO", "spwnd",
            &symbol_offsets.deskinfo_spwnd_offset))
    {
        fprintf(stderr, "Error reading offset to dwDesktopId from tagDESKTOP\n");
        return VMI_FAILURE;
    }

    /* Reads offset to rcWindow from beginning of tagWND */
    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, w32k_json, "tagWND", "rcWindow",
            &symbol_offsets.rc_wnd_offset))
    {
        fprintf(stderr, "Error reading offset to rcWindow from tagWND\n");
        return VMI_FAILURE;
    }

    /* Reads offset to rcWindow from beginning of tagWND */
    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, w32k_json, "tagWND", "rcWindow",
            &symbol_offsets.rc_wnd_offset))
    {
        fprintf(stderr, "Error reading offset to rcWindow from tagWND\n");
        return VMI_FAILURE;
    }

    /* Reads offset to rcClient from beginning of tagWND */
    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, w32k_json, "tagWND", "rcClient",
            &symbol_offsets.rc_client_offset))
    {
        fprintf(stderr, "Error reading offset to rcClient from tagWND\n");
        return VMI_FAILURE;
    }

    /* Reads offset to spwndNext from beginning of tagWND */
    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, w32k_json, "tagWND", "spwndNext",
            &symbol_offsets.spwnd_next))
    {
        fprintf(stderr, "Error reading offset to spwndNext from tagWND\n");
        return VMI_FAILURE;
    }

    /* Reads offset to rcClient from beginning of tagWND */
    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, w32k_json, "tagWND", "spwndChild",
            &symbol_offsets.spwnd_child))
    {
        fprintf(stderr, "Error reading offset to spwndChild from tagWND\n");
        return VMI_FAILURE;
    }

    /* Reads offset to style from beginning of tagWND */
    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, w32k_json, "tagWND", "style",
            &symbol_offsets.wnd_style))
    {
        fprintf(stderr, "Error reading offset to style from tagWND\n");
        return VMI_FAILURE;
    }

    /* Reads offset to ExStyle from beginning of tagWND */
    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, w32k_json, "tagWND", "ExStyle",
            &symbol_offsets.wnd_exstyle))
    {
        fprintf(stderr, "Error reading offset to ExStyle from tagWND\n");
        return VMI_FAILURE;
    }

    /* Reads offset to pcls from beginning of tagWND */
    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, w32k_json, "tagWND", "pcls",
            &symbol_offsets.pcls_offset))
    {
        fprintf(stderr, "Error reading offset to pcls from tagWND\n");
        return VMI_FAILURE;
    }

    /* Reads offset to strName from beginning of tagWND */
    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, w32k_json, "tagWND", "strName",
            &symbol_offsets.wnd_strname_offset))
    {
        fprintf(stderr, "Error reading offset to strName from tagWND\n");
        return VMI_FAILURE;
    }

    /* Reads offset to Buffer from beginning of _LARGE_UNICODE_STRING */
    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, w32k_json, "_LARGE_UNICODE_STRING", "Buffer",
            &symbol_offsets.large_unicode_buf_offset))
    {
        fprintf(stderr, "Error reading offset to Buffer from _LARGE_UNICODE_STRING\n");
        return VMI_FAILURE;
    }

    /* Reads offset to left-field from beginning of tagRECT */
    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, w32k_json, "tagRECT", "left",
            &symbol_offsets.rc_left_offset))
    {
        fprintf(stderr, "Error reading offset to left-field from tagRECT\n");
        return VMI_FAILURE;
    }

    /* Reads offset to top-field from beginning of tagRECT */
    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, w32k_json, "tagRECT", "top",
            &symbol_offsets.rc_top_offset))
    {
        fprintf(stderr, "Error reading offset to top-field from tagRECT\n");
        return VMI_FAILURE;
    }

    /* Reads offset to right-field from beginning of tagRECT */
    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, w32k_json, "tagRECT", "right",
            &symbol_offsets.rc_right_offset))
    {
        fprintf(stderr, "Error reading offset to right-field from tagRECT\n");
        return VMI_FAILURE;
    }

    /* Reads offset to bottom-field from beginning of tagRECT */
    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, w32k_json, "tagRECT", "bottom",
            &symbol_offsets.rc_bottom_offset))
    {
        fprintf(stderr, "Error reading offset to bottom-field from tagRECT\n");
        return VMI_FAILURE;
    }

    /*
     * Reads offset to atomClassName from beginning of tagCLS
     *
     * This contains the key for the atom table
     * See https://www.geoffchappell.com/studies/windows/win32/user32/structs/\
     * cls.htm?tx=56
     */
    if (VMI_FAILURE == vmi_get_struct_member_offset_from_json(
            vmi, w32k_json, "tagCLS", "atomClassName",
            &symbol_offsets.cls_atom_offset))
    {
        fprintf(stderr, "Error reading offset to atomClassName from tagCLS\n");
        return VMI_FAILURE;
    }

    return VMI_SUCCESS;
}

status_t initialize_offsets(vmi_instance_t vmi, json_object* kernel_json, json_object* win32k_json, bool is_x86)
{

    if (VMI_FAILURE == vmi_read_addr_ksym(vmi, "PsActiveProcessHead",
            &symbol_offsets.ps_active_process_head))
    {
        printf("Failed to find PsActiveProcessHead\n");
        return VMI_FAILURE;
    }

    if (VMI_FAILURE == find_offsets_from_ntkr_json(vmi, kernel_json, is_x86))
        return VMI_FAILURE;

    if (VMI_FAILURE == find_offsets_from_win32k_json(vmi, win32k_json))
        return VMI_FAILURE;

    return VMI_SUCCESS;
}
#endif // VMI_WIN_GUI_OFFSETS_H
