#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>

#include "player.h"

int main_descriptor, alive_descriptor;

struct terminal terms[] = {
  {"xterm", "\033[1m", "\033[m", "\033[H\033[2J"}
}

void clear_screen(struct player *p, char *str)
{

}

void tell_player(struct player *p, char *str)
{
  file *output;
  char *oldstack, *script;
  oldstack = stack;

  if((p->fd < 0) || (p->flags & PANIC) || (!(p->location) && 
        current_player != p))
    return;
  if(!(sys_flags & PANIC)) {
    if(!test_receive(p))
      return;
  }
  output = process_output(p, str);

  if(p->script) {
    script = stack;
    sprintf(stack, "%s_emergency", p->name);
    stack = end_string(stack);
    new_log(script, str);
    stack = script;  /* will store script's initial value in stack */
  }

  /* EINTR is an interrupt by the user */
  if(write(p->fd, output->where, output->length) < 0 && errno != EINTR)
    quit(p, NULL);

  out_current += output->length;
  out_pack_current++;
  stack = oldstack;
}

file *process_output(struct player *p, char *str)
{
  int i, hi;
  char *save;
  file *o;

  o->where = stack;
  o->length = 0;

  if(p != current_player)  {
    if((command_type & PERSONAL))  {
      strncpy(stack, terms[p->term - 1].bold);
      while(*stack)  {
        stack++;
        o->length++;
      }
      hi = 1;
    }
    save = stack;

    if((command_type & ECHOCOM) || (p->saved_flags & SEEECHO))  {
      sprintf(stack, "[%s]", current_player->name);
    }
    while(*stack) {
      stack++;
      o->length++;
    }
  }
}

void accept_new_connection()
{
  struct sockaddr_in incoming;
  struct hostent *hp;
  int length, new_socket, socket, dummy, n1, n2, n3, n4;
  char *resolved_address;
  struct player *p;
  char *oldstack;

  oldstack = stack;
  length = sizeof(incoming);
  socket = accept(main_descriptor, (struct sockaddr *) &incoming, &length);
  if((socket < 0) && (errno == EINTR)) {
    new_log("error", "EINTR trapped");
    return;
  }

  if(socket < 0)
    handle_error("socket could not be accepted");

  new_socket = dup(socket);
  close(socket);

  if(ioctl(new_socket, FIONBIO, &dummy) < 0)
    handle_error("can't set the socket to non blocking");

  if(new_socket >= max_players) {
    write(new_socket, full_msg.where, full_msg.length);
    out_current += full_msg.length;
    out_pack_current++;
    close(new_socket);
  }

  p = create_player();
  current_player = p;
  p->fd = new_socket;

  strncpy(p->num_addr, inet_ntoa(incoming.sin_addr, MAXINET_ADDR - 2));
  strncpy(p->inet_addr, p->num_addr, MAXINET_ADDR - 2);

  sscanf(p->num_addr, "%d.%d.%d.%d", &n1, &n2, &n3, &n4);
  if(do_banish(p)) {       /* check if banished */
    write(new_socket, banish_msg.where, banish_msg.length);
    out_current += banish_msg.length;
    out_pack_current++;
    destroy_player(p);
    return;
  }

  else { /* check if splat */
    if((splat_timeout > time(0)) && n1 == splat1 && n2 == splat2 
        && n3 == splat3) {    /* if w.x.y match in w.x.y.z then destroy */
      write(new_socket, splat_msg.where, splat_msg.length);
      out_current += splat_msg.length;
      out_pack_current++;
      destroy_player(p);
      return;
    }
    else 
      connect_to_prog(p);
  }
  current_player = 0;
}
