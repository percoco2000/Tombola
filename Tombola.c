/*
 * Programma : Tombola.c 
 *-------------------------------------------------------
 * Una semplice implementazione del gioco della tombola
 * in C. Il software estrae i numeri, gestisce le vincite
 * ed il tabellone. 
 *-------------------------------------------------------
 * percoco2000@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
/*
 * Defines:
 * CLEAR() -> Clear terminal in linux
 */ 
#define CLEAR() printf("\033[H\033[J")


/*
 * Global Variables
 * -------------------------------------------------
 * tabellone : L'array che rappresenta il tabellone.
 *             Poiche' viene usato da diverse funzioni 
 *             e poiche' e' unico, viene dichiarato
 *             globale
 * combo     : L'array in cui sono memorizzate le stringhe 
 *             delle combinaziuoni vincenti
 */
int tabellone [91]; 
char combo[5][10]={"Ambo","Terno","Quaterna","Cinquina","Tombola"};

/*
 * Function Prototypes
 */

void Swap (int *a, int *b);
void Vincita(int combinazione, char *testo);
void Inizializza (int numeri[], int tabellone[]);
void Setup_Premi(float premi[]);
void input_string(char line[], int n);
void Error (char *error_message);
void Splash_Screen(void);
void Mostra_Estratto (int num);
char Filtra_Input (void);
int RandomInt (int upperlimit);
int Controlla_Vincita(int prox_win);
double input_float (char dot);

int main (void)
  {
   char istruzioni[10][25]={"   ","Premi  <ENTER>","Per Estrarre","un Numero","        ","Premi <V+ENTER>","Per segnalare","una vincita","    "," "};
   char num_s[3],ch;
   int i,estratto,next_combo=0,numeri[91];
   float premi[5];
   
   estratto=0;
   num_s[0]=' ';
   num_s[1]=' ';
   num_s[2]='\0';
   
   Splash_Screen();
   Inizializza(numeri,tabellone);
   Setup_Premi(premi);
   
   while (next_combo<5)
       {
        /*Stampa il tabellone.
          -------------------------------------------------------------------------------------------------------
          * Viene stampato il tabellone usando i caratteri ASCII Standard per
          * evidenziare il layout delle cartelle. Vengono stampati i soli
          * numeri estratti. Per fare cio', i numeri vengono convertiti in 
          * una stringa (num_s) che viene stampata se il corrispondente indice
          * in tabellone[] e' posto ad 1, altrimenti stampa 2 spazi vuoti.
          */        
        CLEAR();
        printf("\n");   
        printf ("|~~~~~~~~~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~~~~~~~~~~|\n");
        for (i=1;i<91;i++)
          {
	       //Converte il numero in una stringa per facilitare il disegno del tabellone
	       num_s[0]=(i/10)+48;
	       num_s[1]=(i%10)+48;
	       printf ("| %s ",tabellone[i]?num_s:"  ");
	       if (!(i%10))
	        {
			 if (!(i%30))
			  {	 
			   printf ("|\n|~~~~~~~~~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~~~~~~~~~~|  %s\n",istruzioni[i/10]);
		       } else {
                       printf ("|\n|------------------------|------------------------|  %s\n",istruzioni[i/10]);
				       }
             }
            }
        
         // ------------------------------------------------------------------------------------------------------
         printf ("\nProssimo Premio -> ");
         printf ("%-8s %.2f € ",combo[next_combo],premi[next_combo]);
	     
	     ch=Filtra_Input();
	     switch (ch)
	          {
	           case 'V':
	               Vincita(next_combo,"Un Giocatore");
	               next_combo++;
	               break;
	           
	           case '\n':
	               estratto++;
	               Mostra_Estratto(numeri[estratto]);
	               tabellone[numeri[estratto]]=1;
	               next_combo+=Controlla_Vincita(next_combo);
	               break;
	            
	           default :
	               Error("Carattere non previsto nello switch(ch)");    
	           }
	       }
	     
	     
		
		// Stampa finale del tabellone per eventuali controlli da parte
		// dei giocatori
		printf ("|~~~~~~~~~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~~~~~~~~~~|\n");
        for (i=1;i<91;i++)
          {
	       num_s[0]=(i/10)+48;
	       num_s[1]=(i%10)+48;
	       printf ("| %s ",tabellone[i]?num_s:"  ");
	       if (!(i%10)) 
	        {
			 if (!(i%30))
			  {	 
			   printf ("|\n|~~~~~~~~~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~~~~~~~~~~|\n");
		       } else 
		            {
                     printf ("|\n|------------------------|------------------------|\n");
				     }
             }
           }
   }
   
   
 /*
  * Function : Inizializza
  * Usage    : Inizializza (numeri, tabellone)
  * ---------------------------------------------------------------
  * Questa funzione inizializza i due array che rappresentano i 
  * numeri da estrarre ed il tabellone. Il tabellone viene 
  * inizializzato a 0, successivamente verra' posto ad 1 il 
  * contenuto dell'elemento corrispondente all'indice del numero
  * estratto. Quindi se, ad esempio, viene estratto 35 allora
  * tabellone[35]=1. La sequenza dei numeri estratti viene 
  * inizializzata con i numeri da 1 a 90, in sequenza, e poi 
  * mischiata, cosi' da avere la sequenza di estrazione random gia'
  * pronta, cosi' da semplificare l'algoritmo di giocata, non dovendo
  * tenere in considerazione i numeri gia' estratti.
  */
  void Inizializza(int numeri[], int tabellone[])
     {
	  int i,temp1,temp2;
	  srand(time(0));
	  
	  for (i=0;i<=90;i++)
	    {
		 tabellone[i]=0;
		 numeri[i]=i;
	     }
	  
	  for (i=0;i<25000;i++)
	    {
		 temp1=RandomInt(90);
		 temp2=RandomInt(90);
		 Swap(&numeri[temp1],&numeri[temp2]);
	     }  
	  }   
      
