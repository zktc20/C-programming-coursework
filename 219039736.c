#include <stdio.h> // include access to the printf statement and file I/O
#include <math.h> // for power (pow) command. Compile with the -lm option.

int dimensions=2; //The simplex is considered in 2 dimensions. x0 and x1.

double F(double x[])// Defines the Rosenbrock Equation
{
	return ((100*(pow(x[1]-(pow(x[0],2)),2)))+(pow((1-x[0]),2))) ;
}

double criterion(double y_l, double y_m, double y_h, double y_bar)// Defines the Criterion to give the condition for the number of iterations required.
{
return sqrt((pow((y_l-y_bar),2)+pow((y_m-y_bar),2)+pow((y_h-y_bar),2))/dimensions);
} 

int i,j,N=0; //Defining the counters for this exercise. Used throughout the loops so defined at the top to prevent confusion.
int n=3;
double condition=1e-8; //The condition can be changed as you please for more accurate values of the minimum.


void write_file(char *filename, double x00, double x01)
{
	// 101 values of F(x) will be printed to file. These lie between x00 and x01. 

	int i;
	double x_1; // integer i to use as loop counter
	double x_0; 
	double FF(double x_0,double x_1)// Defines the Rosenbrock function.
	{
	return ((100*(pow(x_1-(pow(x_0,2)),2)))+(pow((1-x_0),2))) ;
	}

	FILE *p_file; // a pointer to a file.


	p_file = fopen(filename, "w");

	// perform a loop using the loop counter
	for (i = 0; i <= 100; i++)
		{
			x_0 = x00 + i * (x01 - x00) / 100;
			x_1=1;
			fprintf(p_file, "%e, %e, %e\n", x_0,x_1, FF(x_0,x_1)); 
		}
	
	fclose(p_file);
}


int main()

{	
	write_file("datatest.txt", -2., 2.);
	// The three initial verticies for the triangles are defined as given in the task. These will be replaced with the verticies obtained at the end of each iteration.
	double pbar[2],pstar[2],pstarstar[2];
	double vertices[3][2] = {{0.,0.},{2.,0.},{0.,2.}}; //The three vertices are defined in a 3 x 2 matrix
	
	while(N<1000) //While loop implemented. The loop will be repeated until 1000 iterations are completed or the minimum is reached.
	{ 
		N++;
		printf("-------------------------------------------");
		printf("Iteration #%d\n",N);	
		//i loops through the simplex and j loops through the x0 and x1 values
		printf("Vertices:\n"); //Printing the Vertices before they are sorted.
		for (i=0; i<3; i++)
			{for(j=0; j<2; j++)
				{printf("(%.6lf)\n", vertices[i][j]);}}
				
		
	 	// We can use the same counters as before.	 
		//Implementing Bubble Sort to order p values based on the corresponding y values. In ascending order.
		for (i=0; i<n-1; i++) 
		{
			 for(j=0; j < n-1-i; j++)
			 {
			 	if(F(vertices[j]) > F(vertices[j+1])) 
			 	{
			 		double swap[2];
			 		swap[0] = vertices[j][0];
			 		swap[1] = vertices[j][1];
			 		vertices[j][0] = vertices[j+1][0];
			 		vertices[j][1] = vertices[j+1][1];
			 		vertices[j+1][0] = swap[0];
			 		vertices[j+1][1] = swap[1];
			 	} //This method moves through the input list, element by element, and compares the current y value with the one before it and swaps them so that in the end they are placed in ascending order. 	If you wanted them in descending order you would have to change the ">" to a "<" in the above if statement. Doing this however would wreck the rest of the code as it works on the assumption that the array is arranged in ascending order.
			  }
		}
			 
		printf("Sorted Values:\n"); // Prints the sorted vertices.
		for(i=0;i<3;i++)
		{
			for(j=0; j<2; j++)
			{
				printf("(%.6lf)\n",vertices[i][j]);
			}
		}
		
		// Beginning the Flowchart. The pbar and pstar values from the inital vertices are defined.
		for (j=0; j<2; j++)
		{
			pbar[j]=(vertices[0][j]+vertices[1][j])/2;
			pstar[j]= 2*pbar[j] - vertices[2][j];
		}
		
		if (F(pstar)< F(vertices[0])) // y* < yl?
		{
			for (j=0; j<2; j++) //yes
			{
				pstarstar[j] = 2*pstar[j] - pbar[j];
			}
			if (F(pstarstar)<F(vertices[0])) //y**<yl and the Simplex is Expanded.
			{
				printf("Simplex Expanded\n");
				for (j=0; j<2; j++)//yes
				{
					vertices[2][j] = pstarstar[j];
				}
			}
			else //no 
			{ printf("Simplex Reflected\n");
				for (j=0; j<2; j++)
				{
					vertices[2][j] = pstar[j]; //The Simplex is Reflected.
				}
			}
		}
		else	//no
		{
			if(F(pstar) > F(vertices[1])) //y*>ym
			{
				if(F(pstar)< F(vertices[2]))//y*>yh loop but the sign is reversed for cleaner code.
				{
					for(j=0; j<2; j++)
					{
						vertices[2][j] = pstar[j];
					}
				}
				
				for(j=0; j<2; j++)
				{
					pstarstar[j] = (vertices[2][j]+pbar[j])/2;
				}
				
				if (F(pstarstar)>F(vertices[2])) // y**>yh
				{
					vertices[i][j] = (vertices[i][j]+ vertices[0][j])/2;
					printf("Failed Contraction\n");
				}
				else//no
				{	printf("Simplex Contracted\n");
					for(j=0; j<2; j++)
						vertices[2][j] = pstarstar[j];
				}	
			}
			else
			{	
				for(j=0; j<2; j++)
				{
					vertices[2][j] = pstar[j];
				}
			}
		}
		printf("New Vertices:\n"); // The new vertices are printed.
		for (i=0; i<3; i++)
			{
				for(int j=0; j<2; j++)
					{
						printf("(%.6lf)\n", vertices[i][j]);
					}
			}
	//Check if the criterion is fulfilled to consider these points a minimum.
	printf("T = %e\n",criterion(F(vertices[0]),F(vertices[1]),F(vertices[2]),F(pbar))); // Where T is the value of the criterion.
	
	if(criterion(F(vertices[0]),F(vertices[1]),F(vertices[2]),F(pbar))>condition)
		{
		}
	else //If the criterion is fulfilled, the code is interrupted and no more iterations are run.
		{
			printf("The Criterion is fulfilled.\n --End of Code--\n");
			return 0;
		}
	}	
}
