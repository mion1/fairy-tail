/* player options */

struct player {
  char *name;
  char *email;
  char *pass;
  char *inet_addr;
  char *num_addr;
  int residency;
  int gender;
  int saved_flags;
  int fd;
  int script;
};

/* terminals options */

struct terminal {
  char *name;
  char *bold;
  char *off;
  char *cls;
};

/* max lengths */

#define MAXINET_ADDR 40

/* command type */

#define ECHO_COM 16

/* saved flags */

#define SEEECHO (1<<12)

/* systen flags */

#define PANIC (1<<0)

/* globals */

extern int out_curent, out_pack_current, max_players;
extern int test_receive();
extern char *end_string();
extern char *stack;
extern void quit(struct player *, char *);
