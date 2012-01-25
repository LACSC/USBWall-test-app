/**
* \file create_white_list.c
* \brief create the white list
* \author karthika KANDIAH
* \author Guillaume VIRASOLVY
* \version 0.1
* \date 3 january 2012
*
* File pendrivesinfo.c for project usbwall
*
* Made by karthika KANDIAH and Guillaume VIRASOLVY
* Login <kandiah@ece.fr> <virasolv@ece.fr>
*
* Started on Tue Jan 10 14:49:06 2012 karthika KANDIAH
*
* Copyright (C) 2012 - karthika KANDIAH Guillaume VIRASOLVY
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

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
  struct usbwall_token_info key;
  
  /****************************** Get the vendor ID and write it in the "usbwall_token_info" structure ******************************/

  int ko = 0;
  int h;
  char vendor[5];
  while(ko == 0)
  {
     ko = 1;
     printf("\nEnter Vendor Id:\n");
     scanf("%s", vendor); /* Get the value entered by the administrator in a char variable "vendor" */
     scanf("%*[^\n]");
     getchar();
     for(h=0;h<(int)(strlen(vendor));h++) /* This loop enable to test the number entered : number in hexadecimal */
     {
        if((strlen(vendor) >= 5) || ((vendor[h] <= 0x2F) || ((vendor[h] >= 0x3A) && (vendor[h] <= 0x40)) || ((vendor[h] >= 0x47) && (vendor[h] <= 0x60)) || (vendor[h] >= 0x67)))
        {
           ko = 0; /* The character tested is a hexadecimal */
        }
     }
     if(ko == 0) /* If one of the character in "vendor" is not a hexadecimal then we stay in the loop */
     {
        printf("\n-> Warning : you have to use four characters between '0' and '9' or 'a' to 'd'\n\n");
     }
  }
  key.idVendor = strtoll(vendor,NULL,16); /* Convert the char value "vendor" into int. The int value is registered in idVendor */



  /****************************** Get the product ID and write it in the "usbwall_token_info" structure ******************************/

  ko = 0;
  h = 0;
  char product[5];
  while(ko == 0)
  {
     ko = 1;
     printf("\nEnter Product Id:\n");
     scanf("%s", product); /* Get the value entered by the administrator in a char variable "product" */
     scanf("%*[^\n]");
     getchar();
     for(h=0;h<(int)(strlen(product));h++) /* This loop enable to test the number entered : number in hexadecimal */
     {
        if((strlen(product) >= 5) || ((product[h] <= 0x2F) || ((product[h] >= 0x3A) && (product[h] <= 0x40)) || ((product[h] >= 0x47) && (product[h] <= 0x60)) || (product[h] >= 0x67)))
        {
           ko = 0; /* The character tested is a hexadecimal */
        }
     }
     if(ko == 0) /* If one of the character in "vendor" is not a hexadecimal then we stay in the loop */
     {
        printf("\n-> Warning : you have to use four characters between '0' and '9' or 'a' to 'd'\n\n");
     }
  }
  key.idProduct = strtoll(product,NULL,16); /* Convert the char value "product" into int. The int value is registered in idProduct */



  /****************************** Get the serial number and write it in the "usbwall_token_info" structure ******************************/

  char SerialNumber[32]="";
  int i;
  ko = 0;
  while(ko == 0)
  {
    ko = 1;
    printf("\nEnter serial number:\n");
    fgets(SerialNumber,32,stdin); /* Get the value entered by the administrator in a char variable "SerialNumber" */
    for(i=0;i<(int)(strlen(SerialNumber)-1);i++)
    {
      if((strlen(SerialNumber) <= 31) && (SerialNumber[i] < 0x30 || SerialNumber[i] > 0x5A)) /* User have to type the "SerialNumber" in uppercase */
      {
        ko = 0;
      }
    }
    if(ko == 0)
    {
      printf("\n-> Warning : you have to use only characters in uppercase or number between '0' and '9'\n\n");
    }
  }
  strcpy(key.idSerialNumber,SerialNumber);



  /****************************** Get the device's right and write it in the "usbwall_token_info" structure ******************************/

  int flags;
  ko = 0;
  while(ko == 0)
  {
    ko = 1;
    printf("\nEnter the rights\n");
    scanf("%d", &flags);
    scanf("%*[^\n]");
    getchar();
    if((flags != 0) && (flags != 1))
    {
      ko = 0;
      printf("\n-> Warning : type 0 to refuse the device or 1 to authorize the device\n\n");
    }
  }
  key.keyflags = flags;
 


  /****************************** Get the structure "usbwall_token_info" and write it in the file "whitelist" ******************************/

  FILE* file = NULL; /* Creation of a pointer of file */
  file = fopen(file_name, "ab"); /* We open the file in order to add contains in binary mode */
  if(file !=NULL) /* If the file can be opened */
  {
    fprintf(file, "%d %d %d %s", key.keyflags, key.idVendor, key.idProduct, key.idSerialNumber); /* The whole structure is copied into the file */
    fclose(file); /* We close the file */
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
    file = fopen(file_name,"rb"); /*We open the file in writing */
    if (file!= NULL)
    {
      int i, j, vendor, product, flags;
      int line_number = 0; /* Variable used to count the line number */
      char buffer[256]; /* We declare a board of 256 characters */
      while(fgets(buffer,255,file)) /* We read the file while it does not end */
      {
          line_number++; /* We increase the number of line: this number corresponds to the number of pen drive*/
      }
      fseek(file, 0, SEEK_SET); /* We go back to the begining of the file */
      if(line_number != 0)
      {
        struct usbwall_token_info list[line_number];
        for(i=0;i<line_number;i++)
        {
          fscanf(file,"%d %d %d %s",&flags,&vendor,&product,list[i].idSerialNumber);
          list[i].idVendor = vendor;
          list[i].idProduct = product;
          list[i].keyflags = flags;
        }
        fclose(file);
        printf("Device Num : Id Vendor : Id Product : Serial Number : Rights\n\n");
        for(j=0;j<line_number;j++)
        {
          printf("Device %d   : %x : %x : %s : %d\n",j,list[j].idVendor,list[j].idProduct,list[j].idSerialNumber,list[j].keyflags);
        }
      }
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
int main(void)
{


    int q = 0;
    while(q == 0)
    {

      int MenuChoice;

      printf("\n ============================ USB WALL: MENU ============================ \n\n\n");
      printf(" 1- Add a pendrive into the white list\n\n");
      printf(" 2- Consult the white list\n\n");
      printf(" 3- Quit the program\n\n\n");
      scanf("%d", &MenuChoice);
      scanf("%*[^\n]");
      getchar();


      printf("\n\n");

      switch (MenuChoice)
      {
          case 1:
              printf("**************** Add a pen drive into the white list ****************\n\n");
              Add_PenDrive("whitelist");
              break;



          case 2:
              printf("**************** Consult the the white list ****************\n\n");
              Consult_WhiteList("whitelist");
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


