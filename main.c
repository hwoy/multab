#include <stdio.h>
#include "function.h"
#include "opt.h"

#define BSIZE 1024

#define MUL_BEGIN 1
#define MUL_END 12
#define FROM 2
#define TO 9
#define COL 5

static void showHelp(const char *str,const char **param,const char **hparam);
static int showErr(const char **str,int errno,const char *msg);
static unsigned int basename (const char *ch);
static void printtable(unsigned int from,unsigned int to,unsigned int mbegin,unsigned int mend,unsigned int col);

static const char *cptrarr_param[] = { "-f:", "-t:", "-b:", "-e:", "-c:","-h", NULL };
static const char *helpparam[]={"From","To","Mul Begin","Mul End","Cols","Help",NULL};
enum
{
	opt_f,opt_t,opt_b,opt_e,opt_c,opt_h
};


static const char *err_str[]={"Invalid option","Not an unsigned integer","Col is zero","Form > To","M_begin > M_end",NULL};
enum
{
	err_inv,err_ni,err_cz,err_fot,err_boe
};

int
main (int argc, const char *argv[])
{
  static char carray_buff[BSIZE];
  int i;
  unsigned int ui_cindex;
  unsigned int mbegin,mend,from,to,col;
  
mbegin = MUL_BEGIN;
mend= MUL_END;
from =FROM;
to =TO;
col =COL;

  for (ui_cindex = 1; (i =
		       opt_action (argc, argv, cptrarr_param, carray_buff,
				   BSIZE, DSTART)) != e_optend; ui_cindex++)
    {


      switch (i)
	{
	case opt_f:
	if(!isUint(carray_buff))return showErr(err_str,err_ni,carray_buff);
	from=s2ui(carray_buff);
	break;
	
	case opt_t:
	if(!isUint(carray_buff))return showErr(err_str,err_ni,carray_buff);
	to=s2ui(carray_buff);
	break;
	
	case opt_b:
	if(!isUint(carray_buff))return showErr(err_str,err_ni,carray_buff);
	mbegin=s2ui(carray_buff);
	break;
	
	case opt_e:
	if(!isUint(carray_buff))return showErr(err_str,err_ni,carray_buff);
	mend=s2ui(carray_buff);
	break;
	
	case opt_c:
	if(!isUint(carray_buff))return showErr(err_str,err_ni,carray_buff);
	if(!(col=s2ui(carray_buff)))return showErr(err_str,err_cz,carray_buff);
	break;

	case opt_h:showHelp(argv[0],cptrarr_param,helpparam);
	return 0;
	
	default:showHelp(argv[0],cptrarr_param,helpparam);
	return showErr(err_str,err_inv,carray_buff);

	}

    }
carray_buff[0]=0;
if(from>to)return showErr(err_str,err_fot,carray_buff);
if(mbegin>mend)return showErr(err_str,err_boe,carray_buff);

printtable(from,to,mbegin,mend,col);
  return 0;

}

static void showHelp(const char *str,const char **param,const char **hparam)
{
	unsigned int i;
	fprintf(stderr,"\nUSAGE: %s [option list]\n\n", &str[basename(str)] );
	
	fprintf(stderr,"[OPTIONS]\n");
	
	for(i=0;param[i]&&hparam[i];i++)
	{
		fprintf(stderr,"%s\t\t%s\n",param[i],hparam[i]);
	}
	fprintf(stderr,"\n");
	
	fprintf(stderr,"[DEFAULT]\n");
	fprintf(stderr,"%s = %u\n",param[i=0],FROM);
	fprintf(stderr,"%s = %u\n",param[++i],TO);
	fprintf(stderr,"%s = %u\n",param[++i],MUL_BEGIN);
	fprintf(stderr,"%s = %u\n",param[++i],MUL_END);
	fprintf(stderr,"%s = %u\n",param[++i],COL);
	fprintf(stderr,"\n");
}

static int showErr(const char **str,int errno,const char *msg)
{
	fprintf(stderr,"ERR %d: %s : %s\n",errno,msg,str[errno]);
	return -1*(errno+1);
}

static unsigned int basename (const char *ch)
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

static void printtable(unsigned int from,unsigned int to,unsigned int mbegin,unsigned int mend,unsigned int col)
{
unsigned int i,j,k,m;

for(k=col;(k/col)<=(to/col)+(to%col?1:0);k+=col)
{
	for(j=mbegin;j<=mend;j++)
	{
		for(m=i=from+(k==col?0:k-col);(i-m<col)&&(i<=to);i++)
		{
		printf("%ux%u=%u\t\t",i,j,i*j);
		}
		putchar('\n');
	}
	putchar('\n');putchar('\n');
}

}