/*
 * Function : Splash Screen
 * Usage    : Splash_Screen();
 * ------------------------------------------------------------------
 * Stampa la schermata di avvio e aspetta che l'utente prema enter
 */
void Splash_Screen (void)
   {
	//CLEAR();
	printf ("▓▓▓▓▓\n");
	printf ("  ▓                  ▓          ▓\n");
	printf ("  ▓     ▓▓   ▓ ▓ ▓   ▓     ▓▓   ▓   ▓▓▓ \n");
	printf ("  ▓    ▓  ▓   ▓ ▓ ▓  ▓▓▓  ▓  ▓  ▓  ▓  ▓\n");
	printf ("  ▓    ▓  ▓   ▓ ▓ ▓  ▓ ▓  ▓  ▓  ▓  ▓  ▓\n");
	printf ("  ▓     ▓▓    ▓ ▓ ▓  ▓▓▓   ▓▓   ▓   ▓▓ ▓\n");
	printf ("\n\n");
    printf ("Programming Exercise by Marino Gregorio\n");
    printf ("And good luck!");
    printf ("\n\n ----------Press Enter -----------\n");
    while (getchar()!='\n');
    return;
    }

/*
 * Function : Vincita
 * Usage    : Vincita(inazione,testo);
 * ------------------------------------------------------------------
 * Stampa una schermata per segnalare una vincita. combinazione e 
 * testo sono usati per personalizzare la schermata. 'combinazione' 
 * fa riferimento alla combinazione effettuata, testo e' una stringa 
 * che indica chi ha vinto ("il Cartellone"," un giocatore".....)
 */
void Vincita(int combinazione, char *testo)
   {
	if (combinazione<-1 || combinazione>4) Error("combinazione fuori range nella funzione Vincita");
	CLEAR();
	printf("\n\n\n\n\n");
	printf("---------------------------------------------\n");
	printf("|             %15s               |\n",testo);
	printf("|                                           |\n");
	printf("|               ha realizzato               |\n");
	printf("|                                           |\n");
	printf("|         %15s                   |\n",combo[combinazione]);
	printf("|                                           |\n");
	printf("---------------------------------------------\n\n");
	printf (" ----------Press Enter -----------\n");
    while (getchar()!='\n');
    return;
    }
    
/*	
 * Function : Setup_premi
 * Usage    : Setup_Premi(premi)
 * -----------------------------------------------------------
 * Viene richiesto il totale del montepremi, e vengono calcolate
 * le vincite secondo questa tabella:
 * Ambo     ->  5% montepremi
 * Terna    ->  8% montepremi
 * Quaterna -> 14% montepremi
 * Cinquina -> 22% montepremi
 * Tombola  -> il resto del montepremi
 * Il risultato viene inserito nell'array premi[], viene mostrata
 * a video la ripartizione di premi, e si aspetta la pressione
 * del tasto enter
 */
