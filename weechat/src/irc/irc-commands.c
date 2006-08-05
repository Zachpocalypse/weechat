/*
 * Copyright (c) 2003-2006 by FlashCode <flashcode@flashtux.org>
 * See README for License detail, AUTHORS for developers list.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

/* irc-commands.c: implementation of IRC commands, according to
                   RFC 1459,2810,2811,2812 */


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "../common/weechat.h"
#include "irc.h"
#include "../common/command.h"


t_irc_command irc_commands[] =
{ { "admin", N_("find information about the administrator of the server"),
    N_("[target]"),
    N_("target: server"),
    NULL, 0, 1, 0, 1, NULL, irc_cmd_send_admin, NULL },
  { "ame", N_("send a CTCP action to all channels of all connected servers"),
    N_("message"),
    N_("message: message to send"),
    "", 1, MAX_ARGS, 1, 1, NULL, irc_cmd_send_ame, NULL },
  { "amsg", N_("send message to all channels of all connected servers"),
    N_("text"),
    N_("text: text to send"),
    "", 1, MAX_ARGS, 1, 1, NULL, irc_cmd_send_amsg, NULL },
  { "away", N_("toggle away status"),
    N_("[-all] [message]"),
    N_("   -all: toggle away status on all connected servers\n"
       "message: message for away (if no message is given, away status is removed)"),
    "-all", 0, MAX_ARGS, 1, 1, NULL, irc_cmd_send_away, NULL },
  { "ban", N_("bans nicks or hosts"),
    N_("[channel] [nickname [nickname ...]]"),
    N_(" channel: channel for ban\n"
       "nickname: user or host to ban"),
    "%N", 0, MAX_ARGS, 0, 1, NULL, irc_cmd_send_ban, NULL },
  { "ctcp", N_("send a CTCP message (Client-To-Client Protocol)"),
    N_("nickname type [arguments]"),
    N_(" nickname: user to send CTCP to\n"
       "     type: CTCP type (examples: \"version\", \"ping\", ..)\n"
       "arguments: arguments for CTCP"),
    "%n action|ping|version", 2, MAX_ARGS, 1, 1, NULL, irc_cmd_send_ctcp, NULL },
  { "cycle", N_("leave and rejoin a channel"),
    N_("[channel[,channel]] [part_message]"),
    N_("     channel: channel name for cycle\n"
       "part_message: part message (displayed to other users)"),
    "%p", 0, MAX_ARGS, 0, 1, NULL, irc_cmd_send_cycle, NULL },
  { "dehalfop", N_("removes half channel operator status from nickname(s)"),
    N_("[nickname [nickname]]"), "",
    "", 0, MAX_ARGS, 0, 1, irc_cmd_send_dehalfop, NULL, NULL },
  { "deop", N_("removes channel operator status from nickname(s)"),
    N_("[nickname [nickname]]"), "",
    "", 0, MAX_ARGS, 0, 1, irc_cmd_send_deop, NULL, NULL },
  { "devoice", N_("removes voice from nickname(s)"),
    N_("[nickname [nickname]]"), "",
    "", 0, MAX_ARGS, 0, 1, irc_cmd_send_devoice, NULL, NULL },
  { "die", N_("shutdown the server"), "", "",
    NULL, 0, 0, 0, 1, NULL, irc_cmd_send_die, NULL },
  { "error", N_("error received from IRC server"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_error },
  { "halfop", N_("gives half channel operator status to nickname(s)"),
    N_("[nickname [nickname]]"), "",
    "", 0, MAX_ARGS, 0, 1, irc_cmd_send_halfop, NULL, NULL },
  { "info", N_("get information describing the server"),
    N_("[target]"),
    N_("target: server name"),
    NULL, 0, 1, 0, 1, NULL, irc_cmd_send_info, NULL },
  { "invite", N_("invite a nick on a channel"),
    N_("nickname channel"),
    N_("nickname: nick to invite\n"
       " channel: channel to invite"),
    "%n %c", 1, 2, 0, 1, irc_cmd_send_invite, NULL, irc_cmd_recv_invite },
  { "ison", N_("check if a nickname is currently on IRC"),
    N_("nickname [nickname ...]"),
    N_("nickname: nickname"),
    "", 1, MAX_ARGS, 0, 1, NULL, irc_cmd_send_ison, NULL },
  { "join", N_("join a channel"),
    N_("channel[,channel] [key[,key]]"),
    N_("channel: channel name to join\n"
       "    key: key to join the channel"),
    NULL, 1, MAX_ARGS, 0, 1, NULL, irc_cmd_send_join, irc_cmd_recv_join },
  { "kick", N_("forcibly remove a user from a channel"),
    N_("[channel] nickname [comment]"),
    N_(" channel: channel where user is\n"
       "nickname: nickname to kick\n"
       " comment: comment for kick"),
    "%n %-", 1, MAX_ARGS, 0, 1, NULL, irc_cmd_send_kick, irc_cmd_recv_kick },
  { "kickban", N_("kicks and bans a nick from a channel"),
    N_("[channel] nickname [comment]"),
    N_(" channel: channel where user is\n"
       "nickname: nickname to kick and ban\n"
       " comment: comment for kick"),
    "%n %-", 1, MAX_ARGS, 0, 1, NULL, irc_cmd_send_kickban, NULL },
  { "kill", N_("close client-server connection"),
    N_("nickname comment"),
    N_("nickname: nickname\n"
       " comment: comment for kill"),
    "%n %-", 2, MAX_ARGS, 0, 1, NULL, irc_cmd_send_kill, irc_cmd_recv_kill },
  { "links", N_("list all servernames which are known by the server answering the query"),
    N_("[[server] server_mask]"),
    N_("     server: this server should answer the query\n"
       "server_mask: list of servers must match this mask"),
    NULL, 0, 2, 0, 1, NULL, irc_cmd_send_links, NULL },
  { "list", N_("list channels and their topic"),
    N_("[channel[,channel] [server]]"),
    N_("channel: channel to list (a regexp is allowed)\nserver: server name"),
    NULL, 0, MAX_ARGS, 0, 1, NULL, irc_cmd_send_list, NULL },
  { "lusers", N_("get statistics about the size of the IRC network"),
    N_("[mask [target]]"),
    N_("  mask: servers matching the mask only\n"
       "target: server for forwarding request"),
    NULL, 0, 2, 0, 1, NULL, irc_cmd_send_lusers, NULL },
  { "me", N_("send a CTCP action to the current channel"),
    N_("message"),
    N_("message: message to send"),
    "", 0, MAX_ARGS, 1, 1, NULL, irc_cmd_send_me, NULL },
  { "mode", N_("change channel or user mode"),
    N_("{ channel {[+|-]|o|p|s|i|t|n|b|v} [limit] [user] [ban mask] } | "
       "{ nickname {[+|-]|i|w|s|o} }"),
    N_("channel modes:\n"
       "  channel: channel name to modify\n"
       "  o: give/take channel operator privileges\n"
       "  p: private channel flag\n"
       "  s: secret channel flag\n"
       "  i: invite-only channel flag\n"
       "  t: topic settable by channel operator only flag\n"
       "  n: no messages to channel from clients on the outside\n"
       "  m: moderated channel\n"
       "  l: set the user limit to channel\n"
       "  b: set a ban mask to keep users out\n"
       "  e: set exception mask\n"
       "  v: give/take the ability to speak on a moderated channel\n"
       "  k: set a channel key (password)\n"
       "user modes:\n"
       "  nickname: nickname to modify\n"
       "  i: mark a user as invisible\n"
       "  s: mark a user for receive server notices\n"
       "  w: user receives wallops\n"
       "  o: operator flag"),
    "", 1, MAX_ARGS, 0, 1, NULL, irc_cmd_send_mode, irc_cmd_recv_mode },
  { "motd", N_("get the \"Message Of The Day\""),
    N_("[target]"),
    N_("target: server name"),
    NULL, 0, 1, 0, 1, NULL, irc_cmd_send_motd, NULL },
  { "msg", N_("send message to a nick or channel"),
    N_("receiver[,receiver] text"),
    N_("receiver: nick or channel (may be mask, '*' = current channel)\n"
       "text: text to send"),
    "", 2, MAX_ARGS, 1, 1, NULL, irc_cmd_send_msg, NULL },
  { "names", N_("list nicknames on channels"),
    N_("[channel[,channel]]"),
    N_("channel: channel name"),
    NULL, 0, 1, 0, 1, NULL, irc_cmd_send_names, NULL },
  { "nick", N_("change current nickname"),
    N_("[-all] nickname"),
    N_("    -all: set new nickname for all connected servers\n"
       "nickname: new nickname"),
    "-all", 1, 2, 0, 0, irc_cmd_send_nick, NULL, irc_cmd_recv_nick },
  { "notice", N_("send notice message to user"),
    N_("nickname text"),
    N_("nickname: user to send notice to\n"
       "    text: text to send"),
    "%n %-", 2, MAX_ARGS, 1, 1, NULL, irc_cmd_send_notice, irc_cmd_recv_notice },
  { "op", N_("gives channel operator status to nickname(s)"),
    N_("nickname [nickname]"), "",
    "", 1, MAX_ARGS, 0, 1, irc_cmd_send_op, NULL, NULL },
  { "oper", N_("get operator privileges"),
    N_("user password"),
    N_("user/password: used to get privileges on current IRC server"),
    NULL, 2, 2, 0, 1, NULL, irc_cmd_send_oper, NULL },
  { "part", N_("leave a channel"),
    N_("[channel[,channel]] [part_message]"),
    N_("     channel: channel name to leave\n"
       "part_message: part message (displayed to other users)"),
    "%p", 0, MAX_ARGS, 0, 1, NULL, irc_cmd_send_part, irc_cmd_recv_part },
  { "ping", N_("ping server"),
    N_("server1 [server2]"),
    N_("server1: server to ping\nserver2: forward ping to this server"),
    NULL, 1, 2, 0, 1, NULL, irc_cmd_send_ping, irc_cmd_recv_ping },
  { "pong", N_("answer to a ping message"),
    N_("daemon [daemon2]"),
    N_(" daemon: daemon who has responded to Ping message\n"
       "daemon2: forward message to this daemon"),
    NULL, 1, 2, 0, 1, NULL, irc_cmd_send_pong, irc_cmd_recv_pong },
  { "privmsg", N_("message received"), "", "",
    "", 0, 0, 1, 1, NULL, NULL, irc_cmd_recv_privmsg },
  { "query", N_("send a private message to a nick"),
    N_("nickname [text]"),
    N_("nickname: nickname for private conversation\n"
       "    text: text to send"),
    "%n %-", 1, MAX_ARGS, 1, 1, NULL, irc_cmd_send_query, NULL },
  { "quit", N_("close all connections and quit"),
    N_("[quit_message]"),
    N_("quit_message: quit message (displayed to other users)"),
    "%q", 0, MAX_ARGS, 1, 0, NULL, irc_cmd_send_quit, irc_cmd_recv_quit },
  { "quote", N_("send raw data to server without parsing"),
    N_("data"),
    N_("data: raw data to send"),
    "", 1, MAX_ARGS, 1, 1, NULL, irc_cmd_send_quote, NULL },
  { "rehash", N_("tell the server to reload its config file"), "", "",
    NULL, 0, 0, 0, 1, NULL, irc_cmd_send_rehash, NULL },
  { "restart", N_("tell the server to restart itself"), "", "",
    NULL, 0, 0, 0, 1, NULL, irc_cmd_send_restart, NULL },
  { "service", N_("register a new service"),
    N_("nickname reserved distribution type reserved info"),
    N_("distribution: visibility of service\n"
       "        type: reserved for future usage"),
    NULL, 6, 6, 0, 1, NULL, irc_cmd_send_service, NULL },
  { "servlist", N_("list services currently connected to the network"),
    N_("[mask [type]]"),
    N_("mask: list only services matching this mask\n"
       "type: list only services of this type"),
    NULL, 0, 2, 0, 1, NULL, irc_cmd_send_servlist, NULL },
  { "squery", N_("deliver a message to a service"),
    N_("service text"),
    N_("service: name of service\ntext: text to send"),
    NULL, 2, MAX_ARGS, 1, 1, NULL, irc_cmd_send_squery, NULL },
  { "squit", N_("disconnect server links"),
    N_("server comment"),
    N_( "server: server name\n"
       "comment: comment for quit"),
    NULL, 2, 2, 1, 1, NULL, irc_cmd_send_squit, NULL },
  { "stats", N_("query statistics about server"),
    N_("[query [server]]"),
    N_(" query: c/h/i/k/l/m/o/y/u (see RFC1459)\n"
       "server: server name"),
    NULL, 0, 2, 0, 1, NULL, irc_cmd_send_stats, NULL },
  { "summon", N_("give users who are on a host running an IRC server a message "
    "asking them to please join IRC"),
    N_("user [target [channel]]"),
    N_("   user: username\ntarget: server name\n"
       "channel: channel name"),
    NULL, 1, 3, 0, 1, NULL, irc_cmd_send_summon, NULL },
  { "time", N_("query local time from server"),
    N_("[target]"),
    N_("target: query time from specified server"),
    NULL, 0, 1, 0, 1, NULL, irc_cmd_send_time, NULL },
  { "topic", N_("get/set channel topic"),
    N_("[channel] [topic]"),
    N_("channel: channel name\ntopic: new topic for channel "
       "(if topic is \"-delete\" then topic is deleted)"),
    "%t|-delete", 0, MAX_ARGS, 1, 1, NULL, irc_cmd_send_topic, irc_cmd_recv_topic },
  { "trace", N_("find the route to specific server"),
    N_("[target]"),
    N_("target: server"),
    NULL, 0, 1, 0, 1, NULL, irc_cmd_send_trace, NULL },
  { "unban", N_("unbans nicks or hosts"),
    N_("[channel] nickname [nickname ...]"),
    N_(" channel: channel for unban\n"
       "nickname: user or host to unban"),
    "", 1, MAX_ARGS, 0, 1, NULL, irc_cmd_send_unban, NULL },
  { "userhost", N_("return a list of information about nicknames"),
    N_("nickname [nickname ...]"),
    N_("nickname: nickname"),
    "%n", 1, MAX_ARGS, 0, 1, NULL, irc_cmd_send_userhost, NULL },
  { "users", N_("list of users logged into the server"),
    N_("[target]"),
    N_("target: server"),
    NULL, 0, 1, 0, 1, NULL, irc_cmd_send_users, NULL },
  { "version", N_("gives the version info of nick or server (current or specified)"),
    N_("[server | nickname]"),
    N_("  server: server name\n"
       "nickname: nickname"),
    NULL, 0, 1, 0, 1, NULL, irc_cmd_send_version, NULL },
  { "voice", N_("gives voice to nickname(s)"),
    N_("[nickname [nickname]]"), "",
    "", 0, MAX_ARGS, 0, 1, irc_cmd_send_voice, NULL, NULL },
  { "wallops", N_("send a message to all currently connected users who have "
    "set the 'w' user mode for themselves"),
    N_("text"),
    N_("text to send"),
    NULL, 1, MAX_ARGS, 1, 1, NULL, irc_cmd_send_wallops, NULL },
  { "who", N_("generate a query which returns a list of information"),
    N_("[mask [\"o\"]]"),
    N_("mask: only information which match this mask\n"
       "   o: only operators are returned according to the mask supplied"),
    "%C", 0, 2, 0, 1, NULL, irc_cmd_send_who, NULL },
  { "whois", N_("query information about user(s)"),
    N_("[server] nickname[,nickname]"),
    N_("  server: server name\n"
       "nickname: nickname (may be a mask)"),
    "", 1, MAX_ARGS, 0, 1, NULL, irc_cmd_send_whois, NULL },
  { "whowas", N_("ask for information about a nickname which no longer exists"),
    N_("nickname [,nickname [,nickname ...]] [count [target]]"),
    N_("nickname: nickname to search\n"
       "   count: number of replies to return (full search if negative number)\n"
       "  target: reply should match this mask"),
    "", 1, MAX_ARGS, 0, 1, NULL, irc_cmd_send_whowas, NULL },
  { "001", N_("a server message"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_001 },
  { "221", N_("user mode string"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_221 },
  { "301", N_("away message"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_301 },
  { "302", N_("userhost"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_302 },
  { "303", N_("ison"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_303 },
  { "305", N_("unaway"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_305 },
  { "306", N_("now away"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_306 },
  { "307", N_("whois (registered nick)"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_307 },
  { "311", N_("whois (user)"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_311 },
  { "312", N_("whois (server)"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_312 },
  { "313", N_("whois (operator)"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_313 },
  { "314", N_("whowas"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_314 },
  { "315", N_("end of /who list"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_315 },
  { "317", N_("whois (idle)"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_317 },
  { "318", N_("whois (end)"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_318 },
  { "319", N_("whois (channels)"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_319 },
  { "320", N_("whois (identified user)"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_320 },
  { "321", N_("/list start"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_321 },  
  { "322", N_("channel (for /list)"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_322 },
  { "323", N_("/list end"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_323 },
  { "324", N_("channel mode"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_324 },
  { "329", "???", "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_329 },
  { "331", N_("no topic for channel"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_331 },
  { "332", N_("topic of channel"),
    N_("channel :topic"),
    N_("channel: name of channel\n"
       "  topic: topic of the channel"),
    NULL, 2, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_332 },
  { "333", N_("infos about topic (nick and date changed)"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_333 },
  { "338", N_("whois (host))"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_338 },
  { "341", N_("inviting"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_341 },
  { "344", N_("channel reop"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_344 },
  { "345", N_("end of channel reop list"),  "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_345 },
  { "348", N_("channel exception list"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_348 },
  { "349", N_("end of channel exception list"),  "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_349 },
  { "351", N_("server version"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_351 },
  { "352", N_("who"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_352 },
  { "353", N_("list of nicks on channel"),
    N_("channel :[[@|+]nick ...]"),
    N_("channel: name of channel\n"
       "   nick: nick on the channel"),
    NULL, 2, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_353 },
  { "366", N_("end of /names list"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_366 },
  { "367", N_("banlist"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_367 },
  { "368", N_("end of banlist"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_368 },
  { "378", N_("connecting from"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_378 },
  { "401", N_("no such nick/channel"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "402", N_("no such server"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "403", N_("no such channel"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "404", N_("cannot send to channel"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "405", N_("too many channels"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "406", N_("was no such nick"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "407", N_("was no such nick"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "409", N_("no origin"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "410", N_("no services"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "411", N_("no recipient"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "412", N_("no text to send"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "413", N_("no toplevel"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "414", N_("wilcard in toplevel domain"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "421", N_("unknown command"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "422", N_("MOTD is missing"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "423", N_("no administrative info"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "424", N_("file error"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "431", N_("no nickname given"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "432", N_("erroneous nickname"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "433", N_("nickname already in use"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_433 },
  { "436", N_("nickname collision"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "437", N_("resource unavailable"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "438", N_("not authorized to change nickname"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_438 },
  { "441", N_("user not in channel"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "442", N_("not on channel"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "443", N_("user already on channel"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "444", N_("user not logged in"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "445", N_("summon has been disabled"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "446", N_("users has been disabled"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "451", N_("you are not registered"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "461", N_("not enough parameters"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "462", N_("you may not register"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "463", N_("your host isn't among the privileged"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "464", N_("password incorrect"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "465", N_("you are banned from this server"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "467", N_("channel key already set"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "470", N_("forwarding to another channel"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "471", N_("channel is already full"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "472", N_("unknown mode char to me"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "473", N_("cannot join channel (invite only)"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "474", N_("cannot join channel (banned from channel)"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "475", N_("cannot join channel (bad channel key)"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "476", N_("bad channel mask"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "477", N_("channel doesn't support modes"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "481", N_("you're not an IRC operator"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "482", N_("you're not channel operator"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "483", N_("you can't kill a server!"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "484", N_("your connection is restricted!"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "485", N_("user is immune from kick/deop"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "487", N_("network split"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "491", N_("no O-lines for your host"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "501", N_("unknown mode flag"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "502", N_("can't change mode for other users"), "", "",
    NULL, 0, 0, MAX_ARGS, 1, NULL, NULL, irc_cmd_recv_error },
  { "671", N_("whois (secure connection)"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_671 },
  { "973", N_("whois (secure connection)"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_server_mode_reason },
  { "974", N_("whois (secure connection)"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_server_mode_reason },
  { "975", N_("whois (secure connection)"), "", "",
    NULL, 0, 0, 0, 1, NULL, NULL, irc_cmd_recv_server_mode_reason },
  { NULL, NULL, NULL, NULL, NULL, 0, 0, 0, 1, NULL, NULL, NULL }
};
