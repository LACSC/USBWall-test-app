/**
 * \file load_white_list.c
 * \brief load the white list (user space to kernel space)
 * \author David FERNANDES
 * \version 0.1
 * \date 10 january 2012
 *
 * File pendrivesinfo.c for project usbwall
 * 
 * LACSC - ECE PARIS Engineering school 
 *
 * Made by David FERNANDES
 * Login   <dfernand@ece.fr>
 *
 * Started on  Tue Jan  10 14:49:06 2012 David FERNANDES
 *
 * Copyright (C) 2012 - David FERNANDES
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <libusbwall.h>
#include <linux/usb/usbwall.h>

int main()
{
  /* variables */
  FILE *fd_whitelist;
  int  res, tmp_idProduct, tmp_idVendor, tmp_keyflags, line_number, i;
  /* File containing pen drives informations : white list */ 
  char *fileinfo = "whitelist";
  char buffer[256]; 
    
  /* opening file containing pen drives informations */ 
  fd_whitelist = fopen(fileinfo, "rb");
  if(fd_whitelist == NULL)
  {
    fprintf(stderr, "Cannot open whitelist file\n");
    return 1;
  }

 /* reading file containing mass storages informations */
  line_number = 0;
  while(fgets(buffer,255,fd_whitelist)) 
  {
    line_number++;                                   
  }
  fseek(fd_whitelist, 0, SEEK_SET);

  procfs_info_t infopendrive[line_number];

  for(i=0;i<line_number;i++) 
  {
    fscanf(fd_whitelist,"%d %d %d %s", &tmp_keyflags, &tmp_idVendor, &tmp_idProduct, infopendrive[i].info.idSerialNumber);
    infopendrive[i].info.keyflags = tmp_keyflags;
    infopendrive[i].info.idVendor = tmp_idVendor;       
    infopendrive[i].info.idProduct = tmp_idProduct;
    printf("Mass storage, right : %d, idVendor : %x(hex), idProduct : %x(hex), SerialNumber : %s, is loaded in the white list\n", infopendrive[i].info.keyflags, infopendrive[i].info.idVendor, infopendrive[i].info.idProduct, infopendrive[i].info.idSerialNumber);
  }  

  /* closing file containing pen drives informations */
  fclose (fd_whitelist);
  
  /* communication between user space and kernel space */
  /* opening procsf */
  res = usbwall_init();
  if (res != 0) 
  {
    printf("The file key_ctrl doesn't exist %d\n", res);
    return 0;
  }

  /* passing white list from user space to kernel space */
  for(i=0;i<line_number;i++) 
  {
    usbwall_key_add(infopendrive[i].info.idVendor, infopendrive[i].info.idProduct, infopendrive[i].info.idSerialNumber);
  }

  /* closing char device */
  usbwall_release();
  
  return 0;
}

