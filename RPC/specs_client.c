/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "specs.h"
#include<stdio.h>


float
compute_6(char *host,float a,float b,char op)
{
	CLIENT *clnt;
	float  *result_1;
	values  sub_6_arg;
	float  *result_2;
	values  mul_6_arg;

	if(op=='-')
	{
		sub_6_arg.num1 = a;
		sub_6_arg.num2 = b;
		sub_6_arg.operation = op;

		clnt = clnt_create (host, COMPUTE, COMPUTE_VERS, "udp");
		if (clnt == NULL) {
			clnt_pcreateerror (host);
			exit (1);
		}

		result_1 = sub_6(&sub_6_arg, clnt);
		if (result_1 == (float *) NULL) {
			clnt_perror (clnt, "call failed");
		}

		clnt_destroy (clnt);

		return (*result_1);	
	}

	
	else if(op == '*')
	{
		mul_6_arg.num1=a;
		mul_6_arg.num2=b;
		mul_6_arg.operation=op;

		clnt = clnt_create (host, COMPUTE, COMPUTE_VERS, "udp");
		if (clnt == NULL) {
			clnt_pcreateerror (host);
			exit (1);
		}

		result_2 = mul_6(&mul_6_arg, clnt);
		if (result_2 == (float *) NULL) {
			clnt_perror (clnt, "call failed");
		}

		clnt_destroy (clnt);

		return (*result_2);	

	}
}


int
main (int argc, char *argv[])
{
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	while(1){

	float val1,val2;
	char oper;
	printf("Enter the expression to evaluate(only - and * allowed):\n");
	scanf("%f",&val1);
	scanf("%s",&oper);
	scanf("%f",&val2);

	host = argv[1];
	printf("Answer= %f\n",compute_6 (host,val1,val2,oper));
	}
        exit(0);
}