void Setup_Premi (float premi [])
   {
	int i;
	float montepremi;
	CLEAR();
	printf ("\n\n");
	printf("  Inserisci il montepremi : ");
	montepremi=input_float('.');
	
	premi[0]=(montepremi/10)*5;
	premi[1]=(montepremi/10)*8;
	premi[2]=(montepremi/10)*14;
	premi[3]=(montepremi/10)*22;
	
	// Arrotonda ai decimi di €
	for (i=0;i<4;i++)
	  {
	   premi[i]=round(premi[i])/10.0;
       }
    
    // Tombola   
	premi[4]=montepremi-(premi[0]+premi[1]+premi[2]+premi[3]);
    printf ("\n\n");
	printf ("  I premi sono cosi' ripartiti:\n");
	printf (" --------------------------------\n");
	printf ("  Ambo      %.2f\n",premi[0]);
	printf ("  Terno     %.2f\n",premi[1]);
	printf ("  Quaterna  %.2f\n",premi[2]);
	printf ("  Cinquina  %.2f\n",premi[3]);
	printf ("  Tombola   %.2f\n",premi[4]);
	
	printf ("\n ----------Press Enter -----------\n");
    while (getchar()!='\n');
    printf ("\n");
	
	}    
/*
 * Function : RandomInt
 * Usage    : n=RandomInt(limite);
 * --------------------------------------------------------
 * Questa funzione genera un numero casuale tra 1 e limite.
 * E' implementata in modo da normalizzare le uscite, scartando
 * tutti i valori superiori a int(RAND_MAX/limite)*limite
 */  
int RandomInt (int upperlimit)
  {
   int limit,r,result;
   
   
   limit = RAND_MAX - (RAND_MAX % upperlimit);
   
   while((r = rand()) >= limit);
   
   result=(r%upperlimit)+1;
   return (result);
   }
   
 /*
  * Function : Swap
  * Usage    : Swap (&a,&b);
  * ---------------------------------------------------
  *
  * Questa funzione scambia il valore di due variabili
  */
 void Swap (int *a, int *b)
    {
	 int temp;
	 temp=*a;
	 *a=*b;
	 *b=temp;
     }  

/*
 * Function : Mostra_Estratto
 * Usage    : Mostra_Estratto(num);
 * ----------------------------------------------------
 * Mostra a schermo il numero 'num' con caratteri grandi
 * I caratteri sono creati utilizzando il carattere '▓'
 * e sono memorizzati in un array tridimensionale.
 * Il primo indice dell'array corrisponde alla cifra da
 * visualizzare. Il numero da visualizzare viene suddiviso
 * in Decine (variabile d) ed unita' (variabile u).
 * Dopo la visualizzazione viene attesa la pressione del 
 * tasto enter.
 */
 void Mostra_Estratto (int num)
    {
	 int d,u,i;
	 char digit[10][7][16]={
	    	                {" ▓▓▓ ",
		                     "▓  ▓▓",
		                     "▓ ▓ ▓",
		                     "▓▓  ▓",
		                     "▓   ▓",
		                     "▓   ▓",
		                     " ▓▓▓ "},
		              
	                        {"  ▓  ",
		                     " ▓▓  ",
		                     "▓ ▓  ",
		                     "  ▓  ",
		                     "  ▓  ",
		                     "  ▓  ",
		                     "▓▓▓▓▓"},
		             
	                        {" ▓▓▓ ",
		                     "▓   ▓",
		                     "    ▓",
		                     "   ▓ ",
		                     "  ▓  ",
		                     " ▓   ",
		                     "▓▓▓▓▓"},
		                   
		                    {" ▓▓▓ ",
		                     "▓   ▓",
		                     "    ▓",
		                     "  ▓▓ ",
		                     "    ▓",
		                     "▓   ▓",
		                     " ▓▓▓ "},
		                   
		                    {"   ▓ ",
		                     "  ▓▓ ",
		                     " ▓ ▓ ",
		                     "▓  ▓ ",
		                     "▓▓▓▓▓",
		                     "   ▓ ",
		                     "   ▓ "},
		                   
		                    {"▓▓▓▓▓",
		                     "▓    ",
		                     "▓▓▓▓ ",
		                     "    ▓",
		                     "    ▓",
		                     "▓   ▓",
		                     " ▓▓▓ "},
		                    
		                    {" ▓▓▓ ",
		                     "▓   ▓",
		                     "▓    ",
		                     "▓▓▓▓ ",
		                     "▓   ▓",
		                     "▓   ▓",
		                     " ▓▓▓ "},
		                   
		                    {"▓▓▓▓▓",
		                     "    ▓",
		                     "   ▓ ",
		                     "  ▓  ",
		                     " ▓   ",
		                     " ▓   ",
		                     " ▓   "},
		                   
		                    {" ▓▓▓ ",
		                     "▓   ▓",
		                     "▓   ▓",
		                     " ▓▓▓ ",
		                     "▓   ▓",
		                     "▓   ▓",
		                     " ▓▓▓ "},
		                   
		                    {" ▓▓▓ ",
		                     "▓   ▓",
		                     "▓   ▓",
		                     " ▓▓▓▓",
		                     "    ▓",
		                     "    ▓",
		                     " ▓▓▓ "}
		                   };   
	 CLEAR();
	 printf("\n\n\n\n              Numero Estratto \n\n");
	 d=num/10;
     u=num%10;
     for (i=0;i<7;i++)
         {
		  printf("                %s %s \n",digit[d][i],digit[u][i]);
	      }
	 printf("\n");
	      
	 printf ("\n\n      ----------Press Enter -----------\n");
     while (getchar()!='\n');
     return;
	} 
	
	
	
