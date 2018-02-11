
#ifndef FLOWSORT_C
#define FLOWSORT_C

#include "../heads/flowsort.h"

/* display the results */
#define PRINT_STUFFS

/* =========================================================== */
/* Portion of code to use only if one wants to display results */
/* =========================================================== */
#ifdef PRINT_STUFFS


	void write_flowsort_results(float** PI, float** PHI, char** argv)
	{
		int c=0,i=0,a=0;
		categorie* ite;
		FILE *f;
		char path[30];
		char buff[30];

		strcpy(path,"./outputs/flowsort_");
		strcpy(buff,argv[4]);
		strtok(buff,"/");
		strcat(path,strtok(NULL,"/"));

		f=fopen(path,"w");

		printf("%s\n",path);

/* If uncommented, the PIs are displayed on the terminal but for the file for plotting (gnuplot) will not work.... (Do something for it)
		for(a=0;a<N;a++)
		{
			for(i=0;i<N_CAT;i++)
				fprintf(f,"%f ",PI[a][i]);
			fprintf(f,"\n");
			for(i=0;i<N_CAT;i++)
				fprintf(f,"%f ",PI[N+a][i]);
			fprintf(f,"\n");
		}
*/

		for(a=0;a<N;a++)
		{
			for(i=0;i<=N_CAT;i++)
				for(c=0;c<3;c++)
					fprintf(f,"%f ",PHI[c][(N_CAT+1)*a+i]);

					/* PHI is a "3D" array : 3 rows, (N_CAT+1)*N Columns :
					 * 
					 * Fills file as follows (p : phi, _ : sub, ^: up. p^+_R_1(x): positive flows of x wrt R1.) :
					 * 
					 * p^+_R1(r1) | p^-_R1(r1) | p_R1(r1) | p^+_R1(r2) | p^-_R1(r2) | p_R1(r2) | ... | p^+_R1(rNCAT) | p^-_R1(rNCAT) | p_R1(rNCAT) | p^+_R1(a1) | p^-_R1(a1) | p_R1(a1)
					 * ...
					 * p^+_Rj(r1) | p^-_Rj(r1) | p_Rj(r1) | p^+_Rj(r2) | p^-_Rj(r2) | p_Rj(r2) | ... | p^+_Rj(rNCAT) | p^-_Rj(rNCAT) | p_Rj(rNCAT) | p^+_Rj(a1) | p^-_Rj(aj) | p_Rj(aj)
					 * ...
					 * p^+_RN(r1) | p^-_RN(r1) | p_RN(r1) | p^+_RN(r2) | p^-_RN(r2) | p_RN(r2) | ... | p^+_RN(rNCAT) | p^-_RN(rNCAT) | p_RN(rNCAT) | p^+_RN(aN) | p^-_RN(aN) | p_RN(aN)
					 * 
					 */
			fprintf(f,"\n");
		}

		printf("Categories (Note : not printed categories have no alternatives within) :");

		for(c=0;c<3;c++)
		{
			if(c<2)
				printf("\nC[%s] : ", (c==1?"negative":"positive"));
			else
				printf("\nC[net] :      ");

			for(i=0;i<(N_CAT-1);i++)
			{
				ite=C[c][i];
				while(ite!=NULL)
				{
					if(ite->alternative==C[c][i]->alternative)
					{
						printf("[C_%d :", i+1);
					}

					printf(" action_%d ", ite->alternative+1);
					ite=ite->next_alternative;

					if(ite==NULL)
					{
						printf("]\t");
					}
				}
			}
		}

		for(i=0;i<N;i++)
		{
			for(c=0;c<3;c++)
				fprintf(f,"%d ", act_to_cat[i][c]+1);
			fprintf(f,"\n");
		}

		printf("\nEnd print_categories\n");
		
		fclose(f);

		f=popen("gnuplot","w");
		//fprintf(f, "set terminal postscript eps enhanced\n");
		fprintf(f, "set terminal latex\n");
		fprintf(f, "set size 0.7,0.7\n");
		fprintf(f, "set xrange [-0.05 : 1.05]\n");
		fprintf(f, "set yrange [-0.05 : 1.05]\n");
		fprintf(f, "set xtics font \",8\"\n");
		fprintf(f, "set ytics font \",8\"\n");
		fprintf(f, "set style arrow 1 lc 1\n");

		/*c is an alternative*/
		for(c=0;c<N;c++)
		{
			/*
			 * fprintf(f, "set xlabel \"{/Symbol f}^{+}_{R_{%d}}\" font \",8\"\n",c+1);
			 * fprintf(f, "set ylabel \"{/Symbol f}^{-}_{R_{%d}}\" font \",8\"\n",c+1);*/
			fprintf(f, "set xlabel \"$\\\\phi^{+}_{R_{%d}}$\" font \",8\"\n",c+1);
			fprintf(f, "set ylabel \"$\\\\phi^{-}_{R_{%d}}$\" font \",8\"\n",c+1);
			fprintf(f, "set output './outputs/results_%d.tex'\n",c);
			fprintf(f, "a=0\n");
			fprintf(f, "plot for [i=1:%d:3] '%s' every ::%d::%d u ((column(i)+column(i+3))/2):((column(i+1)+column(i+4))/2):((column(i+3)-column(i))/2):((column(i+4)-column(i+1))/2) w boxxy lt 1 lc 3 notitle",(N_CAT-1)*3,path,c,c);
			/*fprintf(f, ", for [i=1:%d:3] a=a+1 '%s' every ::%d::%d u (column(i)):(column(i+1)):(sprintf(\"(C^{+}_{%%d},C^{-}_{%%d})\",a,a)) w labels point pt 7 offset char -6.2,0.7 font \"6\" notitle",(N_CAT-1)*3,path,c,c);*/
			fprintf(f, ", for [i=1:%d:3] '%s' every ::%d::%d u (column(i)):(column(i+1)) w point pt 7 notitle",(N_CAT-1)*3,path,c,c);
			/*fprintf(f, ", '%s' every ::%d::%d u 16:17:(sprintf(\"({/Symbol f}_{R_{%d}}^{+}(a_{%d}),{/Symbol f}_{R_{%d}}^{-}(a_{%d}))\")) lt rgb \"#FF0000\" w labels point pt 3 lc 1 offset char -7,0.7 font \"6\" notitle",path,c,c,c+1,c+1,c+1,c+1);*/
			fprintf(f, ", '%s' every ::%d::%d u 16:17 w p ls 3 notitle\n",path,c,c);
			/*fprintf(f, ", '%s' every ::%d::%d u 16:17:(0):(0.07) w vectors lt rgb \"#FF0000\" notitle",path,c,c);
			fprintf(f, ", '%s' every ::%d::%d u 16:17:(0.07):(0) w vectors lt rgb \"#FF0000\" notitle\n",path,c,c);*/
		}

		pclose(f);

	}


