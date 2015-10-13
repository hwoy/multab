#include <stdio.h>
#include <string.h>
#include "function.h"
#include "opt.h"

#define BSIZE 1024

#define MUL_BEGIN 1
#define MUL_END 12
#define FROM 2
#define TO 9
#define COL 5

static void showHelp (const char *str, const char **param,
		      const char **hparam);
static int showErr (const char **str, int errno, const char *msg);
static unsigned int basename (const char *ch);
static void printtable (unsigned int from, unsigned int to,
			unsigned int mbegin, unsigned int mend,
			unsigned int col, FILE * fp);

static const char *cptrarr_param[] =
  { "-f:", "-t:", "-b:", "-e:", "-c:", "-o:", "-h", NULL };
static const char *helpparam[] =
  { "From", "To", "Mul Begin", "Mul End", "Cols", "Out put to file", "Help",
  NULL
};

enum
{
  opt_f, opt_t, opt_b, opt_e, opt_c, opt_o, opt_h
};


static const char *err_str[] =
  { "Invalid option", "Not an unsigned integer", "Col is zero", "Form > To",
  "M_begin > M_end", "Can not access file", NULL
};

enum
{
  err_inv, err_ni, err_cz, err_fot, err_boe, err_af
};

int
main (int argc, const char *argv[])
{
  static char carray_buff[BSIZE], filename[BSIZE];
  int i;
  unsigned int m;
  unsigned int ui_cindex;
  unsigned int mbegin, mend, from, to, col;
  FILE *fp;

  mbegin = MUL_BEGIN;
  mend = MUL_END;
  from = FROM;
  to = TO;
  col = COL;

  filename[0] = 0;
  fp = stdout;

  for (ui_cindex = DSTART; (i =
		       opt_action (argc, argv, cptrarr_param, carray_buff,
				   BSIZE, DSTART)) != e_optend; ui_cindex++)
    {
      switch (i)
	{
	case opt_f:
	case opt_t:
	case opt_b:
	case opt_e:
	case opt_c:

	  if (!isUint (carray_buff))
	    return showErr (err_str, err_ni, carray_buff);

	  m = s2ui (carray_buff);

	  switch (i)
	    {
	    case opt_f:
	      from = m;
	      break;
	    case opt_t:
	      to = m;
	      break;
	    case opt_b:
	      mbegin = m;
	      break;
	    case opt_e:
	      mend = m;
	      break;
	    case opt_c:
	      if (!(col = m))
		return showErr (err_str, err_cz, carray_buff);
	      break;
	    }
	  break;

	case opt_o:
	  strcpy (filename, carray_buff);
	  break;

	case opt_h:
	  showHelp (argv[0], cptrarr_param, helpparam);
	  return 0;

	default:
	  showHelp (argv[0], cptrarr_param, helpparam);
	  return showErr (err_str, err_inv, carray_buff);
	}
    }

  carray_buff[0] = 0;
  if (from > to)
    return showErr (err_str, err_fot, carray_buff);

  if (mbegin > mend)
    return showErr (err_str, err_boe, carray_buff);

  if (filename[0])
    if (!(fp = fopen (filename, "w")))
      return showErr (err_str, err_af, carray_buff);

  printtable (from, to, mbegin, mend, col, fp);
  fclose (fp);

  return 0;

}

static void
showHelp (const char *str, const char **param, const char **hparam)
{
  unsigned int i;
  fprintf (stderr, "\nUSAGE: %s [option list]\n\n", &str[basename (str)]);

  fprintf (stderr, "[OPTIONS]\n");

  for (i = 0; param[i] && hparam[i]; i++)
    {
      fprintf (stderr, "%s\t\t%s\n", param[i], hparam[i]);
    }
  fprintf (stderr, "\n");

  fprintf (stderr, "[DEFAULT]\n");
  fprintf (stderr, "%s%u\n", param[i = 0], FROM);
  fprintf (stderr, "%s%u\n", param[++i], TO);
  fprintf (stderr, "%s%u\n", param[++i], MUL_BEGIN);
  fprintf (stderr, "%s%u\n", param[++i], MUL_END);
  fprintf (stderr, "%s%u\n", param[++i], COL);
  fprintf (stderr, "\n");
}

static int
showErr (const char **str, int errno, const char *msg)
{
  fprintf (stderr, "ERR %d: %s : %s\n", errno, msg, str[errno]);
  return -1 * (errno + 1);
}

static unsigned int
basename (const char *ch)
{
  unsigned int i, j;
  for (i = 0, j = 0; ch[i]; i++)
    {
      if (ch[i] == '\\' || ch[i] == '/')
	{
	  j = i;
	}
    }
  return (j == 0) ? 0 : j + 1;
}

static void
printtable (unsigned int from, unsigned int to, unsigned int mbegin,
	    unsigned int mend, unsigned int col, FILE * fp)
{
  unsigned int i, j, k;

  for (k = from; k <= to; k += col)
    {
      for (j = mbegin; j <= mend; j++)
	{
	  for (i = k; i < ((k + col) > to ? to + 1 : k + col); i++)
	    {
	      fprintf (fp, "%ux%u=%u\t\t", i, j, i * j);
	    }
	  fprintf (fp, "\n");
	}
      fprintf (fp, "\n");
      fprintf (fp, "\n");
    }
}
