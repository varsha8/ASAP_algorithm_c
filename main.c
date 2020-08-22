#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N 8
//#pragma warning(disable:4996)

int main()
{
	//////////////////////////////////////variables
	char raw[512];
	char* record, * line;
	int i = 0, j = 0, k = 0;
	int max = 20;
	struct Node {
		int nodeNum;
		char operation[4];
		char in1[3];
		char in2[3];
		char op[3];
		int pri;
		struct Node* parent[5];
		int timeStamp;
		int isParent;
	}nodes[N], noParent;
	noParent.nodeNum = -1;
	//////////////////////////////////////initialising parent array with noParent(1 means noParent)
	for (i = 0;i < N;i++) {
		nodes[i].timeStamp = -1;
		nodes[i].isParent = 0;
		for (j = 0;j < 5;j++)
			nodes[i].parent[j] = &noParent;
	}
	//////////////////////////////////////reading values from CSV file
	FILE* fstream = fopen("data.csv", "r");
	if (fstream == NULL) {
		printf("\n file opening failed ");
		return -1;
	}
	printf("read from file\n");

	i = 0;
	while ((line = fgets(raw, sizeof(raw), fstream)) != NULL)
	{
		//printf("%s\n",line);
		record = strtok(line, ",");

		while (record != NULL)
		{

			nodes[i].nodeNum = atoi(record);
			//If you pass a NULL value, you are asking to continue tokenizing the same string as before (usually used in loops).
			record = strtok(NULL, ",");

			strcpy(nodes[i].operation, record);
			record = strtok(NULL, ",");

			strcpy(nodes[i].in1, record);
			record = strtok(NULL, ",");

			strcpy(nodes[i].in2, record);
			record = strtok(NULL, ",");

			strcpy(nodes[i].op, record);
			record = strtok(NULL, ",");

			nodes[i].pri = atoi(record);
			record = strtok(NULL, ",");

			++i;
		}
	}

	//////////////////////////////////////parent assigning
	for (i = 0;i < N;i++)
	{
		int temp = 0;
		for (j = 0;j < N;j++)
		{
			if (!(strcmp(nodes[i].in1, nodes[j].op)))
			{
				nodes[i].parent[temp] = &(nodes[j]);
				temp++;
			}

			if (!(strcmp(nodes[i].in2, nodes[j].op)))
			{
				nodes[i].parent[temp] = &(nodes[j]);
				temp++;
			}

		}

	}


	///////////////////////////////// asap

	for (j = 0;j < N;j++)
	{
		for (i = 0;i < N;i++)
		{
			int t1 = nodes[i].parent[0]->timeStamp;
			int t2 = nodes[i].parent[1]->timeStamp;
			//if (nodes[i].parent[0]->nodeNum == -1)
			//	nodes[i].timeStamp = 1;
			if (nodes[i].timeStamp < 0)
			{
				switch (nodes[i].pri)
				{
				case 0:
					nodes[i].timeStamp = 1;
					break;
				case 1:
					if (t1 > 0)
						nodes[i].timeStamp = t1 + 1;
					break;
				case 2:
					if (t1 > 0)
						nodes[i].timeStamp = t1 + 1;
					break;
				case 3:
					if ((t2 > 0) && (t1 > 0))
						nodes[i].timeStamp = ((t1 > t2) ? t1 + 1 : t2 + 1);
					break;
				default:
					nodes[i].timeStamp = 0;
				}
			}



		}
	}

	////    taking in , out, wires in array
	char input[10][3];
	char output[10][3];
	char wire[10][3];
	for (i = 0;i < 10;i++) {
		strcpy(input[i], "0");
		strcpy(output[i], "0");
		strcpy(wire[i], "0");
	}
	// Finding the inputs and wires  
	int temp = 0, tempw =0;
	for (i = 0; i < N;i++)
	{
		switch (nodes[i].pri)
		{
		case 0: 
			strcpy(input[temp],nodes[i].in1);
			temp++;
			strcpy(input[temp], nodes[i].in2);
			temp++;
			break;
		case 1:
			strcpy(input[temp], nodes[i].in1);
			temp++;
			strcpy(wire[tempw], nodes[i].in2);
			tempw++; break;
		case 2:
			strcpy(wire[tempw], nodes[i].in1);
			tempw++;
			strcpy(input[temp], nodes[i].in2);
			temp++; break;
		case 3:
			strcpy(wire[tempw], nodes[i].in1);
			tempw++;
			strcpy(wire[tempw], nodes[i].in2);
			tempw++; break;
		default:
			break;
		}
	}
	// Finding the output out of all the wire
	//int flagi=0, flagj=0, falgk =0;
	for (i = 0;i < N;i++)
	{
		//nodes[i].parent[j]->nodeNum  nodes[i].nodeNum
		for (k = 0;k < N;k++)
		{	
			if(nodes[i].isParent == 0)
				for (j = 0;j < 5;j++)
					if (nodes[i].nodeNum == nodes[k].parent[j]->nodeNum)
					{	
						nodes[i].isParent = 1;
						
					}
		}
	}
	temp = 0;
	for (i = 0;i < N;i++)
	{
		if (nodes[i].isParent == 0)
		{
			strcpy(output[temp], nodes[i].op);
			temp++;
		}
	}
	printf("Input are\t");
	for (i = 0;i < 10;i++)
	{	if(strcmp(input[i],"0"))
			printf("%s\t", input[i]);
	}
	printf("\nOutput are\t");
	for (i = 0;i < 10;i++)
	{
		if (strcmp(output[i], "0"))
			printf("%s\t", output[i]);
	}
	printf("\nWires are\t");
	for (i = 0;i < 10;i++)
	{
		if (strcmp(wire[i], "0"))
			printf("%s\t", wire[i]);
	}
	printf("\n");
	/////////////////////////////////////////////////////////////////////
	///////////////////Sorting acording to timestamp/////////////////////
	/////////////////////////////////////////////////////////////////////
	int ser[N], tstamp[N];
	for (i = 0;i < N;i++)
	{
		ser[i] = nodes[i].nodeNum;
		tstamp[i] = nodes[i].timeStamp;
		//printf("%d ::: %d\n", ser[i], tstamp[i]);
	}
	for (i = 0;i < N;i++)
	{
		for (j = 0;j < N-i-1;j++) {
			if (tstamp[j] > tstamp[j + 1])
			{
				temp = tstamp[j];
				tstamp[j] = tstamp[j+1];
				tstamp[j + 1] = temp;
				//for node num
				temp = ser[j];
				ser[j] = ser[j + 1];
				ser[j + 1] = temp;
				//printf("Q");
			}

		}
	}
	printf("The nodes in the order of TimeStamp\n");
	for (i = 0;i < N;i++)
	{
		printf("%d ::: %d\n", ser[i], tstamp[i]);
	}

	//////////////////////////////////////printing the structs
	for (i = 0;i < N;i++)
	{
		//int p_count = 0;
		printf("##########\n");
		printf("    Node %d\n", nodes[i].nodeNum);
		printf("Parent Nodes:");
		for (j = 0;j < 5;j++)
			if (nodes[i].parent[j]->nodeNum > 0) {
				printf("%d_", nodes[i].parent[j]->nodeNum);
				//p_count++;
			}
		printf("\nInput 1  : %s", nodes[i].in1);
		printf("\nInput 2  : %s", nodes[i].in2);
		printf("\nOutput   : %s", nodes[i].op);
		printf("\nOperator : %s", nodes[i].operation);//operation has \n at the last index
		printf("\nTimestamp:  %d\n", nodes[i].timeStamp);
		printf("\nisParent:  %d\n", nodes[i].isParent);
		printf("##########\n\n");
	}

	printf("##### The Final Graph after applying ASAP #####\n");
	printf("\t        Time Stamp\n");

	for (i = 0;i < N;i++)
	{
		printf("\tNode %d ::    %d\n", nodes[i].nodeNum, nodes[i].timeStamp);
	}
	//////////////////////////////////////////////
	//////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////// writing to a .v file

	FILE* file = fopen("output.v", "w+");
	char statement[200];
	fputs("\n", file);

	/*
	module asap( input A1, B1, A2, B2, A3, B3, A4, A5, A6, output X8);
	wire X1, X2, X3, X4, X5, X6, X7;
	
	*/
	fputs("module asap(", file);
	char header[300] = "input [7:0] ";
	char finputs[50] = "";
	for (i = 0;i < 10;i++)
	{	
		if (strcmp(input[i], "0")) {
			strcat(finputs, input[i]);
			strcat(finputs, ", ");
		}
	}
	strcat(header, finputs);	
	char foutputs[50] = "output reg [7:0] ";
	if (strcmp(output[0], "0"))
		strcat(foutputs, output[0]);
	for (i = 1;i < 10;i++)
	{
		if (strcmp(output[i], "0")) {
			strcat(foutputs, ", ");
			strcat(foutputs, output[i]);
		}
	}
	strcat(header, foutputs);
	fputs(header, file);
	fputs(");\nreg [7:0] ", file);

	if (strcmp(wire[i], "0"))
		strcpy(statement, wire[0]);
	for (i = 1;i < 10;i++)
	{
		if (strcmp(wire[i], "0")) {
			strcat(statement, ", ");
			strcat(statement, wire[i]);
		}
	}
	strcat(statement, ";\n\n");
	fputs(statement, file);
	fputs("initial\nbegin\n\n", file);
	/* till here its till wire next is assign
		next is assign statements for each node according to timestamp order
		assign out = in1 op in2;
	*/
	for (i = 0;i < N;i++) {
		strcpy(statement, "assign ");
		strcat(statement, nodes[ser[i]-1].op);
		strcat(statement, " = ");
		strcat(statement, nodes[ser[i]-1].in1);
		if (!(strcmp(nodes[ser[i]-1].operation, "MUL")))
			strcat(statement, " * ");
		else if (!(strcmp(nodes[ser[i]-1].operation, "ADD")))
			strcat(statement, " + ");
		else//subtraction
			strcat(statement, " - ");
		strcat(statement, nodes[ser[i]-1].in2);
		strcat(statement, ";\n");
		fputs(statement, file);
	}

	//till here
	fputs("\nend\nendmodule",file);
	fclose(file);
	return 0;
}