/* Function : Controlla_Vincita
 * Usage    : n=Controlla_Vincita(combinazione);
 * ------------------------------------------------------------------
 * Effettua un controllo sul tabellone per verificare se vi sia una
 * combinazione vincente, non ancora reclamata. In caso affermativo
 * viene visualizzata la dicitura a schermo ed attesa la pressione 
 * del tasto enter. La funzione ritorna 1 in caso di vincita, 
 * altrimenti ritorna 0. Essa e' divisa in due sezioni, una per il
 * controllo della tombola, e un'altra per le altre combinazioni. 
 * N.B. il confronto con prox_win+2; e' utile per allineare l'indice 
 * della combinazione (0=ambo,1=terno....) al numero di estratti richiesti
 * per la stessa (ambo=2 num....etc). Il controllo viene fatto sommando
 * il numero di '1' nella stessa riga di 5 numeri. Se e' uguale a prox_win+2
 * allora viene segnalata la vincita.
 * Per la tombola vengono controllate singolarmente le cartelle, preferito
 * all' implementazione con un ulteriore ciclo for/next.
 */
 int Controlla_Vincita(int prox_win)
   {
	int i,j,estratti;
	
	// Controllo Combinazioni (2/4)
	if (prox_win<4)
	 {
	  for (i=1;i<91;i+=5)
        {
         estratti=0; 
         for (j=i;j<i+5;j++)
           {
		    estratti+=tabellone[j];
	        }
         if (estratti==prox_win+2)
          {
		   Vincita (prox_win,"Il Cartellone");
		   return(1);	   
	       }
	   }
	  }
	 
	 // Controllo Tombola
	 if (prox_win==4)
	  {
	   // Controllo Cartella 1
	   estratti=0;
	   for (i=0;i<3;i++)
	     {
		  for (j=1;j<6;j++)
		    {
			 estratti+=tabellone[i*10+j];    
		     }
		  }   	
	    
	    // Controllo Cartella 2
	    if (estratti!=15)
         {
		  estratti=0;
	      for (i=0;i<3;i++)
	        {
		     for (j=6;j<11;j++)
		        {
			     estratti+=tabellone[i*10+j];    
		         }
		     }
		  }
		// Controllo Cartella 3
	    if (estratti!=15)
         {
		  estratti=0;
	      for (i=0;i<3;i++)
	        {
		     for (j=31;j<36;j++)
		        {
			     estratti+=tabellone[i*10+j];    
		         }
		     }
		  }
		  
		// Controllo Cartella 4
	    if (estratti!=15)
         {
		  estratti=0;
	      for (i=0;i<3;i++)
	        {
		     for (j=36;j<41;j++)
		        {
			     estratti+=tabellone[i*10+j];    
		         }
		     }
		  }
		
		// Controllo Cartella 5
	    if (estratti!=15)
         {
		  estratti=0;
	      for (i=0;i<3;i++)
	        {
		     for (j=61;j<66;j++)
		        {
			     estratti+=tabellone[i*10+j];    
		         }
		     }
		  }
		
		// Controllo Cartella 6
	    if (estratti!=15)
         {
		  estratti=0;
	      for (i=0;i<3;i++)
	        {
		     for (j=66;j<71;j++)
		        {
			     estratti+=tabellone[i*10+j];    
		         }
		     }
		  }        
		
		if (estratti==15)
		 {
		  Vincita(prox_win,"Il Tabellone");
		  return(1);
	      }
	 }            	 	    
	return (0);
    }       	    

  
 /* Function : Filtra_input
  * Usage    : ch=Filta_Input();
  * --------------------------------------------------------
  * Funzione usata per gestire l'input da tastiera nel loop 
  * principale. Attende che l'utente prema enter, e ritorna
  * il carattere 'V' se il primo carattere immesso e' 'V'.
  * Ritorna '\n' in tutti gli altri casi
  */  
  char Filtra_Input (void)
     {
      char ch;
                 
      ch=getchar();	  
      if (ch=='V')
       {
	    while (getchar()!='\n');
        } else 
             {
			  while (ch!='\n') ch=getchar();
			  }
   return (ch);      
   }  



