/* max lengths */

#define MAXINET_ADDR 40
#define MAX_ENTER_MSG 40
#define MAX_EMAIL 20
#define MAX_PASS 8
#define MAX_NAME 6

/* list flags */

#define IGNORE (1<<0)
#define BLOCK (1<<1)
#define FRIEND (1<<2)

/* saved flags */

#define SEEECHO (1<<0)
#define BLOCK_TELLS (1<<1)
#define CONVERSE (1<<2)

/* gender */

#define MALE (1<<0)
#define FEMALE (1<<1)
#define HERMA (1<<2)
#define PSYCHO (1<<3)

/* residency */

#define BANISH (1<<0)
#define SPLAT (1<<1)
#define STANDARD (1<<2)
#define ADMIN (1<<3)
#define MODERATOR (1<<4)

/* player options */

struct player {
  char name[MAX_NAME];;
  char email[MAX_EMAIL];
  char pass[MAX_PASS];
  char *inet_addr;
  char *num_addr;
  char enter_msg[MAX_ENTER_MSG];
  int32_t residency;
  int32_t gender;
  int32_t saved_flags;
  int32_t fd;
  int32_t script;
  int32_t term_width;
  int32_t idle;
  int32_t gender
  int32_t age;
};

/* terminals options */

struct terminal {
  char *name;
  char *bold;
  char *off;
  char *cls;
};

/* flag list def */

struct flag_list {
  char *text;
  int change;
};

/* command format */

struct command {
  char *text;
  char *function;
  int8_t level;
  char *help;
};

/* contains saved flags */

struct s_struct {
  char name[MAX_NAME];
  char *last_host;
  int32_t last_on;
  int32_t residency;
};

/* files */

struct special_file {
  char *where;
  int length;
};

/* command type */

#define ECHO_COM 16

/* systen flags */

#define PANIC (1<<0)

/* globals */

extern int out_curent, out_pack_current, max_players;
extern int test_receive();
extern char *end_string();
extern char *stack;
extern void quit(struct player *, char *);
