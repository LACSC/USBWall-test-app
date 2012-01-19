#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <linux/usb/usbwall.h>

/**
 * \fn void Add_PenDrive(char *file_name).
 * \param file_name is the name of the file which contains the white list. 
 * \return no return for this function.
 * Used to add a USB pen drive into the white list.
 */

void Add_PenDrive(char *file_name)
{
  struct usbwall_pendrives_info key;
  
  int ok = 0;
  int ko = 1;
  int h;
  char vendor[5];
  while(ok == 0)
  {
     ko = 1;
     printf("\nEnter Vendor Id:\n");
     scanf("%s", vendor);                                      /* Get the value entered by the administrator in a char variable "vendor" */
     scanf("%*[^\n]");
     getchar();     
     for(h=0;h<(strlen(vendor));h++)                         /* This loop enable to test the number entered : number in hexadecimal */
     {
        if((strlen(vendor) <= 4) && (vendor[h] == '\0' || vendor[h] == '0' || vendor[h] == '1' || vendor[h] == '2' || vendor[h] == '3' || vendor[h] ==  	'4' || vendor[h] == '5' || vendor[h] == '6' || vendor[h] == '7' || vendor[h] == '8' || vendor[h] == '9' || vendor[h] == 'A' || vendor[h] == 'B' 	|| vendor[h] == 'C' || vendor[h] == 'D' || vendor[h] == 'E' || vendor[h] == 'F' || vendor[h] == 'a' || vendor[h] == 'b' || vendor[h] == 'c' || 		vendor[h] == 'd' || vendor[h] == 'e' || vendor[h] == 'f'))
        {
           ok = 1;                                             /* The character tested is a hexadecimal */
        }
        else
        {
           ko = 0;                                             /* The character tested is not a hexadecimal or the string "vendor" is too long */
        }
     }
     if(ko == 0)                                               /* If one of the character in "vendor" is not a hexadecimal then we stay in the loop */
     {
        ok = 0;
        printf("\n-> Warning : you have to use four characters between '0' and '9' or 'a' to 'd'\n\n");
     }
  }
  key.idVendor = strtoll(vendor,NULL,16);                      /* Convert the char value "vendor" into int. The int value is registered in idVendor */


  ok = 0;
  ko = 1;
  h = 0;
  char product[5];
  while(ok == 0)
  {
     ko = 1;
     printf("\nEnter Product Id:\n");
     scanf("%s", product);                                     /* Get the value entered by the administrator in a char variable "product" */
     scanf("%*[^\n]");
     getchar();     
     for(h=0;h<(strlen(product));h++)                          /* This loop enable to test the number entered : number in hexadecimal */
     {
        if((strlen(product) <= 4) && (product[h] == '\0' || product[h] == '0' || product[h] == '1' || product[h] == '2' || product[h] == '3' ||         	product [h] == '4' || product[h] == '5' || product[h] == '6' || product[h] == '7' || product[h] == '8' || product[h] == '9' || product[h] == 		'A' 	|| product[h] == 'B' || product[h] == 'C' || product[h] == 'D' || product[h] == 'E' || product[h] == 'F' || product[h] == 'a' ||  		product[h] == 'b' || product[h] == 'c' || product[h] == 'd' || product[h] == 'e' || product[h] == 'f'))
        {
           ok = 1;                                             /* The character tested is a hexadecimal */
        }
        else
        {
           ko = 0;                                             /* The character tested is not a hexadecimal or the string "vendor" is too long */
        }
     }
     if(ko == 0)                                               /* If one of the character in "vendor" is not a hexadecimal then we stay in the loop */
     {
        ok = 0;
        printf("\n-> Warning : you have to use four characters between '0' and '9' or 'a' to 'd'\n\n");
     }
  }
  key.idProduct = strtoll(product,NULL,16);                    /* Convert the char value "product" into int. The int value is registered in idProduct */


  char SerialNumber[32]="";
  int i, p;
  ok = 0;
  do
  {
  printf("\nEnter serial number:\n");
  printf("-> Warning : you have to use only characters in uppercase or number between '0' and '9'\n\n");
  fgets(SerialNumber,32,stdin);                                 /* Get the value entered by the administrator in a char variable "SerialNumber" */
  for(i=0;i<32;i++)                                           
  {
    if(SerialNumber[i] >= 0x30  && SerialNumber[i] <= 0x5A)    /* User have to type the "SerialNumber" in uppercase */
    {
      //key.idSerialNumber[i] = SerialNumber[i];                 /* Copy the char value "SerialNumber" into idSerialNumber */
      ok = 1;
    }
    else if(SerialNumber[i] <= 0x30  && SerialNumber[i] >= 0x5A)
    {
      ok = 0;
      break;
    }
  }
  while(ok == 0);
  for(p=0;p<32;p++)                                           
  {
    key.idSerialNumber[p] = SerialNumber[p];
  }
  /*while(SerialNumber[i] >= 0x30  && SerialNumber[i] <= 0x5A)
  {
    key.idSerialNumber[i] = SerialNumber[i];
    i++;
  }*/
  


  FILE* file = NULL;                                          /* Creation of a pointer of file */
  file = fopen(file_name, "ab");                              /* We open the file in order to add contains in binary mode */
  if(file !=NULL)                                             /* If the file can be opened */
  {
    fprintf(file, "%d %d %s", key.idVendor, key.idProduct, key.idSerialNumber);    /* The whole structure is copied into the file */
    fclose(file);                                                                  /* We close the file */
  }
  else
  {
     printf("Sorry, there is a problem with the file: it can not be opened\n");
  }
}

/**
 * \fn void Consult_WhiteList(char *file_name).
 * \param file_name is the name of the file which contains the white list. 
 * \return no return for this function.
 * Used to consult the white list.
 */

void Consult_WhiteList(char *file_name)
{
    FILE* file=NULL;
    file = fopen(file_name,"rb");                            /*We open the file in writing */
    if (file!= NULL)
    {
        int i, j, vendor, product;
        int line_number = 0;                                 /* Variable used to count the line number */
        char buffer[256];                                    /* We declare a board of 256 characters */
        while(fgets(buffer,255,file))                        /* We read the file while it does not end */
        {
            line_number++;                                   /* We increase the number of line: this number corresponds to the number of pen drive*/
        }
        fseek(file, 0, SEEK_SET);                            /* We go back to the begining of the file */
        struct usbwall_pendrives_info list[line_number];
        for(i=0;i<line_number;i++)
        {
	    fscanf(file,"%d %d %s",&vendor,&product,list[i].idSerialNumber);
	    list[i].idVendor = vendor;
	    list[i].idProduct = product;
        }
	printf("Device Num : Id Vendor : Id Product : Serial Number\n\n");
	for(j=0;j<line_number;j++)
	{
	    printf("Device %d     %x : %x : %s\n",j,list[j].idVendor,list[j].idProduct,list[j].idSerialNumber);
	}
        fclose(file);
    }
    else
    {
        printf("Sorry, there is a problem with the file: it can not be opened\n");
    }
}


/**
 * \fn int main().  
 * \return End of the software.
 * Display the menu in the console (CLI).
 */

int main()
{


    int q = 0;
    while(q == 0)
{

    int MenuChoice;

    printf("\n ============================ USB WALL: MENU ============================ \n\n\n");
    printf("    1- Add a pendrive into the white list\n\n");
    printf("    2- Consult the white list\n\n");
    printf("    3- Quit the program\n\n\n");
    scanf("%d", &MenuChoice);
    scanf("%*[^\n]");
    getchar();


    printf("\n\n");

    switch (MenuChoice)
    {
        case 1:
            printf("**************** Add a pen drive into the white list ****************\n\n");
            Add_PenDrive("list.bin");
            break;



        case 2:
            printf("**************** Consult the the white list ****************\n\n");
            Consult_WhiteList("list.bin");
            break;



        case 3:
            printf("**************** Quit the program ****************\n\n");
            q = 1;

            break;


        default:
            printf("The number you entered was not correct. Please, enter 1, 2, 3 or 4. \n");
    }

    printf("\n\n");
}


return 0;
}







