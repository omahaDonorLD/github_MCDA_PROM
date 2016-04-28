
#include <stdlib.h>
#include <stdio.h>

#define M 6//Number of "experts"
#define K 3//Number of criteria
#define N 4//Number of alternatives/actions

// in_out_flow array


/** Given the matrix of preferences indices, fills 
 **/
void compute_phi(float in_out_flow[3][N], unsigned int pref_inds[N][N])
{
	int i=0, j=0;
	
	// Summing the scores on the prefs inds
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
		{
			in_out_flow[0][i]+=pref_inds[i][j];// outranking
			in_out_flow[1][i]+=pref_inds[j][i];// outranked
		}
		in_out_flow[2][i]=in_out_flow[0][i]-1;
	}
}


/** Return a table of relations 
 * 0 is NA, 1 is R or incomparable, 2 is I or indifference, 3 is the graal P or Preference relation
 **/
unsigned int** PROM_1(float in_out_flow[2][N])
{
	int i=0, j=0, prior=-1, other=-1;
	
	// Init pref relations
	unsigned int** s_l=(unsigned int**) malloc(N*sizeof(unsigned int*));
	for(i=0;i<N;i++)	s_l[i]=(unsigned int*) calloc(N,sizeof(unsigned int));
	
	for(i=0;i<N-1;i++)
	{
		for(j=i+1;j<N;j++)
		{
			/* May save time and skip performing an N^2 loop */
			// Default :
			prior=i, other=j;
			// Change otherwise
			if ( in_out_flow[0][j] > in_out_flow[0][i] )
			{
				prior=j;
				other=i;
			}

			// Preference
			if ( ( in_out_flow[0][prior] > in_out_flow[0][other] && in_out_flow[1][prior] <= in_out_flow[1][j] )
					|| ( in_out_flow[0][prior] == in_out_flow[0][other] && in_out_flow[1][prior] < in_out_flow[1][j] ) )
					s_l[prior][other]=3;

			// Indifference
			if ( in_out_flow[0][prior] == in_out_flow[0][other] && in_out_flow[1][prior] == in_out_flow[1][j] )
					s_l[prior][other]=2;

			// Incomparability
			if ( ( in_out_flow[0][prior] > in_out_flow[0][other] && in_out_flow[1][prior] > in_out_flow[1][j] )
					|| ( in_out_flow[0][prior] < in_out_flow[0][other] && in_out_flow[1][prior] < in_out_flow[1][j] ) )
					s_l[prior][other]=1;
					
		}
	}
	
	return s_l;
}

int main(void)
{
	printf("preparing first commit for the MCDA Promethee\n");
}