/* 
 * Function : input_string 
 * Usage    : input_string (line,n);
 * -----------------------------------------------
 * This function ask the user for a string, and store
 * it in line[]. The '\n' character is discarged and a '\0'
 * is added to correctly terminate the string. If the user 
 * enters more than n character, the function signals the 
 * error and abort the program.
 */
void input_string (char line[],int n)  
   {
	int i=-1;
	char ch=0;
	 
	while (ch!='\n')
	    {
	     i++;
		 if(i>=n) Error("Error in function input_string: Too many characters");
		 ch=getchar();
		 line[i]=ch;
	     }
	line[i]='\0';
    }     

/* 
 * Function : input_float
 * Usage    : double = input_float(',');
 * ----------------------------------------------
 * This functions,asks the user for a string, using the 
 * input_string function, validate it as a string of decimal
 * digits, and returns the converted double value. 
 * The function can be easily adapted to convert a string
 * passed as parameter. The parameter dot is used to define
 * the character used to separate the integer part from the
 * decimal ones. Usually a '.' or a ','.
 */
double input_float(char dot)
     {
	  char temp[255];
	  int dp,onedot,lenght,index,i;
	  double t,total;
	 
	  /* Ask the user for a string, and, while determining
	   * its lenght, validate it as a numeric string.
	   * If not, display an error and loop. If yes the 
	   * lenght of the string is stored in 'lenght'. The position
	   * of the dot is stored in 'dp. A preliminary check is made
	   * for a '-' in front of the digits.In this case the 
	   * conversion is made starting from the [1] element and 
	   * the result is made negative. The variable 'onedot' is
	   * used to check if more than one dot is entered. This is
	   * necessary because the character 'dot' is a legal character
	   * and will bypass the validate check.
	   */
	  
	  lenght=-1;
	  
	  while (lenght==-1)
	      {
		   index=0;
		   dp=-1;
	       onedot=0;  
		   lenght++;
		   
		   input_string(temp,10);
		   
		   while (lenght>=0&&temp[lenght]!='\0')
		       {
				// The first char is '-'?
				if (lenght==0&&temp[lenght]=='-')
				 { 
				  lenght++;
				  index=1;
			      }
				if ((temp[lenght]>='0'&&temp[lenght]<='9') || temp[lenght]==dot)
				 {
				  // Store the dot position, and store number of dots.
				  if (temp[lenght]==dot)
				    {
					 dp=lenght;
					 onedot++;
				     }
				  lenght++;
			      } else
			           {
						lenght=-1;
					    }
				}
			// The user entered more than one dot?
			if (onedot>1) lenght=-1;
			
			// Invalid input?
			if (lenght==-1)
			 {
			  printf("Not a number. Retry : ");
		      }
		    }  		     
	
	// The number has no dot, so we assume the dot at the end
	if (dp==-1) dp=lenght;
	
	/* String conversion:
	 * Integer part:
	 * For each digit, to the dot, depending from the position, a cycle of 
	 * multiply for 10 is made, and added to total.
	 * 'lenght' keep the value of most significant digit, while index
	 * travel between digits. Thus the position value of every digit
	 * is "lenght-index" position. 
	 */ 	
	total=0;
	for (;index<dp;index++)
	  {
	   t=temp[index]-'0';
	   // A simple power of 10 cicle.
	   //for (i=0;i<lenght-index-1-(lenght-dp);i++) is reduced to:
	     for (i=0;i<dp-index-1;i++)
	     {
		  t*=10;
	      }
	   total+=t;
       }   
	
	// Decimal part:
	index++;
	for (;index<lenght;index++)
	  {
	   t=temp[index]-'0';
	   
	   for (i=0;i<index-dp;i++)
	     {
		  t/=10.0;
	      }
	   total+=t;
       }    
	// The number is negative?
	if (temp[0]=='-')total=-total;
	
	return (total);
	}	
	
	
/*
 * Function : Error
 * Usage    : Error(error_message);
 * -----------------------------------------------------
 * This simple function display the message 'error_message'
 * and abort the execution
 */
void Error (char *error_message)
   {
	printf ("---- ERROR!!! ----\n%s\n",error_message);
	exit(0);
    }
