/**
 *  debug.hpp - Debugging variables and function definitions
 *
 *  Copyright 2007, 2008, 2024 Timothy Ringrose
 *
 *  This file is part of cursedmenu.
 *
 *  cursedmenu is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  cursedmenu is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with cursedmenu.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef __DEBUG__
#define __DEBUG__

void debug( std::string program, int indent, std::string message );

#define SUCCESS                                   0
#define NET_UNRESOLVED_HOSTNAME                  10
#define NET_UNABLE_TO_CONNECT                    11
#define NET_SOCK_IO_READ_ERROR                   12
#define NET_SOCK_IO_WRITE_ERROR                  13
#define NET_PROTOCOL_TIMEOUT                     20
#define NET_PROTOCOL_UNKNOWN_RESPONSE            21
#define IO_READ_FILE_ERROR                       50
#define IO_READ_DIR_ERROR                        51
#define IO_WRITE_FILE_ERROR                      52
#define ERROR_INVALID_FILENAME                   97
#define ERROR_INVALID_USAGE                      98
#define ERROR_INVALID_ARG_VALUES                 99
#define ERROR_UNKNOWN                           255

#endif // __DEBUG__
