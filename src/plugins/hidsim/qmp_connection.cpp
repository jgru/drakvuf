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

#include "../plugins.h"
#include "../output_format.h"
#include "qmp_connection.h"

// Member function definitions
qmp_connection::qmp_connection() {};
qmp_connection::~qmp_connection() {};

/* Takes a caller allocated QMPConnection struct and initializes connection */
int qmp_connection::init_conn_unix_sock(const char* sock_path)
{
    this->path = sock_path;
    PRINT_DEBUG("[QMP_CONNECTION] Trying to intialize connection to socket %s.\n", sock_path);

    int ret = -1;

    /* Creates a socket */
    this->fd = socket(AF_UNIX, SOCK_STREAM, 0);

    if (this->fd == -1)
    {
        fprintf(stderr, "socket: %s\n", strerror(errno));
        return this->fd;
    }
    this->sa.sun_family = AF_UNIX;

    /* Check, if path to domain is too long */
    if ((strlen(path) + 1) > sizeof(this->sa.sun_path))
    {
        fprintf(stderr, "path to domain socket exceeds %ld characters\n", sizeof(this->sa.sun_path));
        return this->fd;
    }

    /* Sets socket path */
    strcpy(this->sa.sun_path, path);

    /* Initializes connection */
    ret = connect(this->fd, (struct sockaddr*)&this->sa, sizeof(this->sa));

    if (ret == -1)
    {
        fprintf(stderr, "connect: %s - %s\n", strerror(errno), this->sa.sun_path);
        return -1;
    }

    /* Reads the capabilities, which are sent by the qmp-server */
    ret = read(this->fd, this->buf, sizeof(this->buf));

    /* Stores capabilities for later use */
    this->capabilities = json_tokener_parse(this->buf);
    memset(this->buf, 0, sizeof(this->buf));

    /* Exits capabilities negotiation mode */
    ret = write(this->fd, EXEC_QMP_CAPABILITIES, strlen(EXEC_QMP_CAPABILITIES));

    if (ret == -1)
    {
        fprintf(stderr, "write to %s - %s\n", this->sa.sun_path, strerror(errno));
        return -1;
    }
    /* Check return value */
    ret = read(this->fd, this->buf, sizeof(this->buf));

    if (ret == -1)
    {
        fprintf(stderr, "read from %s - %s\n", this->sa.sun_path, strerror(errno));
        return -1;
    }

    ret = strcmp(this->buf, QMP_SUCCESS);
    if (ret != 0)
    {
        fprintf(stderr, "Capability negotiation failed, msg: %s\n", this->buf);
        return -1;
    }

    PRINT_DEBUG("[QMP_CONNECTION] Connected to Unix Domain Socket %s.\n", sock_path);
    this->is_connected = true;

    return 0;
}

/* Sends the given data to the QMPConnection and reads the response */
int qmp_connection::communicate_json(struct json_object* in, struct json_object** out)
{

    const char* in_str = json_object_to_json_string(in);
    char* out_str = NULL;
    this->communicate(in_str, &out_str);
    *out = json_tokener_parse(out_str);

    if (out_str)
        free(out_str);

    return 0;
}

/* Sends the given data to the QMPConnection and reads the response */
int qmp_connection::communicate(const char* in, char** out)
{

    int ret = write(this->fd, in, strlen(in));

    if (ret == -1)
    {
        fprintf(stderr, "write to %s - %s\n", this->sa.sun_path, strerror(errno));
        return ret;
    }

    /* Reads the response */
    ret = read(this->fd, this->buf, sizeof(this->buf));

    if (ret == -1)
    {
        fprintf(stderr, "read from %s - %s\n", this->sa.sun_path, strerror(errno));
        return ret;
    }
    if (out)
        *out = strdup(this->buf);

    memset(this->buf, 0, sizeof(this->buf));

    return 0;
}

/* Clean-up */
int qmp_connection::close_conn()
{
    close(this->fd);

    if (this->capabilities)
        free(this->capabilities);

    this->is_connected = false;
    return 0;
}

bool qmp_connection::get_connect_state()
{
    return this->is_connected;
}