#endif
/*==================== End of printing stuffs ========================*/


ptr_cat add_to_categorie(ptr_cat head, int to_add)
{
	/* LIFO : Last In First Out */
	categorie* new_categorie_ll;
	new_categorie_ll=malloc(sizeof(categorie));

	if(new_categorie_ll == NULL){ /* memory allocation failure */ printf("MEMO_ALLOC_FAILURE line %d in file %s\n", __LINE__, __FILE__); }

	new_categorie_ll->alternative=to_add;
	new_categorie_ll->next_alternative=head;
	return new_categorie_ll;
}

void flowsort(float** PHI)
{
	int a=0,b=0,i=0;

	for(i=0;i<3;i++)
		for(a=0;a<N;a++)
			for(b=1;b<N_CAT;b++)
			{
				if( i != 1 )
				{/* The inequality slightly changes when one wants to assigne an action to a category with its negative flow value  */
					if( PHI[i][((N_CAT+1)*a)+N_CAT] >= PHI[i][((N_CAT+1)*a)+b] || b==N_CAT-1 )
					{
						C[i][b-1]=add_to_categorie(C[i][b-1], a);
						act_to_cat[a][i]=b-1;
						break;
					}
				}
				else
				{
					if( PHI[i][((N_CAT+1)*a)+N_CAT] < PHI[i][((N_CAT+1)*a)+b] || b==N_CAT-1 )
					{
						C[i][b-1]=add_to_categorie(C[i][b-1], a);
						act_to_cat[a][i]=b-1;
						break;
					}
				}
			}
}
	

#endif
